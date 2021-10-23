/*
 * Switch.c
 *
 *  Created on: 2021年9月3日
 *      Author: Zuoge
 */

#include <msp430.h>
#include "Menu_gui/Menu_gui.h"
#include "Monitor.h"
#include "Game.h"
#include "Switch.h"

void SWP13_Onclick();
void SWP14_Onclick();

/*
 * 函数名:Switch_init
 * 输   入:无
 * 返   回:无
 * 说   明:按键初始化，设置P1.3 P1.4输入，
 *       设置看门狗16ms进入中断，开启看门狗中断
 *       时候看门狗中断进行按键扫描
 */
void Switch_init()
{
    P1DIR &= ~(BIT3 + BIT4);
    //P1REN |=  BIT3 + BIT4;

    WDTCTL = WDT_ADLY_16;
    IE1 |= WDTIE;
}

/*
 * 函数名:WDT_ISR
 * 输   入:无
 * 返   回:无
 * 说   明:看门狗中断，用于按键扫描
 */
#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void)
{
    Switch_scan();
}

/*
 * 函数名:Switch_scan
 * 输   入:无
 * 返   回:无
 * 说   明:按键扫面主函数，用于判定一个按键是否按下，当按键按下时进入相应的事件
 */
void Switch_scan()
{
    static unsigned char SWP13_Now = 0 , SWP14_Now = 0 ;
    unsigned char SWP13_Past = 0 , SWP14_Past = 0 ;

    SWP13_Past = SWP13_Now;
    SWP14_Past = SWP14_Now;

    if( (P1IN&BIT3) != 0)   SWP13_Now = 1;
    else                    SWP13_Now = 0;
    if( (P1IN&BIT4) != 0)   SWP14_Now = 1;
    else                    SWP14_Now = 0;

    if( (SWP13_Past == 1) && (SWP13_Now == 0) )  SWP13_Onclick();
    if( (SWP14_Past == 1) && (SWP14_Now == 0) )  SWP14_Onclick();
}

/*
 * 函数名:SWP13_Onclick
 * 输   入:无
 * 返   回:无
 * 说   明:P1.3上的按键按下事件，用于多级菜单选择下一个选项或者在游戏时发射鸡蛋
 */
void SWP13_Onclick()
{
    if(game_start)
        egg_shoot = 1;
    else if((menu_main.menu_active) && (menu_monitor_option.menu_active))
        M_gui_select_item_next(&menu_monitor_option);
    else if(menu_main.menu_active)
        M_gui_select_item_next(&menu_main);


}

/*
 * 函数名:SWP14_Onclick
 * 输   入:无
 * 返   回:无
 * 说   明:P1.4上的按键按下事件，用于多级菜单进入或者离开事件，在游戏时离开游戏
 */
void SWP14_Onclick()
{
    if(game_start)
        Game_stop();
    else if((menu_main.menu_active) && (menu_monitor_option.menu_active))
        M_gui_enter_exit_event(&menu_monitor_option);
    else if(menu_main.menu_active)
    {
        M_gui_enter_exit_event(&menu_main);
        Monitor_y = 0;          //离开离线窗口监视器时将显示行数重置为第0行
    }
}
