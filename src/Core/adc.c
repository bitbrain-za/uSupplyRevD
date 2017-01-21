/*
* adc.c
*
* Created: 2016/10/11 10:02:08 PM
*  Author: philb
*/ 

#define ADC_C_

#include "../system.h"

typedef enum
{
  ADC_CONFIGURE_FOR_VOLTAGE,
  ADC_READ_VOLTAGE,
  ADC_CONFIGURE_FOR_CURRENT,
  ADC_READ_CURRENT,
  ADC_CONFIGURE_FOR_TEMPERATURE,
  ADC_READ_TEMPERATURE,
  ADC_PAUSE,
}ADC_STATES;

struct adc_module adc_instance;

/*
Prototypes
*/
void adc_initialise(void);
S16 ss_interpret_temperature(U16 raw_value);
/*
External Functions
*/

void ADC_FSM(bool reset)
{
  static ADC_STATES state;
  static TIMER_HANDLE adc_timer;
  SYS_MESSAGE msg;
  msg.source = SRC_ADC;

  if(reset)
  {
    adc_timer = timer_new(1000);
    adc_initialise();
    state = ADC_CONFIGURE_FOR_VOLTAGE;
    return;
  }

  switch(state)
  {
    case ADC_CONFIGURE_FOR_VOLTAGE:
    adc_reset(&adc_instance);
    adc_initialise();

    adc_set_positive_input(&adc_instance, ADC_VOLTAGE_SENSE);
    adc_start_conversion(&adc_instance);
    state = ADC_READ_VOLTAGE;
    break;

    case ADC_READ_VOLTAGE:
    while(adc_read(&adc_instance, &msg.value) == STATUS_BUSY) {;}
    b_queue_send(&queue_voltage_control, &msg);
    state = ADC_CONFIGURE_FOR_TEMPERATURE;
    break;

    case ADC_CONFIGURE_FOR_TEMPERATURE:
    adc_set_positive_input(&adc_instance, ADC_TEMPERATURE_SENSE);
    adc_start_conversion(&adc_instance);
    state = ADC_READ_TEMPERATURE;
    break;

    case ADC_READ_TEMPERATURE:
    while(adc_read(&adc_instance, &eus_temperature) == STATUS_BUSY) {;}
    state = ADC_CONFIGURE_FOR_CURRENT;
    break;  

    case ADC_CONFIGURE_FOR_CURRENT:
    adc_set_positive_input(&adc_instance, ADC_CURRENT_SENSE);
    adc_start_conversion(&adc_instance);
    state = ADC_READ_CURRENT;
    break;

    case ADC_READ_CURRENT:
    while(adc_read(&adc_instance, &msg.value) == STATUS_BUSY) {;}
    b_queue_send(&queue_current_control, &msg);    
    state = ADC_PAUSE;
    timer_reset(adc_timer);
    break;

    case ADC_PAUSE:
    if(timer_expired(adc_timer))
      state = ADC_CONFIGURE_FOR_VOLTAGE;
    break;
  }
}


void adc_initialise(void)
{
  struct adc_config config_adc;

  adc_get_config_defaults(&config_adc);
  config_adc.reference = ADC_REFERENCE_INTVCC0;
//  config_adc.reference = ADC_REFERENCE_INT1V;
  config_adc.clock_source = GCLK_GENERATOR_3;
  config_adc.negative_input = ADC_NEGATIVE_INPUT_GND;
  config_adc.positive_input = ADC_CURRENT_SENSE;
  config_adc.reference_compensation_enable = false;
  adc_init(&adc_instance, ADC, &config_adc);
  adc_enable(&adc_instance);
}

S16 ss_interpret_temperature(U16 raw_value)
{
    /* ADC = Vin*TOP/Vref 
     
     Vin = ADC * VREF / TOP

     k = VREF(mV) / TOP = 3300 / ( 1.48 * 4095 )
     
     Vin = Temp(10mV/Deg) + 500 mV 

     Temp(10mV/deg) = (Vin - 500mV)

     Temp = ((ADC * k) - 500) / 10
     
         */
  
  /* Vin in mV */
  U32 temp = raw_value * 2230;  
  temp /= 4095;

  /* Subtract 500 mV */
  S16 temperature = (temp - 500) / 10;
  return temperature;

}