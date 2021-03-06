#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_spi.h"

#include "user.h"


#define SPI_WAIT(SPix) while (((SPIx)->SR & (SPI_SR_TXE | SPI_SR_RXNE)) == 0 || ((SPIx)->SR & SPI_SR_BSY))

/*
 *      |PINS PACK 1   |PINS PACK 2    |PINS PACK 3
   SPIX |MOSI MISO SCK |MOSI MISO SCK  |MOSI MISO SCK
   SPI2 |PC3 PC2 PB10  |PB15 PB14 PB13 |PI3 PI2 PI0
 */

void SPI_SlaveInit(SPI_TypeDef* SPIx)
{
    SPI_InitTypeDef SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStruct;

    if (SPIx == SPI2) {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);


        GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_2;
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
        GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_PinAFConfig(GPIOC, GPIO_PinSource3, GPIO_AF_SPI2);
        GPIO_PinAFConfig(GPIOC, GPIO_PinSource2, GPIO_AF_SPI2);


        GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
        GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(GPIOB, &GPIO_InitStruct);

        GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_SPI2);

        //--Enable the SPI2 periph
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

    } else {
        return;
    }

    // Reset SPI Interface
    SPI_I2S_DeInit(SPI2);

    //== SPI2 configuration
    SPI_StructInit(&SPI_InitStructure);
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI2, &SPI_InitStructure);

    // Enable SPI2
    SPI_Cmd(SPI2, ENABLE);
}

uint8_t SPI_Send(SPI_TypeDef* SPIx, uint8_t data)
{
    /* Wait for previous transmissions to complete if DMA TX enabled for SPI */
    SPI_WAIT(SPIx);
    /* Fill output buffer with data */
    SPIx->DR = data;
    /* Wait for transmission to complete */
    SPI_WAIT(SPIx);
    /* Return data from buffer */
    return SPIx->DR;
}

void SPI_SendMulti(SPI_TypeDef* SPIx, uint8_t* dataOut, uint8_t* dataIn, uint16_t count)
{
    uint16_t i;
    /* Wait for previous transmissions to complete if DMA TX enabled for SPI */
    SPI_WAIT(SPIx);
    for (i = 0; i < count; i++) {
        /* Fill output buffer with data */
        SPIx->DR = dataOut[i];
        /* Wait for SPI to end everything */
        SPI_WAIT(SPIx);
        /* Read data register */
        dataIn[i] = SPIx->DR;
    }
}
void SPI_WriteMulti(SPI_TypeDef* SPIx, uint8_t* dataOut, uint16_t count)
{
    uint16_t i;
    /* Wait for previous transmissions to complete if DMA TX enabled for SPI */
    SPI_WAIT(SPIx);
    for (i = 0; i < count; i++) {
        /* Fill output buffer with data */
        SPIx->DR = dataOut[i];
        /* Wait for SPI to end everything */
        SPI_WAIT(SPIx);
        /* Read data register */
        SPIx->DR;
    }
}
void SPI_ReadMulti(SPI_TypeDef* SPIx, uint8_t* dataIn, uint8_t dummy, uint16_t count)
{
    uint16_t i;
    /* Wait for previous transmissions to complete if DMA TX enabled for SPI */
    SPI_WAIT(SPIx);
    for (i = 0; i < count; i++) {
        /* Fill output buffer with data */
        SPIx->DR = dummy;
        /* Wait for SPI to end everything */
        SPI_WAIT(SPIx);
        /* Save data to buffer */
        dataIn[i] = SPIx->DR;
    }
}

void SPI_ReadString(SPI_TypeDef* SPIx, uint8_t* dataIn, uint8_t dummy, uint16_t count)
{
    uint16_t i;
    /* Wait for previous transmissions to complete if DMA TX enabled for SPI */
    SPI_WAIT(SPIx);
    for (i = 0; i < count; i++) {
        /* Fill output buffer with data */
        SPIx->DR = dummy;
        /* Wait for SPI to end everything */
        SPI_WAIT(SPIx);
        /* Save data to buffer */
        dataIn[i] = SPIx->DR;
    }
    dataIn[count]='\0';
}

void SPI_MasterInit(SPI_TypeDef* SPIx) {

    GPIO_InitTypeDef GPIO_InitStruct;
    SPI_InitTypeDef SPI_InitStruct;

    if (SPIx == SPI1) {
// enable clock for used IO pins
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
        /* configure pins used by SPI1
        * PA5 = SCK
        * PA6 = MISO
        * PA7 = MOSI
        */
        GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5;
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(GPIOA, &GPIO_InitStruct);
// connect SPI1 pins to SPI alternate function
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
// enable clock for used IO pins
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
        /* Configure the chip select pin
        in this case we will use PE7 */
        GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_Init(GPIOE, &GPIO_InitStruct);
        GPIOE->BSRRL |= GPIO_Pin_7; // set PE7 high
// enable peripheral clock
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    }
    /* configure SPIx in Mode 0
    * CPOL = 0 --> clock is low when idle
    * CPHA = 0 --> data is sampled at the first edge
    */
    SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // set to full duplex mode, seperate MOSI and MISO lines
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master; // transmit in master mode, NSS pin has to be always high
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; // one packet of data is 8 bits wide
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low; // clock is low when idle
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge; // data sampled at first edge
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set; // set the NSS management to internal and pull internal NSS high
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128; // SPI frequency is APB2 frequency / 4
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;// data is transmitted MSB first
    SPI_Init(SPIx, &SPI_InitStruct);
    SPI_Cmd(SPIx, ENABLE); // enable SPIx
}
