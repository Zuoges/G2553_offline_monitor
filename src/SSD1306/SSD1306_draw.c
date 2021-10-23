
#include <msp430.h>
#include "SSD1306/SSD1306_driver.h"
#include "SSD1306/SSD1306_api.h"
#include "SSD1306/Font_code.h"
#include "SSD1306/SSD1306_draw.h"

//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//sizey:ѡ������ 6x8  8x16
void SSD1306_ShowChar(unsigned char x,unsigned char y, char chr,unsigned char sizey)
{
    unsigned char c=0,sizex=sizey/2;
    unsigned int i=0,size1;
    if(sizey==8)size1=6;
    else size1=(sizey/8+((sizey%8)?1:0))*(sizey/2);
    c=chr-' ';//�õ�ƫ�ƺ��ֵ
    SSD1306_set_pos(x,y);
    for(i=0;i<size1;i++)
    {
        if(i%sizex==0&&sizey!=8) SSD1306_set_pos(x,y++);
        if(sizey==8) SSD1306_write_data(asc2_0806[c][i]);//6X8�ֺ�
        //else if(sizey==16) SSD1306_write_data(asc2_1608[c][i]);//8x16�ֺ�
//      else if(sizey==xx) SSD1306_WR_Byte(asc2_xxxx[c][i],SSD1306_DATA);//�û�����ֺ�
        else return;
    }
}
//m^n����
unsigned long SSD1306_pow(unsigned char m,unsigned char n)
{
    unsigned long result=1;
    while(n--)result*=m;
    return result;
}
//��ʾ����
//x,y :�������
//num:Ҫ��ʾ������
//len :���ֵ�λ��
//sizey:�����С
void SSD1306_ShowNum(unsigned char x,unsigned char y,unsigned long num,unsigned char len,unsigned char sizey)
{
    unsigned char t,temp,m=0;
    unsigned char enshow=0;
    if(sizey==8)m=2;
    for(t=0;t<len;t++)
    {
        temp=(num/SSD1306_pow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                SSD1306_ShowChar(x+(sizey/2+m)*t,y,' ',sizey);
                continue;
            }else enshow=1;
        }
        SSD1306_ShowChar(x+(sizey/2+m)*t,y,temp+'0',sizey);
    }
}
//��ʾһ���ַ��Ŵ�
void SSD1306_ShowString(unsigned char x,unsigned char y, char *chr,unsigned char sizey)
{
    unsigned char j=0;
    while (chr[j]!='\0')
    {
        SSD1306_ShowChar(x,y,chr[j++],sizey);
        if(sizey==8)x+=6;
        else x+=sizey/2;
    }
}
