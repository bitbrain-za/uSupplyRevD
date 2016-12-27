/*
 * adc.h
 *
 * Created: 2016/10/11 10:02:19 PM
 *  Author: philb
 */ 


#ifndef ADC_H_
#define ADC_H_

#ifdef ADC_C_
  #define extern
#endif

extern struct adc_module adc_instance;

extern U16 eus_raw_voltage_reading;
extern U16 eus_raw_current_reading;
extern U16 eus_raw_temperature_reading;

#ifdef ADC_C_
  #undef extern
#endif

extern void adc_initialise(void);
extern U16 ReadVoltage(void);
extern U16 ReadCurrent(void);
extern U16 ReadTemperature(void);



#endif /* ADC_H_ */