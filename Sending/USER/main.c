#include "stm32f10x.h"
#include "sys.h"
#include "stdlib.h"


int main(void){	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组2
	delay_init();//延时函数初始化
	uart_init(1000000);//串口初始化为1000_000
  TIMX_General_User_Init(39, 719); //设置0.1Hz 2.5万个点
	//TIM_Cmd(TIM5, ENABLE); //使能TIMx外设
	DAC_START_DEBUG = 0;
	for (;;){
		
	}	 
}

