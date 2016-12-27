/*
 * eeprom.c
 *
 * Created: 2016/10/11 7:27:14 PM
 *  Author: philb
 */ 

#include "../system.h"

static const U8 DeviceAddress = 0xA0;
static const U16 MemorySize = 1024;

enum status_code EEPROM_Write(U16 address, U8 *data, U8 length)
{
  U8 address_upper = DeviceAddress | (U8)((address & 0x0300) >> 7);
  U8 address_lower = (U8)(address & 0xFF);

  if(length == 0)
    return STATUS_ERR_INVALID_ARG;

  if((address + length) > MemorySize)
    return STATUS_ERR_INVALID_ARG;
  
  U8 *buffer = (U8 *)malloc(1 + length);
  if(buffer == NULL)
    return STATUS_ERR_NO_MEMORY;

  EEWP_DISABLE();

  buffer[0] = address_lower;
  memcpy(&buffer[1], data, length);

  enum status_code res = STATUS_OK;

  struct i2c_master_packet packet = {
    .address     = (address_upper >> 1),
    .data_length = length + 1,
    .data        = buffer,
    .ten_bit_address = false,
    .high_speed      = false,
    .hs_master_code  = 0x0,
  };

  res = i2c_master_write_packet_wait(&i2c_master_instance, &packet);

  EEWP_ENABLE();

  free(buffer);

  return res;
}

enum status_code EEPROM_Read(U16 address, U8 length, U8 *data)
{

/*
  struct i2c_master_packet packet = 
  {
    .address     = (DeviceAddress >> 1),
    .data_length = length,
    .data        = data,
    .ten_bit_address = false,
    .high_speed      = false,
  };  
  return eeprom_read_packet(&i2c_master_instance, address, &packet);
*/

  U8 address_upper = DeviceAddress | (U8)((address & 0x0300) >> 7);
  U8 address_lower = (U8)(address & 0xFF);

  enum status_code res = STATUS_OK;

  if(length == 0)
    return STATUS_ERR_INVALID_ARG;

  if((address + length) > MemorySize)
    return STATUS_ERR_BAD_ADDRESS;

  data[0] = address_lower;
  
  struct i2c_master_packet packet = 
  {
    .address     = (address_upper >> 1),
    .data_length = 1,
    .data        = data,
    .ten_bit_address = false,
    .high_speed      = false,
  };
  
  res = i2c_master_write_packet_wait(&i2c_master_instance, &packet);
  
  if(STATUS_OK != res)
  {
    return res;
  }

  packet.data = data;
  packet.data_length = length;

  return i2c_master_read_packet_wait(&i2c_master_instance, &packet);

}

enum status_code EEPROM_WriteAndVerify(U16 address, U8 length, U8 *data)
{
  U8 i = 0;
  enum status_code res = STATUS_OK;

  if((address + length) > MemorySize)
    return STATUS_ERR_BAD_ADDRESS;

  U8 *temp = (U8 *)malloc(length);
  if(temp == NULL)
    return STATUS_ERR_NO_MEMORY;

  memset(temp, 0, length);

  res = EEPROM_Write(address, data, length);

  if(STATUS_OK != res)
  {
    free(temp);
    return res;
  }

//  _delay_ms(500);

  res = EEPROM_Read(address, length, temp);

  if(STATUS_OK != res)
  {
    free(temp);
    return res;
  }

  for(i = 1; i < length ; i++)
  {
    if(temp[i] != data[i])
    {
      memcpy(data, temp, length);
      free(temp);
      return STATUS_ERR_BAD_DATA;
    }
  }

  free(temp);
  return res;
}

bool EEPROM_Test(void)
{
  U8 buffer[10];
  memset(buffer, 0xAA, 10);

  EEPROM_Read(0x0000, 10, buffer);
  
  buffer[0] = 0x01;
  buffer[1] = 0x02;
  buffer[2] = 0x03;
  buffer[3] = 0x04;
  buffer[4] = 0x05;
  buffer[5] = 0x06;
  buffer[6] = 0x07;
  buffer[7] = 0x08;
  buffer[8] = 0x09;
  buffer[9] = 0x0a;

  if(STATUS_OK != EEPROM_WriteAndVerify(0x0000, 10, buffer))
    return false;

  return true;
}
