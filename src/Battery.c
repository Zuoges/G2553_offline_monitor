/*
 * Battery.c
 *
 *  Created on: 2021��9��4��
 *      Author: Zuoge
 */

#include <msp430.h>
#include "ADC10.h"
#include "SSD1306/SSD1306_api.h"
#include "SSD1306/Font_code.h"
#include "Battery.h"


/*
 * ������:BAT_measure_init
 * ��   ��:��
 * ��   ��:��
 * ˵   ��:���ڳ�ʼ����ز�����IO�ڣ��ڴ�֮ǰӦ���ȳ�ʼ��ADC
 *       ͬʱ����P2.1 P2.2����IO��Ϊ����
 *       P2.1������TP4057��CHRG
 *       P2.2������TP4057��STDBY
 */
void BAT_measure_init()
{
    P2DIR &= ~(BIT1 + BIT2);
    P1SEL |= BIT0;
    P1SEL2 |= BIT0;
}

/*
 * ������:BAT_volt_get
 * ��   ��:��
 * ��   ��:unsigned char BAT_level
 * ˵   ��:�����õ���ص�ѹ��ADC������ֵ��������������ֵȷ����ص����ȼ���
 *       �����ȼ����س�ȥ
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
 * ������:BAT_volt_display
 * ��   ��:unsigned char x        ��Ҫ��ʾ�ĵ�ص�x������
 *       unsigned char y        ��Ҫ��ʾ�ĵ�ص�y������
 *       unsigned char level    ��Ҫ��ʾ�ĵ�صĵ����ȼ�
 * ��   ��:��
 * ˵   ��:��ָ��λ�ø��ݵ����ȼ���ʾ���
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
 * ������:BAT_volt_display
 * ��   ��:unsigned char x        ��Ҫ��ʾ�ĵ�ص�x������
 *       unsigned char y        ��Ҫ��ʾ�ĵ�ص�y������
 * ��   ��:��
 * ˵   ��:��ָ��λ����ʾ���ڳ��ĵ�صĶ���
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
 * ������:BAT_measure_display
 * ��   ��:unsigned char x        ��Ҫ��ʾ�ĵ�ص�x������
 *       unsigned char y        ��Ҫ��ʾ�ĵ�ص�y������
 * ��   ��:��
 * ˵   ��:����TP4057��CHRG��STDBY��״̬ȷ����ص�״̬����ʾ��Ӧ�ĵ�ض���
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
