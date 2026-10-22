#include "stm32g4xx_hal.h"

#define ON	1
#define OFF	0

#define LED1			GPIO_PIN_8
#define LED2    	GPIO_PIN_9
#define LED3    	GPIO_PIN_10
#define LED4    	GPIO_PIN_11
#define LED5    	GPIO_PIN_12
#define LED6    	GPIO_PIN_13
#define LED7    	GPIO_PIN_14
#define LED8    	GPIO_PIN_15
#define LEDALL		GPIO_PIN_All

void LED_Ctrl(uint16_t LED, uint8_t LED_Status);
