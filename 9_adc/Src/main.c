#include <stdio.h>
#include <usart.h>
#include "stm32f303xc.h"
#include "adc.h"

uint32_t sensor_value;

int main(void)
{
    /* Initialize USART for transmitting (115200 baud rate) */
    usart1_tx_init();

    /* Initialize ADC */
    pa0_adc_init();

    /* Start ADC conversion */
    start_conversion();

    printf("Starting read\n\r");

    while (1)
    {
        /* Read value from PA0 pin */
        sensor_value = adc_read();
        printf("Sensor value: %d\n\r", (int)sensor_value);
    }
}
