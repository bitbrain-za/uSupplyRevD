/*
 * fonts.c
 *
 * Created: 2016/10/13 2:02:14 PM
 *  Author: Philip
 */ 

#include "../system.h"
#include "fonts.h"

U8 *get_char(Font *ptr_font, char c)
{
  U8 *ptr = (U8 *)ptr_font->charmap[(U8)c - 0x20];
  return (ptr);
}

U8 char_width(Font *ptr_font, char c)
{
  U8 *ptr = (U8 *)ptr_font->charmap[(U8)c - 0x20];
  return ptr[0];
}

U8 string_width(Font *ptr_font, char *str)
{
  unsigned int i=0;
  U8 length = 0;

  do
  {
    length += char_width(ptr_font, str[i]);
    i++;
  }while(str[i]!='\0');

  return length;
}