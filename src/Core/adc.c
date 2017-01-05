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
  ADC_READ_VOLTAGE,
  ADC_READ_CURRENT,
  ADC_READ_TEMPERATURE,
  ADC_PAUSE,
}ADC_STATES;

struct adc_module adc_instance;

/*
Prototypes
*/
void adc_initialise(void);
U16 ReadVoltage(void);
U16 ReadCurrent(void);
U16 ReadTemperature(void);
S16 ss_interpret_temperature(U16 raw_value);
/*
External Functions
*/

void ADC_FSM(bool reset)
{
  static ADC_STATES state;
  static TIMER_HANDLE adc_timer;
  SYS_MESSAGE msg;

  if(reset)
  {
    adc_timer = timer_new(400);
    adc_initialise();
    state = ADC_READ_TEMPERATURE;
    return;
  }

  switch(state)
  {
    case ADC_READ_VOLTAGE:
    msg.source = SRC_ADC;
    msg.value = ReadVoltage();
    b_queue_send(&queue_voltage_control, &msg);
    state = ADC_READ_CURRENT;
    break;

    case ADC_READ_CURRENT:
    msg.source = SRC_ADC;
    msg.value = ReadCurrent();
    b_queue_send(&queue_current_control, &msg);    
    state = ADC_READ_TEMPERATURE;
    break;

    case ADC_READ_TEMPERATURE:
    eus_temperature = ss_interpret_temperature(ReadTemperature());
    state = ADC_PAUSE;
    timer_reset(adc_timer);

    break;  

    case ADC_PAUSE:
    if(timer_expired(adc_timer))
      state = ADC_READ_VOLTAGE;
    break;
  }
}


void adc_initialise(void)
{
  struct adc_config config_adc;

  adc_get_config_defaults(&config_adc);
  config_adc.reference = ADC_REFERENCE_INTVCC0;
  config_adc.clock_source = GCLK_GENERATOR_3;
  config_adc.negative_input = ADC_NEGATIVE_INPUT_GND;
  config_adc.positive_input = ADC_TEMPERATURE_SENSE;
  config_adc.reference_compensation_enable = true;
  adc_init(&adc_instance, ADC, &config_adc);
  adc_enable(&adc_instance);
}

U16 ReadVoltage(void)
{
  U16 result;
  adc_set_positive_input(&adc_instance, ADC_VOLTAGE_SENSE);
	adc_start_conversion(&adc_instance);
  while(adc_read(&adc_instance, &result) == STATUS_BUSY) {;}
  return result;
}

U16 ReadCurrent(void)
{
  U16 result;
  adc_set_positive_input(&adc_instance, ADC_CURRENT_SENSE);
	adc_start_conversion(&adc_instance);
  while(adc_read(&adc_instance, &result) == STATUS_BUSY) {;}
  return result;
}

U16 ReadTemperature(void)
{
  U16 result;
  adc_set_positive_input(&adc_instance, ADC_TEMPERATURE_SENSE);
	adc_start_conversion(&adc_instance);
  while(adc_read(&adc_instance, &result) == STATUS_BUSY) {;}
  return result;
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