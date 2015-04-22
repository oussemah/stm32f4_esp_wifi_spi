#include "stm32f4xx.h"

#include "stdio.h"

#include "user.h"
#include "usart.h"
#include "spi.h"

uint8_t dataOut[32]={0}, dataIn[32];


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

    /* Init Pin to be used as Master_Select */
    MasterSelect_GPIOInit();
    
    printf("System Started.\r\n");
    for (;;)
    {
        delay_ms(100);
        GPIO_ToggleBits(GPIOD, MASTER_SELECT_PIN);
        SPI_ReadString(SPI2, dataIn, 0xFF, 4);
        printf("Got %s\r\n", (char*)dataIn);
        //SPI_WriteMulti(SPI2, "test", 4);
        //printf("tick\r\n");
    }
}


