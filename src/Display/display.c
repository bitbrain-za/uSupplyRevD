/* 
* LM6029ACW.cpp
*
* Created: 2016/07/21 1:01:27 PM
* Author: Philip
*/
#include "../system.h"
#include "fonts/small_font.h"
#include "fonts/sitka_med.h"
#include "display_driver.h"

/*
Because of RAM limitations, we're going to hard-code the separators:
  1 - page 3, width 4px, bits 2-5
  2 - page 6, width 1px, bits 7
*/


Font SmallFont;
Font SitkaMed;

Font *_font;

U8 pos_x;
U8 pos_y;

void display_init()
{
  disp_init();

  ClearScreen(false);
  SmallFont.charmap = small_font;
  SmallFont.height = 1;

  SitkaMed.charmap = sitka_med_font;
  SitkaMed.height = 3;

  _font = &SitkaMed;

  pos_x = 0;
  pos_y = 0;
}

void ClearScreen(bool Invert)
{
  disp_Clear(Invert);
}

void ClearLine(U8 line, bool invert)
{
  disp_ClearLine(line, invert);
}

void GotoXY(unsigned char x, unsigned char y)
{
  if(x < LCD_COLUMNS - 1)
    pos_x = x;
  else
    pos_x = 0;
  
  if(y < LCD_PAGES)
    pos_y = y;
  else
    pos_y = 0;
}

void PutStr(char *str, bool invert, JUSTIFICATION justification)
{
  unsigned int i=0;

  U8 string_length = string_width(_font, str);
  
  switch(justification)
  {
    case JUST_LEFT:
      pos_x = 0;
    break;

    case JUST_RIGHT:
      pos_x = LCD_COLUMNS - string_length;
    break;

    case JUST_CENTER:
      pos_x = (LCD_COLUMNS - string_length) / 2;
    break;

    case JUST_NONE:

    break;
  }

  do
  {
    PutChar(str[i], invert);
    i++;
  }while(str[i]!='\0');
}


void PutChar(unsigned char c, bool invert)
{
  const U8* character = get_char(_font, c);
  U8 length = character++;
  int i = 0;
  int j = 0;
  U8 data = 0;
  
  if((pos_x + length) >= LCD_COLUMNS)
  {
    pos_x = 0;
    if(pos_y == (LCD_PAGES - 1))
      pos_y = 0;
    else
      pos_y++;
  }
  else
    pos_x++;

  for(j = 0 ; j < _font->height ; j ++)
  {

    disp_SetColumnAddress(pos_x);
    disp_SetPageAddress(pos_y + j);
    
    LCD_ChipSelect();
    LCD_DataMode();

    for(i = 0 ; i < length ; i++) 
    {
      data = (character++);

      /* Low RAM fix */
      if((pos_y + j) == 6)
      {
        data |= 0x80;
      }

      if(invert)
        data = ~data;

      disp_put_data(data);
      disp_trigger_write();
    }
    LCD_ChipDeselect();
  }
  pos_x += length;
}
 
void WriteCharCol(U8 v, U8 x, U8 page, U8 colour)
{
  if(colour == 1)
    v = ~v;

  LCD_DataMode();
  disp_put_data(v);
  disp_trigger_write();
}

 void SetFont(FONT_SIZE size)
 {
  switch(size)
  {
    case FONT_SMALL:
    _font = &SmallFont;
    break;
    
    case FONT_MEDIUM:
    _font = &SitkaMed;
    break;

    case FONT_LARGE:
    break;
  }
 }