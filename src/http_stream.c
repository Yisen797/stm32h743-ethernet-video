/**
 * HTTP MJPEG 流媒体服务器
 * 浏览器打开 http://<ip> 即可看到实时视频
 */
#include "stm32h7xx_hal.h"
#include "lwip/tcp.h"
#include <string.h>
#include <stdio.h>

/* ---- 图像参数 ---- */
#define IMG_W   160
#define IMG_H   120

#define BMP_ROW_SIZE  (((IMG_W * 3 + 3) / 4) * 4)
#define BMP_DATA_SIZE (BMP_ROW_SIZE * IMG_H)
#define BMP_FILE_SIZE (54 + BMP_DATA_SIZE)   /* ~58KB */

#define BOUNDARY  "frame"

static struct tcp_pcb *http_pcb = NULL;
static uint32_t frame_count = 0;
static uint8_t bmp_buf[BMP_FILE_SIZE];       /* 静态缓冲 */

static const char http_header[] =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: multipart/x-mixed-replace; boundary=" BOUNDARY "\r\n"
    "Cache-Control: no-cache\r\n"
    "\r\n";

/* ---- BMP 生成 ---- */
static void generate_bmp(uint32_t counter)
{
    static const uint8_t colors[7][3] = {
        {0, 0, 255}, {0, 255, 0}, {255, 0, 0},
        {0, 255, 255}, {255, 0, 255}, {255, 255, 0}, {255, 128, 0},
    };
    int bar_w = IMG_W / 7;
    int bar_offset = (counter * 4) % IMG_W;
    int x, y, ci, row_pad = BMP_ROW_SIZE - IMG_W * 3;
    uint8_t *p;

    memset(bmp_buf, 0, BMP_FILE_SIZE);

    /* BMP File Header */
    bmp_buf[0] = 'B'; bmp_buf[1] = 'M';
    bmp_buf[2] = BMP_FILE_SIZE & 0xFF;
    bmp_buf[3] = (BMP_FILE_SIZE >> 8) & 0xFF;
    bmp_buf[4] = (BMP_FILE_SIZE >> 16) & 0xFF;
    bmp_buf[5] = (BMP_FILE_SIZE >> 24) & 0xFF;
    bmp_buf[10] = 54;
    bmp_buf[14] = 40;
    bmp_buf[18] = IMG_W & 0xFF;
    bmp_buf[19] = (IMG_W >> 8) & 0xFF;
    bmp_buf[22] = IMG_H & 0xFF;
    bmp_buf[23] = (IMG_H >> 8) & 0xFF;
    bmp_buf[26] = 1;
    bmp_buf[28] = 24;

    /* Pixel data (BGR, bottom-up) */
    p = bmp_buf + 54;
    for (y = IMG_H - 1; y >= 0; y--) {
        for (x = 0; x < IMG_W; x++) {
            ci = ((x + bar_offset) % IMG_W) / bar_w;
            if (ci >= 7) ci = 6;
            *p++ = colors[ci][0];
            *p++ = colors[ci][1];
            *p++ = colors[ci][2];
        }
        p += row_pad;
    }
}

/* ---- MJPEG 帧头 ---- */
static int make_frame_hdr(char *buf, int maxlen)
{
    return snprintf(buf, maxlen,
        "--" BOUNDARY "\r\n"
        "Content-Type: image/bmp\r\n"
        "Content-Length: %lu\r\n"
        "\r\n", (unsigned long)BMP_FILE_SIZE);
}

/* ---- LwIP 回调 ---- */
static err_t http_recv_cb(void *arg, struct tcp_pcb *tpcb,
                           struct pbuf *p, err_t err)
{
    if (p) pbuf_free(p);
    return ERR_OK;
}

static void http_err_cb(void *arg, err_t err) {}

static err_t http_sent_cb(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
    /* 流模式: 发完当前帧后继续发下一帧 */
    char hdr[256];
    int hdr_len;

    frame_count++;
    generate_bmp(frame_count);
    hdr_len = make_frame_hdr(hdr, sizeof(hdr));

    if (tcp_write(tpcb, hdr, hdr_len, TCP_WRITE_FLAG_COPY) != ERR_OK ||
        tcp_write(tpcb, bmp_buf, BMP_FILE_SIZE, TCP_WRITE_FLAG_COPY) != ERR_OK) {
        tcp_close(tpcb);
        return ERR_OK;
    }
    tcp_output(tpcb);
    return ERR_OK;
}

static err_t http_accept_cb(void *arg, struct tcp_pcb *newpcb, err_t err)
{
    char hdr[256];
    int hdr_len;

    tcp_arg(newpcb, NULL);
    tcp_recv(newpcb, http_recv_cb);
    tcp_sent(newpcb, http_sent_cb);
    tcp_err(newpcb, http_err_cb);

    /* 发 HTTP 头 + 第一帧 */
    generate_bmp(frame_count);
    hdr_len = make_frame_hdr(hdr, sizeof(hdr));

    if (tcp_write(newpcb, http_header, strlen(http_header), TCP_WRITE_FLAG_COPY) != ERR_OK ||
        tcp_write(newpcb, hdr, hdr_len, TCP_WRITE_FLAG_COPY) != ERR_OK ||
        tcp_write(newpcb, bmp_buf, BMP_FILE_SIZE, TCP_WRITE_FLAG_COPY) != ERR_OK) {
        tcp_abort(newpcb);
        return ERR_ABRT;
    }
    tcp_output(newpcb);
    /* 不发 close，后续帧由 sent 回调发送 */
    return ERR_OK;
}

void http_stream_init(void)
{
    http_pcb = tcp_new();
    if (!http_pcb) return;
    tcp_bind(http_pcb, IP_ADDR_ANY, 80);
    http_pcb = tcp_listen(http_pcb);
    tcp_accept(http_pcb, http_accept_cb);
    printf("[HTTP] port 80 ready\r\n");
}
