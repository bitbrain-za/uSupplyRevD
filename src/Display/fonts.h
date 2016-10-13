/*
 * fonts.h
 *
 * Created: 2016/10/13 1:58:34 PM
 *  Author: Philip
 */ 


#ifndef FONTS_H_
#define FONTS_H_

typedef struct 
{
  U8 height;
  const U8** charmap;
}Font;


extern U8* get_char(Font *ptr_font, char c);
extern U8 char_width(Font *ptr_font, char c);
extern U8 string_width(Font *ptr_font, char *str);

#endif /* FONTS_H_ */