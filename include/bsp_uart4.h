#ifndef __BSP_UART4_H__
#define __BSP_UART4_H__

#include "stm32h7xx_hal.h"

#define UART4_RX_BUF_SIZE   (64 * 1024)   /* 64KB 环形 DMA 缓冲 */

void UART4_Init(uint32_t baud);
void UART4_Restart(void);
uint8_t* UART4_GetFrame(uint32_t *pLen);
void UART4_ReleaseFrame(void);

#endif
