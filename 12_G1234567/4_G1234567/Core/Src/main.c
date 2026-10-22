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
int CNBR_Size = 0;
int VNBR_Size = 0;
int IDEL_Size = 8;	

float CNBR_Price = 3.50f;
float VNBR_Price = 2.00f;

char Show_CNBR[20];
char Show_VNBR[20];
char Show_IDLE[20];	

char Show_CNBR_Price[20];
char Show_VNBR_Price[20];

//Flags
char Show_Flag = 0; //LCD界面切换标志
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Var_Update(void);
void LCD_Proc(void);
void Key_Proc(void);
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
  /* USER CODE BEGIN 2 */

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
	sprintf(Show_CNBR,"    CNBR:%d           ", CNBR_Size);
	sprintf(Show_VNBR,"    VNBR:%d           ", VNBR_Size);
	sprintf(Show_IDLE,"    IDLE:%d           ", IDEL_Size);	
	
	sprintf(Show_CNBR_Price,"    CNBR:%.2f     ", CNBR_Price);
	sprintf(Show_VNBR_Price,"    VNBR:%.2f     ", VNBR_Price);
}



//LCD processing function
void LCD_Proc(void)
{			
	if(Show_Flag == 0){ 
		LCD_DisplayStringLine(Line1, (uint8_t *)"        Data");
		LCD_DisplayStringLine(Line3, (uint8_t *)Show_CNBR);
		LCD_DisplayStringLine(Line5, (uint8_t *)Show_VNBR);
		LCD_DisplayStringLine(Line7, (uint8_t *)Show_IDLE);
	}else if(Show_Flag == 1){
		LCD_DisplayStringLine(Line1, (uint8_t *)"        Para");
		LCD_DisplayStringLine(Line3, (uint8_t *)Show_CNBR_Price);
		LCD_DisplayStringLine(Line5, (uint8_t *)Show_VNBR_Price);
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
			while(Press(KB1) == 0);	//按键松开效果		
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
			Show_Flag ++;
			Show_Flag %= 2;
			break;
		case 2:
			if(Show_Flag == 1)
			{
				CNBR_Price += 0.5f;
				VNBR_Price += 0.5f;						
			}
			break;
		case 3:
			if(Show_Flag == 1)
			{
				CNBR_Price -= 0.5f;
				VNBR_Price -= 0.5f;
				if(CNBR_Price < 0)
					CNBR_Price = 0.0f;
				if(VNBR_Price < 0)
					VNBR_Price = 0.0f;											
			}
			break;
		case 4:
			break;		
		default:
			;
	}
}

void Led_Proc(void)
{
	if(IDEL_Size > 0)
		LED_Ctrl(LED1, ON);
	else
		LED_Ctrl(LED1, OFF);
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
