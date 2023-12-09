#include "stm32f10x.h"
#include "sys.h"
#include "stdlib.h"

//方案一:发送幅值和频率参数,然后触发执行  串口中断(串口接收抖动) + 定时器中断(优先级问题)
//方案二:串口充当定时器功能,接收端接收信号累加计数,输出SIN波形

//设置点数为50000个点 
//设置最大的幅值为4V(设置单峰值)
//默认为VPP = 4V 

//因为设置的基准电压为3V，后级的同相放大电路的倍数为3故输出4.5V 即2048 == 4.5（单峰值）
//当前的方案通信1-254对应波形输出幅值为0-4.5V 

//通信时，先发送幅值，而后发送频率

//继电器吸合时间缩短，减小运行时间
int main(void){	
	u8 t = 0, len=0;	
	u16 data = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组2
	delay_init();// 延时函数初始化


	Relay_Config();//继电器引脚配置
	Dac_Init();//默认输出2048数值

	lF_Delay(50);//延时5S闭合继电器
	PCout(4) = 1;//吸合继电器
	uart_init(1000000);	 //串口初始化为 1000_000

	for (;;){
		if(USART_RX_STA&0x8000){					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			data = 0; USART_RX_BUF[len] = '\0';//追加空字符
			data = atoi(USART_RX_BUF);//接收字符转化为数据
			//解析指令
			Dac1_Set_Vol(data);
			USART_RX_STA=0;//复位
		}else{
			//未接收到数据,输出水平电位
			//Dac1_Set_Vol(2048);
		}
	}	 
}

