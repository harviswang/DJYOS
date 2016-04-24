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
//�ļ�����: �������뼯
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

void list_init(list_t *l)
{
        l->next = l->prev = l;
}

void list_insert_after(list_t *l, list_t *n)
{
        l->next->prev = n;
        n->next = l->next;

        l->next = n;
        n->prev = l;
}

void list_insert_before(list_t *l, list_t *n)
{
        l->prev->next = n;
        n->prev = l->prev;

        l->prev = n;
        n->next = l;
}

void list_remove(list_t *n)
{
        n->next->prev = n->prev;
        n->prev->next = n->next;

        n->next = n->prev = n;
}

int list_isempty(const list_t *l)
{
        return l->next == l;
}


/*============================================================================*/
u32 GUI_GetTickMS(void)
{
    return (u32)(DjyGetTime( )/1000);
}


static  struct tagMutexLCB *gdd_mutex_lock=NULL;

BOOL    GDD_Lock(void)
{
        Lock_MutexPend(gdd_mutex_lock,5000*mS);
        return TRUE;
}

void    GDD_Unlock(void)
{
        Lock_MutexPost(gdd_mutex_lock);
}

void    GDD_WindowInit(void);
void    GDD_TimerInit(void);

/*============================================================================*/
void    GDD_Init(void)
{
    gdd_mutex_lock =Lock_MutexCreate(NULL);
    if(gdd_mutex_lock!=NULL)
    {
        GDD_WindowInit();
        GDD_TimerInit();
    }
}

/*============================================================================*/

static	u16 evtt_gui_server;


static struct tagGkWinRsc *pGkDesktop;

static	const char **GDD_InputDevName;

static void gdd_input_handler(void)
{
	tpInputMsgQ pMsgQ;
	struct tagInputDeviceMsg msg;
	int i;

	pMsgQ=Stddev_CreatInputMsgQ(20,"input_msg");

	if(pMsgQ!=NULL)
	{
		if(GDD_InputDevName!=NULL)
		{
			i=0;
			while(1)
			{
				if(GDD_InputDevName[i] == NULL)
				{
					break;
				}

				Stddev_SetFocus(GDD_InputDevName[i],pMsgQ);
				i++;
			}
		}

		while(1)
		{
			if(Stddev_ReadMsg(pMsgQ,&msg,0xFFFFFFFF)!=false)
			{
				switch(msg.input_type)
				{
					case EN_STDIN_SINGLE_TOUCH:
					{
						static int x=0,y=0,z=0;

						x =msg.input_data.tagSingleTouchMsg.x;
						y =msg.input_data.tagSingleTouchMsg.y;
						z =msg.input_data.tagSingleTouchMsg.z;
						if(z>0)
						{
							MouseInput(x,y,MK_LBUTTON);
						}
						else
						{
							MouseInput(x,y,0);
						}
						//printf("ts: x=%d,y=%d,z=%d\r\n",x,y,z);
					}
					break;
					////
					case EN_STDIN_KEYBOARD:
					{
						u8* key;
						u32 time;
						u8 val,event;

						key  =msg.input_data.key_board.key_value;
						time =msg.input_data.key_board.time;

						val  =key[0];
						event =key[1];

						if(event==0x00)
						{
							KeyboardInput(val,KEY_EVENT_DOWN,time);
						}

						if(event==0xF0)
						{
							KeyboardInput(val,KEY_EVENT_UP,time);
						}

						//printf("KBD_Event: val:%02XH,%02XH; time:%d\r\n",val,event,time);
					}
					break;
					////
				}


			}
		}

		Stddev_DeleteInputMsgQ(pMsgQ);
	}
}




static	ptu32_t gdd_input_thread(void)
{
	//Djy_EventDelay(100*mS);
	printf("GDD_Input_Thread.\r\n");
	Djy_EventPop(evtt_gui_server, NULL, 0, NULL, 0, 0);
	while(1)
	{
		gdd_input_handler();
	}
	return 0;
}

static  ptu32_t gdd_server_thread(void)
{
	Djy_EventDelay(100*mS);
    printf("GDD_Server_Thread.\r\n");

    while(1)
    {
        GDD_Execu(pGkDesktop);
    }
    return 0;
}

void    ModuleInstall_GDD(struct tagGkWinRsc *desktop,const char *InputDevName[])
{
	u16 evtt;

	GDD_Init();

    pGkDesktop =desktop;

    GDD_InputDevName =InputDevName;

    ////gdd_server
    evtt_gui_server = Djy_EvttRegist(  EN_CORRELATIVE, CN_PRIO_RRS, 0, 0,
                           gdd_server_thread, NULL,2048,"gdd server");
  /*
    if (evtt != CN_EVTT_ID_INVALID)
    {
    	Djy_EventPop(evtt, NULL, 0, NULL, 0, 0);
    }
*/

    ////gdd_input
    evtt = Djy_EvttRegist(  EN_CORRELATIVE, CN_PRIO_RRS, 0, 0,
                          gdd_input_thread, NULL,2048,"gdd input");
    if (evtt != CN_EVTT_ID_INVALID)
    {
    	Djy_EventPop(evtt, NULL, 0, NULL, 0, 0);
    }


}
/*============================================================================*/
