#include "stm32f303xc.h"

void tim2_1hz_init(void)
{
    /* Enable clock access to TIMER2 (APB1) */
    RCC -> APB1ENR |= RCC_APB1ENR_TIM2EN;

    /* Set prescaler value */
    TIM2 -> PSC = 800 - 1; // 8 000 000 / 800 = 10 000

    /* Set auto-reload value */
    TIM2 -> ARR = 10000 - 1; // 10 000 / 10 000 = 1

    /* Clear counter */
    TIM2 -> CNT = 0;

    /* Enable timer */
    TIM2 -> CR1 = TIM_CR1_CEN;
}
