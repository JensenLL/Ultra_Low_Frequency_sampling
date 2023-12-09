#include "lowFreq.h"


//ͨ���б�
//1-254  --- 0-5V ����ֵ
//1-254  --- 3.3V(3V)�ֱ���Ϊ0.012V 12mV
//eg: 0xFE 0X80
void lF_SetAmp(u8 param){
    u16 amp = 0;
    if(param>=1 && param <= 254)
        amp = (float)(param - 1) / 254.0 * GLOBAL_AMP;
    else
        amp = 0;
    //����ȫ�������ֵ
    GLOBAL_VPP = amp * 2; 
}

//����Ƶ�ʲ���ʱ,ȷ��TIM��ʱ�����ڹر�״̬
//Ƶ�������б�
//��Ӧ����Ϊ100000�����
//0.1HZ 10S ---- ��ʱ������ֵΪ9 --- ͨ��ֵΪ1
//0.05HZ 20S ---- ��ʱ������ֵΪ19 --- ͨ��ֵΪ2
//0.02HZ 50S ---- ��ʱ������ֵΪ49 --- ͨ��ֵΪ3
//0.01HZ 100S ---- ��ʱ������ֵΪ99 --- ͨ��ֵΪ4
void lF_SetFreq(u8 param){
    u16 arr = 0;
    switch (param){
        case 1 : arr = 19;  break;
        case 2 : arr = 39; break;
        case 3 : arr = 99; break;
        case 4 : arr = 199; break;  
        default: arr = 19;  break;//Ĭ��Ϊ10s
    }
    TIMX_General_User_Init(arr,719);
}

//��ʱ x ��ms
void lF_Delay(u16 time)
{
		u16 n = 0;
		for(n=0;n < time;++n){
			delay_ms(100);
		}
	
}


