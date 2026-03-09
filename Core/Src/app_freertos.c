/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "user.h"
#include "adc.h"
#include "oled.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
uint32_t key_time = 0;
KeyState_t key_state = KEY_IDLE;
uint8_t current_key;
uint32_t e = 0;
uint32_t y = 0;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId KeyHandle;
osThreadId Data_FuctionHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void Key_Scan(void const * argument);
void Data(void const * argument);

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
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of Key */
  osThreadDef(Key, Key_Scan, osPriorityIdle, 0, 128);
  KeyHandle = osThreadCreate(osThread(Key), NULL);

  /* definition and creation of Data_Fuction */
  osThreadDef(Data_Fuction, Data, osPriorityIdle, 0, 128);
  Data_FuctionHandle = osThreadCreate(osThread(Data_Fuction), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_Key_Scan */
/**
* @brief Function implementing the Key thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Key_Scan */
void Key_Scan(void const * argument)
{
  /* USER CODE BEGIN Key_Scan */
  /* Infinite loop */
  for(;;)
  {
    uint8_t key_flag = 0;//没有按键变化
    
    static uint8_t last_key = 0;   // 默认上拉输入，1=松开
    
    uint32_t now = HAL_GetTick();

    current_key = HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin);

    switch(key_state)
    {
        case KEY_IDLE:
            // 检测按下瞬间
            if(current_key == 1 && last_key == 0)
            {
                key_time = now;          // 记录按下时间
                key_state = KEY_PRESS;   // 进入按下状态
            }
            break;

        case KEY_PRESS:
            // 消抖
            if(now - key_time < DEBOUNCE_TIME)
                break;

            if(current_key == 1)
            {
              // 判断是否达到长按时间
              if(now - key_time >= LONG_PRESS_TIME)
              {
                  if(mode == 5)
                  {
                    mode = 0;
                  }
                  else
                  {
                    mode++;
                  } 
                  // 长按动作
                  key_flag = 1;
                  key_state = KEY_LONG;
              }
            }
            else
            {
                // 松开，并且未达到长按
              if(mode == 1||mode == 2||mode == 5)
              {
                if(v_mode == 1)
                {
                  v_mode = 0;
                }
                else
                {
                  v_mode = 1;
                }
              }
              if(mode == 3)
              {
                if(r_mode == 3)
                {
                  r_mode = 0;
                }
                else
                {
                  r_mode++;
                }
              }        
              // 短按动作
              key_flag = 1;
              key_state = KEY_IDLE;
            }
            break;

        case KEY_LONG:
            // 长按已经触发，只等松开
            if(current_key == 0)
            {
                key_state = KEY_IDLE;
            }
            break;
    }

    last_key = current_key;

    if(key_flag == 1)
    {
      key_flag = 0;
      Mode_show();
    }

    //Mode_show();
    osDelay(1);
  }
  /* USER CODE END Key_Scan */
}

/* USER CODE BEGIN Header_Data */
/**
* @brief Function implementing the Data_Fuction thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Data */
void Data(void const * argument)
{
  /* USER CODE BEGIN Data */
  
  /* Infinite loop */
  for(;;)
  {
    if(mode == 1||mode == 2||mode == 5){
      if(v_mode == 0){
        
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13,1);//20V档
        HAL_ADC_Start_DMA(&hadc1, &v20, 1);//20V
        v20_sum += v20;
        e++;
        if(e >= 999){
          v20 = v20_sum / 1000;
          e = 0;
          v20_sum = 0;
          v20_true = (float)v20 * 40.0f / 4096.0f - 20.0f;
          show_one_decimal(90,50,v20_true,12);
        }
      }
      if(v_mode == 1){
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13,0);//2V档
        HAL_ADC_Start_DMA(&hadc2, &v2, 1);//2V
        v2_sum += v2;
        y++;
        if(y >= 999){
          v2 = v2_sum / 1000;
          y = 0;
          v2_sum = 0;
          v2_true = (float)v2 * 4.0f / 4096.0f - 2.0f;
          show_one_decimal(90,50,v2_true,12);
        }
      }
    }
    osDelay(1);
  }
  /* USER CODE END Data */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

