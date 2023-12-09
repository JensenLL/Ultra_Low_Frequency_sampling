#ifndef __VARIABLES_H
#define __VARIABLES_H
#include "sys.h"

extern volatile u8 DAC_START_DEBUG;
extern volatile u8 DAC_END_DEBUG;

//周期点数设置为200000
#define GLOBAL_POINTS 25000
#define GLOBAL_PI 3.1416
//方案一所用变量
extern volatile u16 GLOBAL_VPP;
extern volatile u16 GLOBAL_FRE;
extern volatile u16 GLOBAL_VREF;
extern volatile u16 GLOBAL_AMP;

extern volatile u8 WAVE_SIN_GENERATOR;//SIN波形预生成过度周期数
extern volatile float WAVE_PRE_INCREMENT;//SIN波形预生成周期计数值
extern volatile u32  WAVE_POINT_DELTA;

extern volatile u8 INTERACTIVE_MARK;

#endif
