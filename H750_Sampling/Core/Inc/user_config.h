#ifndef _USER_CONFIG_H
#define _USER_CONFIG_H

#include "stdio.h"
#include "main.h"

/* Private function prototypes*/
void SystemClock_Config(void);
void HAL_Delay(uint32_t Delay);
void User_switch(uint8_t n);
extern uint16_t ADC_Value[];

#endif







