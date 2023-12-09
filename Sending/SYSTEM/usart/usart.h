#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

//����ͨ�ŵ���ʹ�� 0�����ֹ 1����ʹ��
#define EN_USART1_RX 1		

#define USART_REC_LEN  			200  	//�����������ֽ��� 200	
extern char  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	

#if EN_USART1_RX
void uart_init(u32 bound);
void USART1_SendData(u8 data);
void USART2_SendData(u8 data);
void USART2_SendData_BUFF(u8 buf[],u32 len);
#endif

#endif


