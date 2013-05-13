//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//��Ȩ���� (C), 2004-2009,   lst.
//����ģ��: Ӳ����ʱ��
//���ߣ�lst
//�汾��V1.1.0
//�ļ�����: ����44b0Ӳ����ʱ������
//����˵��:
//�޶���ʷ:
//2. ����: 2009-04-24
//   ����: lst
//   �°汾��: V1.1.0
//   �޸�˵��: ԭdjyos.c�ж�ʱ������Ӳ����صĲ���ת�Ƶ����
//1. ����: 2009-03-10
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ��ֲ��44b0�汾
//------------------------------------------------------
#include "os_inc.h"
#include "cpu_peri.h"

//2440�Ķ�ʱ�����ܼ���:
//1����������ʽ����
//2��ÿ����ʱ����ѡ5��ʱ��Դ���ֱ�Ϊ��Ƶ��1/2��Ƶ~1/16��Ƶ����EXTCLK��
//3��������8λԤ��Ƶ����01����һ����234����һ����
//4��ÿ����ʱ�����бȽϼĴ��������ڲ���PWM���Ρ�
//5��ÿ����ʱ�������趨Ϊ�������к��ظ����У������ֶ�������ֹͣ��

void timer_init(void)
{
    //DMA:No select(All Interrupt),
    pg_timer_reg->TCFG1 = 0;
}

//----���ö�ʱ��ʱ��Դ----------------------------------------------------------
//����: ���ø���ʱ��ʱ��Դ.
//����: timer,��ʱ����
//      value,��ʱ������ʱ��Դ,timer��value�Ķ�Ӧ������:
//      value:  0           1           2            3          4
//      timer0: 1/2��Ƶ     1/4��Ƶ     1/8��Ƶ      1/16��Ƶ   EXTCLK0
//      timer1: 1/2��Ƶ     1/4��Ƶ     1/8��Ƶ      1/16��Ƶ   EXTCLK0
//      timer2: 1/2��Ƶ     1/4��Ƶ     1/8��Ƶ      1/16��Ƶ   EXTCLK1
//      timer3: 1/2��Ƶ     1/4��Ƶ     1/8��Ƶ      1/16��Ƶ   EXTCLK1
//      timer4: 1/2��Ƶ     1/4��Ƶ     1/8��Ƶ      1/16��Ƶ   EXTCLK1
//����: ��
//-----------------------------------------------------------------------------
void timer_set_clk_source(ufast_t timer,ufast_t value)
{
    pg_timer_reg->TCFG1 &= ~(0xf << (timer<<2));
    pg_timer_reg->TCFG1 |= value << (timer<<2);
}

//----���ö�ʱ��Ԥ��Ƶ��-------------------------------------------------------
//����: ���ø���ʱ��Ԥ��Ƶ��.
//����: group,��ʱ�����,01Ϊ1��,234Ϊ1��
//      value,��ʱ��Ԥ��Ƶ��,0~255��Ӧ1~256��Ƶ
//����: ��
//-----------------------------------------------------------------------------
void timer_set_precale(ufast_t group,uint16_t value)
{
    pg_timer_reg->TCFG0 &= ~(0xff << (group<<3));
    pg_timer_reg->TCFG0 |= value << (group<<3);
}

//----���ö�ʱ������ֵ----------------------------------------------------------
//����: ���ø���ʱ������ֵ.��ʱ��������ж�ʱ��Ϊ:value*(Ԥ��Ƶ+1)/ʱ��Դ��Ƶ��
//����: timer,��ʱ����
//      value,����ֵ
//����: ��
//-----------------------------------------------------------------------------
void timer_set_counter(ufast_t timer,uint16_t value)
{
    switch (timer)
    {
        case 0:
            pg_timer_reg->TCNTB0 = value;
            break;
        case 1:
            pg_timer_reg->TCNTB1 = value;
            break;
        case 2:
            pg_timer_reg->TCNTB2 = value;
            break;
        case 3:
            pg_timer_reg->TCNTB3 = value;
            break;
        case 4:
            pg_timer_reg->TCNTB4 = value;
            break;
        default:break;
    }
}

//----���ö�ʱ���Ƚ�ֵ----------------------------------------------------------
//����: ����pwmռ�ձ����
//����: timer,��ʱ����
//      value,�Ƚ�ֵ
//����: ��
//-----------------------------------------------------------------------------
void timer_set_compare(ufast_t timer,uint16_t value)
{
    switch (timer)
    {
        case 0:
            pg_timer_reg->TCMPB0 = value;
            break;
        case 1:
            pg_timer_reg->TCMPB1 = value;
            break;
        case 2:
            pg_timer_reg->TCMPB2 = value;
            break;
        case 3:
            pg_timer_reg->TCMPB3 = value;
            break;
        default:break;
    }
}

//----���ö�ʱ��������ʽ-------------------------------------------------------
//����: �趨��ʱ���������������ǵ��ι���
//����: timer,��ʱ����
//      type,0=���ι���,1=�Զ�������������
//����: ��
//-----------------------------------------------------------------------------
void timer_set_type(ufast_t timer,ufast_t type)
{
    type &= 0x1;
    switch (timer)
    {
        case 0:
            pg_timer_reg->TCON &= ~(1<<3);
            pg_timer_reg->TCON |= type<<3;
            break;
        case 1:
            pg_timer_reg->TCON &= ~(1<<11);
            pg_timer_reg->TCON |= type<<11;
            break;
        case 2:
            pg_timer_reg->TCON &= ~(1<<15);
            pg_timer_reg->TCON |= type<<15;
            break;
        case 3:
            pg_timer_reg->TCON &= ~(1<<19);
            pg_timer_reg->TCON |= type<<19;
            break;
        case 4:
            pg_timer_reg->TCON &= ~(1<<22);
            pg_timer_reg->TCON |= type<<22;
            break;
        default:break;
    }
}

//----���ض�ʱ�������ͱȽ�ֵ---------------------------------------------------
//����: �ֶ����¼��ض�ʱ���ļ����ͱȽϼĴ���ֵ
//����: timer,��ʱ����
//����: ��
//-----------------------------------------------------------------------------
void timer_reload(ufast_t timer)
{
    switch (timer)
    {
        case 0:
            pg_timer_reg->TCON |= 1<<1;
            pg_timer_reg->TCON &= ~(1<<1);
            break;
        case 1:
            pg_timer_reg->TCON |= 1<<9;
            pg_timer_reg->TCON &= ~(1<<9);
            break;
        case 2:
            pg_timer_reg->TCON |= 1<<13;
            pg_timer_reg->TCON &= ~(1<<13);
            break;
        case 3:
            pg_timer_reg->TCON |= 1<<17;
            pg_timer_reg->TCON &= ~(1<<17);
            break;
        case 4:
            pg_timer_reg->TCON |= 1<<21;
            pg_timer_reg->TCON &= ~(1<<21);
            break;
        default:break;
    }
}

//----������ʱ��--------------------------------------------------------
//����: ������ʱ��
//����: timer,��ʱ����
//����: ��
//-----------------------------------------------------------------------------
void timer_start(ufast_t timer)
{
    switch (timer)
    {
        case 0:
            pg_timer_reg->TCON |=1;
            break;
        case 1:
            pg_timer_reg->TCON |=1<<8;
            break;
        case 2:
            pg_timer_reg->TCON |=1<<12;
            break;
        case 3:
            pg_timer_reg->TCON |=1<<16;
            break;
        case 4:
            pg_timer_reg->TCON |=1<<20;
            break;
        default:break;
    }
}

//----ֹͣ��ʱ��--------------------------------------------------------
//����: ֹͣ��ʱ��
//����: timer,��ʱ����
//����: ��
//-----------------------------------------------------------------------------
void timer_stop(ufast_t timer)
{
    switch (timer)
    {
        case 0:
            pg_timer_reg->TCON &=~1;
            break;
        case 1:
            pg_timer_reg->TCON &=~(1<<8);
            break;
        case 2:
            pg_timer_reg->TCON &=~(1<<12);
            break;
        case 3:
            pg_timer_reg->TCON &=~(1<<16);
            break;
        case 4:
            pg_timer_reg->TCON &=~(1<<20);
            break;
        default:break;
    }
}

//----����ʱ����ǰֵ--------------------------------------------------------
//����: ������ʱ���ĵ�ǰ����ֵ
//����: timer,��ʱ����
//����: ��ǰ����ֵ
//-----------------------------------------------------------------------------
uint16_t timer_read(ufast_t timer)
{
    switch (timer)
    {
        case 0:
            return pg_timer_reg->TCNTO0;
            break;
        case 1:
            return pg_timer_reg->TCNTO1;
            break;
        case 2:
            return pg_timer_reg->TCNTO2;
            break;
        case 3:
            return pg_timer_reg->TCNTO3;
            break;
        case 4:
            return pg_timer_reg->TCNTO4;
            break;
        default:break;
    }
    return 0;
}

