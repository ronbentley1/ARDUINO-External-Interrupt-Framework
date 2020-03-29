# ARDUINO-Interrupt-Framework
A repository for haolding end users code and guides for implementing ARDUINO Interrupt Framework

 * This is a framework that can be used to manage and process any number of external
 * interrupts, from 1 up to the maximum number supported by the ARDUINO microcontroller.
 *
 * It works by using a queue into which triggered interrupts are placed, ready for
 * processing outside the interrupt process by main code. 
 *
 * Note that interrupts are queued, that is, they are placed in the queue on a last in last out
 * basis and, conversely, they are taken out of the queue on a first in, first out basis (FIFO).  
 * That is, on the basis of the order in which they have been triggered.
 *
 * The aproach/method assumes nothing about the overlaying end user application,
 * and how each interrupt is procssed is entirely down to the needs of the solution sought. 
 * 
 * The framework has been initially written for the MEGA 2560 board, which allows 
 * up to 6 separate external interrupts, these being on digital pins 2, 3, 18, 19, 20 and 21.
 * 
 * The framework is flexible through editing of the interrupt control data defined in the 
 * array/table 'interrupt_config_data', such that:
 
 * 1. the number of interrupts enabled (made 'active'/'inactive') may be adjusted from 1 up 
 *    to 6 (the maximum external interrupts permissible on the MEGA 2560).
 
 * 2. For each digital input interrupt pin, it is possible to define a number of parameters that steer
 *    how the interrupt will be dealt with and also how it is associated with the priority defined
 *    by the microcontroller specifications.
 *    
 *    This flexibility is achieved through the use of a number of confiurable parameters which can  
 *    be defined within the 'C00_Configurations' tab.
 *    
 * 
 */
