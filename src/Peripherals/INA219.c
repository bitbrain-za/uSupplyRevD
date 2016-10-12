/*
 * INA219.c
 *
 * Created: 2016/10/11 8:42:04 PM
 *  Author: philb
 */ 
#include "../system.h"


/*
Variables
*/
static const U8 DeviceAddress = 0x80;

/*
Prototypes
*/
U16 ReadRegister(INA219_REGISTER reg);
bool WriteRegister(INA219_REGISTER reg, U16 data);
bool Configure(void);
bool SetGain(PGA_GAIN pga_gain);

/*
External Functions
*/
void INA219_init(BUS_VOLTAGE brng, ADC_SETTING adc_setting, OPERATING_MODE operating_mode)
{
  bus_voltage = brng;
  adc = adc_setting;
  mode = operating_mode;
  gain = PGA_GAIN_DIV8;

  Configure();
}

bool Reset(void) 
{ 
  return WriteRegister(INA219_CONFIG, 0x8000); 
}

S16 Current()
{
  S16 shunt_register = ReadRegister(INA219_SHUNT_VOLTAGE);
  return shunt_register;
}

/*
Local Functions
*/
bool Configure()
{
  U16 config = 0x00;

  config |= bus_voltage << 13;
  config |= gain << 11;
  config |= adc << 3;
  config |= mode;

  return WriteRegister(INA219_CONFIG, config);
}
bool SetGain(PGA_GAIN pga_gain)
{
  gain = pga_gain;
  return Configure();
}

U16 ReadRegister(INA219_REGISTER reg)
{
  U8 buffer[2];


  struct i2c_master_packet packet = {
    .address     = DeviceAddress,
    .data_length = 1,
    .data        = buffer,
    .ten_bit_address = false,
    .high_speed      = false,
    .hs_master_code  = 0x0,
  };

  if(!i2c_master_write_packet_wait(&i2c_master_instance, &packet))
    return false;

  packet.data = buffer;
  packet.data_length = 2;

  if(!i2c_master_read_packet_wait(&i2c_master_instance, &packet))
    return false;

  U16 val = buffer[0] << 8;
  val |= buffer[1];

  return val;
}

bool WriteRegister(INA219_REGISTER reg, U16 data)
{
  if( !((reg == INA219_CONFIG) || (reg == INA219_CALIBRATION)) )
  return false;
  
  U8 buffer[3];
  buffer[0] = (U8)reg;
  buffer[1] = ((data >> 8) & 0xFF);
  buffer[2] = (data & 0xFF);

  struct i2c_master_packet packet = {
    .address     = DeviceAddress,
    .data_length = 3,
    .data        = buffer,
    .ten_bit_address = false,
    .high_speed      = false,
    .hs_master_code  = 0x0,
  };

  return !i2c_master_write_packet_wait(&i2c_master_instance, &packet);
}
