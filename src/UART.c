/*
 * UART.c
 *
 *  Created on: 2021��8��18��
 *      Author: Zuoge
 */

#include <string.h>
#include <msp430.h>
#include "UART.h"
#include "Monitor.h"

char RX_buf[100] = "";
//char RX_buf_out[50] = "";
unsigned char RX_buf_count = 0;
//unsigned char RX_buf_count_out = 0;
unsigned char RX_over = 0;

/*
 * ������:Uart_init
 * ��   ��:��
 * ��   ��:��
 * ˵   ��:��USCI_A0���ó�Uartģʽ����ʼ������Ϊ9600
 */
void Uart_init()
{
    P1SEL |= BIT1 + BIT2;
    P1SEL2 |= BIT1 + BIT2;

    UCA0CTL1 |= UCSWRST + UCSSEL_2;
    UCA0CTL0 &= ~(UC7BIT + UCSYNC);
    UCA0MCTL = UCBRF_3 | UCBRS_0 | UCOS16;

    UCA0BR0 = 104;
    UCA0BR1 = 0;

    UCA0CTL1 &= ~UCSWRST;

    IE2 |= UCA0RXIE;
}

/*
 * ������:Uart_TX
 * ��   ��:char  *_ptr
 * ��   ��:��
 * ˵   ��:Uart��������ַ���
 */
void Uart_TX(char *_ptr)
{
    unsigned int i, len;
    len = strlen(_ptr);
    for(i = 0 ; i < len ; i++)
    {
        while(!(IFG2 & UCA0TXIFG));
        UCA0TXBUF = (unsigned char) _ptr[i];
    }
}

/*
 * ������:USCI0RX_ISR
 * ��   ��:��
 * ��   ��:��
 * ˵   ��:USCI0�жϣ�
 *       ��RX_overΪ0�ҽ��յ���'\n'��'\r'�ַ�ʱ�����ַ�����RX_buf���棬ͬʱ�ַ�����RX_buf_count���м���
 *       �����յ�'\n'ʱ����RX_over��־λ��1��֪ͨUartһ���ַ����������
 */
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
//    while(!(IFG2&UCA0TXIFG)); // �ȴ��������
//    UCA0TXBUF = UCA0RXBUF; //

    if((RX_over == 0)&&(UCA0RXBUF != 0x0A)&&(UCA0RXBUF != 0x0D))
    {
        RX_buf[RX_buf_count] = UCA0RXBUF;
        RX_buf_count++;
    }
    if((UCA0RXBUF == 0x0A))
    {
        RX_over = 1;
    }

}

//#pragma vector=USCIAB0RX_VECTOR
//__interrupt void USCI0RX_ISR(void)
//{
////    while(!(IFG2&UCA0TXIFG)); // �ȴ��������
////    UCA0TXBUF = UCA0RXBUF; //
//
//
//    if((UCA0RXBUF != 0x0A)&&(UCA0RXBUF != 0x0D))
//    {
//        RX_buf[RX_buf_count] = UCA0RXBUF;
//        RX_buf_count++;
//    }
//    if((UCA0RXBUF == 0x0A))
//    {
//        strcpy(RX_buf_out , RX_buf);
//        RX_buf_count_out = RX_buf_count;
//        RX_over = 1;
//        memset(RX_buf , '\0' , RX_buf_count);
//        RX_buf_count = 0;
//    }
//
//}

