//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
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
#include "stdint.h"
#include "stddef.h"
#include "cpu_peri.h"
#include "keyboard.h"

bool_t key_hard_init(void);
u32 keyboard_scan(void);
//----��ʼ������ģ��-----------------------------------------------------------
//����: ��ʼ��һ����windows�ļ��̺Ͱ�ťģ��ļ��̣��ü��̹�8������
//����: ��
//����: ��
//-----------------------------------------------------------------------------
bool_t ModuleInstall_Keyboard_TQ2440(const char *dev_name)
{
    static struct tagKeyBoardPrivate key_brd;

    key_hard_init();

    key_brd.read_keyboard = keyboard_scan;
    Keyboard_InstallDevice(dev_name,&key_brd);
    key_brd.vtime_limit = 0;
    key_brd.vtime_count = 100;
    key_brd.key_bak = 0;
    key_brd.key_now = 0;
    return true;
}


//----��ʼ������Ӳ��-----------------------------------------------------------
//����: ���⣬��ʼ�������ɨ�����
//����: ��
//����: ��
//----------------------------------------------------------------------------
bool_t key_hard_init(void)
{
    pg_gpio_reg->GPFCON &= 0xfffffcc0;
    return true;
}


//----����Ӳ��ɨ��------------------------------------------------------------
//����: ��ȡ���̰���״̬��ÿ��������8bit��ʾ��32λ����ֵ���Դ��4��������ɨ�赽
//      �ĵ�һ�������������8λ�����ĸ������������8λ��ɨ�赽4�������󣬲��ټ�
//      ������ͬʱ���³���4��������ֻ��¼4����
//����: key,��ֵ����ָ��,�ɴ��2������ֵ
//����: �����ļ��ĸ���0,1,2
//----------------------------------------------------------------------------
u32 keyboard_scan(void)
{
    u32 readed;
    uint8_t i = 0;

    readed = 0;
    if( ! (pg_gpio_reg->GPFDAT & 1))
    {
        readed |= (u32)(1<<(i<<3));
        i++;
    }
    if( ! (pg_gpio_reg->GPFDAT & 0x2))
    {
        readed |= (u32)(2<<(i<<3));
        i++;
    }
    if( ! (pg_gpio_reg->GPFDAT & 0x4))
    {
        readed |= (u32)(3<<(i<<3));
        i++;
    }
    if( ! (pg_gpio_reg->GPFDAT & 0x10))
    {
        readed |= (u32)(4<<(i<<3));
        i++;
    }
    return(readed);
}

