/*
 * hmi.h
 *
 * Created: 2016/12/20 1:07:34 PM
 *  Author: Philip
 */ 


#ifndef HMI_H_
#define HMI_H_

typedef enum
{
  HMI_ENC_V,
  HMI_ENC_C,
  HMI_BUTTON1,
  HMI_BUTTON2,
  HMI_BUTTON3,
  HMI_BUTTON4,
  HMI_BUTTON5,
  HMI_BUTTON6,
}HMI_INPUT;

typedef struct  
{
  HMI_INPUT source;
  U16       value;
}HMI_MESSAGE;

U16 eus_voltage_encoder_val;
U16 eus_current_encoder_val;

Queue queue_HMI_input;

extern void HMI_FSM(bool reset);

#endif /* HMI_H_ */