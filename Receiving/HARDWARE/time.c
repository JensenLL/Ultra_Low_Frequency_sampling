#include "time.h"


void TIMX_General_User_Init(u16 arr, u16 psc){
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    //===��ͬ��ʱ��ʱ��ʹ��ѡ��--�޸�
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
    TIM_TimeBaseStructure.TIM_Period = arr;                     //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = psc;                  //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;                //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
    //===��ͬ��ʱ���Ĵ�������ѡ��--�޸�
    //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); 
    //===��ʱ���ֲ��ж�����ѡ��--�޸�
    TIM_ITConfig( //ʹ�ܻ���ʧ��ָ����TIM�ж�
        TIM5,     //TIM5
        TIM_IT_Update,
        ENABLE //ʹ��
    );
    //===���ö�ʱ���ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;              //TIM3�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //��ռ���ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;        //�����ȼ�3��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;              //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);                              //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
    //===��ͬ��ʱ��ʹ��ѡ��--�޸�
    TIM_Cmd(TIM5, DISABLE); //ʹ��TIMx����
}

/**
 * @brief TIM5��Ӧ���ж�
*/
void TIM5_IRQHandler(void){
    static u32 tem = 0;
    static u32 VP = 0;      //����ֵ����ֵ
    static float HD = 0.0;  //����
    static float FZ = 0.0;  //�����ֵ����ֵ 
	
	   //����
    static float PI = 3.1416;//��������Ϊ�궨��
    HD = 2*PI / GLOBAL_POINTS; //ÿһ�����Ӧ�Ļ���ֵ
    VP = GLOBAL_VPP / 2;      	      //���ֵһ��
    

    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET){
        //���TIMx���жϴ�����λ:TIM�ж�Դ
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
        
        //user program
        //�����ж����ñ��λ
        if(DAC_START_DEBUG == 1){	
            //1.������ֵ
            VP = WAVE_PRE_INCREMENT/(float)WAVE_SIN_GENERATOR * VP;
            FZ = VP * cos( HD * WAVE_POINT_DELTA + 4*3.1416 + 1.5708) + GLOBAL_VREF;		
            tem = (u16)FZ;      //ǿ��ת�����¾��ȵ���ʧ	
            //2.�������ģ��ֵ	  
            Dac1_Set_Vol(tem);
            //3.���ڵ�������
            ++WAVE_POINT_DELTA;
            if(WAVE_POINT_DELTA > GLOBAL_POINTS){
                WAVE_POINT_DELTA = 1;
            //4.���ù��ȵ���
                if(WAVE_PRE_INCREMENT < WAVE_SIN_GENERATOR){
                ++WAVE_PRE_INCREMENT;
                }
            }
        }else if(DAC_END_DEBUG == 1){
            VP = WAVE_PRE_INCREMENT/(float)WAVE_SIN_GENERATOR * VP;
            if(WAVE_POINT_DELTA > GLOBAL_POINTS){
                Dac1_Set_Vol(GLOBAL_VREF);//������λ
                TIM_Cmd(TIM5, DISABLE); //ʹ��TIMx����
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



