/*
 * Game.c
 *
 *  Created on: 2021年9月10日
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
 * 函数名:TA0_init
 * 输   入:无
 * 返   回:无
 * 说   明:定时器A0初始化
 */
void TA0_init()
{
    //TA0CCR0 = 32767;
    TA0CTL |= TASSEL_2 + ID_0 + MC_2 + TACLR;

}

/*
 * 函数名:TimerA0
 * 输   入:无
 * 返   回:无
 * 说   明:定时器A0中断，这个中断将进行接蛋游戏的各种判定
 */
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TimerA0(void)
{
    basket_move_count++;
    if(basket_move_count > basket_speed)    //改变篮子的坐标，让篮子可以左右移动
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
    if(my_basket_move_count > my_basket_speed) //改变我们的篮子的坐标，让我们的篮子可以左右移动
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

    if(egg_shoot)                           //当鸡蛋发射时
    {
        egg_move_count++;
        if(egg_move_count > 3)
        {
            egg_move_count = 0;
            if(egg_up_down == 0)        //鸡蛋先往上
            {
                egg_y = 8 + 0.5 * 0.06 * egg_t * egg_t;     //通过加速度公式计算鸡蛋的y轴坐标
                egg_t--;
                if(egg_t == -1)
                {
                    egg_up_down = 1;
                    egg_t = 0;           //用于计算鸡蛋抛物线的伪时间置0
                }
            }
            else if(egg_up_down)        //鸡蛋再往下
            {
                egg_y = 8 + 0.5 * 0.06 * egg_t * egg_t;     //通过加速度公式计算鸡蛋的y轴坐标
                egg_t++;
                if(egg_y == 23)         //当鸡蛋y坐标为23时进行判断
                {
                    egg_to_basket = egg_x - basket_x;
                    if(egg_to_basket > -10 && egg_to_basket < 10)       //判断鸡蛋与篮子之间的距离是否小于±10个像素，同时更改此项可以更改判定像素
                    {                                                   //若为是
                        Game_Score++;                                   //加分
                        new_round = 1;                                  //开启新回合
                        sprintf(Game_Score_str , "%d " , Game_Score);
                        egg_y = 56;                                     //重置鸡蛋y轴
                        egg_t = 41;                                     //直接设置鸡蛋抛物线的伪时间到di
                    }
                    else
                    {
                        player_life--;                                  //减少一条生命值
                        life_lost = 1;                                  //减少生命值的标志
                        if(player_life == 0)
                        {
                            //sprintf(Game_Score_str , "%d " , Game_Score);
                            game_over = 1;                              //若生命值为0时，游戏结束
                        }
                    }
                }
                if(egg_t == 41)
                {
                    egg_up_down = 0;        //重置鸡蛋上下状态
                    egg_t = 40;             //重置用于计算鸡蛋抛物线的伪时间
                    egg_shoot = 0;          //重置鸡蛋发射状态
                }
            }
        }
    }

    TA0IV = 0;
}

/*
 * 函数名:Basket_display
 * 输   入:unsigned char x    需要显示的篮子的x轴坐标
 *       unsigned char y    需要显示的篮子的y轴坐标
 * 返   回:无
 * 说   明:在指定的位置显示篮子，注意，这里的Y轴为0-7八个数值
 *       具体参考SSD1306的数据手册，SSD1306以8个像素为一列，我也很难受啊
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
 * 函数名:Basket_display
 * 输   入:unsigned char x    需要显示的篮子的x轴坐标
 *       unsigned char y    需要显示的篮子的y轴坐标
 * 返   回:无
 * 说   明:在指定的位置显示篮子，注意，这里的Y轴为0-63
 *       为了更能精细的显示鸡蛋位置，用了点算法让鸡蛋可以一个一个像素在屏幕上移动，以便更好的游戏
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
 * 函数名:Game_start
 * 输   入:无
 * 返   回:无
 * 说   明:开始游戏，启用定时器并初始化游戏分数等数据，并进入游戏主循环
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
 * 函数名:Game_stop
 * 输   入:无
 * 返   回:无
 * 说   明:结束游戏，关闭定时器，返回多级菜单上级
 */
void Game_stop()
{
    TA0CTL &=~TAIE;
    game_start = 0;
    M_gui_exit_event(&menu_main);
}

/*
 * 函数名:Game_main
 * 输   入:无
 * 返   回:无
 * 说   明:游戏主循环，游戏主循环用于显示各种游戏信息
 */
void Game_main()
{
    int i , j;

    if(game_over)           //当游戏结束时
    {
        game_over_count++;
        SSD1306_ShowString(33, 3, "Game Over!", 8);     //显示游戏结束
        SSD1306_ShowString(30, 4, "Your Score:", 8);    //显示游戏结束
        SSD1306_ShowString(48, 5, Game_Score_str, 8);   //显示得分
        if(game_over_count > 200)                       //循环计数延时
        {
            Game_Score = 0;
            sprintf(Game_Score_str , "%d " , Game_Score);
            game_over_count = 0;
            game_over = 0;
            new_round = 1;                              //开启新回合
            SSD1306_clear();
            SSD1306_ShowString(0, 7, "Score:", 8);
        }
    }
    else
    {
        if(new_round)   //当开启新回合时
        {
            SSD1306_ShowString(0, 1, "                      ", 8);  //清空显示
            SSD1306_ShowString(egg_x, 2, "  ", 8);      //清空显示
            basket_speed = ADC10_in(INCH_6) % 10;       //篮子的速度由A6通道的ADC量化数据个位随机决定
            my_basket_speed = ADC10_in(INCH_6) % 10;    //我们的篮子的速度由A6通道的ADC量化数据个位随机决定
            player_life = 3;                            //初始化三条生命
            for(j = 0 ; j < player_life ; j++)          //将三条生命以三个鸡蛋的方式显示出来
            {
                SSD1306_set_pos(70 + j * 12 , 7);
                for(i = 0 ; i < 12 ; i++)
                {
                    SSD1306_write_data(egg_IMG[i]);
                }
            }
            new_round = 0;                              //新回合初始化结束，清除标志
        }
        if(life_lost)  //当失去了一条生命
        {
            SSD1306_ShowString(70, 7, "      ", 8);     //清空显示
            for(j = 0 ; j < player_life ; j++)          //显示剩余的生命即鸡蛋
            {
                SSD1306_set_pos(70 + j * 12 , 7);
                for(i = 0 ; i < 12 ; i++)
                {
                    SSD1306_write_data(egg_IMG[i]);
                }
            }                                           //失去了一条生命结束，清除标志
            life_lost = 0;
        }
        Basket_display(basket_x , 2);                   //在指定位置显示篮子
        Basket_display(egg_x , 6);                      //在指定位置显示我们的篮子
        Egg_display(egg_x , egg_y);                     //在指定位置显示鸡蛋
        SSD1306_ShowString(36, 7, Game_Score_str, 8);   //在指定位置显示分数
    }

}
