//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//��Ȩ���� (C), 2004-2009,   lst.
//����ģ��:����
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: ɨ��Ӳ��������������ֻ��ֱ�Ӹ���������ǰֵ�����������
//����˵��:
//�޶���ʷ:
//2. ����: 2009-05-29
//   ����: lst
//   �°汾��: V1.1.0
//   �޸�˵��: ����key_hard_init����
//1. ����: 2009-03-10
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "os_inc.h"
#include "cpu_peri.h"
#include "key.h"
#include "gpio_api.h"

//----��ʼ������Ӳ��-----------------------------------------------------------
//����: ���⣬��ʼ�������ɨ�����
//����: ��
//����: ��
//----------------------------------------------------------------------------
bool_t key_hard_init(void)
{
    gpio_power_on(cn_gpio_c);
    gpio_cfg_pin_func(cn_gpio_c,10,cn_gpio_mode_in_pullup);
    gpio_cfg_pin_func(cn_gpio_c,11,cn_gpio_mode_in_pullup);
    gpio_cfg_pin_func(cn_gpio_c,12,cn_gpio_mode_in_pullup);
    gpio_cfg_pin_func(cn_gpio_c,13,cn_gpio_mode_in_pullup);
    return true;
}

//----����Ӳ��ɨ��------------------------------------------------------------
//����: ��4����,�ɶ��븴�ϼ������2��������,key0=gpf0,
//      key1=gpf2,key2=gpg3,key3=gpg11
//����: key,��ֵ����ָ��,�ɴ��2������ֵ
//����: �����ļ��ĸ���0,1,2
//----------------------------------------------------------------------------
ufast_t key_scan_hard(uint16_t *key)
{
    uint8_t i = 0;
    u32 reg;
    reg = gpio_get_data(cn_gpio_c);
    if( ! (reg & 0x0400))
    {
        key[i] = 4;
        i++;
    }
    if( ! (reg & 0x0800))
    {
        key[i] = 3;
        i++;
        if(i == 2)
            return i;
    }
    if( ! (reg & 0x1000))
    {
        key[i] = 2;
        i++;
        if(i == 2)
            return i;
    }
    if( ! (reg & 0x2000))
    {
        key[i] = 1;
        i++;
        if(i == 2)
            return i;
    }
    return(i);
}

