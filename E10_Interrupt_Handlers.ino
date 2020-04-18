// Copyright (c) Ron D Bentley (UK), see copyright notice
//
//  This tab provides the basis for developing applications that require
//  multiple interrupts, occurring from inputs to digital pins.
//  It is a shell to be used with whatever applications are required to have
//  a need for multiple interrupt driven inputs.
//
//  This code is for the Mega 2650 board and the digital pins are set up to trigger when a pin is RISING.
//  Check if using other boards, as this arrangement of pins is not appropriate for some.
//
//  Interrupt handler routines/functions, one for each possible interrupt
//  up to the max defined (at most 6 on the Mega 2560 board)
//

volatile int interrupt_debounce_value; 
volatile int unique_interrupts;

volatile bool initialisation_complete  = false;  // used by interrupt handlers to stop them processing too soon

volatile long unsigned int millis_now, millis_elapsed;

#define millis_max  0xffffffff  //  max value the millis function will achieve before cycling back to 0

//
//  Each interrupt handler will call this function (when triggered) passing it the its associated interrupt 
//  number.  If possible, an entry will be inserted into the IQ.
//
int process_interrupt(int interrupt)
{ int ptr, result;
  /*if (interrupt_config_data[interrupt][0]==inactive)
  { return fail;} // ensures a spurious interrupt will not be processed, only active ones 
  unique_interrupts = interrupt_config_data[interrupt][4]; */
  if (interrupt_config_data[interrupt][4])  //  'unique' marker set up for this generic interrupt type
  { //  check if there is already an interrupt entry in the IQ for this interrupt pin.
    if (bitRead(unique_entry_flags, interrupt)) 
    { // value is set to true, so must already in the IQ.
      return fail;
    }
    //  not already in the IQ, so set flag and insert entry
    result = insert_into_interrupt_IQ(interrupt);
    if (result == success)
    {
      bitWrite(unique_entry_flags, interrupt, true);  // set the flag for this generic interrupt to true
    } 
    return result;  // either success or fail.
  }
  // this is a nonunique interrupt request
  return insert_into_interrupt_IQ(interrupt); // result will be either success or fail
}

void generic_handler0() 
{static long unsigned int millis_previous;
  if (initialisation_complete == true) 
  { //  all variables are initialised so we are okay to continue to process this interrupt.
    //  deal with potential 'bouncing' of this interrupt.  The code will 'debounce' spurious
    //  interrupts.
    millis_now = millis();
    if (millis_now < millis_previous)
    { // millis timer has cycled since last tested, so adjust
        millis_elapsed = millis_max - millis_previous + millis_now + 1;
    }
    else
    { // millis_now is in advance of previous millis 
      millis_elapsed = millis_now - millis_previous + 1;
    }
    interrupt_debounce_value = interrupt_config_data[0][5];
    if (millis_elapsed >= interrupt_debounce_value)
    { //  treat this interrupt as a new one and not associated with a 'bounce' 
      process_interrupt(0); 
    }
    millis_previous = millis_now;    
  }
}

void generic_handler1() 
{static long unsigned int millis_previous;
  if (initialisation_complete == true) 
  { //  all variables are initialised so we are okay to continue to process this interrupt.
    //  deal with potential 'bouncing' of this interrupt.  The code will 'debounce' spurious
    //  interrupts.
    millis_now = millis();
    if (millis_now < millis_previous)
    { // millis timer has cycled since last tested, so adjust
        millis_elapsed = millis_max - millis_previous + millis_now + 1;
    }
    else
    { // millis_now is in advance of previous millis 
      millis_elapsed = millis_now - millis_previous + 1;
    }
    interrupt_debounce_value = interrupt_config_data[1][5];
    if (millis_elapsed >= interrupt_debounce_value)
    { //  treat this interrupt as a new one and not associated with a 'bounce'
      process_interrupt(1); 
    }
    millis_previous = millis_now;
  }
}
 
void generic_handler2() 
{static long unsigned int millis_previous;
  if (initialisation_complete == true) 
  { //  all variables are initialised so we are okay to continue to process this interrupt.
    //  deal with potential 'bouncing' of this interrupt.  The code will 'debounce' spurious
    //  interrupts.
    millis_now = millis();
    if (millis_now < millis_previous)
    { // millis timer has cycled since last tested, so adjust
        millis_elapsed = millis_max - millis_previous + millis_now + 1;
    }
    else
    { // millis_now is in advance of previous millis 
      millis_elapsed = millis_now - millis_previous + 1;
    }
    interrupt_debounce_value = interrupt_config_data[2][5]; 
    if (millis_elapsed >= interrupt_debounce_value)
    { //  treat this interrupt as a new one and not associated with a 'bounce' 
      process_interrupt(2);
    }
    millis_previous = millis_now;   
  }
}

void generic_handler3() 
{static long unsigned int millis_previous;
  if (initialisation_complete == true) 
  { //  all variables are initialised so we are okay to continue to process this interrupt.
    //  deal with potential 'bouncing' of this interrupt.  The code will 'debounce' spurious
    //  interrupts.
    millis_now = millis();
    if (millis_now < millis_previous)
    { // millis timer has cycled since last tested, so adjust
        millis_elapsed = millis_max - millis_previous + millis_now + 1;
    }
    else
    { // millis_now is in advance of previous millis 
      millis_elapsed = millis_now - millis_previous + 1;
    }
    interrupt_debounce_value = interrupt_config_data[3][5];    
    if (millis_elapsed >= interrupt_debounce_value)
    { //  treat this interrupt as a new one and not associated with a 'bounce' 
      process_interrupt(3); 
    }
    millis_previous = millis_now;
  }
}

void generic_handler4()
{static long unsigned int millis_previous;
  if (initialisation_complete == true) 
  { //  all variables are initialised so we are okay to continue to process this interrupt.
    //  deal with potential 'bouncing' of this interrupt.  The code will 'debounce' spurious
    //  interrupts.
    millis_now = millis();
    if (millis_now < millis_previous)
    { // millis timer has cycled since last tested, so adjust
        millis_elapsed = millis_max - millis_previous + millis_now + 1;
    }
    else
    { // millis_now is in advance of previous millis 
      millis_elapsed = millis_now - millis_previous + 1;
    }
    interrupt_debounce_value = interrupt_config_data[4][5];    
    if (millis_elapsed >= interrupt_debounce_value)
    { //  treat this interrupt as a new one and not associated with a 'bounce' 
      process_interrupt(4); 
    }
    millis_previous = millis_now;
  }
}

void generic_handler5() 
{static long unsigned int millis_previous;
  if (initialisation_complete == true) 
  { //  all variables are initialised so we are okay to continue to process this interrupt.
    //  deal with potential 'bouncing' of this interrupt.  The code will 'debounce' spurious
    //  interrupts.
    millis_now = millis();
    if (millis_now < millis_previous)
    { // millis timer has cycled since last tested, so adjust
      millis_elapsed = millis_max - millis_previous + millis_now + 1;
    }
    else
    { // millis_now is in advance of previous millis 
      millis_elapsed = millis_now - millis_previous + 1;
    } 
    interrupt_debounce_value = interrupt_config_data[5][5];
    if (millis_elapsed >= interrupt_debounce_value)
    { //  treat this interrupt as a new one and not associated with a 'bounce' 
      process_interrupt(5);
    }
    millis_previous = millis_now;
  }
}

volatile  (*interrupt_handler_addresses[max_digital_inputs])()=
{
generic_handler0,
generic_handler1,
generic_handler2,
generic_handler3,
generic_handler4,
generic_handler5
};
