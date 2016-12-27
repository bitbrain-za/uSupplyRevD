/*
 * delay.c
 *
 * Created: 2016/12/23 9:33:50 PM
 *  Author: philb
 */ 

 #include "../system.h"

struct tc_module tc_instance;

void delay_init(void)
{
  struct tc_config config_tc;
  tc_get_config_defaults(&config_tc);

  config_tc.counter_size    = TC_COUNTER_SIZE_8BIT;
  /* 8MHz */
  config_tc.clock_source    = GCLK_GENERATOR_3;
  config_tc.clock_prescaler = TC_CLOCK_PRESCALER_DIV1;
  /* 1 MHz -> 1 us*/
  config_tc.counter_8_bit.period = 8;

  tc_init(&tc_instance, DELAY_TC, &config_tc);

  tc_enable(&tc_instance);
  tc_stop_counter(&tc_instance);
}

void delay_us(U32 delay)
{
  /* 10 us */
  tc_set_top_value(&tc_instance, 8);

  tc_start_counter(&tc_instance);
  U32 counter = 0;

  while(++counter < delay)
  {
    while(TC_STATUS_COUNT_OVERFLOW != (TC_STATUS_COUNT_OVERFLOW & tc_get_status(&tc_instance))) {;}
    tc_clear_status(&tc_instance, TC_STATUS_COUNT_OVERFLOW);
  }
  
  tc_stop_counter(&tc_instance);
}

void delay_ms(U32 delay)
{
  /* 10 us */
  tc_set_top_value(&tc_instance, 80);

  if(delay > (0xFFFFFFFE / 100))
    delay = 0xFFFFFFFE;
  else
    delay = delay * 100;

  tc_start_counter(&tc_instance);
  U32 counter = 0;

  while(++counter < delay)
  {
    while(TC_STATUS_COUNT_OVERFLOW != (TC_STATUS_COUNT_OVERFLOW & tc_get_status(&tc_instance))) {;}
    tc_clear_status(&tc_instance, TC_STATUS_COUNT_OVERFLOW);
  }
  
  tc_stop_counter(&tc_instance);
}