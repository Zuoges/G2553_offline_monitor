/*
 * Monitor.c
 *
 *  Created on: 2021��8��19��
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
 * ������:Monitor_update
 * ��   ��:��
 * ��   ��:��
 * ˵   ��:���ߴ��ڼ�������Ҫ����
 *       ��RX_overΪ1ʱ��������յ���һ���������ַ���
 *       ���벢���ַ�����һ��21���ַ��������㻻�еķ�ʽ��ʾ����Ļ��
 *       ��ʾ��Ϻ�RX_over��0��ͬʱ���ַ�����RX_buf_countҲ��0
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
