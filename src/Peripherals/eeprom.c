/*
 * eeprom.c
 *
 * Created: 2016/10/11 7:27:14 PM
 *  Author: philb
 */ 

 #include <asf.h>

 static const U8 DeviceAddress = 0xA0;
 static const U16 MemorySize = 1024;

EEPROM_RESULT EEPROM_Write(U16 address, U8 *data, U8 length)
{
  U8 address_upper = DeviceAddress | (U8)((address & 0x0300) >> 7);
  U8 address_lower = (U8)(address & 0xFF);

  if(length == 0)
    return EEPROM_ERROR_PARAMATER;

  if((address + length) > MemorySize)
    return EEPROM_BOUNDARY_ERROR;
  
  U8 *buffer = (U8 *)malloc(1 + length);
  if(buffer == NULL)
    return EEPROM_ERROR_MEMORY;

  buffer[0] = address_lower;
  memcpy(&buffer[1], data, length);

  EEPROM_RESULT res = EEPROM_OKAY;

  struct i2c_master_packet packet = {
    .address     = address_upper,
    .data_length = length + 1,
    .data        = data,
    .ten_bit_address = false,
    .high_speed      = false,
    .hs_master_code  = 0x0,
  };

  EEWP_DISABLE();
  if(!i2c_master_write_packet_wait(&i2c_master_instance, &packet))
  {
    res = EEPROM_ERROR_WRITING;
  }
  EEWP_ENABLE();

  free(buffer);

  return res;
}

EEPROM_RESULT EEPROM_Read(U16 address, U8 length, U8 *data)
{
  U8 address_upper = DeviceAddress | (U8)((address & 0x0300) >> 7);
  U8 address_lower = (U8)(address & 0xFF);

  if(length == 0)
  return EEPROM_ERROR_PARAMATER;

  if((address + length) > MemorySize)
  return EEPROM_BOUNDARY_ERROR;
  
  EEPROM_RESULT res = EEPROM_OKAY;

  struct i2c_master_packet packet = {
    .address     = address_upper,
    .data_length = 1,
    .data        = &address_lower,
    .ten_bit_address = false,
    .high_speed      = false,
    .hs_master_code  = 0x0,
  };
  
  if(!i2c_master_write_packet_wait(&i2c_master_instance, &packet))
  {
    res = EEPROM_ERROR_ADDRESS;
  }

  packet.data = data;
  packet.data_length = length;

  if(!i2c_master_read_packet_wait(&i2c_master_instance, &packet))
  {
    res = EEPROM_ERROR_READING;
  }

  return res;
}

EEPROM_RESULT EEPROM_WriteAndVerify(U16 address, U8 length, U8 *data)
{
  U8 i = 0;
  EEPROM_RESULT res = EEPROM_OKAY;
  if((address + length) > MemorySize)
  return EEPROM_BOUNDARY_ERROR;

  U8 *temp = (U8 *)malloc(length);
  if(temp == NULL)
  return EEPROM_ERROR_MEMORY;

  memset(temp, 0, length);

  res = EEPROM_Write(address, data, length);

  if(res != EEPROM_OKAY)
  return res;

//  _delay_ms(500);

  res = EEPROM_Read(address, length, temp);

  if(res != EEPROM_OKAY)
  return res;

  for(i = 1; i < length ; i++)
  {
    if(temp[i] != data[i])
    {
      memcpy(data, temp, length);
      return EEPROM_ERROR_DATA_MISMATCH;
    }
  }

  return res;
}

bool EEPROM_Test(void)
{
  U8 buffer[10];
  memset(buffer, 0xAA, 10);

  EEPROM_Read(0, 10, buffer);

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

  if(EEPROM_WriteAndVerify(0x0000, 10, buffer) != EEPROM_OKAY)
  return false;

  return true;
}
