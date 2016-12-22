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
static void init_lcd_gpio(void);
static void config_encoders_and_switches(void);

/***********************
*  External Functions  *
***********************/

void system_board_init(void)
{
  gpio_init();
  config_led();
  init_lcd_gpio();
  config_encoders_and_switches();
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

static void config_encoders_and_switches(void)
{
  struct port_config pin_conf;
  port_get_config_defaults(&pin_conf);
  pin_conf.direction  = PORT_PIN_DIR_INPUT;
  pin_conf.input_pull = PORT_PIN_PULL_UP;

  port_pin_set_config(ENC1A, &pin_conf);
  port_pin_set_config(ENC1B, &pin_conf);
  port_pin_set_config(ENC2A, &pin_conf);
  port_pin_set_config(ENC2A, &pin_conf);

  port_pin_set_config(SW0_PIN, &pin_conf);
  port_pin_set_config(SW1_PIN, &pin_conf);
  port_pin_set_config(SW2_PIN, &pin_conf);
  port_pin_set_config(SW3_PIN, &pin_conf);
  port_pin_set_config(SW4_PIN, &pin_conf);
}

static void init_lcd_gpio(void)
{
  struct port_config pin_conf;
  port_get_config_defaults(&pin_conf);
  pin_conf.direction  = PORT_PIN_DIR_OUTPUT;

  port_pin_set_config(LCD_CS_PIN, &pin_conf);
  port_pin_set_output_level(LCD_CS_PIN, false);

  port_pin_set_config(LCD_WR_PIN, &pin_conf);
  port_pin_set_output_level(LCD_WR_PIN, false);

  port_pin_set_config(LCD_RD_PIN, &pin_conf);
  port_pin_set_output_level(LCD_RD_PIN, false);

  port_pin_set_config(LCD_RS_PIN, &pin_conf);
  port_pin_set_output_level(LCD_RS_PIN, false);

  port_pin_set_config(LCD_RES_PIN, &pin_conf);
  port_pin_set_output_level(LCD_RES_PIN, false);

  port_pin_set_config(LCD_DB0_PIN, &pin_conf);
  port_pin_set_output_level(LCD_DB0_PIN, false);

  port_pin_set_config(LCD_DB1_PIN, &pin_conf);
  port_pin_set_output_level(LCD_DB1_PIN, false);

  port_pin_set_config(LCD_DB2_PIN, &pin_conf);
  port_pin_set_output_level(LCD_DB2_PIN, false);

  port_pin_set_config(LCD_DB3_PIN, &pin_conf);
  port_pin_set_output_level(LCD_DB3_PIN, false);

  port_pin_set_config(LCD_DB4_PIN, &pin_conf);
  port_pin_set_output_level(LCD_DB4_PIN, false);

  port_pin_set_config(LCD_DB5_PIN, &pin_conf);
  port_pin_set_output_level(LCD_DB5_PIN, false);

  port_pin_set_config(LCD_DB6_PIN, &pin_conf);
  port_pin_set_output_level(LCD_DB6_PIN, false);

  port_pin_set_config(LCD_DB7_PIN, &pin_conf);
  port_pin_set_output_level(LCD_DB7_PIN, false);
}