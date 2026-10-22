#include "stm32g4xx_hal.h"

#define Press(x)  (HAL_GPIO_ReadPin(x))

#define KB1 GPIOB, GPIO_PIN_0
#define KB2 GPIOB, GPIO_PIN_1
#define KB3 GPIOB, GPIO_PIN_2
#define KB4 GPIOA, GPIO_PIN_0

uint8_t Key_Scan(void);
