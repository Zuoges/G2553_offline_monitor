/*
 * ADC10.c
 *
 *  Created on: 2021年9月4日
 *      Author: Zuoge
 */

#include <msp430.h>
#include "ADC10.h"

/*
 * 函数名:ADC10_init
 * 参   数:无
 * 返   回:无
 * 说   明:进行ADC10初始化，设置ADC10单次采样，
 *       启用内部2.5V参考源
 */
void ADC10_init()
{
    ADC10CTL0 &= ~(ADC10SC + ENC);
    ADC10CTL0 |= ADC10ON + REFON + ADC10SHT_3 + SREF_1 + REF2_5V;
    ADC10CTL1 |= SHS_0 + ADC10DIV_3 + ADC10SSEL_0 + CONSEQ_0;
}

/*
 * 函数名:ADC10_in
 * 参   数:unsigned int CH        ADC通道号
 * 返   回:unsigned int ADC10BIT  ADC10采样后量化数据
 * 说   明:将进行ADC10单次采样，并返回量化数据
 */
unsigned int ADC10_in(unsigned int CH)
{
    unsigned int ADC10BIT;
    ADC10CTL0 &= ~(ADC10SC + ENC);
    ADC10CTL1 &= ~(BITF + BITE + BITD + BITC);
    ADC10CTL1 |= CH;
    ADC10CTL0 |= ADC10SC + ENC;
    while(ADC10CTL1 & ADC10BUSY);
    ADC10BIT = ADC10MEM;
    ADC10CTL0 &= ~(ADC10SC + ENC);
    return ADC10BIT;
}


