/*
 * Monitor.c
 *
 *  Created on: 2021年8月19日
 *      Author: Zuoge
 */
#include <msp430.h>
#include <string.h>
#include "UART.h"
#include "SSD1306/SSD1306_api.h"
#include "Monitor.h"

unsigned char Monitor_x = 0;
unsigned char Monitor_y = 0;

unsigned char Monitor_y_limit = 7;

char RX_long_buf[22] = "";
unsigned char line_count = 0;

/*
 * 函数名:Monitor_update
 * 输   入:无
 * 返   回:无
 * 说   明:离线串口监视器主要部分
 *       当RX_over为1时即代表接收到了一段完整的字符串
 *       进入并将字符串按一行21个字符，超过便换行的方式显示到屏幕上
 *       显示完毕后将RX_over置0，同时将字符计数RX_buf_count也置0
 */
void Monitor_update()
{
    int i , j;
    int k = 0;
    if(RX_over)
    {
        if(RX_buf_count <= 21)
        {
            SSD1306_ShowString(Monitor_x , Monitor_y , "                     " , 8);
            SSD1306_ShowString(Monitor_x , Monitor_y , RX_buf , 8);
            Monitor_y = Monitor_y + 1;
            if(Monitor_y > Monitor_y_limit)
                Monitor_y = 0;
        }
        else
        {
            if((RX_buf_count % 21) == 0)
                line_count = (RX_buf_count / 21);
            else
                line_count = (RX_buf_count / 21) + 1;
            for(i = 0 ; i < line_count ; i++)
            {
                for(j = i * 21 ; j < (i * 21) + 21 ; j++)
                {
                    RX_long_buf[k] = RX_buf[j];
                    k++;
                }
                k = 0;
                SSD1306_ShowString(Monitor_x , Monitor_y , "                     " , 8);
                SSD1306_ShowString(Monitor_x , Monitor_y , RX_long_buf , 8);
                Monitor_y = Monitor_y + 1;
                if(Monitor_y > Monitor_y_limit)
                    Monitor_y = 0;
            }
        }
        RX_over = 0;
        memset(RX_buf , '\0' , RX_buf_count);
        RX_buf_count = 0;
    }
}


//void Monitor_update()
//{
//    int i , j;
//    int k = 0;
//    if(RX_over)
//    {
//        if(RX_buf_count_out <= 21)
//        {
//            SSD1306_ShowString(Monitor_x , Monitor_y , "                     " , 8);
//            SSD1306_ShowString(Monitor_x , Monitor_y , RX_buf_out , 8);
//            Monitor_y = Monitor_y + 1;
//            if(Monitor_y > Monitor_y_limit)
//                Monitor_y = 0;
//        }
//        else
//        {
//            if((RX_buf_count_out % 21) == 0)
//                line_count = (RX_buf_count_out / 21);
//            else
//                line_count = (RX_buf_count_out / 21) + 1;
//            for(i = 0 ; i < line_count ; i++)
//            {
//                for(j = i * 21 ; j < (i * 21) + 21 ; j++)
//                {
//                    RX_long_buf[k] = RX_buf_out[j];
//                    k++;
//                }
//                k = 0;
//                SSD1306_ShowString(Monitor_x , Monitor_y , "                     " , 8);
//                SSD1306_ShowString(Monitor_x , Monitor_y , RX_long_buf , 8);
//                Monitor_y = Monitor_y + 1;
//                if(Monitor_y > Monitor_y_limit)
//                    Monitor_y = 0;
//            }
//        }
//        RX_over = 0;
//    }
//}
