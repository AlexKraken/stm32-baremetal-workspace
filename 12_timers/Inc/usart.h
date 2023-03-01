/*
 * uart.h
 */

#ifndef USART_H_
#define USART_H_

#include "stm32f303xc.h"
/* Initialize USART1 */
void usart1_tx_init(void);
void usart1_rxtx_init(void);
/* Read a key press */
char usart1_read(void);

#endif /* USART_H_ */
