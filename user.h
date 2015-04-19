/* custom user defines and user functions */

#define USART_DEBUG USART1

#define HEART_BEAT_LED GPIO_Pin_13

/** System Clock Configuration
*/
void SystemClock_Config(void);

void delay_ms(uint32_t Delay);

void HearBeat_LedInit(void);
