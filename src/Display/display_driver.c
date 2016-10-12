/*
 * display_driver.c
 *
 * Created: 2016/10/12 1:52:45 PM
 *  Author: Philip
 */ 

 #include "../system.h"

static const U8 COLUMN_OFFSET = 0x00;

/*
Prototypes
*/

void disp_SetPowerMode(bool VoltageFollower, bool VoltageRegulator, bool VoltageConverter);
void disp_SendData(U8 val);
void disp_SendCommand(U8 val);

void disp_put_data(U8 data);
void disp_trigger_write();

/* Pins */

#define RDPinSet()        port_pin_set_output_level(LCD_RD_PIN, true)
#define RDPinClear()      port_pin_set_output_level(LCD_RD_PIN, false)
#define ChipSelect()      port_pin_set_output_level(LCD_CS_PIN, true)
#define ChipDeselect()    port_pin_set_output_level(LCD_CS_PIN, false)
#define DataMode()        port_pin_set_output_level(LCD_RS_PIN, true)
#define CommandMode()     port_pin_set_output_level(LCD_RS_PIN, false)
#define WRPinSet()        port_pin_set_output_level(LCD_WR_PIN, true)
#define WRPinClear()      port_pin_set_output_level(LCD_WR_PIN, false)

#define _delay_us(x)
#define _delay_ms(x)

#define COLUMNS   128
#define PAGES     8

/*
External Functions
*/

void disp_init()
{
  RDPinSet();
  ChipSelect();

  Reset();
  
  _delay_ms(500);

  disp_SendCommand(DISP_LCD_BIAS_DIV9);
  disp_SendCommand(DISP_ADC_NORMAL);
  disp_SendCommand(DISP_SHL_FLIPPED);
  disp_SetContrast(0x20);

  disp_SetPowerMode(true, true, true);
  disp_SetStartLine(0);

  ChipSelect();
  disp_SendCommand(DISP_ENTIRE_DISP_NORMAL);

  disp_SendCommand(DISP_NORMAL);
  disp_SendCommand(DISP_ON);

  disp_SetPageAddress(0x00);
  disp_SetColumnAddress(0x00);
  
  ChipDeselect();
}

bool disp_SetStartLine(U8 line)
{
  ChipSelect();
  if(line > 0x3F)
  return false;

  disp_SendCommand(DISP_START_LINE_SET | line);
  ChipDeselect();
  return true;
}

bool disp_SetPageAddress(U8 page_address)
{
  ChipSelect();
  if(page_address > 0x0F)
  return false;

  disp_SendCommand(DISP_SET_PAGE_ADDRESS | page_address);
  ChipDeselect();
  return true;
}

bool disp_SetColumnAddress(U8 column_address)
{
  ChipSelect();
  U8 upper = (column_address >> 4) & 0x0F;
  U8 lower = column_address & 0x0F;

  disp_SendCommand(DISP_SET_COLUMN_ADDRESS_UPPER | upper);
  disp_SendCommand(DISP_SET_COLUMN_ADDRESS_LOWER | lower);

  ChipDeselect();
  return true;
}

bool disp_SetContrast(U8 val)
{
  ChipSelect();
  if(val > 0x3F)
  return false;
  
  disp_SendCommand(DISP_ELECTRONIC_VOLUME_MODE_SET);
  disp_SendCommand(val);
  
  ChipDeselect();
  return true;
}

void disp_SetPowerMode(bool VoltageFollower, bool VoltageRegulator, bool VoltageConverter)
{
  ChipSelect();
  U8 val = 0x00;

  val += (VoltageFollower) ? 0x01 : 0x00;
  val += (VoltageRegulator) ? 0x02 : 0x00;
  val += (VoltageConverter) ? 0x04 : 0x00;

  disp_SendCommand(DISP_SET_POWER_MODE | val);
  ChipDeselect();
}

void disp_SendData(U8 val)
{
  DataMode();
  disp_put_data(val);
  disp_trigger_write();
}

void disp_SendCommand(U8 val)
{
  CommandMode();
  disp_put_data(val);
  disp_trigger_write();
}

void disp_Clear(bool Invert)
{
  unsigned char i;
  
  for(i=0 ; i < PAGES ; i++)
  {
    disp_ClearLine(i, Invert);
  }
}

void disp_ClearLine(U8 line, bool Invert)
{
  U8 data = (Invert) ? 0xFF : 0x00;
  unsigned char j;

  if(line == 6)
  {
    data ^= 0x80;
  }
  
  disp_SetColumnAddress(0);
  disp_SetPageAddress(line);

  ChipSelect();
  DataMode();

  for(j = 0 ; (j < COLUMNS) ; j++)
  {
    disp_put_data(data);
    disp_trigger_write();
  }
  ChipDeselect();
}

U8 disp_PutChar(char c)
{
  return 0;
}

void disp_put_data(U8 data)
{
  port_pin_set_output_level(LCD_DB0_PIN, ( (data & (0x01 << 7)) == (0x01 << 7) ));
  port_pin_set_output_level(LCD_DB1_PIN, ( (data & (0x01 << 6)) == (0x01 << 6) ));
  port_pin_set_output_level(LCD_DB2_PIN, ( (data & (0x01 << 5)) == (0x01 << 5) ));
  port_pin_set_output_level(LCD_DB3_PIN, ( (data & (0x01 << 4)) == (0x01 << 4) ));
  port_pin_set_output_level(LCD_DB4_PIN, ( (data & (0x01 << 3)) == (0x01 << 3) ));
  port_pin_set_output_level(LCD_DB5_PIN, ( (data & (0x01 << 2)) == (0x01 << 2) ));
  port_pin_set_output_level(LCD_DB6_PIN, ( (data & (0x01 << 1)) == (0x01 << 1) ));
  port_pin_set_output_level(LCD_DB7_PIN, ( (data & (0x01 << 0)) == (0x01 << 0) ));
}


void disp_trigger_write()
{
  WRPinClear();
  _delay_us(1);
  WRPinSet();
}