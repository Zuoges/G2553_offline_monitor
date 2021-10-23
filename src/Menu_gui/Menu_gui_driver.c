/*
 * Menu_gui_driver.c
 *
 *  Created on: 2021Äê9ÔÂ3ÈÕ
 *      Author: Zuoge
 */

#include <msp430.h>
#include <stdio.h>
#include <string.h>
#include "Menu_gui/Menu_gui.h"
#include "Menu_gui/Menu_gui_driver.h"
#include "SSD1306/SSD1306_api.h"

void M_gui_ERROR(char *error)
{
    //printf("[ERROR] M_gui : %s" , error);
}

#ifdef SINGLE_COLOR

void M_gui_print_string(unsigned char x,unsigned char y, char *chr)
{
    SSD1306_ShowString( x , y , chr , MENU_FONT_SIZE);
}

void M_gui_print_option(menu_t * MENU)
{
    unsigned char i;
    int len = 0 ;

    M_gui_print_string( 0 , SCREEN_H / MENU_FONT_SIZE - 1 , MENU->option[0] );

    for( i = 1 ; i < MENU_OPTION_NUM - 1 ; i++)
    {
        len = strlen(MENU->option[0]);
        M_gui_print_string( ((SCREEN_W / ( MENU_OPTION_NUM - 1 )) - (MENU_FONT_W * len / 2)) , (SCREEN_H / MENU_FONT_SIZE) , MENU->option[i] );
    }

    len = strlen(MENU->option[MENU_OPTION_NUM - 1]);
    M_gui_print_string( SCREEN_W - (len * MENU_FONT_W) , SCREEN_H / MENU_FONT_SIZE - 1 , MENU->option[MENU_OPTION_NUM - 1] );
}

void M_gui_print_custom_option(char *left , char *right)
{
    int len = 0 ;
    M_gui_print_string( 0 , SCREEN_H / MENU_FONT_SIZE , left );
    len = strlen(right);
    M_gui_print_string( SCREEN_W - (len * MENU_FONT_W) , SCREEN_H / MENU_FONT_SIZE - 1 , right );
}

void M_gui_print_select_item(menu_t * MENU , int i)
{
    char s_item[SCREEN_W / MENU_FONT_W] = "";
    sprintf( s_item , ">%s" , MENU->item[MENU->item_active]);
    M_gui_print_string(0 , i , s_item);
}

void M_gui_print_sleep_item(menu_t * MENU , int i)
{
    char s_item[SCREEN_W / MENU_FONT_W] = "";
    sprintf( s_item , " %s" , MENU->item[MENU->item_display_list[i]]);
    M_gui_print_string(0 , i , s_item);
}

void M_gui_clear_display()
{
    SSD1306_clear();
}

#endif

