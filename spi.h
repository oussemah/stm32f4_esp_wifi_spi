#include "stm32f4xx.h"
#include "stm32f4xx_spi.h"


void SPI_SlaveInit(SPI_TypeDef* SPIx);
uint8_t SPI_Send(SPI_TypeDef* SPIx, uint8_t data);
void SPI_SendMulti(SPI_TypeDef* SPIx, uint8_t* dataOut, uint8_t* dataIn, uint16_t count);
void SPI_WriteMulti(SPI_TypeDef* SPIx, uint8_t* dataOut, uint16_t count);
void SPI_ReadMulti(SPI_TypeDef* SPIx, uint8_t* dataIn, uint8_t dummy, uint16_t count) ;
