# STM32 Master MODBUS LL Library

* http://www.github.com/NimaLTD   
* https://www.instagram.com/github.nimaltd/   
* https://www.youtube.com/channel/UCUhY7qY1klJm1d2kulr9ckw   

This is the MMODBUS STM32 LL Library  
- [x] support MODBUS RTU  
- [ ] support MODBUS ASCII  

How to use this Library:
* Select "General peripheral Initalizion as a pair of '.c/.h' file per peripheral" on project settings.   
* Enable USART and enable interrupt.   
* Config USART.   
* Enable TX DMA is optional.
* Enable FREERTOS is optional.
* Enable a gpio as output for RS485 control pin.
* Add mmodbus_callback() into USART IRQ callback.
* If Enabled TX DMA, Add mmodbus_callback_txDMA() into DMA callback.
* Include Header and source into your project.   
* Config "mmodbusConfig.h".   
* Call mmodbus_init(..).   

```
#include "mmodbus.h"

int main()
{
  mmodbus_init(2500);
  mmodbus_writeCoil(10, 0, 1);
  
  while(1)
  {
    
  }
}
```
in stm32f4xx_it.c
```
void USART3_IRQHandler(void)
{
  mmodbus_callback();
}

// if use tx dma
void DMA1_Stream3_IRQHandler(void)
{
  mmodbus_callback_txDMA();
}


```


