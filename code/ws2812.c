#include "ws2812.h"
#include "tim.h"
#include "usart.h"
#include <string.h>

frame_buff_t frame;
//= {  .head[0] = 0,
//                        .head[1] = 0,
//                        .head[2] = 0,
//                        .tail    = 0,
//                    };
/**
 * 写入数据到ws2812
 * @param *color[3] RGB颜色数组
 * @param len   点亮的灯珠长度
 */                    
void ws2812_write(uint8_t (*color)[3], uint16_t len)
{
    uint16_t i;
    uint8_t j;
    for(i=0; i<len; i++)
    {
        for(j=0; j<8; j++)
        {
            frame.data[24*i+j] = (color[i][1] & (0x80>>j))?BIT_1:BIT_0;
            frame.data[24*i+j+8] = (color[i][0] & (0x80>>j))?BIT_1:BIT_0;
            frame.data[24*i+j+16] = (color[i][2] & (0x80>>j))?BIT_1:BIT_0;
        }
    }
    
    memset(frame.head, 0, 3);
    memset(frame.tail, 0, 3);

    HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_2, (uint32_t *)&frame, FRAME_SIZE);
    HAL_Delay(2);
    HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_2, (uint32_t *)&frame, FRAME_SIZE); //再发一次保证显示
}

void ws2812_reset(void) 
{ 	
    uint32_t i;
    uint8_t j;
    uint8_t color[3] = {0};
    memset(frame.head, 0, 3);
    memset(frame.tail, 0, 3);
    for(i=0;i<(24*PIXEL_MAX);i++)
    {
        frame.data[i] = BIT_0;
    }
    
	HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_2, (uint32_t *)&frame, FRAME_SIZE);
}

//PWM回调函数 
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_2);
}

//void Din_1(void)
//{
//    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 153);
//}
//void Din_0(void)
//{
//	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 72);
//}

//void Send_8bits(uint8_t dat) 
//{   
//    uint8_t i; 
//    Din_0();
//    for(i=0;i<8;i++)   
//    { 
//        if(dat & 0x80)//1,for "1",H:0.8us,L:0.45us;
//        {      
//            Din_1();				
//        } 		
//        else 	//0 ,for "0",H:0.4us,L:	0.85us			
//        { 
//            Din_0();					
//        }
//        dat=dat<<1; 
//    }
//}
////G--R--B
////MSB first	
//void Send_2811_24bits(uint8_t GData,uint8_t RData,uint8_t BData)
//{   
//    Send_8bits(GData);  
//    Send_8bits(RData);  
//    Send_8bits(BData);
//}

//void rst() 
//{ 
//	 __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, 0);	
//	 //HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2); 
//	 HAL_Delay (1);
//}

//#define numLEDs 9

//uint8_t rBuffer[numLEDs]={0};
//uint8_t gBuffer[numLEDs]={0};
//uint8_t bBuffer[numLEDs]={0};

//void setAllPixelColor(uint8_t r, uint8_t g, uint8_t b)
//{ 
//    uint8_t i=0;
//    for(i=0;i<numLEDs;i++)
//    {
//        rBuffer[i]=r;
//        gBuffer[i]=g;
//        bBuffer[i]=b;
//    }
//    for(i=0;i<numLEDs;i++)
//    {							  
//        Send_2811_24bits(rBuffer[i],gBuffer[i],bBuffer[i]);
//    }
//}
//void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b)
//{	 
//    uint8_t i=0;
//    rBuffer[n]=r;
//    gBuffer[n]=g;
//    bBuffer[n]=b;
//    for(i=0;i<numLEDs;i++)
//    {							  
//        Send_2811_24bits(rBuffer[i],gBuffer[i],bBuffer[i]);
//    }
//}

//void SetPixelColor(uint16_t n, uint32_t c)
//{	 
//    uint8_t i=0;
//    rBuffer[n]=(uint8_t)(c>>16);
//    gBuffer[n]=(uint8_t)(c>>8);
//    bBuffer[n]=(uint8_t)c;
//    for(i=0;i<numLEDs;i++)
//    {							  
//        Send_2811_24bits(rBuffer[i],gBuffer[i],bBuffer[i]);
//    }
//}

//void PixelUpdate()
//{
//	rst();
//}



//uint32_t Color(uint8_t r, uint8_t g, uint8_t b)
//{
//    return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
//}

//// Fill the dots one after the other with a color
//void colorWipe(uint32_t c, uint8_t wait) 
//{
//    uint16_t i=0;
//    for( i=0; i<numLEDs; i++) 
//    {
//        SetPixelColor(i, c);
//        PixelUpdate();
//        HAL_Delay(wait);
//    }
//}

//uint32_t Wheel(uint8_t WheelPos)
//{
//    WheelPos = 255 - WheelPos;
//    if(WheelPos < 85) 
//    {
//        return Color(255 - WheelPos * 3, 0, WheelPos * 3);
//    }
//    if(WheelPos < 170) {
//        WheelPos -= 85;
//        return Color(0, WheelPos * 3, 255 - WheelPos * 3);
//    }
//    WheelPos -= 170;
//    return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
//}

////彩虹
//void rainbow(uint8_t wait)
//{
//    uint16_t i, j;

//    for(j=0; j<256; j++) 
//    {
//        for(i=0; i<numLEDs; i++)
//        {
//            SetPixelColor(i, Wheel((i+j) & 255));
//        }
//        PixelUpdate();
//        HAL_Delay (wait);
//    }
//}



