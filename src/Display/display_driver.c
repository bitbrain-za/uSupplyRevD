/*
 * display_driver.c
 *
 * Created: 2016/10/12 1:52:45 PM
 *  Author: Philip
 */ 

#include "../system.h"
#include "display_driver.h"

/*
    ____             __        __
   / __ \_________  / /_____  / /___  ______  ___  _____
  / /_/ / ___/ __ \/ __/ __ \/ __/ / / / __ \/ _ \/ ___/
 / ____/ /  / /_/ / /_/ /_/ / /_/ /_/ / /_/ /  __(__  )
/_/   /_/   \____/\__/\____/\__/\__, / .___/\___/____/
                               /____/_/
*/

void disp_SetPowerMode(bool VoltageFollower, bool VoltageRegulator, bool VoltageConverter);
void disp_SendData(U8 val);
void disp_SendCommand(U8 val);
void disp_reset(void);

void disp_put_data(U8 data);
void disp_trigger_write(void);

/*
   ________      __          __   _    __           _       __    __
  / ____/ /___  / /_  ____ _/ /  | |  / /___ ______(_)___ _/ /_  / /__  _____
 / / __/ / __ \/ __ \/ __ `/ /   | | / / __ `/ ___/ / __ `/ __ \/ / _ \/ ___/
/ /_/ / / /_/ / /_/ / /_/ / /    | |/ / /_/ / /  / / /_/ / /_/ / /  __(__  )
\____/_/\____/_.___/\__,_/_/     |___/\__,_/_/  /_/\__,_/_.___/_/\___/____/
*/

static U8 *gucp_lcd_ram_buffer;
static const U8 COLUMN_OFFSET = 0x00;
U8 euc_dirty_mask = 0xFF;

/*
    ______     __                        __   ______                 __  _
   / ____/  __/ /____  _________  ____ _/ /  / ____/_  ______  _____/ /_(_)___  ____  _____
  / __/ | |/_/ __/ _ \/ ___/ __ \/ __ `/ /  / /_  / / / / __ \/ ___/ __/ / __ \/ __ \/ ___/
 / /____>  </ /_/  __/ /  / / / / /_/ / /  / __/ / /_/ / / / / /__/ /_/ / /_/ / / / (__  )
/_____/_/|_|\__/\___/_/  /_/ /_/\__,_/_/  /_/    \__,_/_/ /_/\___/\__/_/\____/_/ /_/____/
*/

void disp_init(U8 *ram_buffer)
{
  gucp_lcd_ram_buffer = ram_buffer;
  LCD_RDPinSet();
  LCD_ChipSelect();

  disp_reset();
  
  delay_ms(500);

  disp_SendCommand(DISP_LCD_BIAS_DIV9);
  disp_SendCommand(DISP_ADC_NORMAL);
  disp_SendCommand(DISP_SHL_FLIPPED);
  disp_SetContrast(0x20);

  disp_SetPowerMode(true, true, true);
  disp_SetStartLine(0);

  LCD_ChipSelect();
  disp_SendCommand(DISP_ENTIRE_DISP_NORMAL);

  disp_SendCommand(DISP_NORMAL);
  disp_SendCommand(DISP_ON);

  disp_SetPageAddress(0x00);
  disp_SetColumnAddress(0x00);
  
  LCD_ChipDeselect();
}

void v_disp_paint(void)
{
  U8 page = 0;

  for(page = 0 ; page < LCD_PAGES ; page++)
  {
    v_disp_paint_page(page);
  }
}

bool disp_SetStartLine(U8 line)
{
  LCD_ChipSelect();
  if(line > 0x3F)
  return false;

  disp_SendCommand(DISP_START_LINE_SET | line);
  LCD_ChipDeselect();
  return true;
}

bool disp_SetPageAddress(U8 page_address)
{
  LCD_ChipSelect();
  if(page_address > 0x0F)
  return false;

  disp_SendCommand(DISP_SET_PAGE_ADDRESS | page_address);
  LCD_ChipDeselect();
  return true;
}

bool disp_SetColumnAddress(U8 column_address)
{
  LCD_ChipSelect();
  U8 upper = (column_address >> 4) & 0x0F;
  U8 lower = column_address & 0x0F;

  disp_SendCommand(DISP_SET_COLUMN_ADDRESS_UPPER | upper);
  disp_SendCommand(DISP_SET_COLUMN_ADDRESS_LOWER | lower);

  LCD_ChipDeselect();
  return true;
}

bool disp_SetContrast(U8 val)
{
  LCD_ChipSelect();
  if(val > 0x3F)
  return false;
  
  disp_SendCommand(DISP_ELECTRONIC_VOLUME_MODE_SET);
  disp_SendCommand(val);
  
  LCD_ChipDeselect();
  return true;
}

void disp_Clear(bool Invert)
{
  unsigned char i;
  
  for(i=0 ; i < LCD_PAGES ; i++)
  {
    disp_ClearLine(i, Invert);
  }
}

void disp_ClearLine(U8 line, bool Invert)
{
  U8 data = (Invert) ? 0xFF : 0x00;
  unsigned char j;
 
  disp_SetColumnAddress(0);
  disp_SetPageAddress(line);

  LCD_ChipSelect();
  LCD_DataMode();

  for(j = 0 ; (j < LCD_COLUMNS) ; j++)
  {
    disp_put_data(data);
    disp_trigger_write();
  }
  LCD_ChipDeselect();
}

U8 disp_PutChar(char c)
{
  return 0;
}

/*
    __                     __   ______                 __  _
   / /   ____  _________ _/ /  / ____/_  ______  _____/ /_(_)___  ____  _____
  / /   / __ \/ ___/ __ `/ /  / /_  / / / / __ \/ ___/ __/ / __ \/ __ \/ ___/
 / /___/ /_/ / /__/ /_/ / /  / __/ / /_/ / / / / /__/ /_/ / /_/ / / / (__  )
/_____/\____/\___/\__,_/_/  /_/    \__,_/_/ /_/\___/\__/_/\____/_/ /_/____/
*/

void disp_reset(void)
{
  port_pin_set_output_level(LCD_RES_PIN, false);
  delay_ms(100); 
  port_pin_set_output_level(LCD_RES_PIN, true);
}

void v_disp_paint_page(U8 page)
{
  U8 mask = euc_dirty_mask >> page;
  if(0x01 != (mask & 0x01))
  {
    return;
  }

  euc_dirty_mask &= ~(0x01 << page);

  U8 column = 0;
  U8 *ptr_data = gucp_lcd_ram_buffer + (page * LCD_COLUMNS);

  disp_SetColumnAddress(column);
  disp_SetPageAddress(page);

  LCD_ChipSelect();
  LCD_DataMode();

  for(column = 0 ; column < LCD_COLUMNS ; column++)
  {
    disp_put_data(*ptr_data);
    disp_trigger_write();
    ptr_data++;
  }

  LCD_ChipDeselect();
}

void disp_SetPowerMode(bool VoltageFollower, bool VoltageRegulator, bool VoltageConverter)
{
  LCD_ChipSelect();
  U8 val = 0x00;

  val += (VoltageFollower) ? 0x01 : 0x00;
  val += (VoltageRegulator) ? 0x02 : 0x00;
  val += (VoltageConverter) ? 0x04 : 0x00;

  disp_SendCommand(DISP_SET_POWER_MODE | val);
  LCD_ChipDeselect();
}

void disp_SendData(U8 val)
{
  LCD_DataMode();
  disp_put_data(val);
  disp_trigger_write();
}

void disp_SendCommand(U8 val)
{
  LCD_CommandMode();
  disp_put_data(val);
  disp_trigger_write();
}

void disp_put_data(U8 data)
{
  port_pin_set_output_level(LCD_DB7_PIN, ( (data & (0x01 << 7)) == (0x01 << 7) ));
  port_pin_set_output_level(LCD_DB6_PIN, ( (data & (0x01 << 6)) == (0x01 << 6) ));
  port_pin_set_output_level(LCD_DB5_PIN, ( (data & (0x01 << 5)) == (0x01 << 5) ));
  port_pin_set_output_level(LCD_DB4_PIN, ( (data & (0x01 << 4)) == (0x01 << 4) ));
  port_pin_set_output_level(LCD_DB3_PIN, ( (data & (0x01 << 3)) == (0x01 << 3) ));
  port_pin_set_output_level(LCD_DB2_PIN, ( (data & (0x01 << 2)) == (0x01 << 2) ));
  port_pin_set_output_level(LCD_DB1_PIN, ( (data & (0x01 << 1)) == (0x01 << 1) ));
  port_pin_set_output_level(LCD_DB0_PIN, ( (data & (0x01 << 0)) == (0x01 << 0) ));
}

void disp_trigger_write(void)
{
  LCD_WRPinClear();
  delay_us(1);
  LCD_WRPinSet();
}

