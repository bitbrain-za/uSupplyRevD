/*
 * hmi.c
 *
 * Created: 2016/12/20 1:07:42 PM
 *  Author: Philip
 */ 

#include "../system.h"

typedef enum
{
  HMI_IDLE,
  TRIGGER_BUTTON,

}HMI_STATE;



bool b_check_current_encoder(void);
bool b_check_voltage_encoder(void);

HMI_STATE gx_state;

void HMI_FSM(bool reset)
{
  if(reset)
  {
    ess_current_encoder_val = 0;
    ess_voltage_encoder_val = 0; 
    gx_state = HMI_IDLE;
  }

  switch(gx_state)
  {
    case HMI_IDLE:
    /* Check Encoders */
    b_check_current_encoder();
    b_check_voltage_encoder();
    break;
  }
}

/* Local Functions */
bool b_check_current_encoder(void)
{
  if(!port_pin_get_input_level(ENC2A))
  {
    if(!port_pin_get_input_level(ENC2B))
    {
      /* Anti Clockwise */
      if(ess_current_encoder_val != CURRENT_ENCODER_MIN)
        ess_current_encoder_val--;
    }
    else
    {
      /* Clockwise */
      if(ess_current_encoder_val != CURRENT_ENCODER_MAX)
        ess_current_encoder_val++;
    }
    return true;
  }
  return false;
}

bool b_check_voltage_encoder(void)
{
  if(!port_pin_get_input_level(ENC1A))
  {
    if(!port_pin_get_input_level(ENC1B))
    {
      /* Anti Clockwise */
      if(ess_voltage_encoder_val != VOLTAGE_ENCODER_MIN)
        ess_voltage_encoder_val--;
    }
    else
    {
      /* Clockwise */
      if(ess_voltage_encoder_val != VOLTAGE_ENCODER_MAX)
        ess_voltage_encoder_val++;
    }
    return true;
  }
  return false;
}