#define CONFIG_C_

/*
 * config.c
 *
 * Created: 2017/01/03 2:20:41 PM
 *  Author: Philip
 */ 

 #include "../system.h"

/*
    ____       _____       _ __  _
   / __ \___  / __(_)___  (_) /_(_)___  ____  _____
  / / / / _ \/ /_/ / __ \/ / __/ / __ \/ __ \/ ___/
 / /_/ /  __/ __/ / / / / / /_/ / /_/ / / / (__  )
/_____/\___/_/ /_/_/ /_/_/\__/_/\____/_/ /_/____/

*/
typedef union
{
  struct  
  {
    U8 reserved               : 4;
    U8 voltage_adj            : 3;
    U8 fine_adjustment        : 1;
    U8 current_limit_enabledd : 1;
    U8 on                     : 1;
  }bit;
  U8 byte;
}config;


/*
    ____             __        __
   / __ \_________  / /_____  / /___  ______  ___  _____
  / /_/ / ___/ __ \/ __/ __ \/ __/ / / / __ \/ _ \/ ___/
 / ____/ /  / /_/ / /_/ /_/ / /_/ /_/ / /_/ /  __(__  )
/_/   /_/   \____/\__/\____/\__/\__, / .___/\___/____/
                               /____/_/
*/

bool b_format(void);

/*
   ________      __          __   _    __           _       __    __
  / ____/ /___  / /_  ____ _/ /  | |  / /___ ______(_)___ _/ /_  / /__  _____
 / / __/ / __ \/ __ \/ __ `/ /   | | / / __ `/ ___/ / __ `/ __ \/ / _ \/ ___/
/ /_/ / / /_/ / /_/ / /_/ / /    | |/ / /_/ / /  / / /_/ / /_/ / /  __(__  )
\____/_/\____/_.___/\__,_/_/     |___/\__,_/_/  /_/\__,_/_.___/_/\___/____/

*/

config system_config;
U8 saved_config;
U16 saved_voltage;
U16 saved_current_limit;

/*
    ______     __                        __   ______                 __  _
   / ____/  __/ /____  _________  ____ _/ /  / ____/_  ______  _____/ /_(_)___  ____  _____
  / __/ | |/_/ __/ _ \/ ___/ __ \/ __ `/ /  / /_  / / / / __ \/ ___/ __/ / __ \/ __ \/ ___/
 / /____>  </ /_/  __/ /  / / / / /_/ / /  / __/ / /_/ / / / / /__/ /_/ / /_/ / / / (__  )
/_____/_/|_|\__/\___/_/  /_/ /_/\__,_/_/  /_/    \__,_/_/ /_/\___/\__/_/\____/_/ /_/____/

*/

bool IsVoltageOn(void) { return (0x1 == system_config.bit.on); }
void TurnVoltageOn(void) { system_config.bit.on = 0x1; }
void TurnVoltageOff(void) { system_config.bit.on = 0x0; }
bool IsCurrentLimitEnabled(void) { return (0x1 == system_config.bit.on); }
void SetCurrentLimitEnabled(void) { system_config.bit.current_limit_enabledd = 0x1; } 
void SetCurrentLimitDisabled(void) { system_config.bit.current_limit_enabledd = 0x0; } 
bool IsAdjustmentFine(void) { return (0x1 == system_config.bit.fine_adjustment); } 
void SetAdjustmentCoars(void) { system_config.bit.fine_adjustment = 0x0; } 
void SetAdjustmentFine(void) { system_config.bit.fine_adjustment = 0x1; } 
VOLTAGE_ADJ GetVoltageAdjust(void){ return (VOLTAGE_ADJ)system_config.bit.voltage_adj; }
void SetVoltageAdjust(VOLTAGE_ADJ val) { system_config.bit.voltage_adj = val; }

bool b_config_load(void)
{
  U16 temp = 0x0000;

  if(STATUS_OK != EEPROM_Read(ADDR_PATTERN, 2, (U8 *)&temp))
    return false;

  if(0xAAAA != temp)
  {
    if(!b_format())
      return false;
  }

  if(STATUS_OK != EEPROM_Read(ADDR_CONFIG, 1, &(saved_config)))
    return false;

  if(STATUS_OK != EEPROM_Read(ADDR_VOLTAGE, 2, (U8 *)&saved_voltage))
    return false;

  if(STATUS_OK != EEPROM_Read(ADDR_CURRENT, 2, (U8 *)&saved_voltage))
    return false;

  eus_ram_voltage_setting = saved_voltage;
  eus_ram_current_setting = saved_current_limit;
  system_config.byte = saved_config;

  return true;
}

bool b_config_save(void)
{
  if(saved_config!= system_config.byte)
  {
    if(STATUS_OK != EEPROM_WriteAndVerify(ADDR_CONFIG, 1, &system_config.byte))
      return false;

    saved_config= system_config.byte;
  }

  if(saved_voltage != eus_ram_voltage_setting)
  {
    if(STATUS_OK != EEPROM_WriteAndVerify(ADDR_VOLTAGE, 2, (U8 *)&eus_ram_voltage_setting))
      return false;

    saved_voltage = eus_ram_voltage_setting;
  }

  if(saved_current_limit != eus_ram_current_setting)
  {
    if(STATUS_OK != EEPROM_WriteAndVerify(ADDR_CURRENT, 2, (U8 *)&eus_ram_current_setting))
      return false;

    saved_current_limit = eus_ram_current_setting;
  }

  return true;
}

/*
    __                     __   ______                 __  _
   / /   ____  _________ _/ /  / ____/_  ______  _____/ /_(_)___  ____  _____
  / /   / __ \/ ___/ __ `/ /  / /_  / / / / __ \/ ___/ __/ / __ \/ __ \/ ___/
 / /___/ /_/ / /__/ /_/ / /  / __/ / /_/ / / / / /__/ /_/ / /_/ / / / (__  )
/_____/\____/\___/\__,_/_/  /_/    \__,_/_/ /_/\___/\__/_/\____/_/ /_/____/

*/

bool b_format(void)
{
  U16 temp;

  eus_ram_voltage_setting = 0x00;
  eus_ram_current_setting = 0x00;
  system_config.bit.on = true;
  system_config.bit.fine_adjustment = false;
  system_config.bit.voltage_adj = VOLTAGE_USER;
  system_config.bit.reserved = 0x00;

  saved_config= 0xFF;
  saved_current_limit = 0xFFFF;
  saved_voltage = 0xFFFF;

  if(b_config_save())
  {
    temp = 0xAAAA;
      
    if(STATUS_OK != EEPROM_WriteAndVerify(ADDR_PATTERN, 2, (U8 *)&temp))
      return false;
  }
   return true;
}