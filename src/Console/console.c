/*
 * console.c
 *
 * Created: 2016/12/23 1:43:59 PM
 *  Author: philb
 */ 
#include "../system.h"
#include <stdarg.h>

typedef struct
{
  const char* cmd;
  const char* help;

  CONSOLE_COMMAND_RESULTS (*function)(char *param);
}console_command;

CONSOLE_COMMAND_RESULTS echo(char* param);
CONSOLE_COMMAND_RESULTS help(char* cmd);
CONSOLE_COMMAND_RESULTS voltage(char* param);
CONSOLE_COMMAND_RESULTS temperature(char* param);

console_command table[] = 
{
  {"help", "Displays whole table", &help},
  {"echo", "Echoes the input text", &echo},
  {"volt", "Prints the output voltage", &voltage},
  {"temp", "Prints the temperature in degrees C", &temperature}
};

ERROR_LEVEL debug_level;

CONSOLE_COMMAND_RESULTS execute(char* cmd, char* param);
bool find(char* cmd, console_command *result);
CONSOLE_COMMAND_RESULTS run_cmd(char* command);
CONSOLE_STATE state;

/* console functions */

/*
    ____        __    ___         __  ___     __  __              __    
   / __ \__  __/ /_  / (_)____   /  |/  /__  / /_/ /_  ____  ____/ /____
  / /_/ / / / / __ \/ / / ___/  / /|_/ / _ \/ __/ __ \/ __ \/ __  / ___/
 / ____/ /_/ / /_/ / / / /__   / /  / /  __/ /_/ / / / /_/ / /_/ (__  ) 
/_/    \__,_/_.___/_/_/\___/  /_/  /_/\___/\__/_/ /_/\____/\__,_/____/  
                                                                        
*/

void v_console_init(void)
{
  debug_level = ERR_LEVEL_2_MINOR;
}

void printff(const char *format, ...)
{
  char buffer[0xFF];
  char *ptr_str = buffer;

  va_list argptr;
  va_start(argptr, format);

  vsprintf(buffer, format, argptr);
  va_end(argptr);

  while('\0' != *ptr_str)
  {
    udi_cdc_putc(*ptr_str++);
  }

}

void debug_message(MODULES module, const char *format, ...)
{
  char buffer[0xFF];
  va_list argptr;
  va_start(argptr, format);

  vsprintf(buffer, format, argptr);
  va_end(argptr);

  switch(module)
  {
    case MODULE_MAIN:
      printff("Main: %s\n", buffer);
    break;

    case MODULE_UART485:
    if(DEBUG_485)
    {
      printff("485: %s\n", buffer);
    }
    break;

    case MODULE_GPRS:
    if(DEBUG_GPRS)
    {
      printff("GPRS: %s\n", buffer);
    }
    break;

    case MODULE_RTC:
    if(DEBUG_RTC)
    {
      printff("RTC: %s\n", buffer);
    }
    break;

    case MODULE_MODBUS:
    if(DEBUG_MODBUS)
    {
      printff("MODBUS: %s\n", buffer);
    }
    break;

    case MODULE_CONSOLE:
    if(DEBUG_CONSOLE)
    {
      printff("COONSOLE: %s\n", buffer);
    }
    break;

    case MODULE_AMR:
    if(DEBUG_AMR)
    {
      printff("AMR: %s\n", buffer);
    }
    break;

    default:
      printff("Unknown: %s\n", buffer);
    break;
  }
}

void error_message(MODULES module, ERROR_LEVEL level, const char *format, ...)
{
  if(display_level == ERR_LEVEL_0_NONE)
    return;

  char buffer[0xFF];
  char buffer2[0xFF];
  va_list argptr;
  va_start(argptr, format);

  vsprintf(buffer, format, argptr);
  va_end(argptr);

  switch(module)
  {
    case MODULE_MAIN:
      sprintf(buffer2, "Main: %s", buffer);
    break;

    case MODULE_UART485:
      sprintf(buffer2, "485: %s", buffer);
    break;

    case MODULE_GPRS:
      sprintf(buffer2, "GPRS: %s", buffer);
    break;

    case MODULE_RTC:
      sprintf(buffer2, "RTC: %s", buffer);
    break;

    case MODULE_AMR:
      sprintf(buffer2, "AMR: %s", buffer);
    break;

    case MODULE_MODBUS:
      sprintf(buffer2, "MODBUS: %s", buffer);
    break;

    default:
      sprintf(buffer2, "Unknown: %s", buffer);
    break;
  }

  switch(level)
  {
    case ERR_LEVEL_0_NONE:
    /* Do nothing, just here to silence warning */
    break;

    case ERR_LEVEL_1_WARNING:
    sprintf(buffer, "WARNING - %s\n", buffer2);
    break;

    case ERR_LEVEL_2_MINOR:
    sprintf(buffer, "ERROR - %s\n", buffer2);
    break;

    case ERR_LEVEL_3_FAILURE:
    sprintf(buffer, "FAILURE - %s\n", buffer2);
    break;
  }

  if(display_level >= level)
  {
    printff(buffer);
  }
}

void console_fsm(bool init)
{
  console_message rx;

  if(init)
  {
    b_queue_init(&queue_cdc_rx, sizeof(console_message), 1);
    return;
  }

  if(b_queue_read(&queue_cdc_rx, &rx))
  {
    execute(rx.command, rx.param);
  }

#if 0
  if (comport.readable())
  {
    ch = comport.getc() & 0x7f;

    if(ch == '\r')
    {
      execute(cmd, param);
      ptr_str = &cmd[0];
      memset(cmd, 0, 0x10);
      memset(param, 0, 0x40);
    }
    else if(ch == ' ')
    {
      ptr_str = &param[0];
    }
    else
    {
      *ptr_str++ = ch;
    }
  }
#endif
}


/*
    ____       _             __          __  ___     __  __              __    
   / __ \_____(_)   ______ _/ /____     /  |/  /__  / /_/ /_  ____  ____/ /____
  / /_/ / ___/ / | / / __ `/ __/ _ \   / /|_/ / _ \/ __/ __ \/ __ \/ __  / ___/
 / ____/ /  / /| |/ / /_/ / /_/  __/  / /  / /  __/ /_/ / / / /_/ / /_/ (__  ) 
/_/   /_/  /_/ |___/\__,_/\__/\___/  /_/  /_/\___/\__/_/ /_/\____/\__,_/____/  
                                                                               
*/

bool find(char* cmd, console_command *result)
{
  int i;
  int count = sizeof(table)/sizeof(console_command);

  debug_message(MODULE_CONSOLE, "Search: %s\n", cmd);

  for(i = 0 ; i < count; i++)
  {
    debug_message(MODULE_CONSOLE, "Compare: %s\n", table[i].cmd);
    if(strcmp(cmd, table[i].cmd) == 0)
    {
      *result = table[i];
      return true;
    }
  }

  return false;
}

CONSOLE_COMMAND_RESULTS execute(char* cmd, char* param)
{
  console_command command;

  if(!find(cmd, &command))
  {
    printff("Command not found\n");
    return CONS_CMD_NOT_FOUND;
  }
  else
  {
    return command.function(param);
  }
}

/*
   ______                       __        ____                 __  _                 
  / ____/___  ____  _________  / /__     / __/_  ______  _____/ /_(_)___  ____  _____
 / /   / __ \/ __ \/ ___/ __ \/ / _ \   / /_/ / / / __ \/ ___/ __/ / __ \/ __ \/ ___/
/ /___/ /_/ / / / (__  ) /_/ / /  __/  / __/ /_/ / / / / /__/ /_/ / /_/ / / / (__  ) 
\____/\____/_/ /_/____/\____/_/\___/  /_/  \__,_/_/ /_/\___/\__/_/\____/_/ /_/____/  
                                                                                     
*/

CONSOLE_COMMAND_RESULTS help(char* param)
{
  int count, i;
  console_command command;

  if(param[0] == 0)
  {
    count = sizeof(table)/sizeof(console_command);

    for(i = 0 ; i < count; i++)
    {
      printff("%s\n", table[i].cmd);
    }
    printff("Done\n");
  }
  else
  {
    if(!find(param, &command))
    {
      printff("Not found\n");
      return CONS_CMD_NOT_FOUND;
    }
    else
    {
      printff("%s - %s\n", command.cmd, command.help);
    }
  }

  return CONS_EXECUTED;
}

CONSOLE_COMMAND_RESULTS echo(char* param)
{
  if(param[0] == 0)
    printff("No parameter\n"); 
  else
    printff("%s\n", param);

  return CONS_EXECUTED;
}

CONSOLE_COMMAND_RESULTS voltage(char* param)
{
  printff("Voltage: %i\n", eus_output_voltage);
  return CONS_EXECUTED;
}

CONSOLE_COMMAND_RESULTS temperature(char* param)
{
  printff("Temperature: %i\n", eus_temperature);
  return CONS_EXECUTED;
}