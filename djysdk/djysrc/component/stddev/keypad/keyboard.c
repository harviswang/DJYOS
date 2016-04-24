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
//����:  ������.
//�汾��V1.1.0
//�ļ�����: �ṩ�����豸�����ܵĹ������֣���ֲ��Ҫ�޸ĵļ�key_xxx.c�ļ�
//����˵��:
//�޶���ʷ:
//3. ����:20120324
//   ����:  ������.
//   �°汾�ţ�v1.1.0
//   �޸�˵��:
//      1.�����µ�stdinģ��Ҫ����д
//      2.����key_read������key_read_direct����
//2. ����:20090131
//   ����:  ������.
//   �°汾�ţ�v1.1.0
//   �޸�˵��:
//      1.��Ӳ����صĺ���key_scan_hard�Ƶ�key_xxx.c��.
//      2.����key_read������key_read_direct����
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "keyboard.h"
#include "djyos.h"
#include "rsc.h"
#include "stddev.h"
#include "systime.h"

ptu32_t KeyBoard_Scan(void);
//----��ʼ������ģ��----------------------------------------------------------
//����: ��ʼ������ģ�飬����:��װ���̱�׼�����豸���ǼǼ���ɨ���¼�
//      ���ͣ�pop����ɨ���¼�������ɨ��(ʵ��ɨ�蹤��Ҫ�ȶ��߳�������ſ�ʼ)
//����: ��
//����: 0=ʧ�ܣ�1=�ɹ�
//----------------------------------------------------------------------------
ptu32_t ModuleInstall_KeyBoard(ptu32_t para)
{
    s16 evtt_key;
    if(!Rsc_SearchTree("stdin input device"))      //��׼�����豸δ��ʼ��
        return 0;
    evtt_key = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,
                                    KeyBoard_Scan,NULL,512,"keyboard");
    if(evtt_key == CN_EVTT_ID_INVALID)
    {
        return 0;
    }
    if(Djy_EventPop(evtt_key,NULL,0,0,0,0)
                        == (uint16_t)CN_EVENT_ID_INVALID)
    {
        Djy_EvttUnregist(evtt_key);
        return 0;
    }
    return 1;
}
//----��װ����-----------------------------------------------------------------
//����: ��Ӳ������������ã�������ʼ�����������豸���ɰ�װ��������豸
//����:
//����: �����豸ID,-1��ʾʧ��
//-----------------------------------------------------------------------------
s32 Keyboard_InstallDevice(char *keyboard_name,struct tagKeyBoardPrivate *keyboard_pr)
{
    if(-1 ==Stddev_InstallDevice(keyboard_name,EN_STDIN_KEYBOARD,keyboard_pr))
        return -1;
    else
        return 0;
}

//----����ɨ������-------------------------------------------------------------
//����: �����Եص���Ӳ��ɨ����򣬻�ȡ�û��û���Ϣ���ϳɱ�׼�����룬�͵���׼
//      �����豸�С�ɨ���������ֲ�ͬ�����ͣ�ͨ��(make code)�Ͷ���(break code)��
//      ��һ���������»������סʱ,�Ͳ���һ��ͨ��;����һ�������ͷ�ʱ,�Ͳ���һ��
//      ���롣ÿ�������������ֽ���ɣ�ͨ��Ϊ0+���룬����Ϊ0xF0+���롣����ֵ����
//      windows��������룬��A��,ͨ��Ϊ0x00,0x41,����Ϊ0xF0��0x41��
//����: ��
//����: ��
//----------------------------------------------------------------------------
ptu32_t KeyBoard_Scan(void)
{
    struct tagStdinDeviceRsc *keyboard_rsc,*stdin_rsc;
    struct tagKeyBoardPrivate *keyboard_pr;
    struct tagKeyBoardMsg key_msg;
    u32 keyvalue;

    stdin_rsc = (struct tagStdinDeviceRsc *)Rsc_SearchTree("stdin input device");
    while(1)
    {
        keyboard_rsc = stdin_rsc;
        while(1)
        {
            keyboard_rsc = (struct tagStdinDeviceRsc*)
                            Rsc_TraveScion(&stdin_rsc->stdin_device_node,
                                            &keyboard_rsc->stdin_device_node);
            if(keyboard_rsc == NULL)
                break;
            if(keyboard_rsc->input_type != EN_STDIN_KEYBOARD)
                continue;
            keyboard_pr = (struct tagKeyBoardPrivate*)keyboard_rsc->stdin_private;
            keyvalue = keyboard_pr->read_keyboard();
            if(keyvalue != keyboard_pr->key_now)
            {
                keyboard_pr->key_now = keyvalue;
                keyboard_pr->vtime_count = 0;
            }
            else
            {
                if(keyboard_pr->vtime_count < keyboard_pr->vtime_limit)
                    keyboard_pr->vtime_count += 10;
            }

            if((keyboard_pr->key_bak != keyboard_pr->key_now)
                && (keyboard_pr->vtime_count >= keyboard_pr->vtime_limit))
            {
                u32 loop;
                u8 key,key_byte[4];
                //��ע: �˴����ʹ��union������������ֲ�Ե����⡣
                key_byte[0] = keyboard_pr->key_bak & 0xff;
                key_byte[1] = (keyboard_pr->key_bak >> 8) & 0xff;
                key_byte[2] = (keyboard_pr->key_bak >> 16) & 0xff;
                key_byte[3] = (keyboard_pr->key_bak >> 24) & 0xff;
                for(loop = 0; loop < 4; loop++)
                {
                    key = (keyboard_pr->key_now >> (loop<<3)) & 0xff;
                    if(key == 0)
                        continue;
                    //�鿴�ǲ����°��µİ���
                    if((key_byte[0] != key)
                        && (key_byte[1] != key)
                        && (key_byte[2] != key)
                        && (key_byte[3] != key) )
                    {

                        key_msg.time = DjyGetTimeTick();
                        key_msg.key_value[1] = 0;
                        key_msg.key_value[0] = key;
                        Stddev_InputMsg(keyboard_rsc->device_id,
                                            (u8*)&key_msg,sizeof(key_msg));
                    }
                }

                //��ע: �˴����ʹ��union������������ֲ�Ե����⡣
                key_byte[0] = keyboard_pr->key_now & 0xff;
                key_byte[1] = (keyboard_pr->key_now >> 8) & 0xff;
                key_byte[2] = (keyboard_pr->key_now >> 16) & 0xff;
                key_byte[3] = (keyboard_pr->key_now >> 24) & 0xff;
                for(loop = 0; loop < 4; loop++)
                {
                    key = (u8)(keyboard_pr->key_bak >> (loop<<3)) & 0xff;
                    if(key == 0)
                        continue;
                    //�鿴��û�а����ͷ�
                    if((key_byte[0] != key)
                        && (key_byte[1] != key)
                        && (key_byte[2] != key)
                        && (key_byte[3] != key) )
                    {

                        key_msg.time = DjyGetTimeTick();
                        key_msg.key_value[1] = CN_BREAK_CODE;
                        key_msg.key_value[0] = key;
                        Stddev_InputMsg(keyboard_rsc->device_id,
                                            (u8*)&key_msg,sizeof(key_msg));
                    }
                }
                keyboard_pr->key_bak = keyboard_pr->key_now;
            }
        }
        Djy_EventDelay(10*mS);
    }
}

