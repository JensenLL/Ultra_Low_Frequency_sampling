#include "stm32f10x.h"
#include "sys.h"
#include "stdlib.h"


int main(void){	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����2
	delay_init();//��ʱ������ʼ��
	uart_init(1000000);//���ڳ�ʼ��Ϊ1000_000
  TIMX_General_User_Init(39, 719); //����0.1Hz 2.5�����
	//TIM_Cmd(TIM5, ENABLE); //ʹ��TIMx����
	DAC_START_DEBUG = 0;
	for (;;){
		
	}	 
}

