#ifndef __TIME_H
#define __TIME_H
#include "sys.h"
#include "variables.h"

/**
 * @brief 通用定时器中断初始化
 * @param arr 自动重装值
 * @param psc 时钟预分频数
 * @note 统一为向上计数模式
 * @note 对应不同的定时器 -- 设置合理的抢占优先级和响应优先级
 * @note 从0开始计数
*/
void TIMX_General_User_Init(u16 arr, u16 psc);


/**
 * @brief TIM2对应的中断
*/
// void TIM2_IRQHandler(void)
// {
//     //检查指定的TIM中断发生与否:TIM 中断源
//     if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
//     {
//         //清除TIMx的中断待处理位:TIM 中断源
//         TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//         //----------user operation--------//
//     }
// }

/**
 * @brief TIM3对应的中断
*/
// void TIM3_IRQHandler(void)
// {
//     //检查指定的TIM中断发生与否:TIM 中断源
//     if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
//     {
//         //清除TIMx的中断待处理位:TIM 中断源
//         TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
//         //----------user operation--------//
//     }
// }

/**
 * @brief TIM4对应的中断
 * @note 注释说明:该定时器中断与外部上升沿输入捕获相冲突
 */
//void TIM4_IRQHandler(void)
//{
//    //检查指定的TIM中断发生与否:TIM 中断源
//    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
//    {
//        //清除TIMx的中断待处理位:TIM 中断源
//        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
//        //----------user operation--------//
//    }
//}

#endif
