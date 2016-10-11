#include <asf.h>
#include <board.h>
#include <conf_board.h>

#if defined(__GNUC__)
void board_init(void) WEAK __attribute__((alias("system_board_init")));
#elif defined(__ICCARM__)
void board_init(void);
#  pragma weak board_init=system_board_init
#endif

/***************
*  Prototypes  *
***************/

static void gpio_init(void);
static void config_led(void);
void configure_i2c_master(struct i2c_master_module *const module);

/***********************
*  External Functions  *
***********************/

void system_board_init(void)
{
  gpio_init();
  config_led();
  configure_i2c_master(&i2c_master_instance);
}

/********************
*  Local Functions  *
********************/

static void gpio_init(void)
{
  struct port_config pin_conf;
  port_get_config_defaults(&pin_conf);
  pin_conf.direction  = PORT_PIN_DIR_OUTPUT;

  port_pin_set_config(EEWP_PIN, &pin_conf);
  port_pin_set_output_level(EEWP_PIN, EEWP_ACTIVE);

  EEWP_ENABLE();
}

static void config_led(void)
{
  struct port_config pin_conf;
  port_get_config_defaults(&pin_conf);
  pin_conf.direction  = PORT_PIN_DIR_OUTPUT;

  port_pin_set_config(LED0, &pin_conf);
  port_pin_set_output_level(LED0, LED0_INACTIVE);

  port_pin_set_config(LED1, &pin_conf);
  port_pin_set_output_level(LED1, LED1_INACTIVE);

  port_pin_set_config(LED2, &pin_conf);
  port_pin_set_output_level(LED2, LED2_INACTIVE);
}

void configure_i2c_master(struct i2c_master_module *const module)
{
  struct i2c_master_config config_i2c_master;
  i2c_master_get_config_defaults(&config_i2c_master);

  config_i2c_master.buffer_timeout = 10000;

  i2c_master_init(module, I2C_MODULE, &config_i2c_master);
  i2c_master_enable(module);
}