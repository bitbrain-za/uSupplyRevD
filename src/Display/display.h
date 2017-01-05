/*
 * display.h
 *
 * Created: 2016/10/13 10:27:24 AM
 *  Author: Philip
 */ 


#ifndef DISPLAY_H_
#define DISPLAY_H_


#define LCD_COLUMNS   128
#define LCD_ROWS      64
#define LCD_PAGES     8

#define LCD_RAM_BYTES (U16)(LCD_COLUMNS * LCD_PAGES)

typedef enum
{
  FONT_SMALL,
  FONT_MEDIUM,
  FONT_LARGE
}FONT_SIZE;

typedef enum
{
  JUST_LEFT,
  JUST_CENTER,
  JUST_RIGHT,
  JUST_NONE
}JUSTIFICATION;

extern void v_paint_pages(U8 page_mask);
extern void display_init(void);
extern void ClearScreen(bool invert);
extern void ClearLine(U8 line, bool invert);
extern void GoToXY(U8 x, U8 y);
extern void PutChar(unsigned char c, bool invert);
extern void PutStr(char *str, bool invert, JUSTIFICATION justification);

extern void SetFont(FONT_SIZE size);

extern void v_disp_draw_line(U8 x_start, U8 y_start, U8 x_end, U8 y_end, bool invert);

extern void v_display_draw_button(U8 x, U8 y, U8 width, U8 height, const char *str, bool invert);

#endif /* DISPLAY_H_ */