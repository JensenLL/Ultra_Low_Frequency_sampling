#include "dac.h"

//DAC通道1和2输出初始化
void Dac_Init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    DAC_InitTypeDef DAC_InitType;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );	  //使能PORTA通道时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE );	  //使能DAC通道时钟 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;      // 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 		      //模拟输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //GPIO_ResetBits(GPIOA,GPIO_Pin_4)	;//PA.4 输出高
    //GPIO_ResetBits(GPIOA,GPIO_Pin_5)	;//PA.5 输出高

    DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//不使用触发功能 TEN1=0
    DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//不使用波形发生
    DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//屏蔽、幅值设置
    DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1输出缓存关闭 BOFF1=1

    DAC_Init(DAC_Channel_1,&DAC_InitType);	 //初始化DAC通道1
    DAC_Init(DAC_Channel_2,&DAC_InitType);	 //初始化DAC通道2

    DAC_Cmd(DAC_Channel_1, ENABLE);  //使能DAC1
    DAC_Cmd(DAC_Channel_2, ENABLE);  //使能DAC2

    //初始化默认输出1.65V
    DAC_SetChannel1Data(DAC_Align_12b_R, GLOBAL_VREF);  //12位右对齐数据格式设置DAC值
    DAC_SetChannel2Data(DAC_Align_12b_R, GLOBAL_VREF);  //12位右对齐数据格式设置DAC值
}

//设置通道1输出电压
//vol:0~3300,代表0~3.3V
void Dac1_Set_Vol(u16 vol){
    //float temp=vol;
    //temp/=1000;
    //temp=temp*4096/3.3;
    //DAC_SetChannel1Data(DAC_Align_12b_R,temp);//12位右对齐数据格式设置DAC值
    DAC_SetChannel1Data(DAC_Align_12b_R,vol);
	  //DAC_SetChannel2Data(DAC_Align_12b_R, vol); 
}

void Relay_Config(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	PCout(4) = 0; //初始化为低电平
}
 
