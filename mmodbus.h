#ifndef _M_MODBUS_H_
#define _M_MODBUS_H_


/*
  Author:     Nima Askari
  WebSite:    http://www.github.com/NimaLTD
  Instagram:  http://instagram.com/github.NimaLTD
  Youtube:    https://www.youtube.com/channel/UCUhY7qY1klJm1d2kulr9ckw
  
  Version:    1.3.2
  
  Reversion History:
  
  (1.3.2)
  Speedup receiver, add write multiple holding
    
  (1.3.1)
  Remove delay in sending function when DMA is disabled
  
  (1.3.0)
  Add 16 bit order
  
  (1.2.1)
  bug fixed.
  
  (1.2.0)
  add read 32bit register order.
  
  (1.1.2)
  fix read holding register.
  
  (1.1.1)
  fix functions name.
  
  (1.1.0)
  add 16bit,32bit,float read.
  
  (1.0.1)
  add delay in receive routin.
  
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

typedef enum
{
  MModBus_16bitOrder_AB = 0,
  MModBus_16bitOrder_BA,
  
}MModBus_16bitOrder_t;

typedef enum
{
  MModBus_32bitOrder_ABCD = 0,
  MModBus_32bitOrder_DCBA,
  MModBus_32bitOrder_BADC,
  MModBus_32bitOrder_CDAB,  
  
}MModBus_32bitOrder_t;

typedef struct
{
  uint16_t              rxIndex;  
  uint8_t               rxBuf[_MMODBUS_RXSIZE];
  uint32_t              rxTime;
  uint8_t               txBusy;
  uint32_t              timeout; 
  MModBus_16bitOrder_t  byteOrder16;
  MModBus_32bitOrder_t  byteOrder32;
  #if (_MMODBUS_TXDMA == 1)
  uint8_t             txDmaDone;
  #endif  
  
}MModBus_t;

//##################################################################################################

void    mmodbus_callback(void);
void    mmodbus_callback_txDMA(void);
bool    mmodbus_init(uint32_t setTimeout);
void    mmodbus_set16bitOrder(MModBus_16bitOrder_t MModBus_16bitOrder_);
void    mmodbus_set32bitOrder(MModBus_32bitOrder_t MModBus_32bitOrder_);
//  coils numbers 00001 to 09999
bool    mmodbus_readCoil(uint8_t slaveAddress, uint16_t number, uint8_t *data);
bool    mmodbus_readCoils(uint8_t slaveAddress, uint16_t startnumber, uint16_t length, uint8_t *data);
//  discrete input 10001 to 19999
bool    mmodbus_readDiscreteInput(uint8_t slaveAddress, uint16_t number, uint8_t *data);
bool    mmodbus_readDiscreteInputs(uint8_t slaveAddress, uint16_t startnumber, uint16_t length, uint8_t *data);
//  input register 30001 to 39999
bool    mmodbus_readInputRegister32f(uint8_t slaveAddress, uint16_t number, float *data);
bool    mmodbus_readInputRegisters32f(uint8_t slaveAddress, uint16_t startnumber, uint16_t length, float *data);
bool    mmodbus_readInputRegister32i(uint8_t slaveAddress, uint16_t number, uint32_t *data);
bool    mmodbus_readInputRegisters32i(uint8_t slaveAddress, uint16_t startnumber, uint16_t length, uint32_t *data);
bool    mmodbus_readInputRegister16i(uint8_t slaveAddress, uint16_t number, uint16_t *data);
bool    mmodbus_readInputRegisters16i(uint8_t slaveAddress, uint16_t startnumber, uint16_t length, uint16_t *data);
//  holding register 40001 to 49999
bool    mmodbus_readHoldingRegister32f(uint8_t slaveAddress, uint16_t number, float *data);
bool    mmodbus_readHoldingRegisters32f(uint8_t slaveAddress, uint16_t startnumber, uint16_t length, float *data);
bool    mmodbus_readHoldingRegister32i(uint8_t slaveAddress, uint16_t number, uint32_t *data);
bool    mmodbus_readHoldingRegisters32i(uint8_t slaveAddress, uint16_t startnumber, uint16_t length, uint32_t *data);
bool    mmodbus_readHoldingRegister16i(uint8_t slaveAddress, uint16_t number, uint16_t *data);
bool    mmodbus_readHoldingRegisters16i(uint8_t slaveAddress, uint16_t startnumber, uint16_t length, uint16_t *data);
// coils numbers 00001 to 09999
bool    mmodbus_writeCoil(uint8_t slaveAddress, uint16_t number, uint8_t data);
//  holding register 40001 to 49999
bool    mmodbus_writeHoldingRegister16i(uint8_t slaveAddress, uint16_t number, uint16_t data);
bool    mmodbus_writeHoldingRegisters16i(uint8_t slaveAddress, uint16_t startnumber, uint16_t length, uint16_t *data);

//##################################################################################################
#endif
