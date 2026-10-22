#include "led.h"

void LED_Ctrl(uint16_t LED, uint8_t LED_Status)
{
	if(LED_Status == ON){ //ÁÁµÆ
		HAL_GPIO_WritePin(GPIOC,LED,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
  }        
  else{ //ÃðµÆ
		HAL_GPIO_WritePin(GPIOC,LED,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET); 
  }
}
