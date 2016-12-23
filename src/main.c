#include "system.h"

struct i2c_master_module i2c_master_instance;

void configure_i2c_master(struct i2c_master_module *const module);

void SysTick_Handler(void)
{
}

void configure_tc(void);

//! [module_inst]
struct tc_module tc_instance;
//! [module_inst]

//! [setup]
void configure_tc(void)
{
  //! [setup_config]
  struct tc_config config_tc;
  //! [setup_config]
  //! [setup_config_defaults]
  tc_get_config_defaults(&config_tc);
  //! [setup_config_defaults]

  //! [setup_change_config]
  config_tc.counter_size    = TC_COUNTER_SIZE_8BIT;
  config_tc.clock_source    = GCLK_GENERATOR_3;
  config_tc.clock_prescaler = TC_CLOCK_PRESCALER_DIV8;
  config_tc.counter_8_bit.period = 100;

  //! [setup_set_config]
  tc_init(&tc_instance, TC3, &config_tc);
  //! [setup_set_config]

  //! [setup_enable]
  tc_enable(&tc_instance);
  //! [setup_enable]
}

int main (void)
{
  U32 counter = 0;
  irq_initialize_vectors();
  cpu_irq_enable();
	
  system_init();
  sleepmgr_init();

	configure_tc();

  console_fsm(true);

//  configure_i2c_master(&i2c_master_instance);
  //display_init();
//  adc_initialise();

  udc_start();

  //PutStr("test", false, JUST_CENTER);

  while(1)
  {
    console_fsm(false);

    while(TC_STATUS_COUNT_OVERFLOW != (TC_STATUS_COUNT_OVERFLOW & tc_get_status(&tc_instance))) {;}
    tc_clear_status(&tc_instance, TC_STATUS_COUNT_OVERFLOW);
    if(counter++ > 5000)
    {
      counter = 0;
      port_pin_toggle_output_level(LED0);
    }
  }
}

void configure_i2c_master(struct i2c_master_module *const module)
{
  struct i2c_master_config config_i2c_master;
  i2c_master_get_config_defaults(&config_i2c_master);

  config_i2c_master.buffer_timeout = 10000;

  i2c_master_init(module, I2C_MODULE, &config_i2c_master);
  i2c_master_enable(module);
}