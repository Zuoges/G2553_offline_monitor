/*
 * Battery.c
 *
 *  Created on: 2021年9月4日
 *      Author: Zuoge
 */

#include <msp430.h>
#include "ADC10.h"
#include "SSD1306/SSD1306_api.h"
#include "SSD1306/Font_code.h"
#include "Battery.h"


/*
 * 函数名:BAT_measure_init
 * 输   入:无
 * 返   回:无
 * 说   明:用于初始化电池测量的IO口，在此之前应该先初始化ADC
 *       同时设置P2.1 P2.2两个IO口为输入
 *       P2.1连接至TP4057的CHRG
 *       P2.2连接至TP4057的STDBY
 */
void BAT_measure_init()
{
    P2DIR &= ~(BIT1 + BIT2);
    P1SEL |= BIT0;
    P1SEL2 |= BIT0;
}

/*
 * 函数名:BAT_volt_get
 * 输   入:无
 * 返   回:unsigned char BAT_level
 * 说   明:测量得到电池电压的ADC量化数值，并根据量化数值确定电池电量等级，
 *       并将等级返回出去
 */
unsigned char BAT_volt_get()
{
    unsigned int BAT_volt_bit;
    unsigned char BAT_level;
    BAT_volt_bit = ADC10_in(INCH_0);
    if(BAT_volt_bit >= 799)
        BAT_level = 4;
    else if(BAT_volt_bit >= 778)
        BAT_level = 3;
    else if(BAT_volt_bit >= 762)
        BAT_level = 2;
    else if(BAT_volt_bit >= 747)
        BAT_level = 1;
    else
        BAT_level = 0;
    return BAT_level;
}

/*
 * 函数名:BAT_volt_display
 * 输   入:unsigned char x        需要显示的电池的x轴坐标
 *       unsigned char y        需要显示的电池的y轴坐标
 *       unsigned char level    需要显示的电池的电量等级
 * 返   回:无
 * 说   明:在指定位置根据电量等级显示电池
 */
void BAT_volt_display(unsigned char x , unsigned char y , unsigned char level)
{
    unsigned char i = 0;
    SSD1306_set_pos(x , y);
    for(i = 0 ; i < 17 ; i++)
    {
        SSD1306_write_data(BAT_IMG[level][i]);
    }
}

/*
 * 函数名:BAT_volt_display
 * 输   入:unsigned char x        需要显示的电池的x轴坐标
 *       unsigned char y        需要显示的电池的y轴坐标
 * 返   回:无
 * 说   明:在指定位置显示正在充电的电池的动画
 */
void BAT_charging_display(unsigned char x , unsigned char y)
{
    static unsigned char i = 0;
    static unsigned char count = 0 ;

    BAT_volt_display(x , y , i);
    count++;
    if(count > 50)
    {
        count = 0;
        i++;
        if(i >= 5)
            i = 0;
    }

}

/*
 * 函数名:BAT_measure_display
 * 输   入:unsigned char x        需要显示的电池的x轴坐标
 *       unsigned char y        需要显示的电池的y轴坐标
 * 返   回:无
 * 说   明:根据TP4057的CHRG、STDBY的状态确定电池的状态以显示相应的电池动画
 */
void BAT_measure_display(unsigned char x , unsigned char y)
{
    unsigned char BAT_level;
    if(TP4057_CHRG_IN == 0)
    {
        BAT_charging_display(x , y);
    }
    else if(TP4057_STDBY_IN == 0)
    {
        BAT_volt_display(x , y , 4);
    }
    else
    {
        BAT_level = BAT_volt_get();
        BAT_volt_display(x , y , BAT_level);
    }
}
