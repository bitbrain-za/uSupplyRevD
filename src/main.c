#include "system.h"

struct i2c_master_module i2c_master_instance;

void configure_i2c_master(struct i2c_master_module *const module);

void SysTick_Handler(void)
{
  port_pin_toggle_output_level(LED0);
}

int main (void)
{
	system_init();
 	SysTick_Config(system_gclk_gen_get_hz(GCLK_GENERATOR_0));
  configure_i2c_master(&i2c_master_instance);
  adc_initialise();

  while(1)
  {

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