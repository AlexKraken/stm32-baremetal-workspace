/**
 * User LD3: Red LED (Position N)
 * I/O PE9
 * Port: E
 * Pin:  9
 *
 * Peripherals Memory Address starts at 0x4000 0000
 *
 * Bus AHB2 0x4800 0000 - 0x4800 17FF (Advanced High-performance Bus)
 * GPIOE    0x4800 1000 - 0x4800 13FF (General Purpose Input/Output)
 *
 * Bus AHB1 0x4002 0000 - 0x4002 43FF
 * RCC      0x4002 1000 - 0x4002 13FF (Reset and Clock Control)
 *
 * RCC_AHBENR (AHB peripheral clock enable register)
 * Address offset: 0x14
 *
 * GPIOx_MODER (GPIO port mode register)
 * Address offset: 0x00
 *
 * MODERy[1:0] (Port x configuration bits)
 * MODER9[1:0] Bits 19:18
 * 01: General purpose output mode
 *
 * GPIOx_ODR (GPIO port output data register)
 * Address offset: 0x14
 * */

#define PERIPHERAL_BASE   (0x40000000UL)

#define AHB2_OFFSET       (0x8000000UL)
#define GPIOE_OFFSET      (0x1000UL)
#define GPIOE_BASE        (PERIPHERAL_BASE + AHB2_OFFSET + GPIOE_OFFSET)

#define AHB1_OFFSET       (0x20000UL)
#define RCC_OFFSET        (0x1000UL)
#define RCC_BASE          (PERIPHERAL_BASE + AHB1_OFFSET + RCC_OFFSET)

#define RCC_AHBENR_OFFSET (0x14UL)
#define RCC_AHBENR        (*(volatile unsigned int *)(RCC_BASE + RCC_AHBENR_OFFSET))

#define MODER_OFFSET      (0x00UL)
#define GPIOE_MODER       (*(volatile unsigned int *)(GPIOE_BASE + MODER_OFFSET))

#define GPIO_ODR_OFFSET   (0x14UL)
#define GPIOE_ODR         (*(volatile unsigned int *)(GPIOE_BASE + GPIO_ODR_OFFSET))

#define IOPEEN            (1U << 21)

#define PIN9              (1U << 9)
#define LD3_PIN            PIN9

int main(void)
{
    /* Enable clock access to GPIOE */
    RCC_AHBENR  |= IOPEEN;

    /* Set PE9 as output pin */
    GPIOE_MODER |=  (1U << 18); // Set bit 10 to 1
    GPIOE_MODER &= ~(1U << 19); // Set bit 11 to 0

    while(1)
    {
        /* Set PE9 high */
        // GPIOE_ODR |= LD3_PIN;

        /* Blink PE9 instead */
        GPIOE_ODR ^= LD3_PIN;
        for(int i = 0; i < 1000000; i++);
    }
}
