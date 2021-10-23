/*
 * Event.c
 *
 *  Created on: 2021��9��4��
 *      Author: Zuoge
 */

#include <msp430.h>
#include <stdio.h>
#include "Monitor.h"
#include "ADC10.h"
#include "SSD1306/SSD1306_api.h"
#include "Menu_gui/Menu_gui.h"
#include "Menu_gui/Menu_gui_driver.h"
#include "Battery.h"
#include "Game.h"
#include "Event.h"


menu_t menu_monitor_option;

//unsigned int A1_bit = 0 , A2_bit = 0 , bat_bit = 0;
//float A1_volt = 0 , A2_volt = 0 , bat_volt = 0;
//int A1_multiple = 0 , A2_multiple = 0 , bat_multiple = 0;
//char A1bit[5] , A2bit[5] ,  batbit[5] , A1volt[6] , A2volt[6] , batvolt[6];

unsigned int ADC_bit = 0;
float ADC_volt = 0;
int ADC_multiple = 0;
char ADC_str[8];

unsigned char baud = 0 , parity = 0 , databits = 0 , stopbits = 0 , msb = 0 , rows = 3;
char baud_str[7] = "9600", parity_str[5] ="null", databits_str[6] ="8bits", stopbits_str[4] ="one", msb_str[4] ="MSB", rows_str[2] ="8";

/*
 * ������:Monitor_Rec_Mode
 * ��   ��:��
 * ��   ��:��
 * ˵   ��:�������ߴ��ڼ�����
 */
void Monitor_Rec_Mode()
{
    Monitor_update();
}

/*
 * ������:Monitor_Rec_Option
 * ��   ��:��
 * ��   ��:��
 * ˵   ��:�������ߴ��ڼ��������ý��棬�������ò����ʣ�У��λ��ֹͣλ������λ�����ߵ�λ����ʾ����
 */
void Monitor_Rec_Option()
{
    M_gui_active(&menu_monitor_option);
    M_gui_update_display(&menu_monitor_option);
    M_gui_event_start_no_clear_display(&menu_monitor_option);
    SSD1306_ShowString(66, 0, baud_str, 8);
    SSD1306_ShowString(66, 1, parity_str, 8);
    SSD1306_ShowString(66, 2, databits_str, 8);
    SSD1306_ShowString(66, 3, stopbits_str, 8);
    SSD1306_ShowString(66, 4, msb_str, 8);
    SSD1306_ShowString(66, 5, rows_str, 8);
    BAT_measure_display(56 , 7);
}

/*
 * ������:Monitor_Baud
 * ��   ��:��
 * ��   ��:��
 * ˵   ��:���ò����ʣ�����9600��19200��38400��57600��115200��230400��460800֮���л�
 */
void Monitor_Baud()
{
    baud++;
    if(baud >= 7)
        baud = 0;
    switch(baud)
    {
        case 0 : UCA0MCTL &=~(BIT7 + BIT6 + BIT5 + BIT4); UCA0MCTL |= UCBRF_3;  UCA0BR0 = 104; UCA0BR1 = 0; sprintf(baud_str , "9600  ");  break;
        case 1 : UCA0MCTL &=~(BIT7 + BIT6 + BIT5 + BIT4); UCA0MCTL |= UCBRF_1;  UCA0BR0 = 52;  UCA0BR1 = 0; sprintf(baud_str , "19200 ");  break;
        case 2 : UCA0MCTL &=~(BIT7 + BIT6 + BIT5 + BIT4); UCA0MCTL |= UCBRF_1;  UCA0BR0 = 26;  UCA0BR1 = 0; sprintf(baud_str , "38400 ");  break;
        case 3 : UCA0MCTL &=~(BIT7 + BIT6 + BIT5 + BIT4); UCA0MCTL |= UCBRF_6;  UCA0BR0 = 17;  UCA0BR1 = 0; sprintf(baud_str , "57600 ");  break;
        case 4 : UCA0MCTL &=~(BIT7 + BIT6 + BIT5 + BIT4); UCA0MCTL |= UCBRF_11; UCA0BR0 = 8;   UCA0BR1 = 0; sprintf(baud_str , "115200");  break;
        case 5 : UCA0MCTL &=~(BIT7 + BIT6 + BIT5 + BIT4); UCA0MCTL |= UCBRF_3;  UCA0BR0 = 4;   UCA0BR1 = 0; sprintf(baud_str , "230400");  break;
        case 6 : UCA0MCTL &=~(BIT7 + BIT6 + BIT5 + BIT4); UCA0MCTL |= UCBRF_2;  UCA0BR0 = 2;   UCA0BR1 = 0; sprintf(baud_str , "460800");  break;
        default : break;
    }
    M_gui_exit_event(&menu_monitor_option);
}

/*
 * ������:Monitor_Parity
 * ��   ��:��
 * ��   ��:��
 * ˵   ��:����У��λ������null��odd��even֮���л�
 */
void Monitor_Parity()
{
    parity++;
    if(parity >= 3)
        parity = 0;
    switch(parity)
    {
        case 0 : UCA0CTL0 &=~UCPEN; sprintf(parity_str , "null");  break;
        case 1 : UCA0CTL0 |= UCPEN; UCA0CTL0 &=~UCPAR; sprintf(parity_str , "odd ");  break;
        case 2 : UCA0CTL0 |= UCPEN; UCA0CTL0 |= UCPAR; sprintf(parity_str , "even");  break;
        default : break;
    }
    M_gui_exit_event(&menu_monitor_option);
}

/*
 * ������:Monitor_Data_Bits
 * ��   ��:��
 * ��   ��:��
 * ˵   ��:��������λ��������8bits��7bits֮���л�
 */
void Monitor_Data_Bits()
{
    databits++;
    if(databits >= 2)
        databits = 0;
    switch(databits)
    {
        case 0 : UCA0CTL0 &=~UC7BIT; sprintf(databits_str , "8bits");  break;
        case 1 : UCA0CTL0 |= UC7BIT; sprintf(databits_str , "7bits");  break;
        default : break;
    }
    M_gui_exit_event(&menu_monitor_option);
}

/*
 * ������:Monitor_Stop_Bits
 * ��   ��:��
 * ��   ��:��
 * ˵   ��:����ֹͣλ������one��two֮���л�
 */
void Monitor_Stop_Bits()
{
    stopbits++;
    if(stopbits >= 2)
        stopbits = 0;
    switch(stopbits)
    {
        case 0 : UCA0CTL0 &=~UCSPB; sprintf(stopbits_str , "one");  break;
        case 1 : UCA0CTL0 |= UCSPB; sprintf(stopbits_str , "two");  break;
        default : break;
    }
    M_gui_exit_event(&menu_monitor_option);
}

/*
 * ������:Monitor_MSB_first
 * ��   ��:��
 * ��   ��:��
 * ˵   ��:���øߵ�λ������MSB��LSB֮���л�
 */
void Monitor_MSB_first()
{
    msb++;
    if(msb >= 2)
        msb = 0;
    switch(msb)
    {
        case 0 : UCA0CTL0 |= UCMSB; sprintf(msb_str , "MSB");  break;
        case 1 : UCA0CTL0 &=~UCMSB; sprintf(msb_str , "LSB");  break;
        default : break;
    }
    M_gui_exit_event(&menu_monitor_option);
}

/*
 * ������:Monitor_Rows
 * ��   ��:��
 * ��   ��:��
 * ˵   ��:������ʾ����������5��6��7��8֮���л�
 */
void Monitor_Rows()
{
    rows++;
    if(rows >= 4)
        rows = 0;
    switch(rows)
    {
        case 0 : Monitor_y_limit = 4 ;sprintf(rows_str , "5");  break;
        case 1 : Monitor_y_limit = 5 ;sprintf(rows_str , "6");  break;
        case 2 : Monitor_y_limit = 6 ;sprintf(rows_str , "7");  break;
        case 3 : Monitor_y_limit = 7 ;sprintf(rows_str , "8");  break;
        default : break;
    }
    M_gui_exit_event(&menu_monitor_option);
}

/*
 * ������:Monitor_Option_Exit
 * ��   ��:��
 * ��   ��:��
 * ˵   ��:�˳����ߴ��ڼ��������ý��棬�����ϼ��˵�
 */
void Monitor_Option_Exit()
{
    M_gui_exit_event(&menu_monitor_option);
    menu_monitor_option.item_active = 0;
    M_gui_sleep(&menu_monitor_option);
    M_gui_exit_event(&menu_main);
}

/*
 * ������:ADC_Mode
 * ��   ��:��
 * ��   ��:��
 * ˵   ��:����ADCģʽ��ѭ�����β���A1��A2��A0����ͨ����ADC�������ݲ���ʾ����
 *       ͬʱ�������ѹֵһ����ʾ����
 *       ע��:����A0�����ڵ����������ǵ�ص�ѹ����������100k�ĵ�����з�ѹ��
 *       ���Լ����ѹʱ������*2�Ĳ����Ի����ȷ�ĵ�ѹֵ
 */
void ADC_Mode()
{

    SSD1306_ShowString(0, 0, "A1 bit:", 8);
    SSD1306_ShowString(0, 1, "A1 volt:", 8);
    SSD1306_ShowString(0, 2, "A2 bit:", 8);
    SSD1306_ShowString(0, 3, "A2 volt:", 8);
    SSD1306_ShowString(0, 4, "BAT bit:", 8);
    SSD1306_ShowString(0, 5, "BAT volt:", 8);
    M_gui_print_custom_option("" , "Exit");

    ADC_bit = ADC10_in(INCH_1);
    sprintf(ADC_str , "%d " , ADC_bit);
    SSD1306_ShowString(54, 0, ADC_str, 8);
    ADC_volt = ADC_bit * (2.5 / 1024);
    ADC_multiple = ADC_volt * 1000;
    sprintf(ADC_str , "%d.%d  " , ADC_multiple/1000 , ADC_multiple - (ADC_multiple/1000) * 1000);
    SSD1306_ShowString(54, 1, ADC_str, 8);

    ADC_bit = ADC10_in(INCH_2);
    sprintf(ADC_str , "%d " , ADC_bit);
    SSD1306_ShowString(54, 2, ADC_str, 8);
    ADC_volt = ADC_bit * (2.5 / 1024);
    ADC_multiple = ADC_volt * 1000;
    sprintf(ADC_str , "%d.%d  " , ADC_multiple/1000 , ADC_multiple - (ADC_multiple/1000) * 1000);
    SSD1306_ShowString(54, 3, ADC_str, 8);

    ADC_bit = ADC10_in(INCH_0);
    sprintf(ADC_str , "%d " , ADC_bit);
    SSD1306_ShowString(54, 4, ADC_str, 8);
    ADC_volt = ADC_bit * (2.5 / 1024) * 2;
    ADC_multiple = ADC_volt * 1000;
    sprintf(ADC_str , "%d.%d  " , ADC_multiple/1000 , ADC_multiple - (ADC_multiple/1000) * 1000);
    SSD1306_ShowString(54, 5, ADC_str, 8);

}

/*
 * ������:Catch_the_egg
 * ��   ��:��
 * ��   ��:��
 * ˵   ��:���뵽�Ӽ�������Ϸģʽ����ѹС��Ϸ���ǳ����
 */
void Catch_the_egg()
{
    Game_start();
}
