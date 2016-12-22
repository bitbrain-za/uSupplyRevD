/*
 * system.h
 *
 * Created: 2016/10/11 7:13:58 PM
 *  Author: philb
 */ 


#ifndef SYSTEM_H_
#define SYSTEM_H_

#define VOLTAGE_ENCODER_MAX 127
#define VOLTAGE_ENCODER_MIN -127

#define CURRENT_ENCODER_MAX 127
#define CURRENT_ENCODER_MIN -127

#include <stdlib.h>
#include <string.h>
#include <asf.h>

#include "Core/adc.h"
#include "Core/qms.h"
#include "Core/hmi.h"

#include "Display/display.h"

#include "Peripherals/DAC.h"
#include "Peripherals/eeprom.h"
#include "Peripherals/INA219.h"

extern struct i2c_master_module i2c_master_instance;

#endif /* SYSTEM_H_ */