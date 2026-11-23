"""
TCP 视频接收服务器 — PC 端
接收 STM32H743 发来的 JPEG 帧并实时显示
使用方法: python tcp_server.py
"""
import socket
import struct
import sys
import os
from datetime import datetime

# ---- 配置 ----
HOST = '0.0.0.0'       # 监听所有网卡
PORT = 6000             # 与 H743 板子一致
SAVE_DIR = 'captured'   # 保存目录

os.makedirs(SAVE_DIR, exist_ok=True)


def main():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.bind((HOST, PORT))
    sock.listen(1)

    local_ip = socket.gethostbyname(socket.gethostname())
    print(f"[服务器] 监听 {local_ip}:{PORT}")
    print(f"[服务器] 等待 H743 连接...")
    print(f"[提示] 如果连不上，请确认 PC IP 是否为 192.168.1.102")
    print(f"[提示] 按 Ctrl+C 退出\n")

    conn = None
    try:
        conn, addr = sock.accept()
        print(f"[连接] {addr[0]}:{addr[1]} 已连接")

        buf = b''
        frame_count = 0

        while True:
            data = conn.recv(4096)
            if not data:
                print(f"\n[断开] 连接已关闭")
                break

            buf += data

            # 简单文本模式：打印收到的数据
            # 后续改成 JPEG 帧解析
            try:
                text = data.decode('utf-8', errors='replace')
                if text.strip():
                    print(f"[收] {text.strip()}")
            except:
                pass

            frame_count += 1

    except KeyboardInterrupt:
        print(f"\n[退出] 用户中断")
    except Exception as e:
        print(f"[错误] {e}")
    finally:
        if conn:
            conn.close()
        sock.close()
        print(f"[服务器] 已关闭")


if __name__ == '__main__':
    main()
