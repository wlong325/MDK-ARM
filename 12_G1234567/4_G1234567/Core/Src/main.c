/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint8_t Key_Scan(void);
void Key_Proc(void);
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
//Variables
signed char B1 = -1;
signed char B2 = -1;
signed char B3 = -1;

char KeyX[3] = {1,2,3};
char Kerror = 0;

int F = 2000;
int D = 10;

char Show_B1[20];
char Show_B2[20];
char Show_B3[20];	

char Show_F[20];
char Show_D[20];

//Flags
char Show_Flag = 0; //LCD界面切换标志

//时间基准（TIM17 1ms 节拍，由中断写入 -> 需 volatile）
volatile uint32_t g_ms_tick = 0;

//按键事件（Key_Proc 写入，Data_Proc 消费）
uint8_t Pwd_OK_Event  = 0;
uint8_t Pwd_Err_Event = 0;

//LED 状态与输出信号（由 Data_Proc 管理）
uint32_t LD1_Start = 0;
uint8_t  LD1_Active = 0;
uint8_t  LD1_Out = 0;
uint32_t LD2_Start = 0;
uint8_t  LD2_Active = 0;
uint8_t  LD2_Out = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Var_Update(void);
void LCD_Proc(void);
void Key_Proc(void);
void Data_Proc(void);
void Led_Proc(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_TIM17_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim17);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	LCD_Init();	
	LCD_Clear(Black);
	LCD_SetBackColor(Black);
	LCD_SetTextColor(White);
	
	LED_Ctrl(LEDALL,OFF);	
  while (1)
  {
		Var_Update();	
		LCD_Proc();
		Key_Proc();	
		Data_Proc();	
		Led_Proc();	
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV3;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
//Update Variables
void Var_Update(void)
{
	if(B1==-1){
		sprintf(Show_B1,"    B1:@              ");
	}else {
		sprintf(Show_B1,"    B1:%d             ", B1);
	}
	if(B2==-1){
		sprintf(Show_B2,"    B2:@              ");
	}else{
		sprintf(Show_B2,"    B2:%d             ", B2);
	}
	if(B3==-1){
		sprintf(Show_B3,"    B3:@              ");
	}else{
		sprintf(Show_B3,"    B3:%d             ", B3);	
	}
	
	sprintf(Show_F,"    F:%dHz        ", F);
	sprintf(Show_D,"    D:%d%%         ", D);
}



//LCD processing function
void LCD_Proc(void)
{			
	if(Show_Flag == 0){ 
		LCD_DisplayStringLine(Line1, (uint8_t *)"       PSD");
		LCD_DisplayStringLine(Line3, (uint8_t *)Show_B1);
		LCD_DisplayStringLine(Line5, (uint8_t *)Show_B2);
		LCD_DisplayStringLine(Line7, (uint8_t *)Show_B3);
	}else if(Show_Flag == 1){
		LCD_DisplayStringLine(Line1, (uint8_t *)"       STA");
		LCD_DisplayStringLine(Line3, (uint8_t *)Show_F);
		LCD_DisplayStringLine(Line5, (uint8_t *)Show_D);
		LCD_DisplayStringLine(Line7, (uint8_t *)"                    ");			
	}
}

uint8_t Key_Scan(void)
{
	char key_value = 0;

	if(Press(KB1) == 0)	{	//key1
		HAL_Delay(2); //去抖
		if(Press(KB1) == 0){
			key_value = 1;
			while(Press(KB1) == 0);	//等待按键松开
		}
	}else	if(Press(KB2) == 0)	{ //key2
		HAL_Delay(2); 
		if(Press(KB2) == 0){
			key_value = 2;
			while(Press(KB2) == 0);	
		}
	}
	else	if(Press(KB3) == 0)	{	//key3
		HAL_Delay(2); 
		if(Press(KB3) == 0){
			key_value = 3;
			while(Press(KB3) == 0);		
		}
	}
	else	if(Press(KB4) == 0)	{	//key4
		HAL_Delay(2); 
		if(Press(KB4) == 0){
			key_value = 4;
			while(Press(KB4) == 0);						
		}
	}
	return key_value;
}

//Key processing function
//Key processing function
void Key_Proc(void)
{
	uint8_t cur_key = 0;
	
	cur_key = Key_Scan();
	
	switch(cur_key)
	{
		case 1:
			B1++;
		  B1%=10;
			break;
		case 2:
			B2++;
		  B2%=10;
			break;
		case 3:
			B3++;
		  B3%=10;
			break;
		case 4:
			if(KeyX[0]==B1&&KeyX[1]==B2&&KeyX[2]==B3){
				Show_Flag++;
				Pwd_OK_Event = 1;
			}
			else{
				B1=-1;
				B2=-1;
				B3=-1;
				Pwd_Err_Event = 1;
			}
			break;		
		default:
			;
	}
}

//数据处理：消费事件 -> 驱动 LED 状态机 -> 输出信号
void Data_Proc(void)
{
	//1. 消费按键事件
	if(Pwd_OK_Event){
		Pwd_OK_Event = 0;
		LD1_Active = 1;
		LD1_Start = g_ms_tick;
	}
	if(Pwd_Err_Event){
		Pwd_Err_Event = 0;
		Kerror++;
		if(Kerror >= 3){
			LD2_Active = 1;
			LD2_Start = g_ms_tick;
		}
	}

	//2. 状态机 -> 输出信号
	if(LD1_Active){
		if(g_ms_tick - LD1_Start >= 5000){
			LD1_Active = 0; LD1_Out = 0;
		}else{
			LD1_Out = 1;
		}
	}
	if(LD2_Active){
		if(g_ms_tick - LD2_Start >= 5000){
			LD2_Active = 0; LD2_Out = 0;
		}else{
			LD2_Out = (g_ms_tick - LD2_Start) / 100 % 2;
		}
	}
}

//LED 输出：仅根据信号点灯灭灯
void Led_Proc(void)
{
	LED_Ctrl(LED1, LD1_Out ? ON : OFF);
	LED_Ctrl(LED2, LD2_Out ? ON : OFF);
}

//TIM17 1ms 中断回调：仅作时间源
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM17){
		g_ms_tick++;
	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
