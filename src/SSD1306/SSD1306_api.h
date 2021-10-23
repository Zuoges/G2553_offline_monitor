/*
 * SSD1306_api.h
 *
 *  Created on: 2021Äê8ÔÂ18ÈÕ
 *      Author: Zuoge
 */

#ifndef SRC_SSD1306_SSD1306_API_H_
#define SRC_SSD1306_SSD1306_API_H_

#define SSD1306_W 128
#define SSD1306_H 64

void SSD1306_write_cmd(unsigned char cmd);
void SSD1306_write_data(unsigned char dat);
void SSD1306_color_turn(unsigned i);
void SSD1306_display_turn(unsigned i);
void SSD1306_set_pos(unsigned char x, unsigned char y);
void SSD1306_clear();
void SSD1306_init();

void SSD1306_ShowChar(unsigned char x,unsigned char y, char chr,unsigned char sizey);
unsigned long SSD1306_pow(unsigned char m,unsigned char n);
void SSD1306_ShowNum(unsigned char x,unsigned char y,unsigned long num,unsigned char len,unsigned char sizey);
void SSD1306_ShowString(unsigned char x,unsigned char y, char *chr,unsigned char sizey);

#endif /* SRC_SSD1306_SSD1306_API_H_ */
