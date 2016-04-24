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
//����ģ��:�û�����ģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����: ��ꡢ���̵������豸����
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2011-01-24
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: �����ļ�
//------------------------------------------------------
#ifndef __STDIN_DEV_H__
#define __STDIN_DEV_H__
#include "rsc.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct tagMsgQueue * tpInputMsgQ;

enum _STDIN_INPUT_TYPE_
{
    EN_CHAR_TERMINERAL,
    EN_STDIN_KEYBOARD,            //����
    EN_STDIN_MOUSE_2D,            //2d���
    EN_STDIN_MOUSE_3D,            //3d���
    EN_STDIN_SINGLE_TOUCH,        //���㴥����
    EN_STDIN_MUTI_TOUCH,          //��㴥����
    EN_STDIN_AREA_TOUCH,          //��������
};

//�ַ��ն��豸���ݽṹ
#define CN_CHAR_BUF_LIMIT   16
struct tagCharTermineralMsg
{
    u32 time;               //�����ַ���ʱ��,ticks��
    u32 num;
    u8 input_char[CN_CHAR_BUF_LIMIT];         //������ַ�
};

//������Ϣ���ݽṹ
struct tagKeyBoardMsg
{
    u32 time;              //�����¼�����ʱ��,ticks��
    u8 key_value[2];       //��ֵ,ÿ������2�������
};

//2ά������ݽṹ����Ŀǰ��õ����
struct tagMouse2D_Msg
{
    u32 time;               //����¼�����ʱ��,ticks��
    u16 key_no;             //������������
};

//3ά������ݽṹ
struct tagMouse3D_Msg
{
    u32 time;              //�����¼�����ʱ��,ticks��
};

//���㴥����������õĴ�����
struct tagSingleTouchMsg
{
    u32 time;              //�����¼�����ʱ��,ticks��
    s32 x,y,z;  //x,y��ʾ����λ�ã�z>0��־����ѹ����0��־δ����
};

//��㴥����
struct tagMultiTouchMsg
{
    u32 time;              //�����¼�����ʱ��,ticks��
};

//������������ʾ����������һ������
struct tagAreaTouchMsg
{
    u32 time;              //�����¼�����ʱ��,ticks��
};

union un_input_data
{
    struct tagCharTermineralMsg char_termineral;
    struct tagKeyBoardMsg key_board;
    struct tagMouse2D_Msg  mouse_2d;
    struct tagMouse3D_Msg  mouse_3d;
    struct tagSingleTouchMsg  tagSingleTouchMsg;
    struct tagMultiTouchMsg   muti_touch;
    struct tagAreaTouchMsg    area_touch;
};

struct tagInputDeviceMsg
{
    enum _STDIN_INPUT_TYPE_ input_type;         //������Ϣ���ͣ�
    s32 device_id;          //�����豸id
    union un_input_data input_data;
};

struct tagStdinDeviceRsc
{
    struct tagRscNode stdin_device_node;
    enum _STDIN_INPUT_TYPE_ input_type;   //�����豸����
    s32 device_id;                      //�����豸id
    u32 input_counter;                  //�������豸�ۼ������˶�����Ϣ
    tpInputMsgQ FocusMsgQ;             //���ձ������豸��������Ϣ����
    ptu32_t stdin_private;              //�����豸��˽�����ݽṹ��
};

bool_t ModuleInstall_Stddev(ptu32_t para);
s32 Stddev_InstallDevice(char *device_name,enum _STDIN_INPUT_TYPE_ stdin_type,
                        void *myprivate);
bool_t Stddev_UnInstallDevice(char *device_name);
tpInputMsgQ Stddev_CreatInputMsgQ(u32 MsgNum,const char *Name);
bool_t Stddev_DeleteInputMsgQ(tpInputMsgQ InputMsgQ);
bool_t Stddev_ReadMsg(tpInputMsgQ InputMsgQ,
                     struct tagInputDeviceMsg *MsgBuf,u32 TimeOut);
bool_t Stddev_ReadDefaultMsg(struct tagInputDeviceMsg *MsgBuf,u32 TimeOut);
bool_t Stddev_SetFocus(char *device_name,tpInputMsgQ FocusMsgQ);
void Stddev_SetFocusDefault(tpInputMsgQ FocusMsgQ);
void Stddev_SetFocusAll(tpInputMsgQ FocusMsgQ);
tpInputMsgQ Stddev_GetFocusDefault(void);
bool_t Stddev_InputMsg(s32 stdin_id,u8 *msg_data, u32 msg_size);

#ifdef __cplusplus
}
#endif

#endif //__STDIN_DEV_H__

