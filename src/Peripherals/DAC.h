/*
 * DAC.h
 *
 * Created: 2016/10/11 8:33:37 PM
 *  Author: philb
 */ 


#ifndef DAC_H_
#define DAC_H_

// Command definitions
#define MCP47X6_CMD_MASK       0x1F
#define MCP47X6_CMD_VOLDAC     0x00
#define MCP47X6_CMD_VOLALL     0x40
#define MCP47X6_CMD_VOLCONFIG  0x80
#define MCP47X6_CMD_ALL        0x60

typedef enum
{
  DAC_GAIN_1X = 0x0,
  DAC_GAIN_2X = 0x1,
}DAC_GAIN;

typedef enum
{
  VREF_VDD = 0x0,
  VREF_VREFPIN = 0x10,
  VREF_VREFPIN_BUFFERED = 0x18,
}DAC_VREF;


bool WriteConfig(U8 c);
void SetReference(DAC_VREF ref, U16 value);
void SetVoltage(U16 level);

#endif /* DAC_H_ */