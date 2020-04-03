#ifndef _WS2812_H_
#define _WS2812_H_

#include "main.h"

//灯珠最大数量
#define PIXEL_MAX   39

#define BIT_1   170
#define BIT_0   55

#define TIMING_ONE  170
#define TIMING_ZERO 55

typedef struct
{
    uint16_t head[3];         //先发送3个0等待DMA稳定
    uint16_t data[24 * PIXEL_MAX];
    uint16_t tail[3];
} frame_buff_t;

#define FRAME_SIZE  sizeof(frame_buff_t)/2

void ws2812_write(uint8_t (*color)[3], uint16_t len);
void ws2812_reset(void);

#endif
