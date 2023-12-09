#include "variables.h"

//说明:用于存储全局互通变量数据

//调试使用变量
volatile u8 DAC_START_DEBUG = 0;
volatile u8 DAC_END_DEBUG = 0;
//方案一所用变量
volatile u16 GLOBAL_VPP = 1820;//对应输出正负2V 1820
volatile u16 GLOBAL_FRE = 0;
volatile u16 GLOBAL_VREF = 2048;
volatile u16 GLOBAL_AMP = 2048;//对应最大单峰值为4V
//volatile u16 GLOBAL_AMP = 2048;//对应最大单峰值为4V

volatile u8 WAVE_SIN_GENERATOR = 4;//SIN波形预生成过度周期数
volatile float WAVE_PRE_INCREMENT = 1;//SIN波形预生成周期计数值
volatile u32  WAVE_POINT_DELTA = 1;

volatile u8 INTERACTIVE_MARK = 0;//信息配置轮转切换幅值频率
