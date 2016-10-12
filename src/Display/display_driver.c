/*
 * display_driver.c
 *
 * Created: 2016/10/12 1:52:45 PM
 *  Author: Philip
 */ 

static const U8 COLUMN_OFFSET = 0x00;

void disp_SetPowerMode(bool VoltageFollower, bool VoltageRegulator, bool VoltageConverter);
void disp_SendData(U8 val);
void disp_SendCommand(U8 val);


/* Pins */


void disp_init()
{
  LCD_HW::RDPinSet();
  LCD_HW::ChipSelect();

  LCD_HW::Reset();
  
  _delay_ms(500);

  disp_SendCommand(DISP_LCD_BIAS_DIV9);
  disp_SendCommand(DISP_ADC_NORMAL);
  disp_SendCommand(DISP_SHL_FLIPPED);
  disp_SetContrast(0x20);

  disp_SetPowerMode(true, true, true);
  disp_SetStartLine(0);

  LCD_HW::ChipSelect();
  disp_SendCommand(DISP_ENTIRE_DISP_NORMAL);

  disp_SendCommand(DISP_NORMAL);
  disp_SendCommand(DISP_ON);

  disp_SetPageAddress(0x00);
  disp_SetColumnAddress(0x00);
  
  LCD_HW::ChipDeselect();
}

bool disp_SetStartLine(U8 line)
{
  LCD_HW::ChipSelect();
  if(line > 0x3F)
  return false;

  disp_SendCommand(DISP_START_LINE_SET | line);
  LCD_HW::ChipDeselect();
  return true;
}

bool disp_SetPageAddress(U8 page_address)
{
  LCD_HW::ChipSelect();
  if(page_address > 0x0F)
  return false;

  disp_SendCommand(DISP_SET_PAGE_ADDRESS | page_address);
  LCD_HW::ChipDeselect();
  return true;
}

bool disp_SetColumnAddress(U8 column_address)
{
  LCD_HW::ChipSelect();
  U8 upper = (column_address >> 4) & 0x0F;
  U8 lower = column_address & 0x0F;

  disp_SendCommand(DISP_SET_COLUMN_ADDRESS_UPPER | upper);
  disp_SendCommand(DISP_SET_COLUMN_ADDRESS_LOWER | lower);

  LCD_HW::ChipDeselect();
  return true;
}

bool disp_SetContrast(U8 val)
{
  LCD_HW::ChipSelect();
  if(val > 0x3F)
  return false;
  
  disp_SendCommand(DISP_ELECTRONIC_VOLUME_MODE_SET);
  disp_SendCommand(val);
  
  LCD_HW::ChipDeselect();
  return true;
}

void disp_SetPowerMode(bool VoltageFollower, bool VoltageRegulator, bool VoltageConverter)
{
  LCD_HW::ChipSelect();
  U8 val = 0x00;

  val += (VoltageFollower) ? 0x01 : 0x00;
  val += (VoltageRegulator) ? 0x02 : 0x00;
  val += (VoltageConverter) ? 0x04 : 0x00;

  disp_SendCommand(DISP_SET_POWER_MODE | val);
  LCD_HW::ChipDeselect();
}

void disp_SendData(U8 val)
{
  LCD_HW::DataMode();
  LCD_HW::SetData(val);
  LCD_HW::TriggerWrite();
}

void disp_SendCommand(U8 val)
{
  LCD_HW::CommandMode();
  LCD_HW::SetData(val);
  LCD_HW::TriggerWrite();
}

void disp_Clear(bool Invert)
{
  unsigned char i;
  
  for(i=0 ; i < LCD_HW::PAGES ; i++)
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

  if((ex_state == SYS_DESKTOP) && (line == 3))
  {
    data ^= 0x18;
  }
  
  disp_SetColumnAddress(0);
  disp_SetPageAddress(line);

  LCD_HW::ChipSelect();
  LCD_HW::DataMode();

  for(j = 0 ; (j < LCD_HW::COLUMNS) ; j++)
  {
    if((line == 7) && ((j % 32) == 0))
    LCD_HW::SetData(~data);
    else
    LCD_HW::SetData(data);

    LCD_HW::TriggerWrite();
  }
  LCD_HW::ChipDeselect();
}

U8 disp_PutChar(char c)
{
  return 0;
}