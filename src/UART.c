/*
 * UART.c
 *
 *  Created on: 2021年8月18日
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
 * 函数名:Uart_init
 * 输   入:无
 * 返   回:无
 * 说   明:将USCI_A0设置成Uart模式，初始波特率为9600
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
 * 函数名:Uart_TX
 * 输   入:char  *_ptr
 * 返   回:无
 * 说   明:Uart向外输出字符串
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
 * 函数名:USCI0RX_ISR
 * 输   入:无
 * 返   回:无
 * 说   明:USCI0中断，
 *       当RX_over为0且接收到非'\n'和'\r'字符时，将字符存入RX_buf缓存，同时字符计数RX_buf_count进行计数
 *       当接收到'\n'时，将RX_over标志位置1以通知Uart一段字符串接收完毕
 */
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
//    while(!(IFG2&UCA0TXIFG)); // 等待发送完成
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
////    while(!(IFG2&UCA0TXIFG)); // 等待发送完成
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

