/*
 * hmi.h
 *
 * Created: 2016/12/20 1:07:34 PM
 *  Author: Philip
 */ 


#ifndef HMI_H_
#define HMI_H_


S16 ess_voltage_encoder_val;
S16 ess_current_encoder_val;


extern void HMI_FSM(bool reset);

#endif /* HMI_H_ */