/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tim.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for aTask */
osThreadId_t aTaskHandle;
const osThreadAttr_t aTask_attributes = {
  .name = "aTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for dTask */
osThreadId_t dTaskHandle;
const osThreadAttr_t dTask_attributes = {
  .name = "dTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for xx */
osSemaphoreId_t xxHandle;
const osSemaphoreAttr_t xx_attributes = {
  .name = "xx"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartATask(void *argument);
void StartDTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of xx */
  xxHandle = osSemaphoreNew(1, 1, &xx_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of aTask */
  aTaskHandle = osThreadNew(StartATask, NULL, &aTask_attributes);

  /* creation of dTask */
  dTaskHandle = osThreadNew(StartDTask, NULL, &dTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartATask */
/**
  * @brief  Function implementing the aTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartATask */
void StartATask(void *argument)
{
  /* USER CODE BEGIN StartATask */
	oled_clear();
	display_string(3,3,"7131");
	display_gra();
	
	
		int ReceiveTimeout=0;
		int SendTimeout=0;
	unsigned char fr=0;
	A7131_WriteReg( PLL1_REG, fr);   
  /* Infinite loop */
  for(;;)
  {
			if(ReceiveTimeout==0){
				StrobeCmd(CMD_RX);
			}
			if(osSemaphoreAcquire(xxHandle,20)==osOK){
				ReceiveTimeout=0;
			}else{
				ReceiveTimeout=1;
			}
			if(ReceiveTimeout==1){
			
				continue;
			}
			if((A7131_ReadReg(MODE_REG)&0x20)==0)
			{
					ReadFIFO(64);
				int a=RfBuf[0]+RfBuf[1]*256;
				int b=RfBuf[2]+RfBuf[3]*256;
				int c=RfBuf[4]+RfBuf[5]*256;
				int d=RfBuf[6]+RfBuf[7]*256;
				__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1,a*2);
				__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2,b*2);
				__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3,c*2);
				__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4,d*2);
				
					WriteThing(RfBuf);
				
				
				
				
					StrobeCmd(CMD_TX);
					osSemaphoreAcquire(xxHandle,20);
					
					
			}
  }
  /* USER CODE END StartATask */
}

/* USER CODE BEGIN Header_StartDTask */
/**
* @brief Function implementing the dTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDTask */
void StartDTask(void *argument)
{
  /* USER CODE BEGIN StartDTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    if (GPIO_Pin&GPIO_PIN_4) {
			osSemaphoreRelease(xxHandle);
		}
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
