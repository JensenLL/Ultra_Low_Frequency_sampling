/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
/* USER CODE END Header */
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h" 

#include "user_config.h"

/* USER CODE BEGIN PV */
uint16_t Value[4]={0}; 
uint32_t current=0; 
uint32_t volt=0; 
uint8_t timer_FLAG=0;//��ʱ����־λ
char TxVoltage[5] = {0};
char TxCurrent[5] = {0};

//========================================================================
//��������֯�ṹ��
// ���ڽ���ָ��Ե�������л�
// ��ʱ��ÿ����̶�ʱ����ADC�����DMA���ݽ��з���

// �ڴ��ڽ����ж��д��� ��ʱ���� + DMA ADC����
// ����user_config�ļ�,�ڲ����ʱ��������ʼ������ + �û��Զ������

//  PD12 == ENA   PD13 == ENB
//  PD8 == SA0    PD9 == SA1
//  PD10 == SB0   PD11 == SB1
//  ���Ƶ��跽�� ������1->8
//========================================================================


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* Enable I-Cache-----*/
  SCB_EnableICache();
  /* Enable D-Cache-----*/
  SCB_EnableDCache();
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  /* Configure the system clock */
  SystemClock_Config();
  /* USER CODE BEGIN SysInit */
	__attribute__((at(0x38000080))) uint16_t ADC_Value[2] = {0};

	
	//========================================================================
	//1��������г�ʼ��
	//2���û��Զ������
	//========================================================================
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
	MX_ADC1_Init();
  MX_TIM1_Init();
  MX_USART1_UART_Init();


  //����У׼������У׼����ͬʱ���ϣ����ӵĻ������Ľ�����к����������ƫ��
	HAL_ADCEx_Calibration_Start(&hadc1,ADC_CALIB_OFFSET_LINEARITY,ADC_SINGLE_ENDED );
	//ƫ��У׼������У׼����ͬʱ���ϣ����ӵĻ������Ľ�����к����������ƫ��
  HAL_ADCEx_Calibration_Start(&hadc1,ADC_CALIB_OFFSET, ADC_SINGLE_ENDED );
	HAL_ADC_Start_DMA(&hadc1,(uint32_t *)ADC_Value,2);//���¿���DMA
  HAL_TIM_Base_Start_IT(&htim1);//��ʼ��ʱ��
  HAL_Delay(5);
	
  //��⴮�ڽ����ж��Ƿ�����
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
	
	//ʹ��ģ�⿪��
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);//SAʹ��
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);//SB��λ
	//Ĭ��A3
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10|GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_SET);
	
  /* Infinite loop */
  for(;;){
    //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);//�����ã���ɾ��
		if(timer_FLAG==1)//��ʱ��Ϊ1ms����ÿ1ms�ض�DMA1�ӣ�ȡ����Ȼ�����ݴ�������volt
		{
		  timer_FLAG=0;
			
			SCB_InvalidateDCache_by_Addr((uint32_t *)ADC_Value,2);//��֤ADC��DMA����ѭ��
		  //SCB_CleanDCache();//���ܷ��ʹ���
			sprintf(TxCurrent, "%04x",ADC_Value[0]);//��DMA����λ�ĵ�����Ϊ�ַ������ɼ�֮�����Ҳ�Ƕ�Ӧ��ѹ��ֵ?
      sprintf(TxVoltage, "%04x",ADC_Value[1]);//DMA����λ��Ϊ�ַ�������Ӧ��ѹֵ
			
			//���ڷ��͵���ֵ,timeout��λΪms,ԭΪHAL_UART_Transmit(&huart1,TxCurrent,4,1000)4λ16�������ݣ�������Ҫ4�ֽ�
			HAL_UART_Transmit(&huart1,(unsigned char*)TxCurrent,4,1);
			//���ڷ��͵�ѹֵ
			HAL_UART_Transmit(&huart1,(unsigned char*)TxVoltage,4,1);
			//���ͻ��з�
		  HAL_UART_Transmit(&huart1,(unsigned char*)"\n",1,1000);
		}
  }
}


//========================================================================
//1�����¶�����HAL_TIM_PeriodElapsedCallback()����
//2����ʱ����־λ�л�
//3�����ʹ��uint8_t timer_FLAG��־λ
//========================================================================
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)/* tim1 1ms*/
	{
		timer_FLAG=1;
	}
}




