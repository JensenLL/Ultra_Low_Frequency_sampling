#include "lowFreq.h"


//通信列表
//1-254  --- 0-5V 单峰值
//1-254  --- 3.3V(3V)分辨率为0.012V 12mV
//eg: 0xFE 0X80
void lF_SetAmp(u8 param){
    u16 amp = 0;
    if(param>=1 && param <= 254)
        amp = (float)(param - 1) / 254.0 * GLOBAL_AMP;
    else
        amp = 0;
    //设置全局输出幅值
    GLOBAL_VPP = amp * 2; 
}

//设置频率参数时,确保TIM定时器处于关闭状态
//频率设置列表：
//对应设置为100000点输出
//0.1HZ 10S ---- 定时器设置值为9 --- 通信值为1
//0.05HZ 20S ---- 定时器设置值为19 --- 通信值为2
//0.02HZ 50S ---- 定时器设置值为49 --- 通信值为3
//0.01HZ 100S ---- 定时器设置值为99 --- 通信值为4
void lF_SetFreq(u8 param){
    u16 arr = 0;
    switch (param){
        case 1 : arr = 19;  break;
        case 2 : arr = 39; break;
        case 3 : arr = 99; break;
        case 4 : arr = 199; break;  
        default: arr = 19;  break;//默认为10s
    }
    TIMX_General_User_Init(arr,719);
}

//延时 x 百ms
void lF_Delay(u16 time)
{
		u16 n = 0;
		for(n=0;n < time;++n){
			delay_ms(100);
		}
	
}


