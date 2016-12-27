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
  ADC_INIT,
  ADC_READ_VOLTAGE,
  ADC_READ_CURRENT,
  ADC_READ_TEMPERATURE,
  ADC_PAUSE,
}ADC_STATES;

/*
Prototypes
*/
void adc_config_voltage(void);
void adc_config_current(void);
void adc_config_temperature(void);

/*
External Functions
*/

void ADC_FSM(bool reset)
{
  static ADC_STATES state = ADC_INIT;

  if(reset)
  {
    state = ADC_INIT;
  }

  switch(state)
  {
    case ADC_INIT:
    adc_initialise();
    state = ADC_READ_VOLTAGE;
    break;

    case ADC_READ_VOLTAGE:
    eus_raw_voltage_reading = ReadVoltage();
    state = ADC_READ_CURRENT;
    break;

    case ADC_READ_CURRENT:
    eus_raw_current_reading = ReadCurrent();
    state = ADC_READ_TEMPERATURE;
    break;

    case ADC_READ_TEMPERATURE:
    eus_raw_temperature_reading = ReadTemperature();
    state = ADC_PAUSE;
    break;  

    case ADC_PAUSE:
    state = ADC_READ_VOLTAGE;
    break;
  }
}


void adc_initialise(void)
{
  struct adc_config config_adc;

  adc_get_config_defaults(&config_adc);
  config_adc.negative_input = ADC_NEGATIVE_INPUT_GND;
  config_adc.positive_input = ADC_POSITIVE_INPUT_PIN19;
  adc_init(&adc_instance, ADC, &config_adc);
  adc_enable(&adc_instance);
}

U16 ReadVoltage(void)
{
  U16 result;
  adc_config_voltage();
  while(adc_read(&adc_instance, &result) == STATUS_BUSY) {;}
  return result;
}

U16 ReadCurrent(void)
{
  U16 result;
  adc_config_current();
  while(adc_read(&adc_instance, &result) == STATUS_BUSY) {;}
  return result;
}

U16 ReadTemperature(void)
{
  U16 result;
  adc_config_temperature();
  while(adc_read(&adc_instance, &result) == STATUS_BUSY) {;}
  return result;
}

/*
Local Functions
*/

void adc_config_voltage(void)
{
  adc_set_positive_input(&adc_instance, ADC_POSITIVE_INPUT_PIN19);
}


void adc_config_current(void)
{
  adc_set_positive_input(&adc_instance, ADC_POSITIVE_INPUT_PIN17);
}


void adc_config_temperature(void)
{
  adc_set_positive_input(&adc_instance, ADC_POSITIVE_INPUT_PIN18);
}

