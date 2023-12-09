#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

//串口通信调试使用 0代表禁止 1代表使能
#define EN_USART1_RX 1		

#define USART_REC_LEN  			200  	//定义最大接收字节数 200	
extern char  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	

#if EN_USART1_RX
void uart_init(u32 bound);
void USART1_SendData(u8 data);
void USART2_SendData(u8 data);
void USART2_SendData_BUFF(u8 buf[],u32 len);
#endif

#endif


