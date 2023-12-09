#include "time.h"


void TIMX_General_User_Init(u16 arr, u16 psc){
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    //===不同定时器时钟使能选择--修改
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
    TIM_TimeBaseStructure.TIM_Period = arr;                     //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler = psc;                  //设置用来作为TIMx时钟频率除数的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;                //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
    //===不同定时器寄存器配置选择--修改
    //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); 
    //===定时器局部中断配置选择--修改
    TIM_ITConfig( //使能或者失能指定的TIM中断
        TIM5,     //TIM5
        TIM_IT_Update,
        ENABLE //使能
    );
    //===设置定时器中断通道
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;              //TIM3中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;        //从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;              //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);                              //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
    //===不同定时器使能选择--修改
    TIM_Cmd(TIM5, DISABLE); //使能TIMx外设
}

/**
 * @brief TIM5对应的中断
*/
void TIM5_IRQHandler(void){
    static u32 tem = 0;
    static u32 VP = 0;      //单峰值数量值
    static float HD = 0.0;  //弧度
    static float FZ = 0.0;  //输出峰值数量值 
	
	   //调试
    static float PI = 3.1416;//不能设置为宏定义
    HD = 2*PI / GLOBAL_POINTS; //每一个点对应的弧度值
    VP = GLOBAL_VPP / 2;      	      //峰峰值一半
    

    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET){
        //清除TIMx的中断待处理位:TIM中断源
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
        
        //user program
        //接收中断设置标记位
        if(DAC_START_DEBUG == 1){	
            //1.调整幅值
            VP = WAVE_PRE_INCREMENT/(float)WAVE_SIN_GENERATOR * VP;
            FZ = VP * cos( HD * WAVE_POINT_DELTA + 4*3.1416 + 1.5708) + GLOBAL_VREF;		
            tem = (u16)FZ;      //强制转换导致精度的损失	
            //2.设置输出模拟值	  
            Dac1_Set_Vol(tem);
            //3.周期点数递增
            ++WAVE_POINT_DELTA;
            if(WAVE_POINT_DELTA > GLOBAL_POINTS){
                WAVE_POINT_DELTA = 1;
            //4.设置过度递增
                if(WAVE_PRE_INCREMENT < WAVE_SIN_GENERATOR){
                ++WAVE_PRE_INCREMENT;
                }
            }
        }else if(DAC_END_DEBUG == 1){
            VP = WAVE_PRE_INCREMENT/(float)WAVE_SIN_GENERATOR * VP;
            if(WAVE_POINT_DELTA > GLOBAL_POINTS){
                Dac1_Set_Vol(GLOBAL_VREF);//输出零电位
                TIM_Cmd(TIM5, DISABLE); //使能TIMx外设
                TIM_SetCounter(TIM5,0);
            }else{
                ++WAVE_POINT_DELTA;
                FZ = VP * cos( HD * WAVE_POINT_DELTA + 4*3.1416 + 1.5708) + GLOBAL_VREF;		
                tem = (u16)FZ;	  
                Dac1_Set_Vol(tem);
            }
        }
    }
}



