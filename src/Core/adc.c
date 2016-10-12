/*
* adc.c
*
* Created: 2016/10/11 10:02:08 PM
*  Author: philb
*/ 

#define ADC_C_

#include "../system.h"

/*
Prototypes
*/
void adc_config_voltage(void);
void adc_config_current(void);
void adc_config_temperature(void);

/*
External Functions
*/

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

