#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"

#include "user.h"

uint32_t millis=0, ticks=0;

/** System Clock Configuration
*/
void SystemClock_Config(void)
{
    RCC_ClocksTypeDef System_Clocks;

    /* This part doesn't have an equivalent in the original code,
     * but we NEED this to generate PRECISE 1ms timing function 5Delay_msfor instance)
     */
    RCC_GetClocksFreq(&System_Clocks);
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    SysTick_Config(System_Clocks.HCLK_Frequency / 1000); //Interrupt generated every 1000 us = 1ms
}

void SysTick_Handler()
{
    if (ticks > 0)
    {
        ticks--;
    }
    /* The following is used as a heart beat indicator that the systick handler is running, flashing the led every on/off every 1s */
    if(!((millis++)%500)) {
        GPIO_ToggleBits(GPIOD,HEART_BEAT_LED);
    }
}

void delay_ms(uint32_t Delay)
{
    ticks = Delay;
    while(ticks> 0) ;
}

void HearBeat_LedInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* Enable GPIO peripheral clock for heart_beat led */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    /**
     * Output type is push-pull
     * Mode is output
     * output pulled down
     * Speed 25MHz
     */
    GPIO_InitStruct.GPIO_Pin = HEART_BEAT_LED;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;

    /* Initialize pin */
    GPIO_Init(GPIOD, &GPIO_InitStruct);
}

void MasterSelect_GPIOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* Enable GPIO peripheral clock for heart_beat led */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    /**
     * Output type is push-pull
     * Mode is output
     * output pulled down
     * Speed 25MHz
     */
    GPIO_InitStruct.GPIO_Pin = MASTER_SELECT_PIN;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

    /* Initialize pin */
    GPIO_Init(GPIOD, &GPIO_InitStruct);	
}
