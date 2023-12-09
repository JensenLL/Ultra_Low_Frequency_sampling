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
uint8_t timer_FLAG=0;//定时器标志位
char TxVoltage[5] = {0};
char TxCurrent[5] = {0};

//========================================================================
//【代码组织结构】
// 串口接收指令对电阻进行切换
// 定时器每间隔固定时长将ADC缓存的DMA数据进行发送

// 在串口接收中断中处理 定时开关 + DMA ADC传输
// 定义user_config文件,内部存放时钟启动初始化程序 + 用户自定义程序

//  PD12 == ENA   PD13 == ENB
//  PD8 == SA0    PD9 == SA1
//  PD10 == SB0   PD11 == SB1
//  控制电阻方向 左往右1->8
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
	//1、外设进行初始化
	//2、用户自定义代码
	//========================================================================
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
	MX_ADC1_Init();
  MX_TIM1_Init();
  MX_USART1_UART_Init();


  //线性校准，两种校准必须同时加上，不加的话出来的结果会有横向或者纵向偏移
	HAL_ADCEx_Calibration_Start(&hadc1,ADC_CALIB_OFFSET_LINEARITY,ADC_SINGLE_ENDED );
	//偏移校准，两种校准必须同时加上，不加的话出来的结果会有横向或者纵向偏移
  HAL_ADCEx_Calibration_Start(&hadc1,ADC_CALIB_OFFSET, ADC_SINGLE_ENDED );
	HAL_ADC_Start_DMA(&hadc1,(uint32_t *)ADC_Value,2);//重新开启DMA
  HAL_TIM_Base_Start_IT(&htim1);//开始定时器
  HAL_Delay(5);
	
  //检测串口接收中断是否正常
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
	
	//使能模拟开关
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);//SA使能
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);//SB复位
	//默认A3
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10|GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_SET);
	
  /* Infinite loop */
  for(;;){
    //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);//调试用，可删除
		if(timer_FLAG==1)//定时器为1ms，即每1ms关断DMA1从，取数，然后将数据存入数组volt
		{
		  timer_FLAG=0;
			
			SCB_InvalidateDCache_by_Addr((uint32_t *)ADC_Value,2);//保证ADC的DMA不断循环
		  //SCB_CleanDCache();//可能发送错误
			sprintf(TxCurrent, "%04x",ADC_Value[0]);//将DMA第三位的电流变为字符串（采集之后电流也是对应电压数值?
      sprintf(TxVoltage, "%04x",ADC_Value[1]);//DMA第四位变为字符串，对应电压值
			
			//串口发送电流值,timeout单位为ms,原为HAL_UART_Transmit(&huart1,TxCurrent,4,1000)4位16进制数据，所以需要4字节
			HAL_UART_Transmit(&huart1,(unsigned char*)TxCurrent,4,1);
			//串口发送电压值
			HAL_UART_Transmit(&huart1,(unsigned char*)TxVoltage,4,1);
			//发送换行符
		  HAL_UART_Transmit(&huart1,(unsigned char*)"\n",1,1000);
		}
  }
}


//========================================================================
//1、重新定义弱HAL_TIM_PeriodElapsedCallback()函数
//2、定时器标志位切换
//3、配合使用uint8_t timer_FLAG标志位
//========================================================================
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)/* tim1 1ms*/
	{
		timer_FLAG=1;
	}
}




