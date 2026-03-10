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
#include "math.h"
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
uint32_t x = 0;
uint32_t l =0;
uint32_t g = 0;
float Rk_table[4] = {100.0f, 1000.0f, 10000.0f, 100000.0f};
uint64_t sum = 0;
int32_t temp;
double adc_rms;
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
                  if(mode == 4)
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
    if(mode == 1||mode == 5){
      if(v_mode == 0){
        
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13,1);//20V档
        HAL_ADC_Start_DMA(&hadc1, &v20, 1);//20V
        v20_sum += v20;
        e++;
        if(e >= 1999){
          OLED_Fill(60,50,92,63,0);  //清空区域
          v20 = v20_sum / 2000;
          e = 0;
          v20_sum = 0;
          v20_true = (float)v20 * 40.0f / 4096.0f - 20.0f;
          v20_true = v20_true + 0.25f;
          if(v20_true > 19.99f){
            v20_true = 19.99f;
          }
          if(v20_true < -19.99f){
            v20_true = -19.99f;
          }
          show_float(60,50,v20_true,12, 2);
        }
      }
      else if(v_mode == 1){
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13,0);//2V档
        HAL_ADC_Start_DMA(&hadc2, &v2, 1);//2V
        v2_sum += v2;
        y++;
        if(y >= 1999){
          OLED_Fill(60,50,92,63,0);  //清空区域
          v2 = v2_sum / 2000;
          y = 0;
          v2_sum = 0;
          v2_true = (float)v2 * 4.0f / 4096.0f - 2.0f;
          v2_true = v2_true + 0.02;
          if(v2_true > 1.999f){
            v2_true = 1.999f;
          }
          if(v2_true < -1.999f){
            v2_true = -1.999f;
          }
          show_float(60,50,v2_true,12, 3);
        }
      }
    }
    
    
    
    if(mode == 3){        //r档
      if(r_mode == 0){   //100
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9,0);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11,0);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13,0);
      }
      else if(r_mode == 1){   //1k
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9,1);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11,0);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13,0);
      }
      else if(r_mode == 2){   //10k
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9,0);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11,1);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13,0);
      }
      else if(r_mode == 3){   //100k
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9,1);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11,1);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13,0);
      }
      HAL_ADC_Start_DMA(&hadc3, &r, 1);
      x++;
      r_sum += r;
      if(x >= 99){
        OLED_Fill(60,50,92,63,0);  //清空区域
        r = r_sum / 100;
        x = 0;
        r_sum = 0;
        if(r >= 3800){
          r = 3799.9999;
        }
        r_true = (float)Rk_table[r_mode] * (float)r / (3800.0f - (float)r);
        if(r >= 3700){
          r_true = 0;
        }
        if(r_mode == 0){   //100
          if(r_true > 199.9){
            r_true = 199.9;
          }
          show_float(60,50,r_true,12, 1);
        }
        else if(r_mode == 1){   //1k
          r_true = r_true / 1000;
          if(r_true > 1.999){
            r_true = 1.999;
          }
          show_float(60,50,r_true,12, 3);
        }
        else if(r_mode == 2){   //10k
          r_true = r_true / 1000;
          if(r_true > 19.99){
            r_true = 19.99;
          }
          show_float(60,50,r_true,12, 2);
        }
        else if(r_mode == 3){   //100k
          r_true = r_true / 1000;
          if(r_true > 199.9){
            r_true = 199.9;
          }
          show_float(60,50,r_true,12, 1);
        }
      }  
    }
    
    
    
    
    if(mode == 4){      //蜂鸣档
      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9,0);
      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11,0);
      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13,1);
      HAL_ADC_Start_DMA(&hadc3, &r, 1);
      x++;
      r_sum += r;
      if(x >= 99){
        r = r_sum / 100;
        x = 0;
        r_sum = 0;
        if(r >= 3800){
          r = 3799.9999;
        }
        r_true = 10.0f * r / (3800.0f - r);
        if(r_true < 10){
          OLED_Fill(60,48,92,63,0);  //清空区域
          OLED_ShowChinese(60, 48, 26, 16);
          OLED_ShowChinese(77, 48, 27, 16);
        }else{
          OLED_Fill(60,48,92,63,0);  //清空区域
          OLED_ShowChinese(60, 48, 29, 16);
          OLED_ShowChinese(77, 48, 30, 16);
        }
        OLED_Refresh_Gram();
      }
    }
    
    
    
    if(mode == 0){      //二极管档
      HAL_ADC_Start_DMA(&hadc4, &two, 1);
        two_sum += two;
        l++;
        if(l >= 999){
          OLED_Fill(60,50,92,63,0);  //清空区域
          two = two_sum / 1000;
          l = 0;
          two_sum = 0;
          two_true = (float)two * 3.3f / 4095.0f;
          two_true = two_true * 1000;
          if(two_true > 3100){
            OLED_Fill(60,48,92,63,0);  //清空区域
            OLED_ShowChinese(60, 48, 31, 16);
            OLED_ShowChinese(77, 48, 32, 16);
            OLED_Refresh_Gram();
          }else{
            OLED_Fill(60,48,92,63,0);  //清空区域
            show_float(60,50,two_true,12, 0);
          }
        }
    }
    
    if(mode == 2){
      if(v_mode == 0){
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13,1);//20V档
        HAL_ADC_Start_DMA(&hadc1, &v20, 1);//20V
        v20_AC[g] = v20;
        if(g == 999){
          g = 0;
        sum = 0;
        uint32_t f = 0;
        for(int i = 0; i < 1000; i++){
          temp = v20_AC[i] - 2000;   // 去直流偏置
          sum += temp * temp;        // 平方
          if(temp < 0){
            f++;
          }
        }

        adc_rms = sqrt((double)sum / 1000);
        v20_true = (float)adc_rms * 40.0f / 4096.0f;
        if(f>500){
          v20_true = v20_true + 0.35f;
        }else{
          v20_true = v20_true - 0.25f;
        }
        show_float(60,50,v20_true,12, 2);
        }else{
          g++;
        }
      }
      else if(v_mode == 1){
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13,0);//2V档
        HAL_ADC_Start_DMA(&hadc2, &v2, 1);//2V
        v2_AC[g] = v2;
        if(g == 999){
          g = 0;
        sum = 0;
        uint32_t f = 0;
        for(int i = 0; i < 1000; i++){
          temp = v2_AC[i] - 2000;   // 去直流偏置
          sum += temp * temp;        // 平方
          if(temp < 0){
            f++;
          }
        }
        adc_rms = sqrt((double)sum / 1000);
        v2_true = (float)adc_rms * 4.0f / 4096.0f;
        if(f>500){
          v2_true = v2_true - 0.035f;
        }else{
          v2_true = v2_true + 0.025f;
        }
        show_float(60,50,v2_true,12, 3);
        }else{
          g++;
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

