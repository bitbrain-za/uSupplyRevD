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

#include "Peripherals/DAC.h"
#include "Peripherals/eeprom.h"
#include "Peripherals/INA219.h"

extern struct i2c_master_module i2c_master_instance;


#endif /* SYSTEM_H_ */