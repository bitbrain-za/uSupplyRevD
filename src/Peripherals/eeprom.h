/*
 * eeprom.h
 *
 * Created: 2016/10/11 7:27:03 PM
 *  Author: philb
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

extern enum status_code EEPROM_Write(U16 address, U8 *data, U8 length);
extern enum status_code EEPROM_Read(U16 address, U8 length, U8 *data);
extern enum status_code EEPROM_WriteAndVerify(U16 address, U8 length, U8 *data);
extern bool EEPROM_Test(void);

#endif /* EEPROM_H_ */