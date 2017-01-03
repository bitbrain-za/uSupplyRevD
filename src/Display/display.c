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
    ____       _____       _ __  _
   / __ \___  / __(_)___  (_) /_(_)___  ____  _____
  / / / / _ \/ /_/ / __ \/ / __/ / __ \/ __ \/ ___/
 / /_/ /  __/ __/ / / / / / /_/ / /_/ / / / (__  )
/_____/\___/_/ /_/_/ /_/_/\__/_/\____/_/ /_/____/

*/

/*
    ____             __        __
   / __ \_________  / /_____  / /___  ______  ___  _____
  / /_/ / ___/ __ \/ __/ __ \/ __/ / / / __ \/ _ \/ ___/
 / ____/ /  / /_/ / /_/ /_/ / /_/ /_/ / /_/ /  __(__  )
/_/   /_/   \____/\__/\____/\__/\__, / .___/\___/____/
                               /____/_/
*/

void v_draw_line_horizontal(U8 y, U8 x, U8 length, bool invert);
void v_draw_line_vertical(U8 x, U8 y, U8 length, bool invert);

/*
   ________      __          __   _    __           _       __    __
  / ____/ /___  / /_  ____ _/ /  | |  / /___ ______(_)___ _/ /_  / /__  _____
 / / __/ / __ \/ __ \/ __ `/ /   | | / / __ `/ ___/ / __ `/ __ \/ / _ \/ ___/
/ /_/ / / /_/ / /_/ / /_/ / /    | |/ / /_/ / /  / / /_/ / /_/ / /  __(__  )
\____/_/\____/_.___/\__,_/_/     |___/\__,_/_/  /_/\__,_/_.___/_/\___/____/

*/

Font SmallFont;
Font SitkaMed;

Font *_font;

U8 pos_x;
U8 pos_y;

static U8 guca_display_frame_buffer[LCD_PAGES][LCD_COLUMNS];

/*
    ______     __                        __   ______                 __  _
   / ____/  __/ /____  _________  ____ _/ /  / ____/_  ______  _____/ /_(_)___  ____  _____
  / __/ | |/_/ __/ _ \/ ___/ __ \/ __ `/ /  / /_  / / / / __ \/ ___/ __/ / __ \/ __ \/ ___/
 / /____>  </ /_/  __/ /  / / / / /_/ / /  / __/ / /_/ / / / / /__/ /_/ / /_/ / / / (__  )
/_____/_/|_|\__/\___/_/  /_/ /_/\__,_/_/  /_/    \__,_/_/ /_/\___/\__/_/\____/_/ /_/____/

*/


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

void v_disp_draw_line(U8 x_start, U8 y_start, U8 x_end, U8 y_end, bool invert)
{
  bool horizontal, vertical = false;

  vertical = (x_start == x_end);
  horizontal = (y_start == y_end);

  if(horizontal)
    v_draw_line_horizontal(y_start, x_start, x_end - x_start, invert);
  else if(vertical)
    v_draw_line_vertical(x_start, y_start, y_end - y_start, invert);
  else
    return;
}

void v_display_draw_button(U8 x, U8 y, U8 width, U8 height, const char *str, bool invert)
{
  unsigned int i=0;

  U8 string_length = string_width(_font, (char *)str);
  pos_x = x + ((width - string_length) / 2);
  pos_y = (y + 4) / 8;

  do
  {
    PutChar(str[i], invert);
    i++;
  }while(str[i]!='\0'); 

  v_draw_line_vertical(x, pos_y, height, invert);
  v_draw_line_vertical(x + width, pos_y, height, invert);
  v_draw_line_horizontal(y, x, width, invert);
  v_draw_line_horizontal(y + height, x, width, invert);

}

/*
    __                     __   ______                 __  _
   / /   ____  _________ _/ /  / ____/_  ______  _____/ /_(_)___  ____  _____
  / /   / __ \/ ___/ __ `/ /  / /_  / / / / __ \/ ___/ __/ / __ \/ __ \/ ___/
 / /___/ /_/ / /__/ /_/ / /  / __/ / /_/ / / / / /__/ /_/ / /_/ / / / (__  )
/_____/\____/\___/\__,_/_/  /_/    \__,_/_/ /_/\___/\__/_/\____/_/ /_/____/

*/

void v_draw_line_horizontal(U8 y, U8 x, U8 length, bool invert)
{
  if(y >= LCD_ROWS)
    return;

  if((length + x) > LCD_COLUMNS)
    length = LCD_COLUMNS - 1 - x;

  U8 i = 0;
  U8 page = y / 8;
  U8 shifter = y % 8;
  U8 data = (0x01 << shifter);

  for( i = 0 ; i < length ; i++ )
  {
    if(invert)
    {
      guca_display_frame_buffer[page][x + i] &= ~data;
    }
    else
    {
      guca_display_frame_buffer[page][x + i] |= data;
    }
  }
}

void v_draw_line_vertical(U8 x, U8 y, U8 length, bool invert)
{
  if(x >= LCD_COLUMNS)
    return;

  if((length + y) > LCD_ROWS)
    length = LCD_ROWS - 1 - x;

  U8 i = 0;
  U8 offset = y % 8;
  U8 pages = (length - offset) / 8;
  U8 data;
  U8 page = y / 8;

  /* Top offset */
  if(offset > 0)
  {
    data = 0x80;
    for( i = 0 ; i < offset ; i++ )
    {
      data = data >> 1;
      data |= 0x80;
    }

    if(invert)
    {
      guca_display_frame_buffer[page][x] &= ~data;
    }
    else
    {
      guca_display_frame_buffer[page][x] |= data;
    }
  }

  /* Full pages  */
  page++;
  data = 0xFF;
  for( i = 0 ; i < pages ; i++ )
  {
    if(invert)
    {
      guca_display_frame_buffer[page][x] &= ~data;
    }
    else
    {
      guca_display_frame_buffer[page][x] |= data;
    }   
  }

  /* Leftovers */
  U8 leftovers = length - offset - (pages * 8);
  if(leftovers > 0)
  {
    data = 0x01;
    for( i = 0 ; i < leftovers ; i++ )
    {
      data = data << 1;
      data |= 0x01;
    }
    if(invert)
    {
      guca_display_frame_buffer[page][x] &= ~data;
    }
    else
    {
      guca_display_frame_buffer[page][x] |= data;
    }
  }
}