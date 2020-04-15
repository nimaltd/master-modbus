#ifndef _M_MODBUS_H_
#define _M_MODBUS_H_


/*
  Author:     Nima Askari
  WebSite:    http://www.github.com/NimaLTD
  Instagram:  http://instagram.com/github.NimaLTD
  Youtube:    https://www.youtube.com/channel/UCUhY7qY1klJm1d2kulr9ckw
  
  Version:    1.0.0
  
  Reversion History:
  
  (1.0.0)
  First release.
*/ 

#include  "mmodbusConfig.h"
#include  "usart.h"
#include  <stdbool.h>
#include  <string.h>
#include  <stdio.h>

#if _MMODBUS_FREERTOS == 1
#include  "cmsis_os.h"
#define   mmodbus_delay(x)  osDelay(x)
#else
#define   mmodbus_delay(x)  HAL_Delay(x)
#endif

//##################################################################################################

typedef enum
{
  MModbusCMD_ReadCoilStatus = 1,
  MModbusCMD_ReadDiscreteInputs = 2,
  MModbusCMD_ReadHoldingRegisters = 3,
  MModbusCMD_ReadInputRegisters = 4,
  MModbusCMD_WriteSingleCoil = 5,
  MModbusCMD_WriteSingleRegister = 6,
  MModbusCMD_WriteMultipleCoils = 15,
  MModbusCMD_WriteMultipleRegisters = 16
  
}MModbusCMD_t;

typedef struct
{
  uint16_t  rxIndex;  
  uint8_t   rxBuf[_MMODBUS_RXSIZE];
  uint32_t  rxTime;
  uint8_t   txBusy;
  #if (_MMODBUS_TXDMA == 1)
  uint8_t   txDmaDone;
  #endif
  uint32_t  timeout;  
  
}MModBus_t;

//##################################################################################################

void    mmodbus_callback(void);
void    mmodbus_callback_txDMA(void);
bool    mmodbus_init(uint32_t setTimeout);

//  coils numbers 00001 to 09999
bool    mmodbus_readCoil(uint8_t slaveAddress, uint16_t number_0_to_9998, uint8_t *coilStatus);
bool    mmodbus_readCoils(uint8_t slaveAddress, uint16_t startNumber_0_to_9998, uint16_t length, uint8_t *coilsStatus);
//  discrete input 10001 to 19999
bool    mmodbus_readDiscreteInput(uint8_t slaveAddress, uint16_t number_0_to_9998, uint8_t *discreteInput);
bool    mmodbus_readDiscreteInputs(uint8_t slaveAddress, uint16_t startNumber_0_to_9998, uint16_t length, uint8_t *discreteInputs);
//  input register 30001 to 39999
bool    mmodbus_readInputRegister(uint8_t slaveAddress, uint16_t number_0_to_9998, uint16_t *inputRegister);
bool    mmodbus_readInputRegisters(uint8_t slaveAddress, uint16_t startNumber_0_to_9998, uint16_t length, uint16_t *inputRegisters);
//  holding register 40001 to 49999
bool    mmodbus_readHoldingRegister(uint8_t slaveAddress, uint16_t number_0_to_9998, uint16_t *holdingRegister);
bool    mmodbus_readHoldingRegisters(uint8_t slaveAddress, uint16_t startNumber_0_to_9998, uint16_t length, uint16_t *holdingRegisters);
// coils numbers 00001 to 09999
bool    mmodbus_writeCoil(uint8_t slaveAddress, uint16_t number_0_to_9998, uint8_t coilStatus);
//  holding register 40001 to 49999
bool    mmodbus_writeHoldingRegister(uint8_t slaveAddress, uint16_t number_0_to_9998, uint16_t holdingRegister);

//##################################################################################################
#endif
