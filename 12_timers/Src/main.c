#include <stdio.h>
#include <usart.h>
#include "stm32f303xc.h"
#include "tim.h"

#define GPIOEEN          (1U << 21)
#define PIN9             (1U << 9)
#define LED_PIN           PIN9

uint32_t sensor_value;

int main(void)
{
    /* Initialize USART for transmitting (115200 baud rate) */
    usart1_tx_init();
    tim2_1hz_init();

    /* Enable GPIOE */
    RCC -> AHBENR |= GPIOEEN;

    /* Configure for general output */
    GPIOE -> MODER |=  (1U << 18);
    GPIOE -> MODER &= ~(1U << 19);

    while (1)
    {
        /* Wait for update interrupt flag UIF*/
        while (!(TIM2 -> SR & TIM_SR_UIF)) {}

        /* Clear UIF */
        TIM2 -> SR &= ~TIM_SR_UIF;

        printf("A second has passed!\n\r");
        /* Blink LED for 1 second */
        GPIOE -> ODR ^= LED_PIN;
    }
}
