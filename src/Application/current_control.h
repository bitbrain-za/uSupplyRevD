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
extern U16 eus_output_current_ua;
extern U16 eus_ram_current_setting;

#ifdef CURRENT_CONTROL_C_
  #undef extern
#endif

void v_current_FSM(bool reset);

#endif /* CURRENT_CONTROL_H_ */