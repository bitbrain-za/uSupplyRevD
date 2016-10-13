/*
 * display.h
 *
 * Created: 2016/10/13 10:27:24 AM
 *  Author: Philip
 */ 


#ifndef DISPLAY_H_
#define DISPLAY_H_


#define COLUMNS   128
#define PAGES     8

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

extern void PutChar(FONT_SIZE size, unsigned char c, bool invert);
extern void PutStr(FONT_SIZE size, char *str, bool invert, JUSTIFICATION justification);

#endif /* DISPLAY_H_ */