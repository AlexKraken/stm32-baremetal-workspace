#include <usart.h>

/* USART1 is an alternative function on GPIOC */
#define GPIOCEN         (1U << 19)
/* Enable USART1 */
#define USART1EN        (1U << 14)
/* USART control register: enable and transmit */
#define CR1_UE          (1U << 0)
#define CR1_TE          (1U << 3)
#define CR1_RE          (1U << 2)
/* USART interrupt and status register */
#define ISR_TXE         (1U << 7)
#define ISR_RXNE        (1U << 5)
/* Target baud rate */
#define USART_BAUDRATE   115200

/* STM32F303VCT6 Clock Frequency */
#define SYS_FREQ        8000000
#define APB2_CLK        SYS_FREQ

/* Calculates USARTDIV to set USART BRR register */
static uint16_t compute_usart_bd(uint32_t PeriphClk, uint32_t BaudRate);
/* Sets USART BRR register */
static void usart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);


/* Write to USART1 */
void usart1_write(int ch);
char usart1_read(void);

int __io_putchar(int ch)
{
    usart1_write(ch);
    return ch;
}

void usart1_rxtx_init(void)
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

    /* Set PC5 mode to alternate function mode */
    GPIOC -> MODER &= ~(1U << 10);
    GPIOC -> MODER |=  (1U << 11);

    /* Set PC5 alternate function type to USART_RX  */
    GPIOC -> AFR[0] |=  (1U << 20);
    GPIOC -> AFR[0] |=  (1U << 21);
    GPIOC -> AFR[0] |=  (1U << 22);
    GPIOC -> AFR[0] &= ~(1U << 23);

    /****** Configure USART module ******/

    /* Enable clock access to USART1 */
    RCC -> APB2ENR |= USART1EN;

    /* Configure baud rate */
    usart_set_baudrate(USART1, APB2_CLK, USART_BAUDRATE);

    /* Enable USART module */
    USART1 -> CR1 = CR1_UE;
    /* Configure transfer direction */
    USART1 -> CR1 |= CR1_TE | CR1_RE;
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

char usart1_read(void)
{
    /* Make sure read data register is not empty */
    while (!(USART1 -> ISR & ISR_RXNE)){}
    return USART1 -> RDR;
}

void usart1_write(int ch)
{
    /* Make sure transmit data register is empty */
    while (!(USART1 -> ISR & ISR_TXE)){}

    /* Write to transmit data register */
    USART1 -> TDR = (ch & 0xFF);
}

static void usart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate)
{
    USARTx -> BRR = compute_usart_bd(PeriphClk, BaudRate);
}

static uint16_t compute_usart_bd(uint32_t PeriphClk, uint32_t BaudRate)
{
    return ((PeriphClk + (BaudRate/2U))/BaudRate);
}
