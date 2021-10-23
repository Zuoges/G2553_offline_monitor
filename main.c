#include <msp430.h> 
#include "SSD1306/SSD1306_api.h"
#include "Menu_gui/Menu_gui.h"
#include "Menu_gui/Menu_gui_driver.h"
#include "UART.h"
#include "ADC10.h"
#include "Switch.h"
#include "Monitor.h"
#include "Battery.h"
#include "Event.h"
#include "Game.h"

/**
 * main.c
 */

menu_t menu_main;
menu_t menu_monitor_option;

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
    DCOCTL = CALDCO_16MHZ;
    BCSCTL1 = CALBC1_16MHZ;

    Uart_init();
    ADC10_init();
    BAT_measure_init();
    Switch_init();
    SSD1306_init();
    BAT_measure_init();
    TA0_init();
    _EINT();
    Uart_TX("Hello\n");
    Uart_TX("Offline Monitor\n");
    Uart_TX("V1.0\n");

    //多级菜单初始化以及设置项目
    menu_main = M_gui_init();
    M_gui_creat_item(&menu_main, 0, "Monitor");
    M_gui_creat_item(&menu_main, 1, "Monitor Option");
    M_gui_creat_item(&menu_main, 2, "ADC");
    M_gui_creat_item(&menu_main, 3, "Catch the egg");
    M_gui_creat_option(&menu_main, 0, "Select");
    M_gui_creat_option(&menu_main, 1, "Enter");

    M_gui_set_event(&menu_main, 0, Monitor_Rec_Mode);
    M_gui_set_event(&menu_main, 1, Monitor_Rec_Option);
    M_gui_set_event(&menu_main, 2, ADC_Mode);
    M_gui_set_event(&menu_main, 3, Catch_the_egg);

    M_gui_active(&menu_main);

    menu_monitor_option = M_gui_init();
    M_gui_creat_item(&menu_monitor_option, 0, "Baud:");
    M_gui_creat_item(&menu_monitor_option, 1, "Parity:");
    M_gui_creat_item(&menu_monitor_option, 2, "Data Bits:");
    M_gui_creat_item(&menu_monitor_option, 3, "Stop Bits:");
    M_gui_creat_item(&menu_monitor_option, 4, "MSB first:");
    M_gui_creat_item(&menu_monitor_option, 5, "Rows:");
    M_gui_creat_item(&menu_monitor_option, 6, "Exit ");
    M_gui_creat_option(&menu_monitor_option, 0, "Select");
    M_gui_creat_option(&menu_monitor_option, 1, "Enter");

    M_gui_set_event(&menu_monitor_option, 0, Monitor_Baud);
    M_gui_set_event(&menu_monitor_option, 1, Monitor_Parity);
    M_gui_set_event(&menu_monitor_option, 2, Monitor_Data_Bits);
    M_gui_set_event(&menu_monitor_option, 3, Monitor_Stop_Bits);
    M_gui_set_event(&menu_monitor_option, 4, Monitor_MSB_first);
    M_gui_set_event(&menu_monitor_option, 5, Monitor_Rows);
    M_gui_set_event(&menu_monitor_option, 6, Monitor_Option_Exit);


    //主循环
    while(1)
    {
        BAT_measure_display(56 , 7);        //显示电池电量
        M_gui_update_display(&menu_main);   //|显示菜单
        M_gui_event_start(&menu_main);      //|当前菜单对应时间
        _NOP();                             //空事件，调试用
    }

	return 0;
}
