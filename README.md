(User Guide) Introduction

This document presents an asynchronous method for a scalable framework for applications requiring the use of external interrupts for ARDUINO microcontrollers (or compatible alternatives, e.g. Elegoo).  

The method will allow external interrupts to be processed in their order of priority (when triggered concurrently and in real time), but at such time that the main processing code dictates (asynchronously).  That is, the method allows for external interrupts to be captured, as triggered, but actioned outside and away from their associated Interrupt Service Routines (ISRs).
The solution has been developed around the ARDUINO MEGA 2560 microcontroller, but it may be modified for any board in a straight forward way for any number of external interrupts up to the maximum limit set by the specific microcontroller and this framework (see Configuration).

The MEGA 2560 supports up to six external interrupts that are readily accessible to end user developers.  These interrupts are mapped to digital input pins; these assignments can be seen at Appendix A.  However, the method is easily scaled to use one, two, three,...,or all six external interrupts concurrently, making it an ideal framework on which to base solutions requiring multiple external interrupt processes and processing.  For microcontrollers that support more than six interrupts an extension to the underlying framework would be needed.  However, it is considered that a limit of six interrupts should not be a limiting factor for most applications likely to be developed.  If more are required then the method could be extended by increasing the number of generic ISRs and associated data support structures.

Asynchronous processing is achieved through the design and implementation of an interrupt queue management concept which provides the means to separate the event of an interrupt from its ultimate processing.  

When an external interrupt is triggered, it is presented the nominated (generic) ISR mapped and established within the setup() process.  ISRs will then do little more than insert the interrupt event into the interrupt queue with no further process taking place at this point.  However, to avoid ‘bounce’ from spurious/dirty interrupts, each ISR incorporates a debounce mechanism which is variable through configuration parameters.  Interrupt ‘bounce’ values may defined differently for each mapped external interrupt, as necessary.  

If several external interrupts are triggered at the same time then the ARDUINO microcontroller will ‘fire’ their respective ISRs in the defined priority order as determined by the microcontroller specifications (see Appendix A for the MEGA 2560 priorities).

The interrupt queue is operated and managed on a first in, first out basis (FIFO).  The approach therefore preserves the order in which external interrupts are triggered in the real world.

Following configuration, to meet a specific solution, end users have the ability to add their own code into the ‘setup()’ and ‘main()’ segments, as required.  The only stipulation here is that, to deal with external interrupts, user added code must utilise a specific ‘get interrupt’ process (i.e. ‘scan_IQ()’) to progressively action interrupts from the interrupt queue.  This function will either return

   1.	no interrupts in the interrupt queue, or
   2.	the value of the generic interrupt handler that was triggered (0, ... ,5) (see below for an explanation of generic interrupt handlers).  Additionally, a  number of variables are set with all of the parameters defined in the configuration data for the respective external interrupt. 

There are no other demands placed on the end user solution.

This interrupt framework is intended for use by anyone with an interest in ARDUINO development requiring external interrupts - novices and experts alike.  The starting point for its use is to thoroughly understand what it is you want to do with external interrupts.  That is, to understand:

   a). 	why interrupts should be a part of your solution and, if so, 
   b). 	individually, how these should be configured for the solution

It is essential for a successful implementation of this framework that due regard and is time spent analysing the specific requirements for external interrupts and then to configure the framework suitably. 


