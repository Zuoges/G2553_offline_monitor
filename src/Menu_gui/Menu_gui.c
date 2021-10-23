/*
 * Menu_gui.c
 *
 *  Created on: 2021Äê9ÔÂ3ÈÕ
 *      Author: Zuoge
 */

#include <msp430.h>
#include <string.h>
#include "Menu_gui/Menu_gui_driver.h"
#include "Menu_gui/Menu_gui.h"

#define max(a,b) (((a)>=(b))?(a):(b))

int Event_layer = 0;


//void M_gui_enable(menu_t * MENU)
//{
//    MENU->menu_enable = 1;
//}
//
//void M_gui_disable(menu_t * MENU)
//{
//    MENU->menu_enable = 0;
//}

void M_gui_active(menu_t * MENU)
{
    MENU->menu_active = 1;
}

void M_gui_sleep(menu_t * MENU)
{
    MENU->menu_active = 0;
}





menu_t M_gui_init()
{
    int i;
    menu_t menu = {0};
    for(i = 0 ; i < MENU_ITEM_DISPLAY_NUM ; i++)
    {
        menu.item_display_list[i] = i;
    }
    return menu;
}

void M_gui_creat_item(menu_t * MENU , int where , char *name)
{
    if(where < MENU_ITEM_NUM)
    {
        MENU->item[where] = name ;
        MENU->item_used = max(MENU->item_used , where);
    }
    else
    {
        M_gui_ERROR("Item number exceeded the limit");
    }
}

void M_gui_creat_option(menu_t * MENU , int num , char *name)
{
    if(num < MENU_OPTION_NUM)
    {
        MENU->option[num] = name ;
    }
    else
    {
        M_gui_ERROR("Option number exceeded the limit");
    }
}

void M_gui_update_list(menu_t * MENU)
{
    int i , mid;
    if(MENU->menu_active)
    {

        if(MENU->item_active > MENU->item_display_list[MENU_ITEM_DISPLAY_NUM - 1])
        {
            mid = MENU->item_active - MENU->item_display_list[MENU_ITEM_DISPLAY_NUM - 1];
            for(i = 0 ; i < MENU_ITEM_DISPLAY_NUM ; i++)
            {
                MENU->item_display_list[i] = MENU->item_display_list[i] + mid;
            }
        }
        else if(MENU->item_active < MENU->item_display_list[0])
        {
            mid = MENU->item_display_list[0] - MENU->item_active;
            for(i = 0 ; i < MENU_ITEM_DISPLAY_NUM ; i++)
            {
                MENU->item_display_list[i] = MENU->item_display_list[i] - mid;
            }
        }

    }
    else
    {
        M_gui_ERROR("Menu not active");
    }
}

void M_gui_update_display(menu_t * MENU)
{
    int i ;
    if(MENU->menu_active)
    {
//        for(i = 0 ; i < (MENU->item_used + 1) ; i++)
//        {
//            if(i == MENU->item_active)
//            {
//                M_gui_print_select_item(MENU);
//            }
//            else
//            {
//                M_gui_print_sleep_item(MENU , i);
//            }
//        }
        M_gui_update_list(MENU);

        for(i = 0 ; i < MENU_ITEM_DISPLAY_NUM ; i++)
        {
            if(MENU->item_display_list[i] == MENU->item_active)
            {
                M_gui_print_select_item(MENU , i);
            }
            else
            {
                M_gui_print_sleep_item(MENU , i);
            }
        }

        M_gui_print_option(MENU);
    }
    else
    {
        M_gui_ERROR("Menu not active");
    }
}

void M_gui_select_item_next(menu_t * MENU)
{
    if(MENU->click_symbol == 0)
    {
        if(MENU->item_active < MENU->item_used)
            MENU->item_active++;
        else
            MENU->item_active = 0;
    }
}

void M_gui_select_item_back(menu_t * MENU)
{
    if(MENU->click_symbol == 0)
    {
        if(MENU->item_active > 0)
            MENU->item_active--;
        else
            MENU->item_active = MENU->item_used;
    }
}

void M_gui_set_item_used(menu_t * MENU , int used_num)
{
    MENU->item_used = used_num ;
}





void M_gui_set_event(menu_t * MENU , int num , void(*ptr)())
{
    MENU->event[num] = ptr;
}

void M_gui_enter_event(menu_t * MENU)
{
    MENU->click_symbol = 1;
    Event_layer++;
}

void M_gui_exit_event(menu_t * MENU)
{
    MENU->click_symbol = 0;
    Event_layer--;
}

void M_gui_enter_exit_event(menu_t * MENU)
{
    if(MENU->click_symbol == 1)
        M_gui_exit_event(MENU);
    else
        M_gui_enter_event(MENU);
}

void M_gui_event_start(menu_t * MENU)
{
    int i = 0;
    while(MENU->click_symbol)
    {
        if(i == 0)
        {
            M_gui_clear_display();
            i++;
        }
        MENU->event[MENU->item_active]();
        if(MENU->click_symbol == 0)
        {
            M_gui_clear_display();
        }
    }
}

void M_gui_event_start_no_clear_display(menu_t * MENU)
{
    while(MENU->click_symbol)
    {
        MENU->event[MENU->item_active]();
    }
}





