/*
 * Switch.c
 *
 *  Created on: 2021��9��3��
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
 * ������:Switch_init
 * ��   ��:��
 * ��   ��:��
 * ˵   ��:������ʼ��������P1.3 P1.4���룬
 *       ���ÿ��Ź�16ms�����жϣ��������Ź��ж�
 *       ʱ���Ź��жϽ��а���ɨ��
 */
void Switch_init()
{
    P1DIR &= ~(BIT3 + BIT4);
    //P1REN |=  BIT3 + BIT4;

    WDTCTL = WDT_ADLY_16;
    IE1 |= WDTIE;
}

/*
 * ������:WDT_ISR
 * ��   ��:��
 * ��   ��:��
 * ˵   ��:���Ź��жϣ����ڰ���ɨ��
 */
#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void)
{
    Switch_scan();
}

/*
 * ������:Switch_scan
 * ��   ��:��
 * ��   ��:��
 * ˵   ��:����ɨ���������������ж�һ�������Ƿ��£�����������ʱ������Ӧ���¼�
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
 * ������:SWP13_Onclick
 * ��   ��:��
 * ��   ��:��
 * ˵   ��:P1.3�ϵİ��������¼������ڶ༶�˵�ѡ����һ��ѡ���������Ϸʱ���伦��
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
 * ������:SWP14_Onclick
 * ��   ��:��
 * ��   ��:��
 * ˵   ��:P1.4�ϵİ��������¼������ڶ༶�˵���������뿪�¼�������Ϸʱ�뿪��Ϸ
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
        Monitor_y = 0;          //�뿪���ߴ��ڼ�����ʱ����ʾ��������Ϊ��0��
    }
}
