
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/
//#define USE_DHCP       /* 是否使能DHCP */

/* 静态IP地址设置 */
#define IP_ADDR0   (uint8_t) 192
#define IP_ADDR1   (uint8_t) 168
#define IP_ADDR2   (uint8_t) 1
#define IP_ADDR3   (uint8_t) 107
   
/*静态子网掩码*/
#define NETMASK_ADDR0   (uint8_t) 255
#define NETMASK_ADDR1   (uint8_t) 255
#define NETMASK_ADDR2   (uint8_t) 255
#define NETMASK_ADDR3   (uint8_t) 0

/*静态默认网关*/
#define GW_ADDR0   (uint8_t) 192
#define GW_ADDR1   (uint8_t) 168
#define GW_ADDR2   (uint8_t) 1
#define GW_ADDR3   (uint8_t) 1

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
