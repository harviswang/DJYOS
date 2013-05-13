//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
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
#include "os_inc.h"

#if cfg_keyboard == 1

#include "key.h"
static s32 s32g_keyboard_stdin_id;
static u16 u16g_evtt_key;
//----��ʼ������ģ��----------------------------------------------------------
//����: ��ʼ������ģ�飬����:��װ���̱�׼�����豸���ǼǼ���ɨ���¼�
//      ���ͣ�pop����ɨ���¼�������ɨ��(ʵ��ɨ�蹤��Ҫ�ȶ��߳�������ſ�ʼ)
//����: ��
//����: �����豸�ţ�-1��ʾ��װ�����豸ʧ��
//----------------------------------------------------------------------------
ptu32_t module_init_keyboard(ptu32_t para)
{
    if( !key_hard_init())
        return -1;
    s32g_keyboard_stdin_id = stdin_install_device("key board",enum_stdin_keyboard);
    if(s32g_keyboard_stdin_id == -1)
        return -1;
    u16g_evtt_key = djy_evtt_regist(enum_correlative,cn_prio_RRS,0,1,
                                    key_scan,512,"keyboard");
    if(u16g_evtt_key == cn_invalid_evtt_id)
    {
        stdin_uninstall_device("key board");
        return -1;
    }
    if(djy_event_pop(u16g_evtt_key,enum_para_async,NULL,0,NULL,0,0,0) 
                        == (uint16_t)cn_invalid_event_id)
    {
        djy_evtt_unregist(u16g_evtt_key);
        stdin_uninstall_device("key board");
        return -1;
    }
    return s32g_keyboard_stdin_id;
}

//----����ɨ������-------------------------------------------------------------
//����: �����Եص���Ӳ��ɨ����򣬻�ȡ�û��û���Ϣ���ϳɱ�׼�����룬�͵���׼
//      �����豸�С������밴��PS/2��׼ɨ���롣ɨ���������ֲ�ͬ�����ͣ�ͨ��
//      (make code)�Ͷ���(break code)����һ���������»������סʱ,�Ͳ���һ��ͨ
//      ��;����һ�������ͷ�ʱ,�Ͳ���һ�����롣���ݼ��̰���ɨ����Ĳ�ͬ,�ɽ�����
//      ��Ϊ���¼��ࣺ
//      ��һ���,ͨ��Ϊ1�ֽ�,����Ϊ0xF0+ͨ�롣��A��,ͨ��Ϊ0x1C,����Ϊ0xF0 0x1C��
//      �ڶ����,ͨ��Ϊ2�ֽ�0xE0+0xXX��ʽ,����Ϊ0xE0+0xF0+0xXX��ʽ����right ctrl
//              ��,��ͨ��Ϊ0xE0 0x14,����Ϊ0xE0 0xF0 0x14��
//      ���������ⰴ��������,print screen��ͨ��Ϊ0xE0 0x12 0xE0 0x7C,����Ϊ0xE0
//              0xF0 0x7C 0xE0 0xF0 0x12; pause��ͨ��Ϊ0x E1 0x14 0x77 0xE1 0xF0
//              0x14 0xF0 0x77,����Ϊ�ա�
//      djyos�ļ�������֧��ǰ���ఴ���������ǵ��������ⰴ����
//����: my_event�����̷����¼����͡�
//����: ��
//ע��: �������Ѿ������˰���������Ӳ���������ĺ���key_scan_hard�ɲ�����
//----------------------------------------------------------------------------
void key_scan(void)
{
    struct key_board_msg key_msg;
    uint16_t key_value_now[2]={0,0};
    uint16_t key_value_bak[2]={0,0};
    uint16_t key_value_temp[2]={0,0};
    static ufast_t holding=5;
    ufast_t get_key=0;
    ufast_t key_num,key_num_bak=0;

    while(1)
    {
        djy_event_delay(10*mS);
        key_num = key_scan_hard(key_value_now);

        if((key_value_now[0] != key_value_temp[0])
                ||(key_value_now[1] != key_value_temp[1])
                ||(key_num != key_num_bak))
        {
            holding = 0;
            key_num_bak = key_num;
            key_value_temp[0] = key_value_now[0];
            key_value_temp[1] = key_value_now[1];
        }
        //״̬�ı�󱣳���ͬ״̬7�ξ�������,����7�β���.
        holding++;
        if(holding < 7)
            continue;
        if(holding > 7)
        {
            holding = 7;
            continue;
        }
        if(key_num == 0)
        {//û��ɨ�赽����
            if(get_key == 0)
                continue;
            key_msg.time = djy_get_time_tick();
            key_msg.key_value[1] = cn_break_code;
            key_msg.key_value[2] = key_value_bak[0]>>8;
            key_msg.key_value[0] = key_value_bak[0];
            stdin_input_msg(s32g_keyboard_stdin_id,(u8*)&key_msg,sizeof(key_msg));
            if(get_key == 2)
            {//����2����������ް���,������2������
                key_msg.key_value[2] = key_value_bak[1]>>8;
                key_msg.key_value[0] = key_value_bak[1];
                stdin_input_msg(s32g_keyboard_stdin_id,(u8*)&key_msg,sizeof(key_msg));
            }
            get_key = 0;
            continue;
        }

        key_msg.time = djy_get_time_tick();
        if(key_num==1)
        {//ɨ�赽��1������
            if(get_key == 0 )
            {//���ް������а���,��ͨ��
                key_msg.key_value[1] = key_value_now[0]>>8;
                key_msg.key_value[0] = key_value_now[0];
                stdin_input_msg(s32g_keyboard_stdin_id,(u8*)&key_msg,sizeof(key_msg));
            }else if(get_key == 1)
            {//�����а���, ���ΰ����ز���ͬ,���ͱ��ݰ����Ķ�����°�����ͨ��
                key_msg.key_value[2] = key_value_bak[0]>>8;
                key_msg.key_value[1] = cn_break_code;
                key_msg.key_value[0] = key_value_bak[0];
                stdin_input_msg(s32g_keyboard_stdin_id,(u8*)&key_msg,sizeof(key_msg));

                key_msg.key_value[1] = key_value_now[0]>>8;
                key_msg.key_value[0] = key_value_now[0];
                stdin_input_msg(s32g_keyboard_stdin_id,(u8*)&key_msg,sizeof(key_msg));
            }else if(get_key == 2)
            {//ԭ������������,�鿴�ĸ���������,���Ͷ���
                if(key_value_bak[0] == key_value_now[0])
                {// key_value_bak[1]������
                    key_msg.key_value[2] = key_value_bak[1]>>8;
                    key_msg.key_value[0] = key_value_bak[1];

                }else
                {// key_value_bak[0]������
                    key_msg.key_value[2] = key_value_bak[0]>>8;
                    key_msg.key_value[0] = key_value_bak[0];
                }
                key_msg.key_value[1] = cn_break_code;
                stdin_input_msg(s32g_keyboard_stdin_id,(u8*)&key_msg,sizeof(key_msg));
            }
            key_value_bak[0] = key_value_now[0];
            get_key = 1;
        }if(key_num == 2)
        {//ɨ�赽��2������
            if(get_key == 0 )
            {//���ް�������2����,��2��ͨ��
                key_msg.key_value[1] = key_value_now[0]>>8;
                key_msg.key_value[0] = key_value_now[0];
                stdin_input_msg(s32g_keyboard_stdin_id,(u8*)&key_msg,sizeof(key_msg));

                key_msg.key_value[1] = key_value_now[1]>>8;
                key_msg.key_value[0] = key_value_now[1];
                stdin_input_msg(s32g_keyboard_stdin_id,(u8*)&key_msg,sizeof(key_msg));
            }else if(get_key == 1)
            {//��1����������2������,���°���ͨ��
                if(key_value_bak[0] == key_value_now[0])
                {//�°����� key_value_now[1]
                    key_msg.key_value[1] = key_value_now[1]>>8;
                    key_msg.key_value[0] = key_value_now[1];
                }else
                {//�°����� key_value_now[0]
                    key_msg.key_value[1] = key_value_now[0]>>8;
                    key_msg.key_value[0] = key_value_now[0];
                }
                stdin_input_msg(s32g_keyboard_stdin_id,(u8*)&key_msg,sizeof(key_msg));
            }else if(get_key == 2)
            {//ԭ������������,�鿴�ĸ�������ͬ,�ֱ��Ͷ����ͨ��
                if((key_value_bak[0] != key_value_now[0])
                        &&(key_value_bak[0] != key_value_now[1]))
                {// key_value_bak[0]������
                    key_msg.key_value[2] = key_value_bak[0]>>8;
                    key_msg.key_value[0] = key_value_bak[0];
                    key_msg.key_value[1] = cn_break_code;
                    stdin_input_msg(s32g_keyboard_stdin_id,(u8*)&key_msg,sizeof(key_msg));
                }
                if((key_value_bak[1] != key_value_now[0])
                        &&(key_value_bak[1] != key_value_now[1]))
                {// key_value_bak[1]������
                    key_msg.key_value[2] = key_value_bak[1]>>8;
                    key_msg.key_value[0] = key_value_bak[1];
                    key_msg.key_value[1] = cn_break_code;
                    stdin_input_msg(s32g_keyboard_stdin_id,(u8*)&key_msg,sizeof(key_msg));
                }
                if((key_value_now[0] != key_value_bak[0])
                        &&(key_value_now[0] != key_value_bak[1]))
                {// key_value_now[0]������
                    key_msg.key_value[1] = key_value_now[0]>>8;
                    key_msg.key_value[0] = key_value_now[0];
                    stdin_input_msg(s32g_keyboard_stdin_id,(u8*)&key_msg,sizeof(key_msg));
                }
                if((key_value_now[1] != key_value_bak[0])
                        &&(key_value_now[1] != key_value_bak[1]))
                {// key_value_now[0]������
                    key_msg.key_value[1] = key_value_now[1]>>8;
                    key_msg.key_value[0] = key_value_now[1];
                    stdin_input_msg(s32g_keyboard_stdin_id,(u8*)&key_msg,sizeof(key_msg));
                }
            }
            key_value_bak[0] = key_value_now[0];
            key_value_bak[1] = key_value_now[1];
            get_key = 2;
        }
        continue;
    }
}

#endif   //for #if cfg_keyboard == 1

