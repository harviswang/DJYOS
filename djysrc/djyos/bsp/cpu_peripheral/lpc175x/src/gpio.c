//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//��Ȩ���� (C), 2004-2009,   lst.
//����ģ��:IO��ʼ��
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����:���ļ�ʵ����������:
//          1�����Ź���ѡ������MCU�����ţ����Ǹ��õģ�ʹ��ǰ��Ҫ��ȷ��ѡ������
//             �Ĺ��ܣ�������Щϵͳ�����й����л�Ҫ��̬�л����ܣ���ЩCPU�ж�����
//             ģ���������Ź���ѡ����Щ�����Щ���ܸ�����gpioģ���ϣ�djyosͳһ
//             ��gpioģ����ʵ�֡�
//          2��gpio����ģʽ���ã����Ǻ;���Ӳ�������ȷǳ��ߵĹ���.���CPU������
//             ��ѡ��pin���������������������ٶȡ����������ѡ�
//          3����gpio���ŵ�ƽ���Լ�����gpio�������
//          5������δ��֮���ܣ��û�������ӡ�
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "os_inc.h"
#include "cpu_peri.h"
#include "gpio_api.h"

//----ѡ�����Ź���-------------------------------------------------------------
//����: ѡ��һ�����ŵĹ��ܣ�MCU������ⲿ���Ż���Ϊ����port����P0~Px���б�ţ�ÿ
//      ��port���������š�
//����: port����������port��ţ�����Ҫ����P1.5,��port=1.
//          ���port=cn_invalid_port����:
//          1�������cpuû��δ��gpio��ŵĿ����ù������ţ���ִ���κβ�����
//          2�������cpu��δ��gpio��ŵĿ����ù������ţ���pinnum�Ա���ţ�
//             func_no���岻�䡣������pinnumʹ��cpu���źţ���Ϊͬһ��оƬ��ͬ��
//             װ���ź�δ����ͬ���ر���BGA��װ���㻹�Ķ�������ת������
//      pinnum��port�ڱ�ţ�����Ҫ����P1.5,��pinnum=5.
//      func_no�����ܱ�ţ��ɾ���ϵͳ����
//����: ��
//example:
//      gpio_cfg_pin_func(0,1,cn_p0_0_txd3)     //��p0.1����Ϊuart3��txd
//-----------------------------------------------------------------------------
void gpio_cfg_pin_func(u32 port,u32 pinnum,u32 func_no)
{
    vu32  *pinsel;
    u32 bitoffset,portoffset;
    pinsel = &pg_pin_config_reg->pinsel0;
    portoffset = (port<<1) + (pinnum>>4);
    bitoffset = pinnum % 16 <<1;
    pinsel[portoffset] = pinsel[portoffset] & (~((u32)0x3<<bitoffset))
                        | (((u32)0x3 & func_no)<<bitoffset);
}

//----ѡ������ģʽ-------------------------------------------------------------
//����: ѡ��һ�����ŵ�ģʽ��MCU������ⲿ���Ż���Ϊ����port����P0~Px���б�ţ�ÿ
//      ��port���������š�
//����: port����������port��ţ�����Ҫ����P1.5,��port=1.
//          ���port=cn_invalid_port����:
//          1�������cpuû��δ��gpio��ŵĿ����ù������ţ���ִ���κβ�����
//          2�������cpu��δ��gpio��ŵĿ����ù������ţ���pinnum�Ա���ţ�
//             func_no���岻�䡣������pinnumʹ��cpu���źţ���Ϊͬһ��оƬ��ͬ��
//             װ���ź�δ����ͬ���ر���BGA��װ���㻹�Ķ�������ת������
//      pinnum��port�ڱ�ţ�����Ҫ����P1.5,��pinnum=5.
//      mode��ģʽ��ţ��ɾ���ϵͳ���壬��175x�У�bit0~1��������pinmode��bit2��
//            ������pinmode_od�����Ƿ�©��
//����: ��
//example:
//      ��p0.1����Ϊ����+��©���
//      gpio_cfg_pin_mode(0,1,cn_gpio_mode_od_pullup)
//-----------------------------------------------------------------------------
void gpio_cfg_pin_mode(u32 port,u32 pinnum,u32 mode)
{
    vu32  *pinmode;
    u32 bitoffset,portoffset;
    //������pinmode�Ĵ���
    pinmode = &pg_pin_config_reg->pinmode0;
    portoffset = (port<<1) + (pinnum>>4);
    bitoffset = pinnum % 16 <<1;
    pinmode[portoffset] = pinmode[portoffset] & (~((u32)0x3<<bitoffset))
                        | (((u32)0x3 & mode)<<bitoffset);
    //������pinmode_od�Ĵ���
    pinmode = &pg_pin_config_reg->pinmode_od0;
    portoffset = port;
    bitoffset = pinnum;
    pinmode[port] = pinmode[port] & (~((u32)1<<pinnum))
                        | (((u32)1 & (mode>>2))<<pinnum);
}

//----����gpioΪ����-----------------------------------------------------------
//����: ��ĳ��gpio��msk=1��Ӧ��gpio����Ϊ����ڣ�����ѡΪgpioʱ����Ч��
//����: port����������port��ţ�����Ҫ����P1.5,��port=1.
//      mask��port����Ҫ��Ϊ�����δ����
//����: ��
//example:
//      gpio_setto_in(1,0x10000010);        //��p1.4,p1.28��Ϊ����
//-----------------------------------------------------------------------------
void gpio_setto_in(u32 port,u32 msk)
{
    switch(port)
    {
        case 0:
            pg_gpio0_reg->dir &= ~msk;
            break;
        case 1:
            pg_gpio1_reg->dir &= ~msk;
            break;
        case 2:
            pg_gpio2_reg->dir &= ~msk;
            break;
        case 4:
            pg_gpio4_reg->dir &= ~msk;
            break;

        default:break;
    }
}

//----����gpioΪ���-----------------------------------------------------------
//����: ��ĳ��gpio��msk����1��bit��Ӧ��gpio����Ϊ����ڣ�����ѡΪgpioʱ����Ч��
//����: port����������port��ţ�����Ҫ����P1.5,��port=1.
//      mask��port����Ҫ��Ϊ�����λ����
//����: ��
//example:
//      gpio_setto_out(1,0x10000010);       //��p1.4,p1.28��Ϊ���
//-----------------------------------------------------------------------------
void gpio_setto_out(u32 port,u32 msk)
{
    switch(port)
    {
        case 0:
            pg_gpio0_reg->dir |= msk;
            break;
        case 1:
            pg_gpio1_reg->dir |= msk;
            break;
        case 2:
            pg_gpio2_reg->dir |= msk;
            break;
        case 4:
            pg_gpio4_reg->dir |= msk;
            break;

        default:break;
    }
}

//----��gpio��ȡ����-----------------------------------------------------------
//����: ��ĳgpio port�������
//����: port����������port��ţ�����Ҫ����P1,��port=1.
//����: ���õ�����
//example:
//      data = gpio_get_data(1);        //��p1�ڵ�����
//-----------------------------------------------------------------------------
u32 gpio_get_data(u32 port)
{
    u32 temp;
    switch(port)
    {
        case 0:
            temp = pg_gpio0_reg->pin;
            break;
        case 1:
            temp = pg_gpio1_reg->pin;
            break;
        case 2:
            temp = pg_gpio2_reg->pin;
            break;
        case 4:
            temp = pg_gpio4_reg->pin;
            break;

        default:break;
    }
    return temp;
}

//----������ݵ�ĳport---------------------------------------------------------
//����: ���������������ĳgpio port
//����: port����������port��ţ�����Ҫ����P1,��port=1.
//      data�������������
//����: ��
//example:
//      gpio_out_data(1,data);      //���������p1��
//-----------------------------------------------------------------------------
void gpio_out_data(u32 port,u32 data)
{
    switch(port)
    {
        case 0:
            pg_gpio0_reg->pin = data;
            break;
        case 1:
            pg_gpio1_reg->pin = data;
            break;
        case 2:
            pg_gpio2_reg->pin = data;
            break;
        case 4:
            pg_gpio4_reg->pin = data;
            break;

        default:break;
    }
    return ;
}

//----IO������ߵ�ƽ-----------------------------------------------------------
//����: ��port��msk����1��bit��Ӧ��λ����ߵ�ƽ
//����: port����������port��ţ�����Ҫ����P1.5,��port=1.
//      mask��port����Ҫ����ߵ�ƽ��λ����
//����: ��
//example:
//      gpio_setto_high(1,0x10000010);      //��p1.4,p1.28����ߵ�ƽ
//-----------------------------------------------------------------------------
void gpio_setto_high(u32 port,u32 msk)
{
    switch(port)
    {
        case 0:
            pg_gpio0_reg->setpin = msk;
            break;
        case 1:
            pg_gpio1_reg->setpin = msk;
            break;
        case 2:
            pg_gpio2_reg->setpin = msk;
            break;
        case 4:
            pg_gpio4_reg->setpin = msk;
            break;

        default:break;
    }
}

//----IO������͵�ƽ-----------------------------------------------------------
//����: ��port��msk����1��bit��Ӧ��λ����͵�ƽ
//����: port����������port��ţ�����Ҫ����P1.5,��port=1.
//      mask��port����Ҫ����ߵ�ƽ��λ����
//����: ��
//example:
//      gpio_setto_high(1,0x10000010);      //��p1.4,p1.28����͵�ƽ
//-----------------------------------------------------------------------------
void gpio_setto_low(u32 port,u32 msk)
{
    switch(port)
    {
        case 0:
            pg_gpio0_reg->clrpin = msk;
            break;
        case 1:
            pg_gpio1_reg->clrpin = msk;
            break;
        case 2:
            pg_gpio2_reg->clrpin = msk;
            break;
        case 4:
            pg_gpio4_reg->clrpin = msk;
            break;

        default:break;
    }
}

//----ʹ��gpioģ��-------------------------------------------------------------
//����: ʹ��gpioģ�飬���cpuΪ�˿��ƹ��ģ�������ģ�鶼�ǿ��Ա��رյģ�gpioģ��
//      Ҳ����ˡ���Щcpu������gpioģ��һ�����ã���Щ���ǲ�ͬport�������á�����
//      û�н�����Ƶ�cpu������������Ϊ�ռ��ɡ�
//      ���ƹ��ĵĹ���ʵ�ַ����������������:
//      1����һ��ͳһ�Ĺ��Ĺ���ģ�顣
//      2�����Ĺ����ܷ�ɢ��������ģ����ȥ��
//      ��������ͳһ�Ĺ��Ĺ���ģ�飬djyos����������������������ʵ��һ�׿�����
//      ���ĵĺ�������ͳһ�Ĺ��Ŀ���ģ�����ṩ���û������������Ƿ���ͳһ��
//      ���Ŀ���ģ�飬Ӧ�ó�����һ�µġ�
//����: port����������port��ţ�����Ҫ����P1,��port=1.
//����: ��
//example:
//      gpio_power_on(1);      //ʹ��p1����lpc���ܰ�portʹ�ܣ�ʵ����ʹ��ȫ��gpio
//-----------------------------------------------------------------------------
void gpio_power_on(u32 port)
{
    //ʹ������gpioģ�飬gpioģ��ֻ��һ������λ�����ܰ�portʹ�ܡ�
    pg_sysctrl_reg->pconp |= (u32)1<<15;
}

//----��ֹgpioģ��-------------------------------------------------------------
//����: ʹ��gpioģ�飬���cpuΪ�˿��ƹ��ģ�������ģ�鶼�ǿ��Ա��رյģ�gpioģ��
//      Ҳ����ˡ���Щcpu������gpioģ��һ�����ã���Щ���ǲ�ͬport�������á�����
//      û�н�����Ƶ�cpu������������Ϊ�ռ��ɡ�
//      ���ƹ��ĵĹ���ʵ�ַ����������������:
//      1����һ��ͳһ�Ĺ��Ĺ���ģ�顣
//      2�����Ĺ����ܷ�ɢ��������ģ����ȥ��
//      ��������ͳһ�Ĺ��Ĺ���ģ�飬djyos����������������������ʵ��һ�׿�����
//      ���ĵĺ�������ͳһ�Ĺ��Ŀ���ģ�����ṩ���û������������Ƿ���ͳһ��
//      ���Ŀ���ģ�飬Ӧ�ó�����һ�µġ�
//����: port����������port��ţ�����Ҫ����P1,��port=1.
//����: ��
//example:
//      gpio_power_off(1);    //��ֹp1����lpc���ܰ�portʹ�ܣ�ʵ���Ͻ�ֹȫ��gpio
//-----------------------------------------------------------------------------
void gpio_power_off(u32 port)
{
    //lpc_17xxû�е�lpc���ܰ�portʹ�ܣ�ʵ���Ͻ�ֹȫ��gpio,Ϊ�˲�Ӱ������ģ�飬
    //��������ִ���κβ���
    return;
}

