#include "sys.h"
#include "usart.h"	  

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if EN_USART1_RX
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 


#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
char USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  
  
void uart_init(u32 bound){
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitTypeDef NVIC_InitStructure1;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);	//使能USART1，GPIOA时钟
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能usart2时钟
	

	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9

	//USART1_RX	  GPIOA.10初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

	//USART2_TX   GPIOA.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOA.9

	//USART2_RX	  GPIOA.3初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOA.10  


	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	//Usart2 NVIC 配置
	NVIC_InitStructure1.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure1.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure1.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure1);	//根据指定的参数初始化VIC寄存器
	

	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	USART_Init(USART3, &USART_InitStructure); //初始化串口1
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_Cmd(USART1, ENABLE);                    //使能串口1 
	USART_Cmd(USART3, ENABLE);                    //使能串口1 

}

void USART1_SendData(u8 data){
    //循环发送,直到发送完毕   
    USART1->DR = (u8)data;   
    while((USART1->SR&0X40)==0);    
}

void USART2_SendData(u8 data){
    //循环发送,直到发送完毕   
    USART3->DR = (u8)data;   
    while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);   
}

void USART2_SendData_BUFF(u8 buf[],u32 len)     
{    
    u32 i;    
    for(i=0;i<len;i++)    
    USART2_SendData(buf[i]);         
} 

//串口1中断服务程序
void USART1_IRQHandler(void){
    u8 REC = 0;
    delay_ms(1);
    //接收端接收到数据
    //delay_ms(1);
    //后期修改调整:
  	//接收设置互斥锁
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
        REC = USART_ReceiveData(USART1);
        //解析指令数据
        if(REC == 0x00){
            DAC_START_DEBUG = 1; 
            DAC_END_DEBUG = 0;
            WAVE_POINT_DELTA = 1;
            WAVE_PRE_INCREMENT = 1;
            TIM_Cmd(TIM5, ENABLE); //使能TIMx外设
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

// //方案2测试 可行
//         switch (REC){
//         case 0x00 :
//             USART1_SendData(0);
//             //USART1_SendData(0X55);
//             DAC_START_DEBUG = 1; 
//             DAC_END_DEBUG = 0;
//             WAVE_POINT_DELTA = 1;
//             WAVE_PRE_INCREMENT = 1;
//             TIM_Cmd(TIM5, ENABLE); //使能TIMx外设
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

