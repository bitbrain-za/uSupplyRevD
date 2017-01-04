/*
 * current_control.h
 *
 * Created: 2017/01/04 10:55:52 AM
 *  Author: Philip
 */ 


#ifndef CURRENT_CONTROL_H_
#define CURRENT_CONTROL_H_


#ifdef CURRENT_CONTROL_C_
  #define extern
#endif

extern Queue queue_current_control;
extern U16 eus_output_current;

#ifdef CURRENT_CONTROL_C_
  #undef extern
#endif



#endif /* CURRENT_CONTROL_H_ */