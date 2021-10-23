/*
 * ADC10.c
 *
 *  Created on: 2021��9��4��
 *      Author: Zuoge
 */

#include <msp430.h>
#include "ADC10.h"

/*
 * ������:ADC10_init
 * ��   ��:��
 * ��   ��:��
 * ˵   ��:����ADC10��ʼ��������ADC10���β�����
 *       �����ڲ�2.5V�ο�Դ
 */
void ADC10_init()
{
    ADC10CTL0 &= ~(ADC10SC + ENC);
    ADC10CTL0 |= ADC10ON + REFON + ADC10SHT_3 + SREF_1 + REF2_5V;
    ADC10CTL1 |= SHS_0 + ADC10DIV_3 + ADC10SSEL_0 + CONSEQ_0;
}

/*
 * ������:ADC10_in
 * ��   ��:unsigned int CH        ADCͨ����
 * ��   ��:unsigned int ADC10BIT  ADC10��������������
 * ˵   ��:������ADC10���β�������������������
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


