/*
 * SSD1306_draw.h
 *
 *  Created on: 2021Äê8ÔÂ18ÈÕ
 *      Author: Zuoge
 */

#ifndef SRC_SSD1306_SSD1306_DRAW_H_
#define SRC_SSD1306_SSD1306_DRAW_H_

void SSD1306_ShowChar(unsigned char x,unsigned char y, char chr,unsigned char sizey);
unsigned long SSD1306_pow(unsigned char m,unsigned char n);
void SSD1306_ShowNum(unsigned char x,unsigned char y,unsigned long num,unsigned char len,unsigned char sizey);
void SSD1306_ShowString(unsigned char x,unsigned char y, char *chr,unsigned char sizey);

#endif /* SRC_SSD1306_SSD1306_DRAW_H_ */
