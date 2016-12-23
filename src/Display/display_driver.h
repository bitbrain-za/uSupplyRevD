/*
 * display_driver.h
 *
 * Created: 2016/10/12 1:52:36 PM
 *  Author: Philip
 */ 


#ifndef DISPLAY_DRIVER_H_
#define DISPLAY_DRIVER_H_

typedef enum
{
  DISP_ON = 0xAF,
  DISP_OFF = 0xAE,
  DISP_START_LINE_SET = 0x40,
  DISP_SET_PAGE_ADDRESS = 0xB0,
  DISP_SET_COLUMN_ADDRESS_UPPER = 0x10,
  DISP_SET_COLUMN_ADDRESS_LOWER = 0x00,
  DISP_ADC_NORMAL = 0xA0,
  DISP_ADC_REVERSE = 0xA1,
  DISP_NORMAL= 0xA6,
  DISP_REVERSE= 0xA7,
  DISP_ENTIRE_DISP_NORMAL = 0xA4,
  DISP_ENTIRE_DISP_ON = 0xA5,
  DISP_LCD_BIAS_DIV7 = 0xA3,
  DISP_LCD_BIAS_DIV9 = 0xA2,
  DISP_SET_RMW_MODE = 0xE0,
  DISP_RESET_RMW_MODE = 0xEE,
  DISP_RESET = 0xE2,
  DISP_POWER_CONTROL_SET = 0x28,
  DISP_REGULATOR_RESISTOR_RATIO = 0x20,
  DISP_SHL_NORMAL = 0xC0,
  DISP_SHL_FLIPPED = 0xC8,
  DISP_SET_POWER_MODE = 0x28,
  DISP_ELECTRONIC_VOLUME_MODE_SET = 0x81,
  DISP_SLEEP_MODE_SELECT_SLEEP = 0xAD,
  DISP_SLEEP_MODE_SELECT_NORMAL = 0xAC,
  DISP_SLEEP_MODE_TRIGGER = 0x00,
  DISP_NOP = 0xE3,
}DISPLAY_COMMANDS;

#define LCD_RDPinSet()        port_pin_set_output_level(LCD_RD_PIN, true)
#define LCD_RDPinClear()      port_pin_set_output_level(LCD_RD_PIN, false)
#define LCD_ChipSelect()      port_pin_set_output_level(LCD_CS_PIN, false)
#define LCD_ChipDeselect()    port_pin_set_output_level(LCD_CS_PIN, true)
#define LCD_DataMode()        port_pin_set_output_level(LCD_RS_PIN, true)
#define LCD_CommandMode()     port_pin_set_output_level(LCD_RS_PIN, false)
#define LCD_WRPinSet()        port_pin_set_output_level(LCD_WR_PIN, true)
#define LCD_WRPinClear()      port_pin_set_output_level(LCD_WR_PIN, false)

extern void disp_init(void);
extern void disp_put_data(U8 data);
extern void disp_trigger_write(void);
extern bool disp_SetStartLine(U8 line);
extern bool disp_SetPageAddress(U8 page_address);
extern bool disp_SetColumnAddress(U8 column_address);
extern bool disp_SetContrast(U8 val);
extern void disp_Clear(bool Invert);
extern void disp_ClearLine(U8 line, bool Invert);
extern U8 disp_PutChar(char c);



#endif /* DISPLAY_DRIVER_H_ */