/* 
* Desktop.cpp
*
* Created: 2016/09/05 2:31:21 PM
* Author: Philip
*/


#include "../system.h"

/*
    ____       _____       _ __  _
   / __ \___  / __(_)___  (_) /_(_)___  ____  _____
  / / / / _ \/ /_/ / __ \/ / __/ / __ \/ __ \/ ___/
 / /_/ /  __/ __/ / / / / / /_/ / /_/ / / / (__  )
/_____/\___/_/ /_/_/ /_/_/\__/_/\____/_/ /_/____/

*/

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

/*
    ____             __        __
   / __ \_________  / /_____  / /___  ______  ___  _____
  / /_/ / ___/ __ \/ __/ __ \/ __/ / / / __ \/ _ \/ ___/
 / ____/ /  / /_/ / /_/ /_/ / /_/ /_/ / /_/ /  __(__  )
/_/   /_/   \____/\__/\____/\__/\__, / .___/\___/____/
                               /____/_/
*/

void DisplayDesktop();
void v_display_voltage(U16 voltage);
void v_draw_grid(void);
void v_draw_menu_bar(void);
/*

    ______     __                        __   ______                 __  _
   / ____/  __/ /____  _________  ____ _/ /  / ____/_  ______  _____/ /_(_)___  ____  _____
  / __/ | |/_/ __/ _ \/ ___/ __ \/ __ `/ /  / /_  / / / / __ \/ ___/ __/ / __ \/ __ \/ ___/
 / /____>  </ /_/  __/ /  / / / / /_/ / /  / __/ / /_/ / / / / /__/ /_/ / /_/ / / / (__  )
/_____/_/|_|\__/\___/_/  /_/ /_/\__,_/_/  /_/    \__,_/_/ /_/\___/\__/_/\____/_/ /_/____/

*/

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

      default:
        v_draw_menu_bar();        
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

/*
    __                     __   ______                 __  _
   / /   ____  _________ _/ /  / ____/_  ______  _____/ /_(_)___  ____  _____
  / /   / __ \/ ___/ __ `/ /  / /_  / / / / __ \/ ___/ __/ / __ \/ __ \/ ___/
 / /___/ /_/ / /__/ /_/ / /  / __/ / /_/ / / / / /__/ /_/ / /_/ / / / (__  )
/_____/\____/\___/\__,_/_/  /_/    \__,_/_/ /_/\___/\__/_/\____/_/ /_/____/

*/

void ClearVoltageArea(bool invert)
{
  ClearLine(0, invert);
  ClearLine(1, invert);
  ClearLine(2, invert);
}

void ClearSecondArea(bool invert)
{
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

void v_draw_grid(void)
{
  v_disp_draw_line(0, 28, LCD_COLUMNS - 1, 28, false);
  v_disp_draw_line(0, 55, LCD_COLUMNS - 1, 55, false);
}

void v_draw_menu_bar(void)
{
  /* Btn 1 - On/Off */
  /* Btn 2 - 3.3V 5V ADJ */
  /* Btn 3 - C-Limit */
  /* Btn 4 - Zero */

  if(eb_voltage_on)
    v_display_draw_button(0, 55, 32, 9, "On", false);
  else
    v_display_draw_button(0, 55, 32, 9, "Off", false);

  switch(ex_voltage_adj)
  {
    case VOLTAGE_3V3:
      v_display_draw_button(32, 55, 32, 9, "3V3", false);
    break;

    case VOLTAGE_5V0:
      v_display_draw_button(32, 55, 32, 9, "5V0", false);
    break;

    case VOLTAGE_USER:
      v_display_draw_button(32, 55, 32, 9, "User", false);
    break;
  }

  v_display_draw_button(64, 55, 32, 9, "LIM", eb_current_limit_enabled);
  v_display_draw_button(96, 55, 32, 9, "Zero", false);
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

