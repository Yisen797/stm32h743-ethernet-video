#ifndef __BSP_LED_H_
#define __BSP_LED_H_

/* ????? ----------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* ???? ------------------------------------------------------------------*/
typedef enum
{
  LED_OFF = 0,
  LED_ON  = 1,
  LED_TOGGLE = 2,
}LEDState_TypeDef;

/* GPIO????? */
#define LED1_RCC_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOE_CLK_ENABLE()
#define LED1_PIN                    GPIO_PIN_9
#define LED1_GPIO_PORT              GPIOE

#define LED2_RCC_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOE_CLK_ENABLE()
#define LED2_PIN                    GPIO_PIN_10
#define LED2_GPIO_PORT              GPIOE


/* ????? */
#define LED1_ON()                   HAL_GPIO_WritePin(LED1_GPIO_PORT,\
                                                      LED1_PIN, GPIO_PIN_RESET);
#define LED1_OFF()                  HAL_GPIO_WritePin(LED1_GPIO_PORT,\
                                                      LED1_PIN, GPIO_PIN_SET);
#define LED1_TOGGLE()               HAL_GPIO_TogglePin(LED1_GPIO_PORT,LED1_PIN);

#define LED2_ON()                   HAL_GPIO_WritePin(LED2_GPIO_PORT,\
                                                      LED2_PIN, GPIO_PIN_RESET);
#define LED2_OFF()                  HAL_GPIO_WritePin(LED2_GPIO_PORT,\
                                                      LED2_PIN, GPIO_PIN_SET);
#define LED2_TOGGLE()               HAL_GPIO_TogglePin(LED2_GPIO_PORT,LED2_PIN);

/* ???? ------------------------------------------------------------------*/

/* ???? ------------------------------------------------------------------*/

void LED_GPIO_Init(void);

#endif /* __BSP_LED_H_ */


/**********************END OF FILE************************************/
