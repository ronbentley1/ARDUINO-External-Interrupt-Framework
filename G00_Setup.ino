// Copyright (c) Ron D Bentley (UK), see copyright notice
//
void setup() {
  int input, interrupt_pin, pinmode_type, trigger_type;
  
  initialisation_complete = false;  //  ensures no interrupts are processed before ready

  //  set up the free chain for interrupt queue handling
  start_of_IQ_chain       =  -1;    //  start with no interrupt blocks in IQ chain
  end_of_IQ_chain         =  -1;    //  start with no interrupt blocks in IQ chain
  num_IQ_interrupts       =   0;    //  start with no interrupt blocks in IQ chain
  create_IQ_free_chain();           //  set up the free chain of IQ blocks

  if (diags_on)
  {
    Serial.begin(115200);
  }

  //  establish the interrupt handler routines to the defined input pins
  //  and associated parameter values.

  for (input=0; input < max_digital_inputs; input++)
  {
    if (interrupt_config_data[input][0] == active)
    { // This interrupt and associated pin is active, so set up structures for it
      interrupt_pin     =interrupt_config_data[input][1];
      pinmode_type      =interrupt_config_data[input][2];
      trigger_type      =interrupt_config_data[input][3];
      pinMode(interrupt_pin,pinmode_type);
      attachInterrupt(digitalPinToInterrupt(interrupt_pin),
                      interrupt_handler_addresses[input],
                      trigger_type);
    }
  }
  //
  //  Place any other set up code here, but BEFORE 'initialisation_complete = true'
  //

  //
  //  End of user inserted set up code
  //
  initialisation_complete = true;   // can now allow interrupts to be processed
}
