#include "system.h"

#include "conf_usb.h"
#include "uart.h"


static volatile bool main_b_cdc_enable = false;
struct i2c_master_module i2c_master_instance;

void configure_i2c_master(struct i2c_master_module *const module);

void SysTick_Handler(void)
{
  port_pin_toggle_output_level(LED0);
}

int main (void)
{
  irq_initialize_vectors();
  cpu_irq_enable();
  sleepmgr_init();

	system_init();
// 	SysTick_Config(system_gclk_gen_get_hz(GCLK_GENERATOR_0));
//  configure_i2c_master(&i2c_master_instance);
//  display_init();
//  adc_initialise();

  udc_start();

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

/*
CDC
*/

void main_suspend_action(void)
{
//	ui_powerdown();
}

void main_resume_action(void)
{
//  ui_wakeup();
}

void main_sof_action(void)
{
  if (!main_b_cdc_enable)
  return;
  //ui_process(udd_get_frame_number());
}

bool main_cdc_enable(uint8_t port)
{
  main_b_cdc_enable = true;
  // Open communication
  uart_open(port);
  return true;
}

void main_cdc_disable(uint8_t port)
{
  main_b_cdc_enable = false;
  // Close communication
  uart_close(port);
}

void main_cdc_set_dtr(uint8_t port, bool b_enable)
{
  if (b_enable) 
  {
    // Host terminal has open COM
    //ui_com_open(port);
  }
  else
  {
    // Host terminal has close COM
    //ui_com_close(port);
  }
}

#ifdef USB_DEVICE_LPM_SUPPORT
void main_suspend_lpm_action(void)
{
}

void main_remotewakeup_lpm_disable(void)
{
}

void main_remotewakeup_lpm_enable(void)
{
}
#endif