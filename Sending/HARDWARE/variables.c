#include "variables.h"

//˵��:���ڴ洢ȫ�ֻ�ͨ��������

//����ʹ�ñ���
volatile u8 DAC_START_DEBUG = 0;
volatile u8 DAC_END_DEBUG = 0;
//����һ���ñ���
volatile u16 GLOBAL_VPP = 1820;//��Ӧ�������2V 1820
volatile u16 GLOBAL_FRE = 0;
volatile u16 GLOBAL_VREF = 2048;
volatile u16 GLOBAL_AMP = 2048;//��Ӧ��󵥷�ֵΪ4V
//volatile u16 GLOBAL_AMP = 2048;//��Ӧ��󵥷�ֵΪ4V

volatile u8 WAVE_SIN_GENERATOR = 4;//SIN����Ԥ���ɹ���������
volatile float WAVE_PRE_INCREMENT = 1;//SIN����Ԥ�������ڼ���ֵ
volatile u32  WAVE_POINT_DELTA = 1;

volatile u8 INTERACTIVE_MARK = 0;//��Ϣ������ת�л���ֵƵ��
