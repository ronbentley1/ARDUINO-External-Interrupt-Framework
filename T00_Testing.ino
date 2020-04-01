// Copyright (c) Ron D Bentley (UK), see copyright notice
//
/* 
  * Version 2.03 Testing Strategy:
  * 1.  To test the correct setting and clearing of generic interrupt flags were these are configured 
  *     as for 'unique' entries in the IQ.
  * 2.  To rerun the soak performnce testing run under version 2.02 and to record tiings for 
  *     a.  averge IQ inserts
  *     b.  averge IQ extracts.
  *     
  * The results obtained showed that: 
  *   i.    the average time to insert an interrupt into the interrupt queue is 15 microseconds,
  *   ii.   the average time to remove an interrupt from the interrupt queue is 12 microseconds.
  *   
  * Observation - whilst the changes did not make any signifant performance improvement, it did result
  * in maginally less code generation.
*/
