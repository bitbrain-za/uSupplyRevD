/*
 * system.h
 *
 * Created: 2016/10/11 7:13:58 PM
 *  Author: philb
 */ 


#ifndef SYSTEM_H_
#define SYSTEM_H_


#include <stdlib.h>
#include <string.h>
#include <asf.h>

#include "Core/adc.h"
#include "Core/delay.h"
#include "Core/i2c.h"
#include "Core/qms.h"
#include "Core/hmi.h"
#include "Core/timer.h"

#include "Console/console.h"

#include "Display/fonts.h"
#include "Display/display.h"
#include "Display/desktop.h"

#include "Peripherals/DAC.h"
#include "Peripherals/eeprom.h"
#include "Peripherals/INA219.h"

extern struct i2c_master_module i2c_master_instance;

#endif /* SYSTEM_H_ */