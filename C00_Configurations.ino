//  This tab contains the parameters available to configure the interrupt handlers and associated
//  supporting structures.

bool  diags_on = true;  //  set to 'true' for diagnostics, otherwise 'false' for none

#define active    1
#define inactive  -1

//  **** Interrupt configurable varables/values ****
#define max_digital_inputs  6   //  Max digital interrupts pins available on Mega 2650 board.
                                //  The number of pins to be interrupt monitored.
                                //  DO NOT  vary this number, simply change active flag and associated
                                //  interrupt data, even if using with a different board.

// Note that the array/table 'interrupt_config_data' is sized for the maximum number of digital
// interrupt pins for the Mega 2560.  If selecting fewer interrupt pins, simple deactivate the
// lines not required by setting column 0 value t0 'inactive'

volatile int interrupt_config_data[max_digital_inputs][7]=
{
active,     21,     INPUT_PULLUP,   RISING,     false,           15,           0, // generic interrupt handler 0 entries
active,     20,     INPUT_PULLUP,   RISING,     false,           15,           1, // generic interrupt handler 1 entries
active,     19,     INPUT_PULLUP,   RISING,     false,           15,           2, // generic interrupt handler 2 entries
active,     18,     INPUT_PULLUP,   RISING,     false,           15,           3, // generic interrupt handler 3 entries
active,      2,     INPUT_PULLUP,   RISING,     false,           15,           4, // generic interrupt handler 4 entries
active,      3,     INPUT_PULLUP,   RISING,     false,           15,           5};// generic interrupt handler 5 entries
/*  ^        ^             ^           ^           ^              ^            ^
    |        |             |           |           |              |            |
 active    digital      pinMode       interrupt  unique/      interrupt     interrupt number
inactive  interrupt    mode value      trigger  nonunique     debounce        this pin is 
 flag      pin no.                      type      flag      value (msecs)      linked to

*/
//  ****Interrupt Queue configurable values ****
#define max_IQ_free_chain_blocks 32       //  This defines the number of free blocks in the chain.
                                          //  At least the maximum number of interrupts being monitored,
                                          //  if all interrupts are defined as unique.
                                          //  Must be > than this value,  if nonunique interrupts are being allowed
