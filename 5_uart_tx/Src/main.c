/**
 * USART1
 *  PC4, PC5                    (User Manual - VCP configuration)
 *
 * APB2                         (Datasheet)
 *  RCC_APB2ENR                 (Reference Manual)
 *  USART1EN 14
 *
 * GPIOC
 *  IOPCEN
 *  AHB2 19
 *
 * USART1_TX
 * PC4 AF7
 *
 * GPIOC_MODER
 *  MODER4[1:0]
 *  9:8
 *  10: Alternate Function mode
 *
 * AFR4[3:0]
 * 19:16
 * 0111: AF7
 *
 * AFR[0:1] => AFRL, AFRH
 *
 * RC 8 MHz oscillator clock    (Datasheet)
 **/

#include "stm32f303xc.h"

/* USART1 is an alternative function on GPIOC */
#define GPIOCEN         (1U << 19)
/* Enable USART1 */
#define USART1EN        (1U << 14)
/* USART control register: enable and transmit */
#define CR1_UE          (1U << 0)
#define CR1_TE          (1U << 3)
/* USART interrupt and status register */
#define ISR_TXE         (1U << 7)
/* Target baud rate */
#define USART_BAUDRATE   115200

/* STM32F303VCT6 Clock Frequency */
#define SYS_FREQ        8000000
#define APB2_CLK        SYS_FREQ

/* Calculates USARTDIV to set USART BRR register */
static uint16_t compute_usart_bd(uint32_t PeriphClk, uint32_t BaudRate);
/* Sets USART BRR register */
static void usart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);

/* Initialize USART1 */
void usart1_tx_init(void);
/* Write to USART1 */
void usart1_write(int ch);

/* LD4 Blue LED */
#define GPIOEEN     (1U << 21)
#define LED_PIN     (1U << 8)
/* Setup LD4 Blue LED */
void led_setup(void);

int main(void)
{
    led_setup();
    usart1_tx_init();

    char *message = "Hello world! ";

    while(1)
    {
        char *loop = message;
        while (*loop) usart1_write(*loop++);
    }
}

void usart1_tx_init(void)
{
    /****** Configure USART GPIO pin ******/

    /* Enable clock access to GPIOC */
    RCC -> AHBENR |= GPIOCEN;

    /* Set PC4 mode to alternate function mode */
    GPIOC -> MODER &= ~(1U << 8);
    GPIOC -> MODER |=  (1U << 9);

    /* Set PC4 alternate function type to USART_TX  */
    GPIOC -> AFR[0] |=  (1U << 16);
    GPIOC -> AFR[0] |=  (1U << 17);
    GPIOC -> AFR[0] |=  (1U << 18);
    GPIOC -> AFR[0] &= ~(1U << 19);

    /****** Configure USART module ******/

    /* Enable clock access to USART1 */
    RCC -> APB2ENR |= USART1EN;

    /* Configure baud rate */
    usart_set_baudrate(USART1, APB2_CLK, USART_BAUDRATE);

    /* Enable USART module */
    USART1 -> CR1 = CR1_UE;
    /* Configure transfer direction */
    USART1 -> CR1 |= CR1_TE;
}

void usart1_write(int ch)
{
    /* Make sure transmit data register is empty */
    while (!(USART1 -> ISR & ISR_TXE)){}

    /* Write to transmit data register */
    USART1 -> TDR = (ch & 0xFF);
}

void led_setup(void)
{
    /* Enable clock access to GPIOE */
    RCC -> AHBENR |= GPIOEEN;

    /* Set PC5 mode to general purpose output mode */
    GPIOE -> MODER |=  (1U << 16);
    GPIOE -> MODER &= ~(1U << 17);

    /* Set PC4 alternate function type to USART_TX  */
    GPIOC -> AFR[0] |=  (1U << 16);
    GPIOC -> AFR[0] |=  (1U << 17);
    GPIOC -> AFR[0] |=  (1U << 18);
    GPIOC -> AFR[0] &= ~(1U << 19);

    /* Set ODR register */
    GPIOE -> BSRR |= LED_PIN;
}

static void usart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate)
{
    USARTx -> BRR = compute_usart_bd(PeriphClk, BaudRate);
}

static uint16_t compute_usart_bd(uint32_t PeriphClk, uint32_t BaudRate)
{
    return ((PeriphClk + (BaudRate/2U))/BaudRate);
}
