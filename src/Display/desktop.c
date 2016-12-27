/* 
* Desktop.cpp
*
* Created: 2016/09/05 2:31:21 PM
* Author: Philip
*/


#include "../system.h"

typedef enum
{
  DISP_DESKTOP,
  DISP_MENU,
}DESKTOP_STATE;

typedef enum
{
  FUNC_CURRENT_LIM,
  FUNC_CURRENT_SENSE,
  FUNC_TIMER,
}FUNCTIONS;

void DisplayDesktop();
void v_display_voltage(U16 voltage);

void desktop_FSM(bool reset)
{
  char str[64];
  HMI_MESSAGE hmi_msg;
  static DESKTOP_STATE state = DISP_DESKTOP;
  DESKTOP_CMD cmd;
  DESKTOP_MESSAGE message;

  U16 us_temp;

  if(reset)
  {
    display_init();
    ClearScreen(false);
    state = DISP_DESKTOP;
    b_queue_init(&queue_desktop_command, sizeof(DESKTOP_MESSAGE), 1);
  }

  while(b_queue_read(&queue_HMI_input, &hmi_msg))
  {
    switch(hmi_msg.source)
    {
      case HMI_ENC_V:
         sprintf(str, "%d.%02d V", hmi_msg.value/1000, (hmi_msg.value%1000) / 10);
         GoToXY(0, 0);
         PutStr(str, false, JUST_CENTER);
      break;

      case HMI_ENC_C:
         sprintf(str, "%d.%02d A", hmi_msg.value/1000, (hmi_msg.value%1000) / 10);
         GoToXY(0, 4);
         PutStr(str, false, JUST_CENTER);
      break;
    }
  }

  if(b_queue_read(&queue_desktop_command, &message))
  {
    switch(message.command)
    {
      case DSKTP_CMD_REFRESH:
      switch(state)
      {
        case DISP_DESKTOP:
        break; 
      }
      break;

      case DSKTP_CMD_UPDATE_VOLTAGE:
      memcpy(&us_temp, message.param, 2);
      v_display_voltage(us_temp);
      break;
    }
  }

  switch(state)
  {
    case DISP_DESKTOP:
      DisplayDesktop();
      break;
  }
}

void ClearVoltageArea(bool invert)
{
  ClearLine(0, invert);
  ClearLine(1, invert);
  ClearLine(2, invert);
}

void ClearSecondArea(bool invert)
{
  ClearLine(3, invert);
  ClearLine(4, invert);
  ClearLine(5, invert);
  ClearLine(6, invert);
}

void desktop_ClearMenuBar(bool invert)
{
  ClearLine(7, invert);
}

void v_display_voltage(U16 voltage)
{
  char str[32];

  SetFont(FONT_MEDIUM);
  
  ClearVoltageArea(false);
  sprintf(str, "%d.%02d V", voltage/1000, (voltage%1000) / 10);
  GoToXY(0, 0);
  PutStr(str, false, JUST_CENTER);
}

void DisplayDesktop()
{
  char str[32];

  SetFont(FONT_MEDIUM);

 
  /*
  Function Area
  */
  /*
  if(CurrentLimit::Dirty())
  {
    ClearSecondArea(false);
    sprintf(str, "%d.%03d A", config.ram_current/1000, (config.ram_current%1000));
    GoToXY(0, 4);
    PutStr(str, false, JUST_CENTER);
    current_changed = false;
  }
  */

  /*
    Menu Bar
  */
 /* 
  if(menu_changed)
  {
    ClearMenuBar(false);

    SetFont(FONT_SMALL);
    
    sprintf(str, "Menu");
    DrawButton(0, str, false);
    
    sprintf(str, "Amps");
    DrawButton(1, str, disp_limit);
    
    sprintf(str, "Timer");
    DrawButton(2, str, false);

    sprintf(str, "Limit");
    DrawButton(3, str, false);

    menu_changed = false;
  }
  */
}

