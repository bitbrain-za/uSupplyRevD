#ifndef USER_BOARD_H
#define USER_BOARD_H

void system_board_init(void);

#define BOARD_NAME                "USUPPLY_REVD"

/*
Clock
*/

#define BOARD_FREQ_SLCK_XTAL      (32768U)
#define BOARD_FREQ_SLCK_BYPASS    (32768U)
#define BOARD_FREQ_MAINCK_XTAL    0 /* Not Mounted */
#define BOARD_FREQ_MAINCK_BYPASS  0 /* Not Mounted */
#define BOARD_MCK                 CHIP_FREQ_CPU_MAX
#define BOARD_OSC_STARTUP_US      15625

/*
TC
*/

#define DELAY_TC TC3

/*
GPIO
*/

#define EEWP_PIN                  PIN_PA28
#define EEWP_ACTIVE               false
#define EEWP_INACTIVE             !EEWP_ACTIVE
#define EEWP                      EEWP_PIN

#define EEWP_ENABLE()             port_pin_set_output_level(EEWP_PIN,true)
#define EEWP_DISABLE()            port_pin_set_output_level(EEWP_PIN,false)

#define I2C_SDA_PINMUX            PINMUX_PA16C_SERCOM1_PAD0
#define I2C_SCL_PINMUX            PINMUX_PA17C_SERCOM1_PAD1

/*
LCD
*/

#define LCD_CS_PIN                PIN_PB08
#define LCD_WR_PIN                PIN_PA02
#define LCD_RD_PIN                PIN_PA04
#define LCD_RS_PIN                PIN_PB09
#define LCD_RES_PIN               PIN_PA03

#define LCD_DB0_PIN               PIN_PA01
#define LCD_DB1_PIN               PIN_PA05
#define LCD_DB2_PIN               PIN_PA00
#define LCD_DB3_PIN               PIN_PA06
#define LCD_DB4_PIN               PIN_PB03
#define LCD_DB5_PIN               PIN_PA07
#define LCD_DB6_PIN               PIN_PB02
#define LCD_DB7_PIN               PIN_PA08

/*
LEDs
*/

#define LED_COUNT                 3

#define LED_0_NAME                "LED0 (Power)"
#define LED0_PIN                  PIN_PB22
#define LED0_ACTIVE               false
#define LED0_INACTIVE             !LED0_ACTIVE
#define LED0                      LED0_PIN

#define LED_1_NAME                "LED1 (Voltage)"
#define LED1_PIN                  PIN_PB11
#define LED1_ACTIVE               false
#define LED1_INACTIVE             !LED1_ACTIVE
#define LED1                      LED1_PIN

#define LED_2_NAME                "LED2 (Current)"
#define LED2_PIN                  PIN_PB10
#define LED2_ACTIVE               false
#define LED2_INACTIVE             !LED2_ACTIVE
#define LED2                      LED2_PIN

#define LED_Off(led_gpio)     port_pin_set_output_level(led_gpio,true)
#define LED_On(led_gpio)      port_pin_set_output_level(led_gpio,false)
#define LED_Toggle(led_gpio)  port_pin_toggle_output_level(led_gpio)

/*
Switches
*/

#define BUTTON_COUNT 5

#define SW0_NAME                  "SW0"
#define SW0_PIN                   PIN_PA15
#define SW0_ACTIVE                false
#define SW0_INACTIVE              !SW0_ACTIVE
#define SW0_EIC_PIN               PIN_PA15A_EIC_EXTINT15
#define SW0_EIC_MUX               MUX_PA15A_EIC_EXTINT15
#define SW0_EIC_PINMUX            PINMUX_PA15A_EIC_EXTINT15
#define SW0_EIC_LINE              15

#define SW1_NAME                  "SW1"
#define SW1_PIN                   PIN_PA14
#define SW1_ACTIVE                false
#define SW1_INACTIVE              !SW1_ACTIVE
#define SW1_EIC_PIN               PIN_PA14A_EIC_EXTINT14
#define SW1_EIC_MUX               MUX_PA14A_EIC_EXTINT14
#define SW1_EIC_PINMUX            PINMUX_PA14A_EIC_EXTINT14
#define SW1_EIC_LINE              14

#define SW2_NAME                  "SW2"
#define SW2_PIN                   PIN_PA13
#define SW2_ACTIVE                false
#define SW2_INACTIVE              !SW2_ACTIVE
#define SW2_EIC_PIN               PIN_PA13A_EIC_EXTINT13
#define SW2_EIC_MUX               MUX_PA13A_EIC_EXTINT13
#define SW2_EIC_PINMUX            PINMUX_PA13A_EIC_EXTINT13
#define SW2_EIC_LINE              13

#define SW3_NAME                  "SW3"
#define SW3_PIN                   PIN_PA11
#define SW3_ACTIVE                false
#define SW3_INACTIVE              !SW3_ACTIVE
#define SW3_EIC_PIN               PIN_PA11A_EIC_EXTINT11
#define SW3_EIC_MUX               MUX_PA11A_EIC_EXTINT11
#define SW3_EIC_PINMUX            PINMUX_PA11A_EIC_EXTINT11
#define SW3_EIC_LINE              11

#define SW4_NAME                  "SW4"
#define SW4_PIN                   PIN_PA20
#define SW4_ACTIVE                false
#define SW4_INACTIVE              !SW4_ACTIVE
#define SW4_EIC_PIN               PIN_PA20A_EIC_EXTINT4
#define SW4_EIC_MUX               MUX_PA20A_EIC_EXTINT4
#define SW4_EIC_PINMUX            PINMUX_PA20A_EIC_EXTINT4
#define SW4_EIC_LINE              4


/*
Rotary Encoders
*/

#define ENCODER_1_NAME            "Voltage Encoder"
#define ENC1A                     PIN_PA19
#define ENC1B                     PIN_PA18

#define ENCODER_2_NAME            "Current Encoder"
#define ENC2A                     PIN_PA22
#define ENC2B                     PIN_PA21

/*
ADC
*/

/*
PWM
*/

/*
I2C
*/

#define I2C_MODULE                SERCOM1
#define I2C_SERCOM_PINMUX_PAD0    PINMUX_PA16C_SERCOM1_PAD0
#define I2C_SERCOM_PINMUX_PAD1    PINMUX_PA17C_SERCOM1_PAD1
#define I2C_SERCOM_DMAC_ID_TX     SERCOM1_DMAC_ID_TX
#define I2C_SERCOM_DMAC_ID_RX     SERCOM1_DMAC_ID_RX

/*
USB
*/

#define USB_ID
#define USB_TARGET_DP_PIN            PIN_PA25G_USB_DP
#define USB_TARGET_DP_MUX            MUX_PA25G_USB_DP
#define USB_TARGET_DP_PINMUX         PINMUX_PA25G_USB_DP
#define USB_TARGET_DM_PIN            PIN_PA24G_USB_DM
#define USB_TARGET_DM_MUX            MUX_PA24G_USB_DM
#define USB_TARGET_DM_PINMUX         PINMUX_PA24G_USB_DM
#define USB_VBUS_PIN                 PIN_PA23
#define USB_VBUS_EIC_LINE            7
#define USB_VBUS_EIC_MUX             MUX_PA23A_EIC_EXTINT7
#define USB_VBUS_EIC_PINMUX          PINMUX_PA23A_EIC_EXTINT7

#endif // USER_BOARD_H
