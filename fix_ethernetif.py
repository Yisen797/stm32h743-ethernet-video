"""Apply all fixes to ethernetif.c"""
import os

SRC = r"%TOOLS%/STM32H743VIT6资料\src\ethernetif.c"
REF = r"%TOOLS%/STM32H743VIT6资料\【模组星球】STM32H743VIT6_IOT\配套程序\高级例程-以太网HTTP Server\高级例程-以太网HTTP Server\Bsp\Src\ethernetif.c"

# Step 1: Copy reference
with open(REF, 'rb') as f:
    data = f.read()

# Step 2: Add GCC DMA block
old1 = b'__attribute__((at(0x30040200))) uint8_t Rx_Buff[ETH_RX_DESC_CNT][ETH_MAX_PACKET_SIZE]; /* Ethernet Receive Buffer */\r\n#endif'
new1 = b'__attribute__((at(0x30040200))) uint8_t Rx_Buff[ETH_RX_DESC_CNT][ETH_MAX_PACKET_SIZE]; /* Ethernet Receive Buffer */\r\n\r\n#elif defined ( __GNUC__ )   /* GCC */\r\nETH_DMADescTypeDef DMARxDscrTab[ETH_RX_DESC_CNT] __attribute__((aligned(32)));\r\nETH_DMADescTypeDef DMATxDscrTab[ETH_TX_DESC_CNT] __attribute__((aligned(32)));\r\nuint8_t Rx_Buff[ETH_RX_DESC_CNT][ETH_MAX_PACKET_SIZE] __attribute__((aligned(32)));\r\n#endif'
data = data.replace(old1, new1)

# Step 3: Add header guards
old2 = b'#include "stm32h7xx_hal.h"\r\n'
new2 = b'#define STM32H7xx_HAL_ETH_H\r\n#define HAL_ETH_MODULE_ENABLED\r\n#include "stm32h7xx_hal.h"\r\n#include "stm32h7xx_hal_eth_old.h"\r\n'
data = data.replace(old2, new2)

# Step 4: Replace HAL_ETH_Start_IT with direct DMA init
old3 = b'\t\tHAL_ETH_GetMACConfig(&heth, &MACConf); \r\n\t\tMACConf.DuplexMode = duplex;\r\n\t\tMACConf.Speed = speed;\r\n\t\t\r\n\t\tHAL_ETH_SetMACConfig(&heth, &MACConf);\t\t\t\r\n\t\tHAL_ETH_Start_IT(&heth);\t\r\n\t\t\r\n\t\tnetif_set_up(netif);\r\n\t\tnetif_set_link_up(netif);'
new3 = b'\t\t/* Direct ETH DMA init */\r\n\t\t__HAL_RCC_SYSCFG_CLK_ENABLE();\r\n\t\tSYSCFG->PMCR |= SYSCFG_PMCR_EPIS_SEL_2;\r\n\t\tETH->MACCR = ETH_MACCR_IPC | ETH_MACCR_RE | ETH_MACCR_TE;\r\n\t\tETH->DMACRDLAR = (uint32_t)DMARxDscrTab;\r\n\t\tETH->DMACTDLAR = (uint32_t)DMATxDscrTab;\r\n\t\tETH->DMACRDRLR = (uint32_t)(ETH_RX_DESC_CNT - 1);\r\n\t\tETH->DMACTCR |= ETH_DMACTCR_ST;\r\n\t\tETH->DMACRCR |= ETH_DMACRCR_SR;\r\n\t\tETH->DMACIER = ETH_DMACIER_NIE | ETH_DMACIER_RIE | ETH_DMACIER_TIE;\r\n\t\tETH->DMACRDTPR = (uint32_t)&DMARxDscrTab[ETH_RX_DESC_CNT - 1];\r\n\r\n\t\tnetif_set_up(netif);\r\n\t\tnetif_set_link_up(netif);'
if new3 in data:
    print("Step4 already applied")
else:
    if old3 in data:
        data = data.replace(old3, new3)
        print("Step4 applied")
    else:
        print("Step4 OLD3 not found, searching...")
        idx = data.find(b'HAL_ETH_Start_IT')
        if idx >= 0:
            print(f"Found HAL_ETH_Start_IT at offset {idx}")
            print(data[idx-50:idx+100])

with open(SRC, 'wb') as f:
    f.write(data)
print("Done!")
