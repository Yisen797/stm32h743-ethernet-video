"""OpenMV 持续发帧 — 3000ms 间隔, 无需握手"""
import sensor, time
from pyb import UART, LED

FRAMESIZE = sensor.QQVGA
UART_BAUD = 921600

uart = UART(3, UART_BAUD, timeout=1000, timeout_char=5)
led = LED(2)

sensor.reset()
sensor.set_pixformat(sensor.JPEG)
sensor.set_framesize(FRAMESIZE)
sensor.set_auto_exposure(True)
sensor.set_auto_gain(True)
sensor.set_auto_whitebal(True)
time.sleep_ms(1500)
led.on()
print("OpenMV ready")

while True:
    try:
        img = sensor.snapshot()
        data = img.bytearray()
        n = len(data)
        if not (0 < n <= 65535):
            continue
        uart.write(bytes([0xAA, 0x55, (n >> 8) & 0xFF, n & 0xFF]))
        for i in range(0, n, 256):
            uart.write(data[i:i+256])
            time.sleep_us(200)
        time.sleep_ms(3000)
    except Exception as e:
        time.sleep_ms(100)
