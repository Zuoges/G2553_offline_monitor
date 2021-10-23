/*
 * UART.h
 *
 *  Created on: 2021Äê8ÔÂ18ÈÕ
 *      Author: Zuoge
 */

#ifndef SRC_UART_H_
#define SRC_UART_H_

extern char RX_buf[100];
//extern char RX_buf_out[50];
extern unsigned char RX_buf_count;
extern unsigned char RX_over;

void Uart_init();
void Uart_TX(char *_ptr);




#endif /* SRC_UART_H_ */
