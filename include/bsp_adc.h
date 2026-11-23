#ifndef __ADC_H__
#define	__ADC_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/


/****************************ADC_AIN4引脚配置**********************************/
#define ADCx_RCC_CLK_ENABLE()            __HAL_RCC_ADC12_CLK_ENABLE()
#define ADCx                             ADC1

#define ADC_GPIO_ClK_ENABLE()            __HAL_RCC_GPIOA_CLK_ENABLE()
#define ADC_GPIO                         GPIOA
#define ADC_GPIO_PIN                     GPIO_PIN_5        
#define ADC_CHANNEL                      ADC_CHANNEL_19    

/* 扩展变量 ------------------------------------------------------------------*/
extern ADC_HandleTypeDef hadcx;

/* 函数声明 ------------------------------------------------------------------*/
void MX_ADCx_Init(void);
int32_t YS_ADC_GET_Average(uint8_t count);

#endif /* __ADC_H__ */


/**********************END OF FILE************************************/
