#include "sys.h"
#include "usart.h"	  

//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if EN_USART1_RX
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 


#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
char USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  
  
void uart_init(u32 bound){
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitTypeDef NVIC_InitStructure1;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��USART1��GPIOAʱ��
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��usart2ʱ��
	

	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9

	//USART1_RX	  GPIOA.10��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

	//USART2_TX   GPIOA.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOA.9

	//USART2_RX	  GPIOA.3��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOA.10  


	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	//Usart2 NVIC ����
	NVIC_InitStructure1.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�3
	NVIC_InitStructure1.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure1.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure1);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	

	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	USART_Init(USART3, &USART_InitStructure); //��ʼ������1
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 
	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���1 

}

void USART1_SendData(u8 data){
    //ѭ������,ֱ���������   
    USART1->DR = (u8)data;   
    while((USART1->SR&0X40)==0);    
}

void USART2_SendData(u8 data){
    //ѭ������,ֱ���������   
    USART3->DR = (u8)data;   
    while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);   
}

void USART2_SendData_BUFF(u8 buf[],u32 len)     
{    
    u32 i;    
    for(i=0;i<len;i++)    
    USART2_SendData(buf[i]);         
} 

//����1�жϷ������
void USART1_IRQHandler(void){
    u8 REC = 0;
    delay_ms(1);
    //���ն˽��յ�����
    //delay_ms(1);
    //�����޸ĵ���:
  	//�������û�����
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
        REC = USART_ReceiveData(USART1);
        //����ָ������
        if(REC == 0x00){
            DAC_START_DEBUG = 1; 
            DAC_END_DEBUG = 0;
            WAVE_POINT_DELTA = 1;
            WAVE_PRE_INCREMENT = 1;
            TIM_Cmd(TIM5, ENABLE); //ʹ��TIMx����
            TIM_SetCounter(TIM5,0);
            //TIM_Cmd(TIM5, ENABLE); 
        }else if(REC == 0xFF){
            DAC_START_DEBUG = 0;
            DAC_END_DEBUG = 1;
        }else if(REC >= 0X01 && REC <= 0XFE){
            if(INTERACTIVE_MARK == 0){
                lF_SetAmp(REC);
                INTERACTIVE_MARK = 1;
            }else if(INTERACTIVE_MARK == 1){
                lF_SetFreq(REC);
                INTERACTIVE_MARK = 0;
            }

        }

// //����2���� ����
//         switch (REC){
//         case 0x00 :
//             USART1_SendData(0);
//             //USART1_SendData(0X55);
//             DAC_START_DEBUG = 1; 
//             DAC_END_DEBUG = 0;
//             WAVE_POINT_DELTA = 1;
//             WAVE_PRE_INCREMENT = 1;
//             TIM_Cmd(TIM5, ENABLE); //ʹ��TIMx����
//             TIM_SetCounter(TIM5,0);      
//             break;
//         case 0xFF :
//             USART1_SendData(255);
//             DAC_START_DEBUG = 0;
//             DAC_END_DEBUG = 1; 
//             break;
//         case 0x80 : lF_SetAmp(REC); break;
//         case 0x04 : lF_SetFreq(REC); break;
//         default:
//             break;
//         }
    }  
} 
#endif	

