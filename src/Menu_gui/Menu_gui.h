/*
 * Menu_gui.h
 *
 *  Created on: 2021Äê9ÔÂ3ÈÕ
 *      Author: Zuoge
 */

#ifndef SRC_MENU_GUI_H_
#define SRC_MENU_GUI_H_

#define MENU_FONT_SIZE          8
#define MENU_FONT_W             6
#define MENU_ITEM_NUM           7
#define MENU_OPTION_NUM         2
#define MENU_ITEM_DISPLAY_NUM   7

extern int Event_layer;

typedef struct
{
    char *item[MENU_ITEM_NUM];
    void (*event[MENU_ITEM_NUM])();
    char *option[MENU_OPTION_NUM];
    unsigned char item_used;
    unsigned char item_active;
    unsigned char item_display_list[MENU_ITEM_DISPLAY_NUM];
    unsigned char click_symbol;
    unsigned char menu_active;
}menu_t;

extern menu_t menu_main;
extern menu_t menu_monitor_option;

menu_t M_gui_init();

//void M_gui_enable(menu_t * MENU);
//void M_gui_disable(menu_t * MENU);
void M_gui_active(menu_t * MENU);
void M_gui_sleep(menu_t * MENU);

void M_gui_creat_item(menu_t * MENU , int where , char *name);
void M_gui_creat_option(menu_t * MENU , int num , char *name);
void M_gui_update_display(menu_t * MENU);
void M_gui_select_item_next(menu_t * MENU);
void M_gui_select_item_back(menu_t * MENU);
void M_gui_set_item_used(menu_t * MENU , int used_num);

void M_gui_set_event(menu_t * MENU , int num , void(*ptr)());
void M_gui_enter_event(menu_t * MENU);
void M_gui_exit_event(menu_t * MENU);
void M_gui_enter_exit_event(menu_t * MENU);
void M_gui_event_start(menu_t * MENU);
void M_gui_event_start_no_clear_display(menu_t * MENU);


#endif /* SRC_MENU_GUI_H_ */
