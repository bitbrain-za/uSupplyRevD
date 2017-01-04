/*
 * hmi.c
 *
 * Created: 2016/12/20 1:07:42 PM
 *  Author: Philip
 */ 

#include "../system.h"

#define VOLTAGE_STEP  50
#define VOLTAGE_MAX   12000
#define VOLTAGE_MIN   0
#define CURRENT_STEP  50
#define CURRENT_MAX   1000
#define CURRENT_MIN   0

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
  HMI_MESSAGE tx_msg;

  if(reset)
  {
    b_queue_init(&queue_HMI_input, sizeof(HMI_MESSAGE), 5);
    eus_current_encoder_val = 0;
    eus_voltage_encoder_val = 0; 
    gx_state = HMI_IDLE;
  }

  switch(gx_state)
  {
    case HMI_IDLE:
    /* Check Encoders */
    if(b_check_current_encoder())
    {
      tx_msg.source = HMI_ENC_C;
      tx_msg.value = eus_current_encoder_val;
      b_queue_send(&queue_HMI_input, &tx_msg);
    }
    if(b_check_voltage_encoder())
    {
      tx_msg.source = HMI_ENC_V;
      tx_msg.value = eus_voltage_encoder_val;
      b_queue_send(&queue_HMI_input, &tx_msg);
    }
    break;
  }
}

/* Local Functions */
bool b_check_current_encoder(void)
{
  if(!port_pin_get_input_level(ENC2A))
  {
    if(port_pin_get_input_level(ENC2B))
    {
      /* Anti Clockwise */
      if(CURRENT_MIN < eus_current_encoder_val)
      {
        eus_current_encoder_val -= CURRENT_STEP;
        if(CURRENT_MIN > eus_current_encoder_val)
          eus_current_encoder_val = CURRENT_MIN;
      }
    }
    else
    {
      /* Clockwise */
      if(CURRENT_MAX > eus_current_encoder_val)
      {
        eus_current_encoder_val += CURRENT_STEP;
        if(CURRENT_MAX < eus_current_encoder_val)
          eus_current_encoder_val = CURRENT_MAX;
      }
    }
    while(!port_pin_get_input_level(ENC2A)) { ; }

    return true;
  }
  return false;
}

bool b_check_voltage_encoder(void)
{
  if(!port_pin_get_input_level(ENC1A))
  {
    if(port_pin_get_input_level(ENC1B))
    {
      /* Anti Clockwise */
      if(VOLTAGE_MIN < eus_voltage_encoder_val)
      {
        eus_voltage_encoder_val -= VOLTAGE_STEP;
        if(eus_voltage_encoder_val < VOLTAGE_MIN)
          eus_voltage_encoder_val = VOLTAGE_MIN;
      }
    }
    else
    {
      /* Clockwise */
      if(VOLTAGE_MAX > eus_voltage_encoder_val)
      {
        eus_voltage_encoder_val += VOLTAGE_STEP;
        if(VOLTAGE_MAX < eus_voltage_encoder_val)
          eus_voltage_encoder_val = VOLTAGE_MAX;
      }
    }
    while(!port_pin_get_input_level(ENC1A)) { ; }
    return true;
  }
  return false;
}