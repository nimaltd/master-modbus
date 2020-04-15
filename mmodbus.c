

#include "mmodbus.h"
#include "usart.h"

MModBus_t mmodbus;

//#####################################################################################################
#if( _MMODBUS_RTU == 1)
static const uint16_t wCRCTable[] =
{
  0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
  0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
  0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
  0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
  0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
  0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
  0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
  0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
  0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
  0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
  0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
  0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
  0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
  0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
  0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
  0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
  0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
  0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
  0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
  0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
  0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
  0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
  0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
  0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
  0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
  0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
  0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
  0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
  0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
  0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
  0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
  0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040 
};
uint16_t mmodbus_crc16(const uint8_t *nData, uint16_t wLength)
{
  uint8_t nTemp;
  uint16_t wCRCWord = 0xFFFF;
  while (wLength--)
  {
    nTemp = *nData++ ^ wCRCWord;
    wCRCWord >>= 8;
    wCRCWord  ^= wCRCTable[nTemp];
  }
  return wCRCWord;
} 
#endif
//#####################################################################################################
void mmodbus_callback(void)
{
  if(LL_USART_IsActiveFlag_RXNE(_MMODBUS_USART) && LL_USART_IsEnabledIT_RXNE(_MMODBUS_USART))
  {
    if(mmodbus.rxIndex <_MMODBUS_RXSIZE - 1)
    {
      mmodbus.rxBuf[mmodbus.rxIndex] = LL_USART_ReceiveData8(_MMODBUS_USART);      
      mmodbus.rxIndex++;
    }
    else
      LL_USART_ReceiveData8(_MMODBUS_USART);      
  }
  mmodbus.rxTime = HAL_GetTick();
}
//#####################################################################################################

void  mmodbus_callback_txDMA(void)
{
  #if _MMODBUS_TXDMA == 1
  #if _MMODBUS_DMASTREAM == LL_DMA_STREAM_0
  if(LL_DMA_IsActiveFlag_TC0(_MMODBUS_DMA))
  {
    LL_DMA_ClearFlag_TC0(_MMODBUS_DMA);
    mmodbus.txDmaDone = 1;    
  }      
  #elif _MMODBUS_DMASTREAM == LL_DMA_STREAM_1
  if(LL_DMA_IsActiveFlag_TC1(_MMODBUS_DMA))
  {
    LL_DMA_ClearFlag_TC1(_MMODBUS_DMA);
    mmodbus.txDmaDone = 1;    
  }      
  #elif _MMODBUS_DMASTREAM == LL_DMA_STREAM_2
  if(LL_DMA_IsActiveFlag_TC2(_MMODBUS_DMA))
  {
    LL_DMA_ClearFlag_TC2(_MMODBUS_DMA);
    mmodbus.txDmaDone = 1;    
  }      
  #elif _MMODBUS_DMASTREAM == LL_DMA_STREAM_3
  if(LL_DMA_IsActiveFlag_TC3(_MMODBUS_DMA))
  {
    LL_DMA_ClearFlag_TC3(_MMODBUS_DMA);
    mmodbus.txDmaDone = 1;    
  }      
  #elif _MMODBUS_DMASTREAM == LL_DMA_STREAM_2
  if(LL_DMA_IsActiveFlag_TC4(_MMODBUS_DMA))
  {
    LL_DMA_ClearFlag_TC4(_MMODBUS_DMA);
    mmodbus.txDmaDone = 1;    
  }      
  #elif _MMODBUS_DMASTREAM == LL_DMA_STREAM_2
  if(LL_DMA_IsActiveFlag_TC5(_MMODBUS_DMA))
  {
    LL_DMA_ClearFlag_TC5(_MMODBUS_DMA);
    mmodbus.txDmaDone = 1;    
  }      
  #elif _MMODBUS_DMASTREAM == LL_DMA_STREAM_6
  if(LL_DMA_IsActiveFlag_TC6(_MMODBUS_DMA))
  {
    LL_DMA_ClearFlag_TC6(_MMODBUS_DMA);
    mmodbus.txDmaDone = 1;    
  }      
  #elif _MMODBUS_DMASTREAM == LL_DMA_STREAM_7
  if(LL_DMA_IsActiveFlag_TC7(_MMODBUS_DMA))
  {
    LL_DMA_ClearFlag_TC7(_MMODBUS_DMA);
    mmodbus.txDmaDone = 1;    
  }      
  #endif
  #endif
}
//##################################################################################################
uint16_t mmodbus_receiveRaw(uint32_t timeout)
{
  uint32_t startTime = HAL_GetTick();
  while(1)
  {
    if(HAL_GetTick() - startTime > timeout)
      return 0;
    if((mmodbus.rxIndex > 0) && (HAL_GetTick() - mmodbus.rxTime > 10))
      return mmodbus.rxIndex;    
  }    
}  
//##################################################################################################
bool mmodbus_sendRaw(uint8_t *data, uint16_t size, uint32_t timeout)
{
  while(mmodbus.txBusy == 1)
    mmodbus_delay(1);
  mmodbus.txBusy = 1;
  memset(mmodbus.rxBuf, 0, _MMODBUS_RXSIZE);
  mmodbus.rxIndex = 0;
  uint32_t startTime = HAL_GetTick();
  HAL_GPIO_WritePin(_MMODBUS_CTRL_GPIO, _MMODBUS_CTRL_PIN, GPIO_PIN_SET);
  mmodbus_delay(1);
  #if (_MMODBUS_TXDMA == 0)
  for (uint16_t i = 0; i<size; i++)
  {
    while (!LL_USART_IsActiveFlag_TXE(_MMODBUS_USART))
    {
      if(HAL_GetTick() - startTime > timeout)
      {
        HAL_GPIO_WritePin(_MMODBUS_CTRL_GPIO, _MMODBUS_CTRL_PIN, GPIO_PIN_RESET);
        mmodbus.txBusy = 0;
        return false;
      }   
    }
    LL_USART_TransmitData8(_MMODBUS_USART, data[i]);
  }
  while (!LL_USART_IsActiveFlag_TC(_MMODBUS_USART))
  {
    if(HAL_GetTick() - startTime > timeout)
    {
      HAL_GPIO_WritePin(_MMODBUS_CTRL_GPIO, _MMODBUS_CTRL_PIN, GPIO_PIN_RESET);
      mmodbus.txBusy = 0;
      return false;
    }  
  }
  #else
  LL_USART_ClearFlag_TC(_MMODBUS_USART);
  mmodbus.txDmaDone = 0;
  LL_DMA_ConfigAddresses(_MMODBUS_DMA, _MMODBUS_DMASTREAM,\
    (uint32_t)data, LL_USART_DMA_GetRegAddr(_MMODBUS_USART),\
    LL_DMA_GetDataTransferDirection(_MMODBUS_DMA, _MMODBUS_DMASTREAM));
  LL_DMA_SetDataLength(_MMODBUS_DMA, _MMODBUS_DMASTREAM, size);
  LL_DMA_EnableStream(_MMODBUS_DMA, _MMODBUS_DMASTREAM);
  while(mmodbus.txDmaDone == 0)
  {
    mmodbus_delay(1);
    if(HAL_GetTick() - startTime > timeout)
    {
      HAL_GPIO_WritePin(_MMODBUS_CTRL_GPIO, _MMODBUS_CTRL_PIN, GPIO_PIN_RESET);
      mmodbus.txBusy = 0;
      return false;
    }  
  }
  while (!LL_USART_IsActiveFlag_TC(_MMODBUS_USART))
    mmodbus_delay(1);      
  #endif
  HAL_GPIO_WritePin(_MMODBUS_CTRL_GPIO, _MMODBUS_CTRL_PIN, GPIO_PIN_RESET);
  mmodbus.txBusy = 0;
  return true;
}
//##################################################################################################
bool mmodbus_init(uint32_t timeout)
{
  HAL_GPIO_WritePin(_MMODBUS_CTRL_GPIO, _MMODBUS_CTRL_PIN, GPIO_PIN_RESET);
  memset(&mmodbus, 0, sizeof(mmodbus));  
  #if (_MMODBUS_TXDMA == 1)
  LL_DMA_EnableIT_TC(_MMODBUS_DMA, _MMODBUS_DMASTREAM);
  LL_USART_EnableDMAReq_TX(_MMODBUS_USART);
  #endif
  LL_USART_EnableIT_RXNE(_MMODBUS_USART);
  mmodbus.timeout = timeout;
  return true;
}
//##################################################################################################
bool mmodbus_readCoil(uint8_t slaveAddress, uint16_t number_0_to_9998, uint8_t *coilStatus)
{
  return mmodbus_readCoils(slaveAddress, number_0_to_9998, 1, coilStatus);
}
//##################################################################################################
bool mmodbus_readCoils(uint8_t slaveAddress, uint16_t startNumber_0_to_9998, uint16_t length, uint8_t *coilsStatus)
{
  if(startNumber_0_to_9998 > 9998)
    return false;
  #if( _MMODBUS_RTU == 1)
  uint8_t txData[8];
  txData[0] = slaveAddress;
  txData[1] = MModbusCMD_ReadCoilStatus;
  txData[2] = (startNumber_0_to_9998 & 0xFF00) >> 8;
  txData[3] = (startNumber_0_to_9998 & 0x00FF);
  txData[4] = (length & 0xFF00) >> 8;
  txData[5] = (length & 0x00FF);
  static uint16_t  crc;
  crc = mmodbus_crc16(txData, 6);
  txData[6] = (crc & 0x00FF);
  txData[7] = (crc & 0xFF00) >> 8;
  mmodbus_sendRaw(txData, 8, 100);
  uint16_t recLen = mmodbus_receiveRaw(mmodbus.timeout);
  if(recLen == 0)
    return false;
  if(mmodbus.rxBuf[0] != slaveAddress)
    return false;
  if(mmodbus.rxBuf[1] != MModbusCMD_ReadCoilStatus)
    return false;
  crc = mmodbus_crc16(mmodbus.rxBuf, mmodbus.rxBuf[2] + 3);
  if(((crc & 0x00FF) != mmodbus.rxBuf[mmodbus.rxBuf[2] + 3]) || (((crc & 0xFF00) >> 8) != mmodbus.rxBuf[mmodbus.rxBuf[2] + 4]))
    return false; 
  if(coilsStatus != NULL)
    memcpy(coilsStatus, &mmodbus.rxBuf[3], mmodbus.rxBuf[2]);   
  return true;
  #endif
  #if( _MMODBUS_ASCII == 1)
  
  #endif
}
//##################################################################################################
bool mmodbus_readDiscreteInput(uint8_t slaveAddress, uint16_t number_0_to_9998, uint8_t *discreteInput)
{
  return mmodbus_readDiscreteInputs(slaveAddress, number_0_to_9998, 1, discreteInput); 
}
//##################################################################################################
bool mmodbus_readDiscreteInputs(uint8_t slaveAddress, uint16_t startNumber_0_to_9998, uint16_t length, uint8_t *discreteInputs)
{
  if(startNumber_0_to_9998 > 9998)
    return false;
  #if( _MMODBUS_RTU == 1)
  uint8_t txData[8];
  txData[0] = slaveAddress;
  txData[1] = MModbusCMD_ReadDiscreteInputs;
  txData[2] = (startNumber_0_to_9998 & 0xFF00) >> 8;
  txData[3] = (startNumber_0_to_9998 & 0x00FF);
  txData[4] = (length & 0xFF00) >> 8;
  txData[5] = (length & 0x00FF);
  static uint16_t  crc;
  crc = mmodbus_crc16(txData, 6);
  txData[6] = (crc & 0x00FF);
  txData[7] = (crc & 0xFF00) >> 8;
  mmodbus_sendRaw(txData, 8, 100);
  uint16_t recLen = mmodbus_receiveRaw(mmodbus.timeout);
  if(recLen == 0)
    return false;
  if(mmodbus.rxBuf[0] != slaveAddress)
    return false;
  if(mmodbus.rxBuf[1] != MModbusCMD_ReadDiscreteInputs)
    return false;
  crc = mmodbus_crc16(mmodbus.rxBuf, mmodbus.rxBuf[2] + 3);
  if(((crc & 0x00FF) != mmodbus.rxBuf[mmodbus.rxBuf[2] + 3]) || (((crc & 0xFF00) >> 8) != mmodbus.rxBuf[mmodbus.rxBuf[2] + 4]))
    return false; 
  if(discreteInputs != NULL)
    memcpy(discreteInputs, &mmodbus.rxBuf[3], mmodbus.rxBuf[2]);   
  return true;
  #endif
}
//##################################################################################################
bool mmodbus_readHoldingRegister(uint8_t slaveAddress, uint16_t number_0_to_9998, uint16_t *holdingRegister)
{
  return mmodbus_readHoldingRegisters(slaveAddress, number_0_to_9998, 1, holdingRegister); 
}
//##################################################################################################
bool mmodbus_readHoldingRegisters(uint8_t slaveAddress, uint16_t startNumber_0_to_9998, uint16_t length, uint16_t *holdingRegisters)
{
  if(startNumber_0_to_9998 > 9998)
    return false;
  #if( _MMODBUS_RTU == 1)
  uint8_t txData[8];
  txData[0] = slaveAddress;
  txData[1] = MModbusCMD_ReadHoldingRegisters;
  txData[2] = (startNumber_0_to_9998 & 0xFF00) >> 8;
  txData[3] = (startNumber_0_to_9998 & 0x00FF);
  txData[4] = (length & 0xFF00) >> 8;
  txData[5] = (length & 0x00FF);
  static uint16_t  crc;
  crc = mmodbus_crc16(txData, 6);
  txData[6] = (crc & 0x00FF);
  txData[7] = (crc & 0xFF00) >> 8;
  mmodbus_sendRaw(txData, 8, 100);
  uint16_t recLen = mmodbus_receiveRaw(mmodbus.timeout);
  if(recLen == 0)
    return false;
  if(mmodbus.rxBuf[0] != slaveAddress)
    return false;
  if(mmodbus.rxBuf[1] != MModbusCMD_ReadHoldingRegisters)
    return false;
  crc = mmodbus_crc16(mmodbus.rxBuf, mmodbus.rxBuf[2] + 3);
  if(((crc & 0x00FF) != mmodbus.rxBuf[mmodbus.rxBuf[2] + 3]) || (((crc & 0xFF00) >> 8) != mmodbus.rxBuf[mmodbus.rxBuf[2] + 4]))
    return false; 
  if(holdingRegisters != NULL)
  {
    for(uint8_t i=0 ; i<mmodbus.rxBuf[2] ; i+=2)
    {    
      uint8_t H = mmodbus.rxBuf[i+3];
      mmodbus.rxBuf[i+3] = mmodbus.rxBuf[i+3+1];
      mmodbus.rxBuf[i+3+1] = H;      
    }
    memcpy(holdingRegisters, &mmodbus.rxBuf[3], mmodbus.rxBuf[2]);   
  }
  return true;
  #endif
}
//##################################################################################################
bool mmodbus_readInputRegister(uint8_t slaveAddress, uint16_t number_0_to_9998, uint16_t *inputRegister)
{
  return mmodbus_readInputRegisters(slaveAddress, number_0_to_9998, 1, inputRegister); 
}
//##################################################################################################
bool mmodbus_readInputRegisters(uint8_t slaveAddress, uint16_t startNumber_0_to_9998, uint16_t length, uint16_t *inputRegisters)
{
  if(startNumber_0_to_9998 > 9998)
    return false;
  #if( _MMODBUS_RTU == 1)
  uint8_t txData[8];
  txData[0] = slaveAddress;
  txData[1] = MModbusCMD_ReadInputRegisters;
  txData[2] = (startNumber_0_to_9998 & 0xFF00) >> 8;
  txData[3] = (startNumber_0_to_9998 & 0x00FF);
  txData[4] = (length & 0xFF00) >> 8;
  txData[5] = (length & 0x00FF);
  static uint16_t  crc;
  crc = mmodbus_crc16(txData, 6);
  txData[6] = (crc & 0x00FF);
  txData[7] = (crc & 0xFF00) >> 8;
  mmodbus_sendRaw(txData, 8, 100);
  uint16_t recLen = mmodbus_receiveRaw(mmodbus.timeout);
  if(recLen == 0)
    return false;
  if(mmodbus.rxBuf[0] != slaveAddress)
    return false;
  if(mmodbus.rxBuf[1] != MModbusCMD_ReadInputRegisters)
    return false;
  crc = mmodbus_crc16(mmodbus.rxBuf, mmodbus.rxBuf[2] + 3);
  if(((crc & 0x00FF) != mmodbus.rxBuf[mmodbus.rxBuf[2] + 3]) || (((crc & 0xFF00) >> 8) != mmodbus.rxBuf[mmodbus.rxBuf[2] + 4]))
    return false; 
  if(inputRegisters != NULL)
  {
    for(uint8_t i=0 ; i<mmodbus.rxBuf[2] ; i+=2)
    {    
      uint8_t H = mmodbus.rxBuf[i+3];
      mmodbus.rxBuf[i+3] = mmodbus.rxBuf[i+3+1];
      mmodbus.rxBuf[i+3+1] = H;      
    }
    memcpy(inputRegisters, &mmodbus.rxBuf[3], mmodbus.rxBuf[2]);   
  }
  return true;
  #endif
}
//##################################################################################################
bool mmodbus_writeCoil(uint8_t slaveAddress, uint16_t number_0_to_9998, uint8_t coilStatus)
{
  if(number_0_to_9998 > 9998)
    return false;
  #if( _MMODBUS_RTU == 1)
  uint8_t txData[8];
  txData[0] = slaveAddress;
  txData[1] = MModbusCMD_WriteSingleCoil;
  txData[2] = (number_0_to_9998 & 0xFF00) >> 8;
  txData[3] = (number_0_to_9998 & 0x00FF);
  if(coilStatus == 0)
    txData[4] = 0;
  else
    txData[4] = 0xFF;
  txData[5] = 0;
  static uint16_t  crc;
  crc = mmodbus_crc16(txData, 6);
  txData[6] = (crc & 0x00FF);
  txData[7] = (crc & 0xFF00) >> 8;
  mmodbus_sendRaw(txData, 8, 100);
  uint16_t recLen = mmodbus_receiveRaw(mmodbus.timeout);
  if(recLen == 0)
    return false;
  if(memcmp(txData, mmodbus.rxBuf, 8) == 0)
    return true;
  else
    return false;
  #endif
  #if( _MMODBUS_ASCII == 1)
  
  #endif
}
//##################################################################################################
bool mmodbus_writeHoldingRegister(uint8_t slaveAddress, uint16_t number_0_to_9998, uint16_t holdingRegister)
{
  if(number_0_to_9998 > 9998)
    return false;
  #if( _MMODBUS_RTU == 1)
  uint8_t txData[8];
  txData[0] = slaveAddress;
  txData[1] = MModbusCMD_WriteSingleRegister;
  txData[2] = (number_0_to_9998 & 0xFF00) >> 8;
  txData[3] = (number_0_to_9998 & 0x00FF);
  txData[4] = (holdingRegister & 0xFF00) >> 8;
  txData[5] = holdingRegister & 0x00FF;
  static uint16_t  crc;
  crc = mmodbus_crc16(txData, 6);
  txData[6] = (crc & 0x00FF);
  txData[7] = (crc & 0xFF00) >> 8;
  mmodbus_sendRaw(txData, 8, 100);
  uint16_t recLen = mmodbus_receiveRaw(mmodbus.timeout);
  if(recLen == 0)
    return false;
  if(memcmp(txData, mmodbus.rxBuf, 8) == 0)
    return true;
  else
    return false;
  #endif
  #if( _MMODBUS_ASCII == 1)
  
  #endif
}
