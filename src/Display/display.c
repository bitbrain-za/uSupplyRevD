/*
 * display.c
 *
 * Created: 2016/10/13 10:27:13 AM
 *  Author: Philip
 */ 

#include "../system.h"
#include "display_driver.h"
#include "fonts.h"
#include "fonts/small_font.h"
#include "fonts/sitka_med.h"

Font small;
Font medium;
Font large;

/*
Variables
*/

U8 ram_buffer[128][8];
U8 pos_x;
U8 pos_y;

/*
Prototypes
*/

void paint_line(U8 line);
void paint_display(void);
U8 strwidth(FONT_SIZE size, char *str);

/*
External Functions
*/

void display_init(void)
{
  disp_init();
  memset(ram_buffer, 0, 128*8);

  small.height = small_font_height;
  small.charmap = small_font;

  medium.height = sitka_med_height;
  medium.charmap = sitka_med_font;
}

void ClearScreen(bool invert)
{
  U8 temp = invert ? 0xFF : 0x00;
  memset(ram_buffer, temp, COLUMNS * PAGES);
}

void ClearLine(U8 line, bool invert)
{
  U8 i;
  U8 temp = invert ? 0xFF : 0x00;
  for(i = 0 ; i < COLUMNS ; i++)
  {
    ram_buffer[i][line] = temp;
  }
}

void GoToXY(U8 x, U8 y)
{
  if(x < COLUMNS - 1)
    pos_x = x;
  else
    pos_x = 0;
  
  if(y < PAGES)
    pos_y = y;
  else
    pos_y = 0;
}

/*
Strings
*/

void PutChar(FONT_SIZE size, unsigned char c, bool invert)
{
  Font *_font = &small;

  switch(size)
  {
    case FONT_SMALL:
      _font = &small;
      break;

    case FONT_MEDIUM:
      _font = &medium;
      break;

    case FONT_LARGE:
      _font = &large;
      break;
  }
  U8 *character = get_char(_font, c);
  U8 length = char_width(_font, c);
  int i = 0;
  int j = 0;
  U8 data = 0;
  
  if((pos_x + length) >= COLUMNS)
    return;

  for(j = 0 ; j < _font->height ; j ++)
  {

    disp_SetColumnAddress(pos_x);
    disp_SetPageAddress(pos_y + j);
    
    LCD_ChipSelect();
    LCD_DataMode();

    for(i = 0 ; i < length ; i++)
    {
      data = *character++;

      if(invert)
        data = ~data;

      disp_put_data(data);
      disp_trigger_write();
    }
    LCD_ChipDeselect();
  }
  pos_x += length;
}

void PutStr(FONT_SIZE size, char *str, bool invert, JUSTIFICATION justification)
{
  unsigned int i=0;

  U8 string_length = strwidth(size, str);
  
  switch(justification)
  {
    case JUST_LEFT:
    pos_x = 0;
    break;

    case JUST_RIGHT:
    pos_x = COLUMNS - string_length;
    break;

    case JUST_CENTER:
    pos_x = (COLUMNS - string_length) / 2;
    break;

    case JUST_NONE:

    break;
  }

  do
  {
    PutChar(size, str[i], invert);
    if(size == FONT_SMALL)
      pos_x++;

    i++;
  }while(str[i]!='\0');
}


/*
Local Functions
*/

void paint_line(U8 line)
{
  U8 i;

  disp_SetColumnAddress(0);
  disp_SetPageAddress(line);

  LCD_ChipSelect();
  LCD_DataMode();

  for(i = 0; i < COLUMNS; i++)
  {
    disp_put_data(ram_buffer[i][line]);
    disp_trigger_write();
  }

  LCD_ChipDeselect();
}

void paint_display(void)
{
  U8 i;

  for(i = 0; i < PAGES; i++)
  {
    paint_line(i);
  }
}

U8 strwidth(FONT_SIZE size, char *str)
{
  switch(size)
  {
    case FONT_SMALL:
      return string_width(&small, str);

    case FONT_MEDIUM:
      return string_width(&medium, str);

    case FONT_LARGE:
      return string_width(&large, str);

    default:
      return 0;
  }
}