#include "stm32f303xc.h"

void start_conversion(void);
uint32_t adc_read(void);

void pa0_adc_init (void)
{
    /****** Configure ADC GPIOA pin ******/

    /* Enable clock access to GPIOA */
    RCC -> AHBENR |= RCC_AHBENR_GPIOAEN;

    /* Set PA0 to analog mode */
    GPIOA -> MODER |= GPIO_MODER_MODER0;

    /****** Configure ADC peripheral ******/

    /* Enable clock access to ADC */
    RCC -> AHBENR |= RCC_AHBENR_ADC12EN;

    /* Set ADC common clock mode */
    ADC12_COMMON -> CCR |= ADC_CCR_CKMODE_1;

    /* Set ADC to single/continuous conversion mode */
    ADC1 -> CFGR = ADC_CFGR_CONT;

    /* Enable ADC internal voltage regulator */
    ADC1 -> CR &= ~(ADC_CR_ADVREGEN);
    ADC1 -> CR |= ADC_CR_ADVREGEN_0;

    /* Enable ADC module */
    ADC1 -> CR |= ADC_CR_ADEN;

    /* Wait for startup time of voltage regulator */
    while (!(ADC1 -> ISR & ADC_ISR_ADRDY)){}

    /* Set conversion sequence ranking */
    ADC1 -> SQR1 = ADC_SQR1_SQ1_0;
    /* Set conversion sequence length */
    ADC1 -> SQR1 |= ADC_SQR1_L_0;
}

void start_conversion(void)
{
    /* Start ADC regular conversion */
    ADC1 -> CR |= ADC_CR_ADSTART;
}

uint32_t adc_read(void)
{
    /* Read converted result */
    return ADC1 -> DR;
}
