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
//����ģ��:��׼����--������ģ��
//����:  ������.
//�汾��V1.1.0
//�ļ�����: �������豸��������
//����˵��:
//�޶���ʷ:
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "stddev.h"
#include "rsc.h"
#include "djyos.h"
#include "Touch.h"
#include "string.h"
#include "systime.h"
//----������ɨ������-----------------------------------------------------------
//����: �����Եص���Ӳ��ɨ����򣬻�ȡ�û��û�������Ϣ�����ѻ�õ���Ϣ���뵽��׼
//      ���뻺����(���ú���stdin_input_msg)��ע�⣬̧�����ʺ󣬻�Ҫ����һ����
//      �����뿪����Ϣ
//����: my_event�����̷����¼����͡�
//����: ��
//----------------------------------------------------------------------------
ptu32_t Touch_Scan(void)
{
    struct tagStdinDeviceRsc *touch_rsc,*stdin_rsc;
    struct tagSingleTouchPrivate *touch_pr;
    struct tagSingleTouchMsg touch_temp = {0,0,0,0};

    stdin_rsc = (struct tagStdinDeviceRsc *)Rsc_SearchTree("stdin input device");
    while(1)
    {
        touch_rsc = stdin_rsc;
        while(1)
        {
            touch_rsc = (struct tagStdinDeviceRsc*)
                            Rsc_TraveScion(&stdin_rsc->stdin_device_node,
                                            &touch_rsc->stdin_device_node);
            if(touch_rsc == NULL)
                break;
            if(touch_rsc->input_type != EN_STDIN_SINGLE_TOUCH)
                continue;
            touch_pr = (struct tagSingleTouchPrivate*)touch_rsc->stdin_private;
            if(touch_pr->read_touch(&touch_temp) != 0)
            {
                if((touch_temp.x != touch_pr->touch_loc.x)
                    ||(touch_temp.y != touch_pr->touch_loc.y)
                    ||(touch_temp.z != touch_pr->touch_loc.z))
                {
                    touch_temp.time = DjyGetTimeTick();
                    Stddev_InputMsg(touch_rsc->device_id,(u8*)&touch_temp,
                                                        sizeof(touch_temp));
                    touch_pr->touch_loc = touch_temp;
                    touch_pr->status = true;
                }
            }
            else
            {
                if(touch_pr->status == true)
                {
                    touch_pr->touch_loc.time = DjyGetTimeTick();
                    touch_pr->touch_loc.z = 0;
                    Stddev_InputMsg(touch_rsc->device_id,
                                    (u8*)&touch_pr->touch_loc,
                                    sizeof(touch_pr->touch_loc));
                    touch_pr->status = false;
                }
            }
        }
        Djy_EventDelay(10*mS);
    }
}


s32 Touch_InstallDevice(char *touch_name,struct tagSingleTouchPrivate *touch_pr)
{
    s32  ret = 0;
    ret =  Stddev_InstallDevice(touch_name, EN_STDIN_SINGLE_TOUCH, touch_pr);
    if(-1 == ret)
        touch_pr->status = false;
    return ret;
}
//----��ʼ��������ģ��---------------------------------------------------------
//����: ��ʼ��������ģ�飬����:��װ��������׼�����豸���ǼǴ�����ɨ���¼�
//      ���ͣ�pop������ɨ���¼�������ɨ��(ʵ��ɨ�蹤��Ҫ�ȶ��߳�������ſ�ʼ)
//����: ��
//����: 0=ʧ�ܣ�1=�ɹ�
//----------------------------------------------------------------------------
ptu32_t ModuleInstall_Touch(ptu32_t para)
{
    s16 touch_scan_evtt;
    if(!Rsc_SearchTree("stdin input device"))      //��׼�����豸δ��ʼ��
        return 0;
    touch_scan_evtt = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_REAL,0,0,
                            Touch_Scan,NULL,1024,"touch");
    if(touch_scan_evtt == CN_EVTT_ID_INVALID)
    {
        return 0;
    }
    if(Djy_EventPop(touch_scan_evtt, NULL,0,0,0,0)
                        == (uint16_t)CN_EVENT_ID_INVALID)
    {
        Djy_EvttUnregist(touch_scan_evtt);
        return 0;
    }
    return 1;
}

