#include "stm32h7xx_hal.h"
#include "string.h"
#include "lwip/opt.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/timeouts.h"
#include "netif/etharp.h"
#include "ethernetif.h"
#include "app_ethernet.h"

static void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);

int main(void)
{
  CPU_CACHE_Enable();
  HAL_Init();
  SystemClock_Config();
  Netif_Config();
  while (1) { sys_check_timeouts(); }
}

static void SystemClock_Config(void)
{
  RCC_OscInitTypeDef osc={0}; RCC_ClkInitTypeDef clk={0};
  MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0);
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  while(!(PWR->D3CR & PWR_D3CR_VOSRDY)){}
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);
  osc.OscillatorType=RCC_OSCILLATORTYPE_HSE; osc.HSEState=RCC_HSE_ON;
  osc.PLL.PLLState=RCC_PLL_ON; osc.PLL.PLLSource=RCC_PLLSOURCE_HSE;
  osc.PLL.PLLM=5; osc.PLL.PLLN=160; osc.PLL.PLLP=2; osc.PLL.PLLQ=2; osc.PLL.PLLR=2;
  osc.PLL.PLLRGE=RCC_PLL1VCIRANGE_2; osc.PLL.PLLVCOSEL=RCC_PLL1VCOWIDE; osc.PLL.PLLFRACN=0;
  HAL_RCC_OscConfig(&osc);
  clk.ClockType=RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2|RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  clk.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK; clk.SYSCLKDivider=RCC_SYSCLK_DIV1;
  clk.AHBCLKDivider=RCC_HCLK_DIV2; clk.APB3CLKDivider=RCC_APB3_DIV2;
  clk.APB1CLKDivider=RCC_APB1_DIV2; clk.APB2CLKDivider=RCC_APB2_DIV2; clk.APB4CLKDivider=RCC_APB4_DIV2;
  HAL_RCC_ClockConfig(&clk, FLASH_LATENCY_2);
  HAL_RCC_EnableCSS();
  HAL_SYSTICK_Config(SystemCoreClock/1000);
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  HAL_NVIC_SetPriority(SysTick_IRQn,0,0);
}
static void CPU_CACHE_Enable(void){ SCB_EnableICache(); SCB_EnableDCache(); }
void* husart_debug;
