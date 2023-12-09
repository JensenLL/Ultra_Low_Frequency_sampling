#ifndef __USART_USER_H
#define __USART_USER_H
#include "sys.h"

#define USER_USART_ENABLE 0

#if USER_USART_ENABLE
void uart_init(u32 bound);
void USART1_SendData(u8 data);
#endif

#endif

