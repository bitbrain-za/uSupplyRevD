/*
 * eeprom.h
 *
 * Created: 2016/10/11 7:27:03 PM
 *  Author: philb
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

typedef enum
{
  EEPROM_OKAY,
  EEPROM_BOUNDARY_ERROR,
  EEPROM_ERROR_ADDRESS,
  EEPROM_ERROR_READING,
  EEPROM_ERROR_WRITING,
  EEPROM_ERROR_PARAMATER,
  EEPROM_ERROR_MEMORY,
  EEPROM_ERROR_DATA_MISMATCH
}EEPROM_RESULT;


extern EEPROM_RESULT EEPROM_Write(U16 address, U8 *data, U8 length);
extern EEPROM_RESULT EEPROM_Read(U16 address, U8 length, U8 *data);
extern EEPROM_RESULT EEPROM_WriteAndVerify(U16 address, U8 length, U8 *data);
extern bool EEPROM_Test(void);

#endif /* EEPROM_H_ */