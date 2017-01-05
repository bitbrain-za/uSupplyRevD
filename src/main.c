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
  timer_module_init();

  TIMER_HANDLE test = timer_new(100);

  dac_SetReference(VREF_VREFPIN, 2048);
  dac_SetVoltage(10000);

  udc_start();

  ADC_FSM(true);
  HMI_FSM(true);
  desktop_FSM(true);
  console_fsm(true);
  v_voltage_control_FSM(true);
  v_current_FSM(true);

	system_interrupt_enable_global();
  timer_start(test);

  while(1)
  {
    ADC_FSM(false);
    v_voltage_control_FSM(false);
    v_current_FSM(false);
    console_fsm(false);
    HMI_FSM(false);
    desktop_FSM(false);
    if(timer_expired(test))
    {
      port_pin_toggle_output_level(LED0);
      timer_reset(test);
    }
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