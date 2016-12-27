/*
* Desktop.h
*
* Created: 2016/09/05 2:31:22 PM
* Author: Philip
*/


#ifndef __DESKTOP_H__
#define __DESKTOP_H__


typedef enum
{
  DSKTP_CMD_RESET,
  DSKTP_CMD_REFRESH,
  DSKTP_CMD_UPDATE_VOLTAGE,
}DESKTOP_CMD;

typedef struct  
{
  DESKTOP_CMD command;
  U8 param[0xFF];
}DESKTOP_MESSAGE;

Queue queue_desktop_command;

extern void desktop_FSM(bool reset);

#endif //__DESKTOP_H__
