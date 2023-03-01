#include "stm32f303xc.h"

#define SYSTICK_LOAD_VAL            8000
#define CTRL_ENABLE                 (1U << 0)
#define CTRL_CLKSOURCE              (1U << 2)
#define CTRL_COUNTFLAG              (1U << 16)

void systickDelayMs (int delay)
{
    /* Reload with number of clocks per millisecond */
    SysTick -> LOAD = SYSTICK_LOAD_VAL;

    /* Clear SYSTICK current value register */
    SysTick -> VAL = 0;

    /* Enable SYSTICK and select internal clock source */
    SysTick -> CTRL = CTRL_ENABLE | CTRL_CLKSOURCE;

    for (int i = 0; i < delay; i++)
    {
        /* Wait until COUNTFLAG is set */
        while (!(SysTick -> CTRL & CTRL_COUNTFLAG)){}
    }
    SysTick -> CTRL = 0;
}
