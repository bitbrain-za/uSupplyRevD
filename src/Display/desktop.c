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
  DISP_VOLTAGE_SET,
  DISP_CURRENT_SET,
  DISP_MENU,
}DESKTOP_STATE;

typedef enum
{
  FUNC_CURRENT_LIM,
  FUNC_CURRENT_SENSE,
  FUNC_TIMER,
}FUNCTIONS;

TIMER_HANDLE timer_disp;

/*
    ____             __        __
   / __ \_________  / /_____  / /___  ______  ___  _____
  / /_/ / ___/ __ \/ __/ __ \/ __/ / / / __ \/ _ \/ ___/
 / ____/ /  / /_/ / /_/ /_/ / /_/ /_/ / /_/ /  __(__  )
/_/   /_/   \____/\__/\____/\__/\__, / .___/\___/____/
                               /____/_/
*/

void ClearVoltageArea(bool invert);
void ClearSecondArea(bool invert);
void desktop_ClearMenuBar(bool invert);
void DisplayDesktop(void);
void v_display_voltage(U16 voltage);
void v_display_current(U16 milliamps);
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
  HMI_MESSAGE hmi_msg;
  static DESKTOP_STATE state = DISP_DESKTOP;

  if(reset)
  {
    display_init();
    ClearScreen(false);
    state = DISP_DESKTOP;
    v_draw_menu_bar();
    timer_disp = timer_new(500);
    timer_reset(timer_disp);
    return;
  }

  while(b_queue_read(&queue_HMI_input, &hmi_msg))
  {
    switch(hmi_msg.source)
    {
      case HMI_ENC_V:
        timer_restart(timer_disp, 1000);
        state = DISP_VOLTAGE_SET;
        v_display_voltage(hmi_msg.value);
        /*
        sprintf(str, "%d.%02d V", hmi_msg.value/1000, (hmi_msg.value%1000) / 10);
        GoToXY(0, 0);
        PutStr(str, false, JUST_CENTER);
        */
      break;

      case HMI_ENC_C:
        timer_restart(timer_disp, 1000);
        state = DISP_CURRENT_SET;
        v_display_current(hmi_msg.value);
        /*
         sprintf(str, "%d.%02d A", hmi_msg.value/1000, (hmi_msg.value%1000) / 10);
         GoToXY(0, 4);
         PutStr(str, false, JUST_CENTER);
         */
      break;

      default:
        v_draw_menu_bar();        
      break;
    }
  }

  switch(state)
  {
    case DISP_DESKTOP:
      if(timer_expired(timer_disp))
      {
        DisplayDesktop();
        timer_reset(timer_disp);
      }
    break;

    case DISP_CURRENT_SET:
    case DISP_VOLTAGE_SET:
    if(timer_expired(timer_disp))
    {
      state = DISP_DESKTOP;
      timer_restart(timer_disp, 500);
    }
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

void v_display_voltage(U16 millivolts)
{
  char str[32];

  SetFont(FONT_MEDIUM);
  
  ClearVoltageArea(false);
  sprintf(str, "%d.%02d V", millivolts/1000, (millivolts%1000) / 10);
  GoToXY(0, 0);
  PutStr(str, false, JUST_CENTER);
  v_paint_pages(0x07);
}

void v_display_current(U16 uamps)
{
  char str[32];

  SetFont(FONT_MEDIUM);
  
  ClearSecondArea(false);
  sprintf(str, "%d.%03d mA", uamps/1000, (uamps%1000));
  GoToXY(0, 4);
  PutStr(str, false, JUST_CENTER);
  v_paint_pages(0x38);
}

void v_draw_menu_bar(void)
{
  /* Btn 1 - On/Off */
  /* Btn 2 - 3.3V 5V ADJ */
  /* Btn 3 - C-Limit */
  /* Btn 4 - Zero */


  SetFont(FONT_SMALL);

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

void DisplayDesktop(void)
{
  SetFont(FONT_MEDIUM);

  v_display_voltage(eus_output_voltage);
  v_disp_draw_line(0, 28, LCD_COLUMNS - 1, 28, false);
  v_display_current(eus_output_current_ua);
  v_draw_menu_bar();
  v_paint_pages(0xC0);
 
  /*
  Function Area
  */
  /*
  if(CurrentLimit::Dirty())
  {
    ClearSecondArea(false);
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

