/*
 * Menu_gui_driver.h
 *
 *  Created on: 2021Äê9ÔÂ3ÈÕ
 *      Author: Zuoge
 */

#ifndef SRC_MENU_GUI_MENU_GUI_DRIVER_H_
#define SRC_MENU_GUI_MENU_GUI_DRIVER_H_

#include "Menu_gui/Menu_gui.h"

#define SINGLE_COLOR

void M_gui_ERROR(char *error);

#ifdef SINGLE_COLOR

#define SCREEN_W        128
#define SCREEN_H        64

void M_gui_print_string(unsigned char x,unsigned char y, char *chr);
void M_gui_print_option(menu_t * MENU);
void M_gui_print_custom_option(char *left , char *right);
void M_gui_print_select_item(menu_t * MENU , int i);
void M_gui_print_sleep_item(menu_t * MENU , int i);
void M_gui_clear_display();

#endif



#endif /* SRC_MENU_GUI_MENU_GUI_DRIVER_H_ */
