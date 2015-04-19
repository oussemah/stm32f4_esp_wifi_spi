#include "stm32f4xx.h"

#include "stdio.h"

#include "user.h"
#include "usart.h"
#include "spi.h"

uint8_t dataOut[32], dataIn[32];


int main(void) {

 /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  SystemInit();

  /* Configure the system clock */
  SystemClock_Config();
 
  /* Init usart port for debug purpose */
  USARTInit(USART_DEBUG, 115200);
  
  /* Init SPI wifi port */
  SPI_SlaveInit(SPI2);

  /* Init heart-beat Led */
  HearBeat_LedInit();
  
  printf("System Started.\r\n");
  for (;;)
  {
	delay_ms(1000);
    SPI_ReadString(SPI2, dataIn, 0xFF, 4);
    printf("Got %s\r\n", (char*)dataIn);
  }
}
