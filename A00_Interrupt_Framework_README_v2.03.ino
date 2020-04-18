/* (External) Interrupt Queue Framwork, Ron D Bentley (Stafford UK)
 * ____________________________________________________________________________________________________
 * Copyright (c) Ron D Bentley (UK)
 * The extent this licence shall be limited to Non-profit Use.
 * Permission is hereby granted, free of charge, for non-profit purposes to any person obtaining
 * a copy of this software and associated documentation files (the "Software"), to deal in the 
 * Software without restriction, including without limitation the rights to use, copy, modify, merge, 
 * publish, distribute, sublicense copies of the Software, and to permit persons to whom the Software 
 * is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice, acknowledgements and this permission notice shall be included in all 
 * copies or substantial portions of the Software. 
 * 
 * THE SOFTWARE IS LIMITED TO NON-PROFIT USE AND IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR 
 * A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * ____________________________________________________________________________________________________
 * 
 * Version History    _________________________________________________________________________________
 *
 *18 Apr 2020:v2.03.1 Point release - corrected '#define millis_max' in E10_Interrupt Handlers tab, which  
 *                    was incorrectly set for 64 bit operation.  Corect value is 0xffffffff, which
 *                    refects 32 bit long unsigned integer architecture.
 * 4 Mar  2020:v2.03  Efficiency improvements regarding scanning IQ for existing unique interrupts.  New                    
 *                    approach no longer scans chain, but uses binary flags, one for each generic interrupt.
 *                    These are set/cleared only if a generic interrupt handler is configured for unique 
 *                    interrupts. Note, the bit set/cleared refers to the number of the generic interrupt 
 *                    handler and not the actual linked interrupt that triggered on the assigned generic 
 *                    handler.
 *  1 Mar  2020:v2.02 Introduced concept of 'generic' interrupt ISRs to delink actual dgital interrupts 
 *                    pins from a particular ISR.
 * 27 Feb 2020:v2.01  Further extension to the interrupt channel data structure ('interrupt_config_data')
 *                    to provide greater flexiblity per digital interrupt input.  
 *                    This version now allows each interrupt to be treated individually for 
 *                    unique/nonunique interrupts plus independent interrupt debounce.
 * 24 Feb 2020:v2.00  Modified to consolidate the interrupt channel data into a more helpful                    
 *                    structure for end users to change, as required.
 * 23 Feb 2020:v1.04  Introduction of further flexibility by addition of modifiable variables
 *                    to allow pinMode input types and attachInterrupt trigger types.
 * 22 Feb 2020:v1.03  Modifications to deal with interrupt 'bounce'. 
 * 22 Feb 2020:v1.02  Layout work to program structure, including end user configuration tab
 * 20 Feb 2020:v1.01  Extensions to allow unique/nonunique interrupt handling. 
 * 13 Feb 2020:v1.00  Initial version, concepts and development.
 * ___________________________________________________________________________________________________
 * 
 * README..._________________________________________________________________________________________
 * This is a framework that can be used to manage and process any number of external
 * interrupts, from 1 up to the maximum number support by the microcontroller.
 * It works by using a queue into which triggered interrupts are placed, ready for
 * processing outside the interrupt process by main code. 
 * 
 * Note that interrupts are queued, that is, they are placed in the queue on a last in last out
 * basis and, conversely, they are taken out of the queue on a first in, first out basis (FIFO).  
 * That is, on the basis of the order in which they have been triggered.
 * 
 * The aproach/method assumes nothing about the overlaying end user appliaction,
 * and how each interrupt is procssed is entirely down to the needs of the solution sought. 
 * 
 * The framework has been initially written for the MEGA 2560 board, which allows 
 * up to 6 separate external interrupts, these being on digital pins 2, 3, 18, 19, 20 and 21.
 * 
 * The framework is flexible in through editing of the interrupt control data defined in the 
 * array/table 'interrupt_config_data', such that:
 * 1. the number of interrupts enabled (made 'active'/'inactive') may be adjusted from 1 up 
 *    to 6 (the maximum external interrupts permissible on the MEGA 2560).
 *    
 * 2. For each digital input interrupt pin, it is possible to define a number of parameters that steer
 *    how the interrupt will be dealt with and also how it is associated with the priority defined
 *    by the microcontroller specifications.
 *    
 *    This flexibility is achieved through the use of a number of configurable parameters which can  
 *    be defined within the 'C00_Configurations' tab (see below).
 *    
 * Configurable parameters:
 * A. Assignimg parameters to each digital interrupt:
 *    In the 'C00_Configurations' tab, the data declaration 'interrupt_config_data' contains all of the 
 *    variable parameters that define how each external interrupt should be handled/processed.  The attributes
 *    define by this declaration are as follows:
 *
 *    'interrupt_config_data' Array/table row values (by column):
 *    For each row (external interrupt), decide how each external interrupt is to be configured:
 *      active/inactive flag        - set the value to an 'active' or inactive state.  
 *                                    If 'inactive' no interrupts will be processed for this externl interrupt.
 *      digial pin number           - set the digital pin number associated with this external interrupt.                              
 *                                    Refer to the board documentation if using a micrcontroller otherthan the MEGA 2560.
 *      pinMode mode value          - set the required pinMode mode value, INPUT, INPUT_PULLUP, etc for the defined                               
 *                                    interrupt pin.
 *      trigger type                - set the interrupt trigger type for this interrupt, eg Falling, Change, Rising, etc.             
 *      unique/nonunique processing - set to 'true' if unique processing required, 'false' otherwise'.
 *                                    If set to 'true' then only one interrupt of the same number will be held in the 
 *                                    interrupt queue at a time.  Otherwise, the interrupt queue may contain any number of
 *                                    interrupts, up to the defined size of the interrupt queue free chain.
 *      debounce value              - this is the time that interrupt handler for this external interrupt will wait between 
 *                                    interrupts before deciding that the interrupt can be processed as a new interrupt event.                                      
 *                                    The value is in milliseconds.
 *      interrupt number            - this parameter defines the externa interrupt number assigned by the microcontroller                                   
 *                                    to the associated digital interrupt pin.
 *    
 * B. Changing the size of the inetrrupt queue (IQ):
 *    In the 'C00_Configurations' tab: 
 *    a.  If configuring for all unique interrupt processing set the defined value 'max_IQ_free_chain_blocks'
 *        to be the same value as the number of active digital interrupts, ie 'max_digital_inputs'.
 *    b). If configuring for nonunique interrupt prcessing then consider the following:
 *        i)  The defined value 'max_IQ_free_chain_blocks' needs to be set large enough to handle the
 *            maximum number of interrupts likley to be in the queue at any one time (before it is processed
 *            by 'scan_IQ()'). Otherwise, there may be no space for triggered inetrrupts in the queue.
 *        ii) The interrupt handlers do have debounce coding to deal with spurious interrupts, but do 
 *            ensure that the preset value defined by 'interrupt_debounce' is suitable to ensure that 
 *            genuine interrupts are not missed.  The number given by the define value is in mlliseconds (msecs).
 *            
 * C. Diagnostics are defined in the tab 'D00 Diags'.  Use these routines or add your own.
 *    Note that: 
 *    1.  diagnostics may be turned on/off at compile time by setting the boolean variable
 *        'diags_on' (in the 'D00 Diags' tab) to 'true' for diagnostics and 'false' for no diagnostics.
 *    2.  if using the print queue routines provided than these will disable interrupts whilst they are printed
 *        and re-enable then afterwards.  This ensures no conflict if interrrupts are received during the print cycle.
 *        
 *    
 * 
 */
