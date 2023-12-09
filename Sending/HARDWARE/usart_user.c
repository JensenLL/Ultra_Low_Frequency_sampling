#include "usart_user.h"

#if USER_USART_ENABLE
void uart_init(u32 bound){
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
        
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��

    //USART1_TX   GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9

    //USART1_RX	  GPIOA.10��ʼ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

    //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0X00; //��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0X01;		//�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

    //USART ��ʼ������ 
    USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART1, &USART_InitStructure);      //��ʼ������1
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 
}

void USART1_SendData(u8 data){
    //ѭ������,ֱ���������   
    USART1->DR = (u8)data;   
    while((USART1->SR&0X40)==0);    
}

//����1�жϷ������
void USART1_IRQHandler(void){
    u8 REC = 0;

    //���ն˽��յ�����
    delay_ms(1);
    //�����޸ĵ���:
  	//�������û�����
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
        REC = USART_ReceiveData(USART1);
        //����ָ������
        if(REC == 0x00){
            USART1_SendData(0);//DEBUG����
            DAC_START_DEBUG = 1; 
            DAC_END_DEBUG = 0;
            WAVE_POINT_DELTA = 1;
            WAVE_PRE_INCREMENT = 1;
            TIM_Cmd(TIM5, ENABLE); //ʹ��TIMx����
            TIM_SetCounter(TIM5,0);
            //TIM_Cmd(TIM5, ENABLE); 
        }else if(REC == 0xFF){
            USART1_SendData(255);//DEBUG����
            DAC_START_DEBUG = 0;
            DAC_END_DEBUG = 1;
        }else if(REC >= 0X01 && REC <= 0XFE){
            if(INTERACTIVE_MARK == 0){
                USART1_SendData(1);//DEBUG����
                lF_SetAmp(REC);
                INTERACTIVE_MARK = 1;
            }else if(INTERACTIVE_MARK == 1){
                USART1_SendData(2);//DEBUG����
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







