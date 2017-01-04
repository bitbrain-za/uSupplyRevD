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

extern U16 eus_raw_temperature_reading;

#ifdef ADC_C_
  #undef extern
#endif

extern void ADC_FSM(void);



#endif /* ADC_H_ */