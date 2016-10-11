/*
 * adc.c
 *
 * Created: 2016/10/11 10:02:08 PM
 *  Author: philb
 */ 

 #include <asf.h>

 adc_negative_input negative_input;
 adc_positive_input adc_voltage_out = ADC_POSITIVE_INPUT_PIN19;
 adc_positive_input adc_temp_sense = ADC_POSITIVE_INPUT_PIN18;
 adc_positive_input adc_current_sense = ADC_POSITIVE_INPUT_PIN17;

 struct adc_config config_adc;

 /*
 External Functions
 */

 void adc_init(void)
 {
  adc_get_config_defaults(&config_adc);
  config_adc.negative_input = negative_input;
  config_adc.positive_input = adc_voltage_out;
  adc_init(&adc_instance, ADC, &config_adc);
  adc_enable(&adc_instance);
 }