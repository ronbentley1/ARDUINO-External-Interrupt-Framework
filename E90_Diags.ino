// Copyright (c) Ron D Bentley (UK), see copyright notice
//
//  Insert any diagnostic routines in this tab, and use 'diags_on' to switch
//  them on/off (true/false)
//  Uses 'initialisation_complete' flag to ensure external interrupts will not
//  corrupt the printing process. 
//

void print_IQ()
{int ptr, count;
  if (diags_on)
  {
    noInterrupts();
    initialisation_complete = false;    // block interrupt processing
    interrupts(); 
    count = num_IQ_interrupts;
    ptr = start_of_IQ_chain;
    Serial.println("_______________________________");
    Serial.println("       INTERRUPT QUEUE");   
    Serial.print("          Num in IQ = ");
    Serial.println(count);
    Serial.print(" start of IQ chain = ");
    Serial.println(ptr);
    Serial.print("   end of IQ chain = ");
    Serial.println(end_of_IQ_chain);
    if (count > 0)
    {
    do
    {
      Serial.print("IQ[");
      Serial.print(ptr);
      Serial.print("][0] = ");
      Serial.print(IQ[ptr][0]);
      Serial.print(char(9));
      Serial.print("IQ[");
      Serial.print(ptr);
      Serial.print("][1] = ");
      Serial.println(IQ[ptr][1]);
      ptr = IQ[ptr][0]; //  look at next entry/block
      count --;
    }
    while ( (ptr != end_of_chain_value) && (count > 0));
    }
    Serial.println("");
    Serial.println("_______________________________");
    Serial.println("");
    Serial.flush();
    noInterrupts();
    initialisation_complete = true;   // can now allow interrupts to be processed
    interrupts();
  }
}

void print_free_chain()
{int ptr, count;
if (diags_on)
  {
    noInterrupts();
    initialisation_complete = false;   // block interrupt processing
    interrupts();
    count = num_free_IQ_blocks;
    ptr = start_of_free_IQ_chain;
    Serial.println("");
    Serial.println("_______________________________");
    Serial.println("         FREE CHAIN"); 
    Serial.print("  Num in free chain = ");
    Serial.println(count);
    Serial.print("start of free chain = ");
    Serial.println(ptr);
    if (count > 0)
    {
    do
    {
      Serial.print("IQ[");
      Serial.print(ptr);
      Serial.print("][0] = ");
      Serial.print(IQ[ptr][0]);
      Serial.print(char(9));
      Serial.print("IQ[");
      Serial.print(ptr);
      Serial.print("][1] = ");
      Serial.println(IQ[ptr][1]);
      ptr = IQ[ptr][0]; //  look at next entry/block
      count --;
    }
    while ( (ptr != end_of_chain_value) && (count > 0));
    }
    Serial.println("_______________________________");
    Serial.println("");
    Serial.flush();
    noInterrupts();
    initialisation_complete = true;   // can now allow interrupts to be processed
    interrupts();
  }
}
