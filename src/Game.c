/*
 * Game.c
 *
 *  Created on: 2021��9��10��
 *      Author: Zuoge
 */

#include <msp430.h>
#include <stdio.h>
#include "ADC10.h"
#include "SSD1306/SSD1306_api.h"
#include "SSD1306/Font_code.h"
#include "Menu_gui/Menu_gui.h"
#include "Event.h"
#include "Game.h"

unsigned char game_start = 0;
unsigned char game_over = 0;
unsigned char new_round = 1;

int Game_Score = 0;
char Game_Score_str[5] = "";

int egg_to_basket = 0;
int egg_x = 58 , egg_y = 56;
int basket_x = 2 , basket_y = 0;
unsigned char egg_dir = 1 , basket_dir = 1;
unsigned char egg_up_down = 0;
int egg_t = 40 ;

int basket_speed = 0;
int my_basket_speed = 0;

int egg_move_count = 0;
int basket_move_count = 0;
int my_basket_move_count = 0;
int game_over_count = 0;

unsigned char egg_shoot = 0;

int player_life = 0;
unsigned char life_lost = 0;

//int basket;

//  a = 0.24
//  t = 20
//  s = 0.5 * a * t * t

/*
 * ������:TA0_init
 * ��   ��:��
 * ��   ��:��
 * ˵   ��:��ʱ��A0��ʼ��
 */
void TA0_init()
{
    //TA0CCR0 = 32767;
    TA0CTL |= TASSEL_2 + ID_0 + MC_2 + TACLR;

}

/*
 * ������:TimerA0
 * ��   ��:��
 * ��   ��:��
 * ˵   ��:��ʱ��A0�жϣ�����жϽ����нӵ���Ϸ�ĸ����ж�
 */
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TimerA0(void)
{
    basket_move_count++;
    if(basket_move_count > basket_speed)    //�ı����ӵ����꣬�����ӿ��������ƶ�
    {
        basket_move_count = 0;
        if(basket_dir == 1)
        {
            if(basket_x < 116)
                basket_x++;
            else
                basket_dir = 0;
        }
        else if(basket_dir == 0)
        {
            if(basket_x >= 2)
                basket_x--;
            else
                basket_dir = 1;
        }
    }

    my_basket_move_count++;
    if(my_basket_move_count > my_basket_speed) //�ı����ǵ����ӵ����꣬�����ǵ����ӿ��������ƶ�
    {
        my_basket_move_count = 0;
        if(egg_dir == 1)
        {
            if(egg_x < 116)
                egg_x++;
            else
                egg_dir = 0;
        }
        else if(egg_dir == 0)
        {
            if(egg_x >= 2)
                egg_x--;
            else
                egg_dir = 1;
        }
    }

    if(egg_shoot)                           //����������ʱ
    {
        egg_move_count++;
        if(egg_move_count > 3)
        {
            egg_move_count = 0;
            if(egg_up_down == 0)        //����������
            {
                egg_y = 8 + 0.5 * 0.06 * egg_t * egg_t;     //ͨ�����ٶȹ�ʽ���㼦����y������
                egg_t--;
                if(egg_t == -1)
                {
                    egg_up_down = 1;
                    egg_t = 0;           //���ڼ��㼦�������ߵ�αʱ����0
                }
            }
            else if(egg_up_down)        //����������
            {
                egg_y = 8 + 0.5 * 0.06 * egg_t * egg_t;     //ͨ�����ٶȹ�ʽ���㼦����y������
                egg_t++;
                if(egg_y == 23)         //������y����Ϊ23ʱ�����ж�
                {
                    egg_to_basket = egg_x - basket_x;
                    if(egg_to_basket > -10 && egg_to_basket < 10)       //�жϼ���������֮��ľ����Ƿ�С�ڡ�10�����أ�ͬʱ���Ĵ�����Ը����ж�����
                    {                                                   //��Ϊ��
                        Game_Score++;                                   //�ӷ�
                        new_round = 1;                                  //�����»غ�
                        sprintf(Game_Score_str , "%d " , Game_Score);
                        egg_y = 56;                                     //���ü���y��
                        egg_t = 41;                                     //ֱ�����ü��������ߵ�αʱ�䵽di
                    }
                    else
                    {
                        player_life--;                                  //����һ������ֵ
                        life_lost = 1;                                  //��������ֵ�ı�־
                        if(player_life == 0)
                        {
                            //sprintf(Game_Score_str , "%d " , Game_Score);
                            game_over = 1;                              //������ֵΪ0ʱ����Ϸ����
                        }
                    }
                }
                if(egg_t == 41)
                {
                    egg_up_down = 0;        //���ü�������״̬
                    egg_t = 40;             //�������ڼ��㼦�������ߵ�αʱ��
                    egg_shoot = 0;          //���ü�������״̬
                }
            }
        }
    }

    TA0IV = 0;
}

/*
 * ������:Basket_display
 * ��   ��:unsigned char x    ��Ҫ��ʾ�����ӵ�x������
 *       unsigned char y    ��Ҫ��ʾ�����ӵ�y������
 * ��   ��:��
 * ˵   ��:��ָ����λ����ʾ���ӣ�ע�⣬�����Y��Ϊ0-7�˸���ֵ
 *       ����ο�SSD1306�������ֲᣬSSD1306��8������Ϊһ�У���Ҳ�����ܰ�
 */
void Basket_display(unsigned char x , unsigned char y)
{
    unsigned char i = 0;
    SSD1306_set_pos(x - 1 , y);
    SSD1306_write_data(0x00);
    SSD1306_set_pos(x , y);
    for(i = 0 ; i < 12 ; i++)
    {
        SSD1306_write_data(basket_IMG[i]);
    }
    SSD1306_write_data(0x00);
}

/*
 * ������:Basket_display
 * ��   ��:unsigned char x    ��Ҫ��ʾ�����ӵ�x������
 *       unsigned char y    ��Ҫ��ʾ�����ӵ�y������
 * ��   ��:��
 * ˵   ��:��ָ����λ����ʾ���ӣ�ע�⣬�����Y��Ϊ0-63
 *       Ϊ�˸��ܾ�ϸ����ʾ����λ�ã����˵��㷨�ü�������һ��һ����������Ļ���ƶ����Ա���õ���Ϸ
 */
void Egg_display(unsigned char x , unsigned char y)
{
    unsigned char i;
    unsigned char line_y = 0 , remain_y = 0;
    line_y = y / 8 - 1;
    remain_y = y % 8;
    if(remain_y != 0)
    {
        if(line_y > 0)
        {
            SSD1306_set_pos(x , line_y - 1);
            for(i = 0 ; i < 12 ; i++)
            {
                SSD1306_write_data(0x00);
            }
        }
        SSD1306_set_pos(x , line_y);
        for(i = 0 ; i < 12 ; i++)
        {
            SSD1306_write_data(egg_IMG[i] << remain_y);
        }
        SSD1306_set_pos(x , line_y + 1);
        for(i = 0 ; i < 12 ; i++)
        {
            SSD1306_write_data(egg_IMG[i] >> (8 - remain_y) );
        }
        if(line_y < 6 - 1)
        {
            SSD1306_set_pos(x , line_y + 2);
            for(i = 0 ; i < 12 ; i++)
            {
                SSD1306_write_data(0x00);
            }
        }
    }
    else
    {
        if(line_y > 0)
        {
            SSD1306_set_pos(x , line_y - 1);
            for(i = 0 ; i < 12 ; i++)
            {
                SSD1306_write_data(0x00);
            }
        }
        SSD1306_set_pos(x , line_y);
        for(i = 0 ; i < 12 ; i++)
        {
            SSD1306_write_data(egg_IMG[i]);
        }
        SSD1306_set_pos(x , line_y + 1);
        if(line_y < 7 - 1)
        {
            for(i = 0 ; i < 12 ; i++)
            {
                SSD1306_write_data(0x00);
            }
        }
    }
}

//void Basket_Egg_display(unsigned char x , unsigned char y)
//{
//    unsigned char i = 0;
//    SSD1306_set_pos(x - 1 , y);
//    SSD1306_write_data(0x00);
//    SSD1306_set_pos(x , y);
//    for(i = 0 ; i < 12 ; i++)
//    {
//        SSD1306_write_data(basket_IMG[i] | egg_IMG[i]);
//    }
//    SSD1306_write_data(0x00);
//}

/*
 * ������:Game_start
 * ��   ��:��
 * ��   ��:��
 * ˵   ��:��ʼ��Ϸ�����ö�ʱ������ʼ����Ϸ���������ݣ���������Ϸ��ѭ��
 */
void Game_start()
{
    int i , j;
    TA0CTL |= TAIE;
    game_start = 1;

    SSD1306_ShowString(0, 7, "Score:", 8);
    sprintf(Game_Score_str , "%d " , Game_Score);

    for(j = 0 ; j < player_life ; j++)
    {
        SSD1306_set_pos(70 + j * 12 , 7);
        for(i = 0 ; i < 12 ; i++)
        {
            SSD1306_write_data(egg_IMG[i]);
        }
    }

    while(game_start)
        Game_main();
}

/*
 * ������:Game_stop
 * ��   ��:��
 * ��   ��:��
 * ˵   ��:������Ϸ���رն�ʱ�������ض༶�˵��ϼ�
 */
void Game_stop()
{
    TA0CTL &=~TAIE;
    game_start = 0;
    M_gui_exit_event(&menu_main);
}

/*
 * ������:Game_main
 * ��   ��:��
 * ��   ��:��
 * ˵   ��:��Ϸ��ѭ������Ϸ��ѭ��������ʾ������Ϸ��Ϣ
 */
void Game_main()
{
    int i , j;

    if(game_over)           //����Ϸ����ʱ
    {
        game_over_count++;
        SSD1306_ShowString(33, 3, "Game Over!", 8);     //��ʾ��Ϸ����
        SSD1306_ShowString(30, 4, "Your Score:", 8);    //��ʾ��Ϸ����
        SSD1306_ShowString(48, 5, Game_Score_str, 8);   //��ʾ�÷�
        if(game_over_count > 200)                       //ѭ��������ʱ
        {
            Game_Score = 0;
            sprintf(Game_Score_str , "%d " , Game_Score);
            game_over_count = 0;
            game_over = 0;
            new_round = 1;                              //�����»غ�
            SSD1306_clear();
            SSD1306_ShowString(0, 7, "Score:", 8);
        }
    }
    else
    {
        if(new_round)   //�������»غ�ʱ
        {
            SSD1306_ShowString(0, 1, "                      ", 8);  //�����ʾ
            SSD1306_ShowString(egg_x, 2, "  ", 8);      //�����ʾ
            basket_speed = ADC10_in(INCH_6) % 10;       //���ӵ��ٶ���A6ͨ����ADC�������ݸ�λ�������
            my_basket_speed = ADC10_in(INCH_6) % 10;    //���ǵ����ӵ��ٶ���A6ͨ����ADC�������ݸ�λ�������
            player_life = 3;                            //��ʼ����������
            for(j = 0 ; j < player_life ; j++)          //���������������������ķ�ʽ��ʾ����
            {
                SSD1306_set_pos(70 + j * 12 , 7);
                for(i = 0 ; i < 12 ; i++)
                {
                    SSD1306_write_data(egg_IMG[i]);
                }
            }
            new_round = 0;                              //�»غϳ�ʼ�������������־
        }
        if(life_lost)  //��ʧȥ��һ������
        {
            SSD1306_ShowString(70, 7, "      ", 8);     //�����ʾ
            for(j = 0 ; j < player_life ; j++)          //��ʾʣ�������������
            {
                SSD1306_set_pos(70 + j * 12 , 7);
                for(i = 0 ; i < 12 ; i++)
                {
                    SSD1306_write_data(egg_IMG[i]);
                }
            }                                           //ʧȥ��һ�����������������־
            life_lost = 0;
        }
        Basket_display(basket_x , 2);                   //��ָ��λ����ʾ����
        Basket_display(egg_x , 6);                      //��ָ��λ����ʾ���ǵ�����
        Egg_display(egg_x , egg_y);                     //��ָ��λ����ʾ����
        SSD1306_ShowString(36, 7, Game_Score_str, 8);   //��ָ��λ����ʾ����
    }

}
