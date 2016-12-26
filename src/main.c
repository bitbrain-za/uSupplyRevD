#include "system.h"

struct i2c_master_module i2c_master_instance;

void configure_i2c_master(void);

void SysTick_Handler(void)
{
}

int main (void)
{
  irq_initialize_vectors();
  cpu_irq_enable();
	
  system_init();
  sleepmgr_init();
  configure_i2c_master();
  delay_init();
 
  display_init();

  console_fsm(true);

  dac_SetReference(VREF_VREFPIN, 2048);
  dac_SetVoltage(10000);
//  adc_initialise();

  //udc_start();
  if(EEPROM_Test())
  {
    PutStr((char *)"Pass", false, JUST_LEFT);
  }
  else
  {
    PutStr((char *)"Fail", false, JUST_LEFT);
  }

  while(1)
  {
    console_fsm(false);

    delay_ms(500);
    port_pin_toggle_output_level(LED0);
  }
}

void configure_i2c_master(void)
{
  struct i2c_master_config config_i2c_master;
  i2c_master_get_config_defaults(&config_i2c_master);

  config_i2c_master.buffer_timeout = 65535;
  config_i2c_master.generator_source = GCLK_GENERATOR_3;
  config_i2c_master.pinmux_pad0 = I2C_SDA_PINMUX;
  config_i2c_master.pinmux_pad1 = I2C_SCL_PINMUX;

  while(i2c_master_init(&i2c_master_instance, I2C_MODULE, &config_i2c_master) != STATUS_OK);

  i2c_master_enable(&i2c_master_instance);
}