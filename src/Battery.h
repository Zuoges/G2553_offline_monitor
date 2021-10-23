/*
 * Battery.h
 *
 *  Created on: 2021Äê9ÔÂ4ÈÕ
 *      Author: Zuoge
 */

#ifndef SRC_BATTERY_H_
#define SRC_BATTERY_H_

#define TP4057_CHRG_IN      (P2IN&BIT1)
#define TP4057_STDBY_IN     (P2IN&BIT2)

void BAT_measure_init();
unsigned char BAT_volt_get();
void BAT_volt_display(unsigned char x , unsigned char y , unsigned char level);
void BAT_charging_display(unsigned char x , unsigned char y);
void BAT_measure_display(unsigned char x , unsigned char y);

#endif /* SRC_BATTERY_H_ */
