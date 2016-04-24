//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. As a constituent part of djyos,do not transplant it to other software
//    without specific prior written permission.

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
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������
// 3. �������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ�������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------

//����ģ��: GDD
//����:  LiuWei.
//�汾��V1.0.0
//�ļ�����: �¼���������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2014-11-10
//   ����:  LiuWei.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include    "gdd.h"
#include    "./include/gdd_private.h"

/*============================================================================*/
#define VK_LBUTTON  1
#define VK_RBUTTON  2
#define VK_MBUTTON  3

#define VK_NUM  256

static  u8 key_map[VK_NUM];

#define KEY_STATE_UP    (0<<0)
#define KEY_STATE_DOWN  (1<<0)

/*============================================================================*/

static BOOL _IsKeyDown(int vkey)
{
    if(key_map[vkey]&KEY_STATE_DOWN)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static void _SetKeyDown(int vkey)
{
    key_map[vkey] |=  KEY_STATE_DOWN;
}

static void _SetKeyUp(int vkey)
{
    key_map[vkey] &= ~KEY_STATE_DOWN;
}

/*============================================================================*/
#if 0
#define KEY_STATE_UP    0
#define KEY_STATE_DOWN  1

void    GDD_KeyboardInput(u8 vkey,u8 statte)
{
    if(vkey<VK_NUM)
    {

    }
}
#endif


void    _SetKeyState(u8 vkey,u8 key_state)
{
    key_map[vkey] = key_state;
}

/*============================================================================*/

u8  GetMouseKeyState(void)
{
    u8 key_state=0;

    if(_IsKeyDown(VK_LBUTTON))
    {
        key_state |= MK_LBUTTON;
    }

    if(_IsKeyDown(VK_RBUTTON))
    {
        key_state |= MK_RBUTTON;
    }

    if(_IsKeyDown(VK_MBUTTON))
    {
        key_state |= MK_MBUTTON;
    }

    return key_state;
}

/*============================================================================*/

static  int cursor_x,cursor_y;
static  HWND HWND_LButton_Down=NULL;
static  int LButton_Msg=0;

void    MouseInput(s32 x,s32 y,u32 key_state)
{
    HWND hwnd;
    POINT pt;
    RECT rc;

    pt.x =x;
    pt.y =y;

    GDD_Lock();
    hwnd =GetWindowFromPoint(&pt); //��¼LButtonDown����.

    // LBUTTON
    if(key_state&MK_LBUTTON)
    {
        if(!_IsKeyDown(VK_LBUTTON))
        {
            _SetKeyDown(VK_LBUTTON);

            HWND_LButton_Down =hwnd;    //��¼LButtonDown����.
            GetClientRectToScreen(hwnd,&rc);
            if(_PtInRect(&rc,&pt))
            {
                LButton_Msg =MSG_LBUTTON_DOWN;
                _ScreenToClient(hwnd,&pt,1);    //�ͻ���ʹ�ÿͻ�����.
            }
            else
            {
                LButton_Msg =MSG_NCLBUTTON_DOWN;
            }

            PostMessage(hwnd,LButton_Msg,0,(pt.y<<16)|(pt.x));
        }
    }
    else
    {
        if(_IsKeyDown(VK_LBUTTON))
        {
            _SetKeyUp(VK_LBUTTON);
            hwnd =HWND_LButton_Down;
            if(LButton_Msg==MSG_LBUTTON_DOWN)
            {
                _ScreenToClient(hwnd,&pt,1);    //�ͻ���ʹ�ÿͻ�����.
                LButton_Msg =MSG_LBUTTON_UP;
            }
            else
            {
                LButton_Msg =MSG_NCLBUTTON_UP;
            }
            PostMessage(hwnd,LButton_Msg,0,(pt.y<<16)|(pt.x));
        }
    }



    if((cursor_x!=x) || (cursor_y!=y))
    {
        cursor_x =x;
        cursor_y =y;
        pt.x =x;
        pt.y =y;
        ScreenToClient(hwnd,&pt,1);
        PostMessage(hwnd,MSG_MOUSE_MOVE,key_state,(pt.y<<16)|(pt.x));
    }

    GDD_Unlock();
}

/*============================================================================*/

void	KeyboardInput(u16 key_val,EN_GDD_KEY_EVENT key_event,u32 time)
{
	HWND hwnd;
	switch(key_event)
	{
		case	KEY_EVENT_DOWN:
		{
			hwnd =GetFocusWindow();
			if(hwnd!=NULL)
			{
				PostMessage(hwnd,MSG_KEY_DOWN,key_val,time);
			}
		}
		break;
		////

		case	KEY_EVENT_UP:
		{
			hwnd =GetFocusWindow();
			if(hwnd!=NULL)
			{
				PostMessage(hwnd,MSG_KEY_UP,key_val,time);
			}
		}
		break;
		////

		default:	break;
		////

	}
}

/*============================================================================*/
