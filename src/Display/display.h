/*
 * display.h
 *
 * Created: 2016/10/13 10:27:24 AM
 *  Author: Philip
 */ 


#ifndef DISPLAY_H_
#define DISPLAY_H_


#define LCD_COLUMNS   128
#define LCD_PAGES     8

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

extern void display_init(void);
extern void ClearScreen(bool invert);
extern void ClearLine(U8 line, bool invert);
extern void GoToXY(U8 x, U8 y);

extern void PutChar(unsigned char c, bool invert);
extern void PutStr(char *str, bool invert, JUSTIFICATION justification);

#endif /* DISPLAY_H_ */