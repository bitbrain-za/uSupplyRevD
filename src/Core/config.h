/*
 * config.h
 *
 * Created: 2017/01/03 2:20:32 PM
 *  Author: Philip
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_


#define ADDR_PATTERN 0x0000
#define ADDR_CONFIG  ADDR_PATTERN + 2
#define ADDR_VOLTAGE ADDR_CONFIG  + 1
#define ADDR_CURRENT ADDR_VOLTAGE + 2


typedef enum
{
  VOLTAGE_3V3 = 0x0,
  VOLTAGE_5V0 = 0x1,
  VOLTAGE_USER = 0x2,
}VOLTAGE_ADJ;


#ifdef CONFIG_C_
  #define extern
#endif

extern U8 system_flags;

extern bool eb_voltage_on;
extern bool eb_current_limit_enabled;
extern bool eb_fine;

extern VOLTAGE_ADJ ex_voltage_adj;

#ifdef CONFIG_C_
  #undef extern
#endif

extern bool IsVoltageOn(void);
extern void TurnVoltageOn(void);
extern void TurnVoltageOff(void);
extern bool IsCurrentLimitEnabled(void);
extern void SetCurrentLimitEnabled(void);
extern void SetCurrentLimitDisabled(void);
extern bool IsAdjustmentFine(void);
extern void SetAdjustmentCoars(void);
extern void SetAdjustmentFine(void);
extern VOLTAGE_ADJ GetVoltageAdjust(void);
extern void SetVoltageAdjust(VOLTAGE_ADJ val);


extern bool b_config_load(void);
extern bool b_config_save(void);

#endif /* CONFIG_H_ */