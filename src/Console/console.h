#ifndef CONSOLE_H_
#define CONSOLE_H_

#define DEBUG_485 0
#define DEBUG_GPRS 1
#define DEBUG_RTC 0
#define DEBUG_MODBUS 1
#define DEBUG_CONSOLE 0
#define DEBUG_AMR 1

typedef enum
{
  MODULE_MAIN,
  MODULE_UART485,
  MODULE_GPRS,
  MODULE_RTC,
  MODULE_MODBUS,
  MODULE_CONSOLE,
  MODULE_AMR,
}MODULES;

typedef enum
{
  ERR_LEVEL_0_NONE,
  ERR_LEVEL_3_FAILURE,
  ERR_LEVEL_2_MINOR,
  ERR_LEVEL_1_WARNING,
}ERROR_LEVEL;

typedef enum {
  CON_CLOSED,
  CON_OPEN,
  CON_OPEN_SECURE,
}CONSOLE_STATE;

typedef enum
{
  CONS_CMD_NOT_FOUND,
  CONS_INSUFFICIENT_PRIVELEGES,
  CONS_WRONG_STATE,
  CONS_EXECUTED,
}CONSOLE_COMMAND_RESULTS;

typedef struct  
{
  char command[0x10];
  char param[0x40];
}console_message;

Queue queue_cdc_rx;

extern void v_console_init(void);
extern ERROR_LEVEL display_level;

extern void printff(const char *format, ...);
extern void debug_message(MODULES module, const char *format, ...);
extern void error_message(MODULES module, ERROR_LEVEL level, const char *format, ...);
extern void console_fsm(bool init);

#endif