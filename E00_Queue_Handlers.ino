// Copyright (c) Ron D Bentley (UK), see copyright notice
//
//  This tab contains all of the routines to establish and maintain interrupt
//  queue management in a queued framework.
//
//  A free chain of blocks is set up that are then allocated to interrupt handlers
//  to record that they have been triggered. The interrupt trigger digital pin will
//  be recorded in the allocated queue block.
//
//  'IQ' is used as the blocks of memory to be allocated/deallocated on request.
//  'IQ' comprises a number of free blocks each of 2 integers, the first word of each block
//  contains a forward pointer to next block or end of chain value(free/used), the second word
//  should be treated as undefined for free blocks.
//  When a free block is allocated to an interrupt, it is inserted at the beginning of the
//  interrupt queue with the first word of the block being a forward pointer to next interrupt 
//  block and the second word containing the digital pin number of the triggered inerrupt. 
//
//  Note:
//      1.  The number of free blocks in the IQ free chain is defined by 'max_IQ_free__chain_blocks'.
//          this should be tailored to the number of interrupts being monitored, as...
//      2.  the implementation is as a queue, first in first out (FIFO).
//      3.  Interrupts in the IQ can be unique or nonunique (multiple).  This is controlled via the
//          defined boolean variable 'unique_interrupts' in configuration data (C00_Configurations tab)-  
//            ~ set to 'true' for unique entries or
//            ~ set to 'false' for nonunique entries in the IQ. This is set
//          

int unique_entry_flags=0;   //  used to determine if given interrupt has 'unique' flag set in IQ

volatile int IQ[max_IQ_free_chain_blocks][2]; 

//  Pointers to manage the free blocks in the IQ
volatile int start_of_free_IQ_chain; 
volatile int num_free_IQ_blocks;

//  Pointers to manage the allocated blocks in the IQ
volatile int start_of_IQ_chain =  -1;
volatile int end_of_IQ_chain =    -1;
volatile int num_IQ_interrupts =   0;

//  general values to mnage IQ processes
#define end_of_chain_value    -1

#define fail                  -1
#define success                1

#define no_interrupt_request  -1
#define no_entry              -1

int  int_number;    //  used by scan_IQ to set up the parametsr for the triggered interrupt if they are needed
int  int_pin;       //  ditto
int  int_pinmode;   //  ditto
int  int_trigger;   //  ditto
int  int_unique;    //  ditto
int  int_debounce;  //  ditto
//
//  Set up the free IQ chain as defined by 'max_IQ_free__chain_blocks'
//  This is called just once per start/reset from te setup() process.
//
void  create_IQ_free_chain()
{int  ptr, last_IQ_block;
  last_IQ_block=max_IQ_free_chain_blocks-1;
  for (ptr=0; ptr < max_IQ_free_chain_blocks;ptr++)
  {
    if (ptr == last_IQ_block){IQ[ptr][0]=end_of_chain_value;} // set end of chain
    else                     {IQ[ptr][0]=ptr+1;}              // set forward block pointer
    IQ[ptr][1] = -1;  // clear data word out of range for interrupt numbers
  }
  num_free_IQ_blocks = max_IQ_free_chain_blocks;
  start_of_free_IQ_chain=0;
}
//
//  Allocates a block from the free chain, if one exists.
//  NOTE: assumes that interrupts are disabled by calling function(s).
//
int acquire_block_from_free_IQ()
{int  block;
  if (num_free_IQ_blocks > 0)
  { //  There is at least 1 free block left.
    //  Take the next block off the free chain.
    num_free_IQ_blocks--;  //  reduce free blocks available.
    block = start_of_free_IQ_chain;
    start_of_free_IQ_chain = IQ[block][0];  //  take the first free block off the free chain.
    IQ[block][0] = end_of_chain_value;      //  set the forward pointer in this free block to out of range.
    IQ[block][1] = -1;    //  ensure the value held in the new block data word is initialised to other than an interrupt value.
    return block;         //  return with the 'address' of the free block provided to calling routine.
  }
  return  fail;           //  no free blocks! Return error condition.
}
//
// Returns the given block back to the free chain.
// NOTE: assumes that interrupts are disabled by calling function(s).
//
int relinquish_block_to_free_IQ(int block)
{
  if (num_free_IQ_blocks < max_IQ_free_chain_blocks) 
  { //  there is space to add this block back to the free chain.
    num_free_IQ_blocks++;  //  increase number of free blocks on free chain by 1
    IQ[block][0] = start_of_free_IQ_chain;
    IQ[block][1] = -1;    //  clear data word
    start_of_free_IQ_chain = block;
    return  success;      //  relinquish was successful.
  }
  return  fail;           //  free chain seems to be full of free blocks! No space to add another.
}
//
//  The function creates an entry in the interrupt queue for the given interrupt.
//
int insert_into_interrupt_IQ(int interrupt)
{ int  block;
  block = acquire_block_from_free_IQ();
  if (block == fail)
  { //  no free block available!
    return fail; 
  }
  //  we have a free block, so chain it into the interrupt queue
  //  placing it at the end of the current queue.
  if (num_IQ_interrupts == 0)
  { // queue is empty, so set start pointer
    start_of_IQ_chain = block;
  }
  else
  { // at least on entry in IQ queue, so modify forward pointer of last block
    IQ[end_of_IQ_chain][0] = block;
  }
  //  now deal with rest of updates
  num_IQ_interrupts++; 
  end_of_IQ_chain = block;
  IQ[end_of_IQ_chain][0] = end_of_chain_value;  // set new end of chain
  IQ[end_of_IQ_chain][1] = interrupt;
  return success;
}

//
//  See if there are any outstanding (unprocessed) ineterrupts requests in the 
//  interrupt queue.  If so, take the first one on the queue and return it to the
//  free chain.  The answer to the function is either the interrupt that was triggered
//  or that no outstanding request exists.
//
//  Note that this function is not entered via an interrupt routine, so we do need to 
//  set noInterrupts and reset them after processing to ensure exclusivity.
//
int scan_IQ()
{ int IQ_block, interrupt;
  noInterrupts();   //  ensure exclusive access of the IQ and its pointers
  if (num_IQ_interrupts == 0)
  {
    //  Clear down the interrupt parameters for this interrupt, 
    //  solely for end user reference if required
    int_number  = no_interrupt_request;
    int_pin     = no_interrupt_request;
    int_pinmode = no_interrupt_request;
    int_trigger = no_interrupt_request;
    int_unique  = no_interrupt_request;
    int_debounce= no_interrupt_request;
    interrupts(); 
    return no_interrupt_request;
  }
  // take the first entry off the IQ as this is the next interrupt to be processed.
  num_IQ_interrupts--;
  IQ_block = start_of_IQ_chain;
  start_of_IQ_chain = IQ[IQ_block][0];    //  point to next interrupt to be processed, or end of chain
  if (num_IQ_interrupts == 0)
  {// last used block to be relinquished, so adjust end of chain pointer
    end_of_IQ_chain = end_of_chain_value;
  }
  interrupt = IQ[IQ_block][1];            //  the interrupt that was activated
  relinquish_block_to_free_IQ(IQ_block);  //  put this block back on the free IQ chain
  //
  //  set up the interrupt parameters for this interrupt, 
  //  solely for end user reference if required
  int_number  = interrupt_config_data[interrupt][6];
  int_pin     = interrupt_config_data[interrupt][1];
  int_pinmode = interrupt_config_data[interrupt][2];
  int_trigger = interrupt_config_data[interrupt][3];
  int_unique  = interrupt_config_data[interrupt][4];
  int_debounce= interrupt_config_data[interrupt][5];
  //  check if this is a 'unique' interrupt type. 
  //  If so, clear 'unique_entry_flags' bit for this generic interrupt number
  if (int_unique) 
  {
    bitWrite(unique_entry_flags, interrupt,false); // clear the unique interrupt in IQ flag
  }
  interrupts();
  return interrupt; // return with the internal value of the interrupt handler that was assigned to this pin
}
