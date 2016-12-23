#include "system.h"

struct i2c_master_module i2c_master_instance;

void configure_i2c_master(struct i2c_master_module *const module);

void SysTick_Handler(void)
{
}

int main (void)
{
  irq_initialize_vectors();
  cpu_irq_enable();
	
  system_init();
  sleepmgr_init();
  delay_init();

  console_fsm(true);

//  configure_i2c_master(&i2c_master_instance);
  display_init();
//  adc_initialise();

  udc_start();

  PutStr("test", false, JUST_CENTER);

  while(1)
  {
    console_fsm(false);

    delay_ms(500);
    port_pin_toggle_output_level(LED0);
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