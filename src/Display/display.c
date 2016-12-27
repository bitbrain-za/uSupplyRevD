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

static U8 guca_display_frame_buffer[LCD_PAGES][LCD_COLUMNS];

void display_init()
{
  disp_init(&guca_display_frame_buffer[0][0]);

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
  U8 temp = Invert ? 0xFF : 0x00;
  memset(guca_display_frame_buffer, temp, LCD_COLUMNS * LCD_PAGES);
  disp_Clear(Invert);
}

void ClearLine(U8 line, bool invert)
{
  U8 temp = invert ? 0xFF : 0x00;
  memset(&guca_display_frame_buffer[line][0], temp, LCD_COLUMNS);
  disp_ClearLine(line, invert);
}

void GoToXY(U8 x, U8 y)
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
  U8 length = *character++;
  int i = 0;
  int j = 0;
  U8 data = 0;
  U8 y_index;
  
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

  y_index = pos_y;

  for(j = 0 ; j < _font->height ; j ++)
  {
    for(i = 0 ; i < length ; i++)
    {
      data = *character++;
      if(invert)
      {
        data = ~data;
      }
      guca_display_frame_buffer[y_index][pos_x + i] = data;
    }
    y_index++;
  }
  pos_x += length;
  v_disp_paint();
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