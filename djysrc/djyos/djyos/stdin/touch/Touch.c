//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
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

#include "os_inc.h"

#if (cfg_single_touch == 1)

#include "Touch.h"
#include "string.h"

s16 s16g_touch_scan_evtt;
struct st_touch_adjust tg_touch_adjust;
static s32 s32g_touch_stdin_id;


//----������ɨ������-----------------------------------------------------------
//����: �����Եص���Ӳ��ɨ����򣬻�ȡ�û��û�������Ϣ�����ѻ�õ���Ϣ���뵽��׼
//      ���뻺����(���ú���stdin_input_msg)��ע�⣬̧�����ʺ󣬻�Ҫ����һ����
//      �����뿪����Ϣ
//����: my_event�����̷����¼����͡�
//����: ��
//----------------------------------------------------------------------------
void touch_scan(void)
{
    struct single_touch_msg now = {0,0,0},old = {0,0,0};
    bool_t getted=false;
    char **display_name;

    //�¼������Ǵ�������Ӧ����ʾ��������(�ַ���)
    display_name = (char**)djy_get_event_para(NULL);
    touch_ratio_adjust((char *)(*display_name),&tg_touch_adjust);

    while(1)
    {
        if(touch_scan_hard(&now) != 0)
        {
            now.time = djy_get_time_tick();
            stdin_input_msg(s32g_touch_stdin_id,(u8*)&now,sizeof(now));
            old = now;
            getted = true;
        }
        else
        {
            if(getted == true)
            {
                now.time = djy_get_time_tick();
                now.z = 0;
                stdin_input_msg(s32g_touch_stdin_id,(u8*)&now,sizeof(now));
                getted = false;
            }
        }
        djy_event_delay(10*mS);
    }
}

//----��ʼ��������ģ��---------------------------------------------------------
//����: ��ʼ��������ģ�飬����:��װ��������׼�����豸���ǼǴ�����ɨ���¼�
//      ���ͣ�pop������ɨ���¼�������ɨ��(ʵ��ɨ�蹤��Ҫ�ȶ��߳�������ſ�ʼ)
//����: ��
//����: �������豸�ţ�-1��ʾ��װ�������豸ʧ��
//----------------------------------------------------------------------------
ptu32_t module_init_touch(ptu32_t para)
{
    if( !touch_hard_init())
        return -1;
    s32g_touch_stdin_id = stdin_install_device("touch scan",
                            enum_stdin_single_touch);
    if(s32g_touch_stdin_id == -1)
        return -1;
    s16g_touch_scan_evtt = djy_evtt_regist(enum_correlative,cn_prio_real,0,1,
                            touch_scan,1024,"touch");
    if(s16g_touch_scan_evtt == cn_invalid_evtt_id)
    {
        stdin_uninstall_device("touch scan");
        return -1;
    }
    if(djy_event_pop(s16g_touch_scan_evtt,enum_para_async,
                                        NULL,0,&para,sizeof(ptu32_t),0,0)
                        == (uint16_t)cn_invalid_event_id)
    {
        djy_evtt_unregist(s16g_touch_scan_evtt);
        stdin_uninstall_device("touch scan");
        return -1;
    }
    return s32g_touch_stdin_id;
}

#endif  //for #if (cfg_single_touch == 1)
