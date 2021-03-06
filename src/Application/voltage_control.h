/*
 * voltage_control.h
 *
 * Created: 2017/01/04 10:55:30 AM
 *  Author: Philip
 */ 


#ifndef VOLTAGE_CONTROL_H_
#define VOLTAGE_CONTROL_H_

#ifdef VOLTAGE_CONTROL_C_
  #define extern
#endif

extern U16 eus_ram_voltage_setting;
extern Queue queue_voltage_control;
extern U16 eus_output_voltage;

#ifdef VOLTAGE_CONTROL_C_
  #undef extern
#endif

extern void v_voltage_control_FSM(bool reset);

#endif /* VOLTAGE_CONTROL_H_ */