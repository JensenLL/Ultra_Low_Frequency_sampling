#ifndef __TIME_H
#define __TIME_H
#include "sys.h"
#include "variables.h"

/**
 * @brief ͨ�ö�ʱ���жϳ�ʼ��
 * @param arr �Զ���װֵ
 * @param psc ʱ��Ԥ��Ƶ��
 * @note ͳһΪ���ϼ���ģʽ
 * @note ��Ӧ��ͬ�Ķ�ʱ�� -- ���ú������ռ���ȼ�����Ӧ���ȼ�
 * @note ��0��ʼ����
*/
void TIMX_General_User_Init(u16 arr, u16 psc);


/**
 * @brief TIM2��Ӧ���ж�
*/
// void TIM2_IRQHandler(void)
// {
//     //���ָ����TIM�жϷ������:TIM �ж�Դ
//     if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
//     {
//         //���TIMx���жϴ�����λ:TIM �ж�Դ
//         TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//         //----------user operation--------//
//     }
// }

/**
 * @brief TIM3��Ӧ���ж�
*/
// void TIM3_IRQHandler(void)
// {
//     //���ָ����TIM�жϷ������:TIM �ж�Դ
//     if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
//     {
//         //���TIMx���жϴ�����λ:TIM �ж�Դ
//         TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
//         //----------user operation--------//
//     }
// }

/**
 * @brief TIM4��Ӧ���ж�
 * @note ע��˵��:�ö�ʱ���ж����ⲿ���������벶�����ͻ
 */
//void TIM4_IRQHandler(void)
//{
//    //���ָ����TIM�жϷ������:TIM �ж�Դ
//    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
//    {
//        //���TIMx���жϴ�����λ:TIM �ж�Դ
//        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
//        //----------user operation--------//
//    }
//}

#endif
