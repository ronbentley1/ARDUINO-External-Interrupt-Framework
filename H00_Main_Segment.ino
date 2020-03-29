
void loop() 
{int  interrupt;
/*  The following section of code will allow interrupts to be processed at each loop cycle
    Add whatever other code for the specific application required to process the interrupts received plus
    any other requirements of a noninterrupt nature.

    Design your code structure as necessary, but note that to obtain interrupts
    from the interrupt queue it is necessary to call the routine 'scanIQ()'.  This will examine
    the interrupt queue and if an unprocessed interrupts exists the result of the call 
    returns the generic interrupt and and associated interrupt dat that triggered the interrupt,
    if available.
    
    For example, 'interrupt=scanIQ();'.
    Results of this call will be:
    1.  an interrupt has been obtained from the IQ.  In which case the following varables 
        will be set up: 
          int_number    - actual real world interrupt number
          int_pin       - actual digital pin number triggering the interrupt
          int_pinmode   - pinMode value used to set up this digital interrupt pin
          int_trigger   - interrupt trigger value used to set up this digital interrupt
          int_unique    - flag that defines if interrupt processing is unique or nonunique
          int_debounce  - debounce value in msecs for this interrupt
    2.  if no interrupt is returned (none in the IQ) then all of the above variables will be set
        to 'no_interrupt_request'.
*/

do { // Keep processing interrupts whilst there are interrupts in the queue..
    interrupt = scan_IQ();  //  get the next interrupt in IQ if there is one.
     if (interrupt != no_interrupt_request)
      { /*  Process this interrupt request. 'interrupt' defines the 
            generic interrupt number that triggered.  Other variables (see above) give all
            other interrupt atrributes if required 
            Insert whatever code appropriate here, if any, 
            when not processing an interrupt request  */

      }
    }
  while (interrupt != no_interrupt_request);
  /*  No interrupts left in the queue, so do other things....
      Insert whatever code appropriate here, if any, 
      when not processing an interrupt request. */



}
