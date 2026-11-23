# STM32H743VIT6 以太网视频传输

在 **STM32H743VIT6**（WeAct Mini H743VITx）上使用 **LwIP + 板载以太网** 传输摄像头视频流的最小工程，配套 OpenMV 端采集与 PC 端接收脚本。

## 组成

| 部分 | 文件 |
|---|---|
| MCU 端 | `src/`（`main.c`、`ethernetif.c`、`app_ethernet.c`、`http_stream.c`） |
| 头文件 | `include/` |
| LwIP 库 | `lib/lwip/` |
| 链接脚本 | `custom.ld`、`custom_flash.ld`、`eth_sections.ld` |
| OpenMV 发送端 | `openmv_stream.py` |
| PC 接收 / TCP 测试 | `tcp_server.py` |
| 辅助脚本 | `fix_ethernetif.py`、`exclude_new_eth.py` |

## 编译与烧录

```bash
pio run
pio run -t upload     # ST-Link
```

串口波特率 115200。

## 说明

- 使用 H7 的 ETH 外设 + LwIP，`ethernetif.c` 与 HAL 以太网驱动按工程需要做过修改（见 `include/` 中的 `_old` 备份头文件）。
- 链接脚本将以太网描述符 / 缓冲放到 D2 SRAM，使用前请按自己的板级内存布局核对 `eth_sections.ld`。
