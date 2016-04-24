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
//�ļ�����: ���ڼ��ؼ����Ƽ�����
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

/*
typedef struct  __WNDCLASS  //���������ݽṹ
{
    WNDPROC *WinProc;
    u32 DrawColor;
    u32 FillColor;
    u32 TextColor;
}WNDCLASS;
*/


/*============================================================================*/

static  HWND HWND_Desktop=NULL;
static	HWND HWND_Focus=NULL;

static  struct tagGkWinRsc *pGkWinDesktop=NULL;

//----���䴰�ڶ����ڴ�-----------------------------------------------------------
//����: ��
//��������
//���أ����ڶ����ڴ��ڴ�ָ��
//------------------------------------------------------------------------------
static  WINDOW* WIN_Alloc(void)
{
    WINDOW *pwin;

    pwin=malloc(sizeof(WINDOW));
    if(NULL!=pwin)
    {
        pwin->Tag =WIN_TAG;
    }
    return pwin;
}

//----�ͷŴ��ڶ����ڴ�-----------------------------------------------------------
//����: ��
//���������ڶ����ڴ��ڴ�ָ��
//���أ���
//------------------------------------------------------------------------------
static  void    WIN_Free(WINDOW *pwin)
{
    pwin->Tag =INVALD_TAG;
    free(pwin);
}

//----��������------------------------------------------------------------------
//����: ��������,���ڶԴ��ڻ������,�ú�������TRUEʱ,�������HWND_Unlock����;
//      �����ú�������FALSE,���������HWND_Unlock.
//������hwnd:���ھ��
//���أ��ɹ�:TRUE; ʧ��:FLASE;
//------------------------------------------------------------------------------
BOOL    HWND_Lock(HWND hwnd)
{
    GDD_Lock();
    if(NULL!=hwnd)
    if(WIN_TAG==hwnd->Tag)
    {
        //Lock_MutexPend(hwnd->mutex_lock,5000000);
        //GDD_Unlock();
        return TRUE;
    }
    GDD_Unlock();
    return  FALSE;
}

//----��������------------------------------------------------------------------
//����: �����������ɹ���,�ɸú������н�������.
//������hwnd:���ھ��
//���أ���
//------------------------------------------------------------------------------
void    HWND_Unlock(HWND hwnd)
{
    GDD_Unlock();
    //Lock_MutexPost(hwnd->mutex_lock);
}

//----��Ļ����ת�ͻ�����---------------------------------------------------------
//����: �ú���Ϊ�ڲ�����,����麯�������ĺϷ���.
//������hwnd:���ھ��
//      pt:  ��Ҫת���������ָ��
//      count: ��Ҫת�������������
//���أ���
//------------------------------------------------------------------------------
void    _ScreenToClient(HWND hwnd,POINT *pt,s32 count)
{
    int i;
    for(i=0;i<count;i++)
    {
        pt[i].x -= hwnd->CliRect.left;
        pt[i].y -= hwnd->CliRect.top;
    }
}

//----��Ļ����ת�ͻ�����---------------------------------------------------------
//����: ��
//������hwnd:���ھ��
//      pt:  ��Ҫת���������ָ��
//      count: ��Ҫת�������������
//���أ�TRUE:�ɹ�; FALSE:ʧ��.
//------------------------------------------------------------------------------
BOOL    ScreenToClient(HWND hwnd,POINT *pt,s32 count)
{
    if(NULL!=pt)
    {
        if(HWND_Lock(hwnd))
        {
            _ScreenToClient(hwnd,pt,count);
            HWND_Unlock(hwnd);
            return TRUE;
        }
    }
    return  FALSE;
}

//----�ͻ�����ת��Ļ����---------------------------------------------------------
//����: �ú���Ϊ�ڲ�����,����麯�������ĺϷ���,���̰߳�ȫ
//������hwnd:���ھ��
//      pt:  ��Ҫת���������ָ��
//      count: ��Ҫת�������������
//���أ���
//------------------------------------------------------------------------------
void    _ClientToScreen(HWND hwnd,POINT *pt,s32 count)
{
    int i;

    for(i=0;i<count;i++)
    {
        pt[i].x += hwnd->CliRect.left;
        pt[i].y += hwnd->CliRect.top;
    }
}

//----�ͻ�����ת��Ļ����---------------------------------------------------------
//����: ��
//������hwnd:���ھ��
//      pt:  ��Ҫת���������ָ��
//      count: ��Ҫת�������������
//���أ�TRUE:�ɹ�; FALSE:ʧ��.
//------------------------------------------------------------------------------
BOOL    ClientToScreen(HWND hwnd,POINT *pt,s32 count)
{
    if(NULL!=pt)
    {
        if(HWND_Lock(hwnd))
        {
            _ClientToScreen(hwnd,pt,count);
            HWND_Unlock(hwnd);
            return  TRUE;
        }

    }
    return  FALSE;
}

//----��Ļ����ת��������---------------------------------------------------------
//����: �ú���Ϊ�ڲ�����,����麯�������ĺϷ���,���̰߳�ȫ
//������hwnd:���ھ��
//      pt:  ��Ҫת���������ָ��
//      count: ��Ҫת�������������
//���أ���
//------------------------------------------------------------------------------
void    _ScreenToWindow(HWND hwnd,POINT *pt,s32 count)
{
    int i;

    for(i=0;i<count;i++)
    {
        pt[i].x -= hwnd->WinRect.left;
        pt[i].y -= hwnd->WinRect.top;
    }
}

//----��Ļ����ת��������---------------------------------------------------------
//����: ��
//������hwnd:���ھ��
//      pt:  ��Ҫת���������ָ��
//      count: ��Ҫת�������������
//���أ�TRUE:�ɹ�; FALSE:ʧ��.
//------------------------------------------------------------------------------
BOOL    ScreenToWindow(HWND hwnd,POINT *pt,s32 count)
{
    if(NULL!=pt)
    {
        if(HWND_Lock(hwnd))
        {
            _ScreenToWindow(hwnd,pt,count);
            HWND_Unlock(hwnd);
            return  TRUE;
        }
    }
    return  FALSE;
}

//----��������ת��Ļ����---------------------------------------------------------
//����: �ú���Ϊ�ڲ�����,����麯�������ĺϷ���,���̰߳�ȫ
//������hwnd:���ھ��
//      pt:  ��Ҫת���������ָ��
//      count: ��Ҫת�������������
//���أ���
//------------------------------------------------------------------------------
void    _WindowToScreen(HWND hwnd,POINT *pt,s32 count)
{
    int i;
    for(i=0;i<count;i++)
    {
        pt->x += hwnd->WinRect.left;
        pt->y += hwnd->WinRect.top;
    }
}

//----��������ת��Ļ����---------------------------------------------------------
//����: ��
//������hwnd:���ھ��
//      pt:  ��Ҫת���������ָ��
//      count: ��Ҫת�������������
//���أ�TRUE:�ɹ�; FALSE:ʧ��.
//------------------------------------------------------------------------------
BOOL    WindowToScreen(HWND hwnd,POINT *pt,s32 count)
{
    if(NULL!=pt)
    {
        if(HWND_Lock(hwnd))
        {
            _WindowToScreen(hwnd,pt,count);
            HWND_Unlock(hwnd);
            return TRUE;
        }
    }
    return FALSE;
}

//----������洰�ھ��-----------------------------------------------------------
//����: ��
//��������
//���أ����洰�ھ��.
//------------------------------------------------------------------------------
HWND    GetDesktopWindow(void)
{
    HWND hwnd;

    hwnd =NULL;
    if(HWND_Lock(HWND_Desktop))
    {
        hwnd =HWND_Desktop;
        HWND_Unlock(HWND_Desktop);
    }
    return hwnd;
}

//----��ô��ھ�����-----------------------------------------------------------
//����: �ú���Ϊ�ڲ�����,����麯�������ĺϷ���,���̰߳�ȫ
//������hwnd:���ھ��
//      prc:���ڱ���������ݵ�ָ��.
//���أ���
//------------------------------------------------------------------------------
void    _GetWindowRect(HWND hwnd,RECT *prc)
{
    _CopyRect(prc,&hwnd->WinRect);
}

//----��ô��ھ�����-----------------------------------------------------------
//����: ��ô��ھ�����,����Ϊ��Ļ����.
//������hwnd:���ھ��
//      prc:���ڱ���������ݵ�ָ��.
//���أ�TRUE:�ɹ�; FALSE:ʧ��.
//------------------------------------------------------------------------------
BOOL    GetWindowRect(HWND hwnd,RECT *prc)
{
    if(NULL!=prc)
    if(HWND_Lock(hwnd))
    {
        _GetWindowRect(hwnd,prc);
        HWND_Unlock(hwnd);
        return TRUE;
    }
    return FALSE;
}

//----��ô��ڿͻ�������---------------------------------------------------------
//����: �ú���Ϊ�ڲ�����,����麯�������ĺϷ���,���̰߳�ȫ
//������hwnd:���ھ��
//      prc:���ڱ���������ݵ�ָ��.
//���أ���
//------------------------------------------------------------------------------
void    _GetClientRect(HWND hwnd,RECT *prc)
{

    prc->left =0;
    prc->top =0;
    prc->right =RectW(&hwnd->CliRect);
    prc->bottom =RectH(&hwnd->CliRect);
}

//----��ô��ڿͻ�������---------------------------------------------------------
//����: ��ô��ڿͻ�������,����Ϊ�ͻ�����.
//������hwnd:���ھ��
//      prc:���ڱ���������ݵ�ָ��.
//���أ�TRUE:�ɹ�; FALSE:ʧ��
//------------------------------------------------------------------------------
BOOL    GetClientRect(HWND hwnd,RECT *prc)
{
    if(NULL!=prc)
    if(HWND_Lock(hwnd))
    {
        _GetClientRect(hwnd,prc);
        HWND_Unlock(hwnd);
        return TRUE;
    }
    return FALSE;
}

//----��ô��ڿͻ���������ת��Ϊ��Ļ����--------------------------------------------
//����: �ú���Ϊ�ڲ�����,����麯�������ĺϷ���,���̰߳�ȫ
//������hwnd:���ھ��
//      prc:���ڱ���������ݵ�ָ��.
//���أ���
//------------------------------------------------------------------------------
void    _GetClientRectToScreen(HWND hwnd,RECT *prc)
{
    _CopyRect(prc,&hwnd->CliRect);
}

//----��ô��ڿͻ���������ת��Ϊ��Ļ����------------------------------------------
//����: ��
//������hwnd:���ھ��
//      prc:���ڱ���������ݵ�ָ��.
//���أ�TRUE:�ɹ�; FALSE:ʧ��.
//------------------------------------------------------------------------------
BOOL    GetClientRectToScreen(HWND hwnd,RECT *prc)
{
    if(NULL!=prc)
    {
        if(HWND_Lock(hwnd))
        {
            _GetClientRectToScreen(hwnd,prc);
            HWND_Unlock(hwnd);
            return TRUE;
        }
    }
    return FALSE;
}

//----��ô�����Ϣ����-----------------------------------------------------------
//����: �ú���Ϊ�ڲ�����,����麯�������Ϸ���,���̰߳�ȫ
//������hwnd:���ھ��
//���أ�������Ϣ����ָ��.
//------------------------------------------------------------------------------
MSGQ*   _GetWindowMsgQ(HWND hwnd)
{
    return hwnd->pMsgQ;
}

//----��ô��ڹ��̻ص�-----------------------------------------------------------
//����: �ú���Ϊ�ڲ�����,����麯�������Ϸ���,���̰߳�ȫ
//������hwnd:���ھ��
//���أ����ڹ���ָ��.
//------------------------------------------------------------------------------
WNDPROC* _GetWindowProc(HWND hwnd)
{
    return hwnd->WndProc;
}

//----��ô����߳�Id-------------------------------------------------------------
//����: �ú���Ϊ�ڲ�����,����麯�������Ϸ���,���̰߳�ȫ
//������hwnd:���ھ��
//���أ������߳�Id.
//------------------------------------------------------------------------------
u32 _GetWindowEvent(HWND hwnd)
{
    return hwnd->EventID;
}

//----��ø�����-----------------------------------------------------------------
//����: �ú���Ϊ�ڲ�����,����麯�������Ϸ���,���̰߳�ȫ
//������hwnd:���ھ��
//���أ������ھ��.
//------------------------------------------------------------------------------
HWND    _GetParent(HWND hwnd)
{
    return  hwnd->Parent;
}

//----��ø�����-----------------------------------------------------------------
//����: ��
//������hwnd:���ھ��
//���أ������ھ��.
//------------------------------------------------------------------------------
HWND    GetParent(HWND hwnd)
{
    HWND wnd=NULL;
    if(HWND_Lock(hwnd))
    {
        wnd= _GetParent(hwnd);
        HWND_Unlock(hwnd);
    }
    return wnd;
}

//----��ô��ڷ��---------------------------------------------------------------
//����: �ú���Ϊ�ڲ�����,����麯�������ĺϷ�,���̰߳�ȫ
//������hwnd:���ھ��
//���أ����ڷ��.
//------------------------------------------------------------------------------
u32 _GetWindowStyle(HWND hwnd)
{
    return  hwnd->Style;
}

u32	GetWindowStyle(HWND hwnd)
{
	u32 style=0;

	if(HWND_Lock(hwnd))
	{
		style =_GetWindowStyle(hwnd);
		HWND_Unlock(hwnd);
	}
	return style;
}

//----��ô���˽������-----------------------------------------------------------
//����: �ú���Ϊ�ڲ�����,����麯�������ĺϷ�,���̰߳�ȫ
//������hwnd:���ھ��
//���أ�����˽������.
//------------------------------------------------------------------------------
void* _GetWindowPrivateData(HWND hwnd)
{
    return  hwnd->PrivateData;
}

//----��ô���˽������-----------------------------------------------------------
//����: ��
//������hwnd:���ھ��
//���أ�����˽������.
//------------------------------------------------------------------------------
void* GetWindowPrivateData(HWND hwnd)
{
	void *data;

	data=NULL;
	if(HWND_Lock(hwnd))
	{
		data =_GetWindowPrivateData(hwnd);
		HWND_Unlock(hwnd);
	}
	return data;
}

//----���ô���˽������-----------------------------------------------------------
//����: �ú���Ϊ�ڲ�����,����麯�������ĺϷ�,���̰߳�ȫ
//������hwnd:���ھ��
//      data: ˽������
//���أ���.
//------------------------------------------------------------------------------
void _SetWindowPrivateData(HWND hwnd,void *data)
{
   hwnd->PrivateData=data;
}

//----���ô���˽������-----------------------------------------------------------
//����: ��
//������hwnd:���ھ��
//      data: ˽������
//���أ���.
//------------------------------------------------------------------------------
void SetWindowPrivateData(HWND hwnd,void *data)
{
	if(HWND_Lock(hwnd))
	{
		_SetWindowPrivateData(hwnd,data);
		HWND_Unlock(hwnd);
	}
}

//----��ô����û�����-----------------------------------------------------------
//����: �ú���Ϊ�ڲ�����,����麯�������ĺϷ�,���̰߳�ȫ
//������hwnd:���ھ��
//���أ������û�����.
//------------------------------------------------------------------------------
void* _GetWindowUserData(HWND hwnd)
{
    return  hwnd->UserData;
}

//----��ô����û�����-----------------------------------------------------------
//����: ��
//������hwnd:���ھ��
//���أ������û�����.
//------------------------------------------------------------------------------
void* GetWindowUserData(HWND hwnd)
{
	void *data;

	data=NULL;
	if(HWND_Lock(hwnd))
	{
		data =_GetWindowUserData(hwnd);
		HWND_Unlock(hwnd);
	}
	return data;
}

//----���ô����û�����-----------------------------------------------------------
//����: �ú���Ϊ�ڲ�����,����麯�������ĺϷ�,���̰߳�ȫ
//������hwnd:���ھ��
//      data: �û�����
//���أ���.
//------------------------------------------------------------------------------
void _SetWindowUserData(HWND hwnd,void *data)
{
   hwnd->UserData=data;
}

//----���ô����û�����-----------------------------------------------------------
//����: ��
//������hwnd:���ھ��
//      data: �û�����
//���أ���.
//------------------------------------------------------------------------------
void SetWindowUserData(HWND hwnd,void *data)
{
	if(HWND_Lock(hwnd))
	{
		_SetWindowUserData(hwnd,data);
		HWND_Unlock(hwnd);
	}
}

//----���������������ض���ϵ�Ĵ���----------------------------------------------
//����: �ú���Ϊ�ڲ�����,����麯�������ĺϷ���,���̰߳�ȫ
//������hwnd:���ھ��
//      nCmd:�������
//���أ���ָ���������ض���ϵ�Ĵ��ھ��.
//------------------------------------------------------------------------------
HWND    _GetWindow(HWND hwnd,int nCmd)
{
    switch(nCmd)
    {
        case GW_CHILD:
            return  hwnd->Child;
            ////
        case GW_HWNDPREV:
            return  hwnd->Prev;
            ////
        case GW_HWNDNEXT:
            return hwnd->Next;
            ////
        case GW_HWNDFIRST:
            while(hwnd->Prev)
            {
                hwnd =hwnd->Prev;
            }
            return hwnd;
            ////
        case GW_HWNDLAST:
            while(hwnd->Next)
            {
                hwnd =hwnd->Next;
            }
            return hwnd;
            ////
        default:
            return NULL;

    }
}

//----���������������ض���ϵ�Ĵ���----------------------------------------------
//����: ��
//������hwnd:���ھ��
//      nCmd:�������
//���أ���ָ���������ض���ϵ�Ĵ��ھ��.
//------------------------------------------------------------------------------
HWND    GetWindow(HWND hwnd,int nCmd)
{
    HWND wnd=NULL;

    if(HWND_Lock(hwnd))
    {
        wnd =_GetWindow(hwnd,nCmd);
        HWND_Unlock(hwnd);
    }
    return wnd;
}

//----��Id���Ҵ����е��Ӵ���(�ؼ�)-----------------------------------------------
//����: �ú���Ϊ�ڲ�����,����麯�������ĺϷ���,���̰߳�ȫ
//������hwnd:���ھ��
//     Id: ��Ҫ���ҵ��Ӵ���(�ؼ�)Id.
//���أ����ҵ����Ӵ���(�ؼ�)���.
//------------------------------------------------------------------------------
HWND    _GetDlgItem(HWND hwnd,int Id)
{
    hwnd =hwnd->Child;
    while(hwnd)
    {
        if(hwnd->WinId == Id)
        {
            return hwnd;
        }

        hwnd =hwnd->Next;
    }
    return NULL;

}

//----��Id���Ҵ����е��Ӵ���(�ؼ�)-----------------------------------------------
//����: ��.
//������hwnd:���ھ��
//     Id: ��Ҫ���ҵ��Ӵ���(�ؼ�)Id.
//���أ����ҵ����Ӵ���(�ؼ�)���.
//------------------------------------------------------------------------------
HWND    GetDlgItem(HWND hwnd,int Id)
{
    if(HWND_Lock(hwnd))
    {
        hwnd =_GetDlgItem(hwnd,Id);
        HWND_Unlock(hwnd);
    }
    return hwnd;
}

//----���ô�����������-----------------------------------------------------------
//����: ��.
//������hwnd:���ھ��.
//     text: ��������.
//     max_len: ��������ֽڳ���.
//���أ���.
//------------------------------------------------------------------------------
void    _SetWindowText(HWND hwnd,const char *text,s32 max_len)
{
	s32 len;

	if(text!=NULL)
	{
		len =MIN(max_len,sizeof(hwnd->Text));

		strncpy(hwnd->Text,text,len);
	}
}

void	SetWindowText(HWND hwnd,const char *text,s32 max_len)
{
	if(HWND_Lock(hwnd))
	{
		_SetWindowText(hwnd,text,max_len);
		HWND_Unlock(hwnd);
	}
}

//----��ô�����������-----------------------------------------------------------
//����: ��.
//������hwnd:���ھ��.
//     text: �������ݻ�����.
//     max_len: �������ݻ���������ֽڳ���.
//���أ����ֻ�����ָ��.
//------------------------------------------------------------------------------
char* _GetWindowText(HWND hwnd,char *buf,s32 max_len)
{
	s32 len;

	if(buf!=NULL)
	{
		len =MIN(max_len,sizeof(hwnd->Text));

		strncpy(buf,hwnd->Text,len);
		return buf;
	}

	return hwnd->Text;

}

char* GetWindowText(HWND hwnd,char *buf,s32 max_len)
{
	char *p;

	p=NULL;
	if(HWND_Lock(hwnd))
	{
		p =_GetWindowText(hwnd,buf,max_len);
		HWND_Unlock(hwnd);
	}
	return p;
}

//----��������Ҵ����е��Ӵ���(�ؼ�)----------------------------------------------
//����: �ú���Ϊ�ڲ�����.
//������hwnd:���ھ��
//     pt: �����
//���أ����ҵ����Ӵ���(�ؼ�)���.
//------------------------------------------------------------------------------
HWND    GetChildWindowFromPoint(HWND hwnd,POINT *pt)
{
    RECT rc;

    if(NULL!=hwnd)
    {
        hwnd =GetWindow(hwnd,GW_CHILD);
        while(NULL!=hwnd)
        {
            if(NULL!=hwnd->Child)
            {
                GetChildWindowFromPoint(hwnd->Child,pt);
            }
            GetWindowRect(hwnd,&rc);

            if(PtInRect(&rc,pt))
            {
                return hwnd;
            }
            hwnd =GetWindow(hwnd,GW_HWNDNEXT);
        }
    }

    return NULL;
}

//----���������������-----------------------------------------------------------
//����: �ú���Ϊ�ڲ�����.
//������pt:�����
//���أ����ҵ��������ھ��.
//------------------------------------------------------------------------------
HWND    GetMainWindowFromPoint(POINT *pt)
{
    HWND hwnd;
    RECT rc;

    hwnd =GetWindow(GetDesktopWindow(),GW_CHILD);
    while(NULL!=hwnd)
    {
        GetWindowRect(hwnd,&rc);
        if(_PtInRect(&rc,pt))
        {
            return hwnd;
        }
        hwnd =GetWindow(hwnd,GW_HWNDNEXT);

    }
    /////
    return  GetDesktopWindow();
}

//----��������Ҵ���(�����ڻ��Ӵ���)----------------------------------------------
//����: ��.
//������pt:�����
//���أ����ҵ��Ĵ��ھ��.
//------------------------------------------------------------------------------
HWND    GetWindowFromPoint(POINT *pt)
{
    HWND hwnd;
    HWND wnd=NULL;;

    hwnd =GetMainWindowFromPoint(pt);
    if(hwnd != GetDesktopWindow())
    {
        wnd =GetChildWindowFromPoint(hwnd,pt);
        if(wnd != NULL)
        {
            hwnd =wnd;
        }
    }

    return hwnd;
}

//----���ý��㴰��---------------------------------------------------------------
//����: ��.
//������hwnd: �µĽ��㴰��
//���أ��ɵĽ��㴰��.
//------------------------------------------------------------------------------
HWND	SetFocusWindow(HWND hwnd)
{
	HWND wnd=NULL;

	if(GDD_Lock())
	{
		wnd =HWND_Focus;
		HWND_Focus =hwnd;
		GDD_Unlock();
	}
	else
	{
		return NULL;
	}

	if(wnd!=NULL)
	{
		SendMessage(wnd,MSG_KILLFOCUS,0,0);
	}
	if(hwnd!=NULL)
	{
		SendMessage(hwnd,MSG_SETFOCUS,0,0);
	}

	return wnd;

}

//----��ý��㴰��---------------------------------------------------------------
//����: ��.
//��������.
//���أ����㴰��.
//------------------------------------------------------------------------------
HWND	GetFocusWindow(void)
{
	HWND wnd=NULL;

	if(GDD_Lock())
	{
		wnd =HWND_Focus;
		GDD_Unlock();
	}
	return wnd;
}

//----�ж��Ƿ�Ϊ���㴰��---------------------------------------------------------
//����: ��.
//��������.
//���أ����ָ���Ĵ����ǽ��㴰��,������TRUE,���򷵻�FALSE.
//------------------------------------------------------------------------------
BOOL	IsFocusWindow(HWND hwnd)
{
	BOOL res=FALSE;

	if(HWND_Lock(hwnd))
	{
		if(GDD_Lock())
		{
			if(hwnd == HWND_Focus)
			{
				res=TRUE;
			}
			GDD_Unlock();
		}
		HWND_Unlock(hwnd);
	}
	return res;

}

//----��ʼ���������ݽṹ---------------------------------------------------------
//����: �ú���Ϊ�ڲ�����.
//������pwin:�������ݽṹָ��.
//      proc:���ڹ��̻ص�����ָ�룮
//      Text:������������ָ��.
//      Style:���ڷ��.
//      x,y,w,h:����λ�úʹ�С.
//      hParent:�����ھ��.
//      WinId: ����Id.
//���أ���.
//------------------------------------------------------------------------------
static  void    _InitWindow(WINDOW *pwin,WNDPROC *proc,const char *Text,u32 Style,
                            int x,int y,int w,int h,HWND hParent,u32 WinId)
{
    RECT rc;

    _SetRect(&rc,x,y,w,h);
    pwin->Parent =hParent;
    pwin->Child  =NULL;
    pwin->Prev  =NULL;
    pwin->Next  =NULL;
    pwin->PrivateData =NULL;
    pwin->UserData    =NULL;
    pwin->WndProc   =proc;

    //pwin->Text        =Text;
    strncpy(pwin->Text,Text,250);
    itoa((u32)pwin,pwin->Name,16);

    pwin->Style     =Style;
    pwin->WinId     =WinId&0x0000FFFF;
    list_init(&pwin->list_timer);
    list_init(&pwin->node_msg_close);
    list_init(&pwin->node_msg_ncpaint);
    list_init(&pwin->node_msg_paint);

    if(hParent != NULL)
    {
        _ClientToScreen(hParent,(POINT*)&rc,2);
    }

    _CopyRect(&pwin->WinRect,&rc);

    if(Style&WS_BORDER)
    {
        pwin->BorderSize =DEF_BORDER_SIZE;
        _InflateRectEx(&rc, -DEF_BORDER_SIZE,
                            -DEF_BORDER_SIZE,
                            -DEF_BORDER_SIZE,
                            -DEF_BORDER_SIZE);

    }
    else
    {
        pwin->BorderSize =0;
    }

    if(Style&WS_DLGFRAME)
    {
        pwin->DlgFrameSize =DEF_DLGFRAME_SIZE;
        _InflateRectEx(&rc, -DEF_DLGFRAME_SIZE,
                            -DEF_DLGFRAME_SIZE,
                            -DEF_DLGFRAME_SIZE,
                            -DEF_DLGFRAME_SIZE);
    }
    else
    {
        pwin->DlgFrameSize =0;
    }

    if(Style&WS_CAPTION)
    {
        pwin->CaptionSize =DEF_CAPTION_SIZE;
        _InflateRectEx(&rc,0,-DEF_CAPTION_SIZE,0,0);
    }
    else
    {
        pwin->CaptionSize =0;
    }
    _CopyRect(&pwin->CliRect,&rc);

}

//----���һ���Ӵ��ڵ�������-----------------------------------------------------
//����: �ú���Ϊ�ڲ�����,����麯�������ĺϷ���,���̰߳�ȫ.
//������hParent:�����ھ��.
//      hChild:��Ҫ��ӵ��Ӵ��ھ��.
//���أ���
//------------------------------------------------------------------------------
static void _AddChild(HWND hParent,HWND hChid)
{
    WINDOW *old;

    old =hParent->Child;

    hChid->Prev =NULL;
    hChid->Next =old;

    if(NULL!=old)
    {
        old->Prev =hChid;
    }

    hParent->Child =hChid;
}

//----��ʼ���洰��--------------------------------------------------------------
//����: �ú���Ϊ�ڲ�����,�����ڴ�������.
//������proc: ���洰�ڹ��̻ص�����
//      Text: ���洰������ָ��
//      x,y,w,h: ���洰��λ�úʹ�С
//      pdata: �û��Զ��帽������.
//���أ����洰�ھ��.
//------------------------------------------------------------------------------
HWND    CreateDesktop(
                    WNDPROC *proc,
                    const char *Text,
                    int x,int y,int w,int h,
                    const void *pdata)
{
    WINDOW *pwin=NULL;
    struct tagGkWinRsc *pGkWin=NULL;
    u32 Style;

    if(NULL!=proc)
    {
        pwin =WIN_Alloc();
        if(NULL!=pwin)
        {

            Style = 0;
            Style &=~(WS_CHILD);

            _InitWindow(pwin,proc,Text,Style,x,y,w,h,NULL,0);
            pGkWin =(struct tagGkWinRsc*)malloc(sizeof(struct tagGkWinRsc));

            if(!GK_ApiCreateGkwin(pGkWinDesktop, pGkWin, x,y,x+w,y+h,
                                RGB(0,0,0), CN_WINBUF_BUF, "desktop",
                                CN_SYS_PF_DISPLAY, 0,0,0))
            {
                free(pGkWin);
                return FALSE;
            }

            pwin->pGkWin     =pGkWin;
            pwin->mutex_lock =Lock_MutexCreate(NULL);
            pwin->pMsgQ      =GUI_CreateMsgQ(32);
            pwin->EventID   =Djy_MyEventId();
            pwin->Zorder     =0;
            GK_ApiSetPrio(pGkWin,pwin->Zorder,100*mS);

            HWND_Desktop =pwin;
            SendMessage(pwin,MSG_CREATE,(u32)pdata,0);
        }
    }
    return pwin;
}

//----��������------------------------------------------------------------------
//����: �ú������Դ��������ں��Ӵ���(�ؼ�)
//������proc: ���ڹ��̻ص�����
//      Text: ��������ָ��
//      Style: ���ڷ��(����ο����ڷ�����˵��)
//      x,y,w,h: ���洰��λ�úʹ�С
//      hParent: �����ھ��.�����NULL,��Ĭ������Ϊ������.
//      WinId: ����Id.�����������,�ò���������.
//      pdata: �û��Զ��帽������.
//���أ����ھ��.
//------------------------------------------------------------------------------
HWND    CreateWindow(WNDPROC *pfWinProc,
                    const char *Text,u32 Style,
                    s32 x,s32 y,s32 w,s32 h,
                    HWND hParent,u32 WinId,const void *pdata)
{
    WINDOW *pwin=NULL;
    struct tagGkWinRsc *pGkWin=NULL;

    if(NULL==hParent)
    {
        hParent =GetDesktopWindow();
    }

    if(HWND_Lock(hParent))
    {
        pwin =WIN_Alloc();
        if(NULL!=pwin)
        {
            //��ʼ����������
            _InitWindow(pwin,pfWinProc,Text,Style,x,y,w,h,hParent,WinId);

            x =pwin->WinRect.left;
            y =pwin->WinRect.top;
            w =RectW(&pwin->WinRect);
            h =RectH(&pwin->WinRect);

            pGkWin =(struct tagGkWinRsc*)malloc(sizeof(struct tagGkWinRsc));

            if(Style&WS_CHILD)
            {
                if(!GK_ApiCreateGkwin(pGkWinDesktop, pGkWin,x,y,x+w,y+h,
                                    RGB(0,0,0), CN_WINBUF_BUF, pwin->Name,
                                    CN_SYS_PF_DISPLAY, 0,0,0))
                {
                    free(pGkWin);
                    return NULL;
                }

                pwin->mutex_lock =hParent->mutex_lock;  //�Ӵ���ʹ�ø�������
                pwin->pMsgQ      =hParent->pMsgQ;       //�Ӵ���ʹ�ø�������Ϣ����
                pwin->EventID   =hParent->EventID;
                pwin->Zorder     =hParent->Zorder-1;
                GK_ApiSetPrio(pGkWin,pwin->Zorder,100*mS);

            }
            else
            {
                if(!GK_ApiCreateGkwin(pGkWinDesktop, pGkWin,x,y,x+w,y+h,
                                    RGB(0,0,0), CN_WINBUF_BUF, pwin->Name,
                                    CN_SYS_PF_DISPLAY, 0,0,0))
                {
                    free(pGkWin);
                    return NULL;
                }

                pwin->mutex_lock =Lock_MutexCreate(NULL);
                pwin->pMsgQ      =GUI_CreateMsgQ(32);
                pwin->EventID   =Djy_MyEventId();
                pwin->Zorder     =hParent->Zorder-1;
                GK_ApiSetPrio(pGkWin,pwin->Zorder,100*mS);

            }
            pwin->pGkWin = pGkWin;

            //���´�����ӵ�������
            _AddChild(hParent,pwin);

        }
        HWND_Unlock(hParent);

        if(NULL!=pwin)
        {
            SendMessage(pwin,MSG_CREATE,(u32)pdata,0);
            InvalidateWindow(pwin);
        }

    }
    return pwin;
}

//----ɾ��һ���Ӵ������ݽṹ-----------------------------------------------------
//����:�ú���Ϊ�ڲ�����,�����ͷŴ��ڽṹ�ڴ�,�����Լ��Ը�����ȥ�Ƴ�.
//��������Ҫɾ�����Ӵ��ھ��
//���أ���.
//------------------------------------------------------------------------------
static void _DeleteChildWindowData(HWND hwnd)
{
    WINDOW *prev,*next;

    if(hwnd->Style&WS_CHILD)
    {
        prev =hwnd->Prev;
        next =hwnd->Next;

        if(NULL!=next)
        {
            next->Prev =prev;
        }

        if(NULL!=prev)
        {
            prev->Next =next;
        }
        else
        {
            hwnd->Parent->Child =NULL;
        }

        hwnd->Prev =NULL;
        hwnd->Next =NULL;

        list_remove(&hwnd->node_msg_close);
        list_remove(&hwnd->node_msg_ncpaint);
        list_remove(&hwnd->node_msg_paint);
        _RemoveWindowTimer(hwnd);
        GK_ApiDestroyWin(hwnd->pGkWin);
        free(hwnd->pGkWin);
        hwnd->pGkWin =NULL;

        hwnd->mutex_lock =NULL; //�Ӵ���û��˽�е� mutex_lock,�����ͷ�.

        WIN_Free(hwnd);
    }

}
//----ɾ��һ�����������ݽṹ-----------------------------------------------------
//����:�ú���Ϊ�ڲ�����,�����ͷŴ��ڽṹ�ڴ�,�����Լ��Ը�����ȥ�Ƴ�.
//��������Ҫɾ���������ھ��
//���أ���.
//------------------------------------------------------------------------------
void _DeleteMainWindowData(HWND hwnd)
{
    WINDOW *prev,*next;

    //GDD_Lock();

    prev =hwnd->Prev;
    next =hwnd->Next;

    if(NULL!=next)
    {
        next->Prev =prev;
    }

    if(NULL!=prev)
    {
        prev->Next =next;
    }
    else
    {
        hwnd->Parent->Child =NULL;
    }

    hwnd->Prev =NULL;
    hwnd->Next =NULL;

    list_remove(&hwnd->node_msg_close);
    list_remove(&hwnd->node_msg_ncpaint);
    list_remove(&hwnd->node_msg_paint);

    _RemoveWindowTimer(hwnd);
    GUI_DeleteMsgQ(hwnd->pMsgQ);
    UpdateDisplay();
    GK_ApiDestroyWin(hwnd->pGkWin);
    UpdateDisplay();
    free(hwnd->pGkWin);
    hwnd->pGkWin =NULL;

    Lock_MutexDelete(hwnd->mutex_lock);
    hwnd->mutex_lock =NULL;
    WIN_Free(hwnd);

    //GDD_Unlock();
}


//----����һ���Ӵ���-------------------------------------------------------------
//����: �ú���Ϊ�ڲ�����,����ɾ�������ڽṹ,��������WSG_DESTROY��Ϣ��������.
//��������Ҫ���ٵ��Ӵ��ھ��
//���أ���.
//------------------------------------------------------------------------------
static void _DestroyChildWindow(HWND hwnd)
{
    HWND wnd,next;

    if(NULL!=hwnd->Child)   //�������Ӵ���
    {
        wnd =hwnd->Child;
        while(NULL!=wnd)
        {
            if(NULL!=wnd->Child)
            {
                _DestroyChildWindow(wnd);
            }
            next =wnd->Next;

            SendMessage(wnd,MSG_DESTROY,0,0);
            _DeleteChildWindowData(wnd);

            wnd =next;
        }
    }

}

//----����һ������--------------------------------------------------------------
//����: �����������ں��Ӵ���,����ɾ�������ڽṹ,��������WSG_DESTROY��Ϣ��������.
//������hwnd:��Ҫ���ٵĴ��ھ��
//���أ���.
//------------------------------------------------------------------------------
void    DestroyWindow(HWND hwnd)
{
	if(IsFocusWindow(hwnd))
	{
		SetFocusWindow(NULL);
	}

    UpdateDisplay();

    GDD_Lock();
    if(HWND_Lock(hwnd))
    {
        _DestroyChildWindow(hwnd);

        SendMessage(hwnd,MSG_DESTROY,0,0);
        if(hwnd->Style&WS_CHILD)
        {
            _DeleteChildWindowData(hwnd);
        }

        HWND_Unlock(hwnd);

    }
    GDD_Unlock();

}

//----ƫ��һ������--------------------------------------------------------------
//����: ƫ��һ������λ��,�������Ӵ���.�ú���Ϊ�ڲ�����,���̰߳�ȫ
//������hwnd:���ھ��.
//      dx,dy: ˮƽ�ʹ�ֱ�����ƫ����.
//���أ���.
//------------------------------------------------------------------------------
static  void    _OffsetWindowRect(HWND hwnd,int dx,int dy)
{
    _OffsetRect(&hwnd->WinRect,dx,dy);
    _OffsetRect(&hwnd->CliRect,dx,dy);

    GK_ApiMoveWin(  hwnd->pGkWin,
                    hwnd->WinRect.left,hwnd->WinRect.top,
                    0*mS);
    PostMessage(hwnd,MSG_PAINT,0,0);

}

//----ƫ�ƴ���------------------------------------------------------------------
//����: ƫ��һ������λ��,�����Ӵ���.�ú���Ϊ�ڲ�����,���̰߳�ȫ
//������hwnd:���ھ��.
//      dx,dy: ˮƽ�ʹ�ֱ�����ƫ����.
//���أ���.
//------------------------------------------------------------------------------
static  void    _OffsetWindow(HWND hwnd,int dx,int dy)
{
    HWND wnd;

    _OffsetWindowRect(hwnd,dx,dy);

    wnd =hwnd->Child;
    while(NULL!=wnd)
    {
        _OffsetWindowRect(wnd,dx,dy);
        wnd =wnd->Next;
    }

}

//----ƫ�ƴ���------------------------------------------------------------------
//����: ƫ��һ������λ��,�����Ӵ���.
//������hwnd:���ھ��.
//      dx,dy: ˮƽ�ʹ�ֱ�����ƫ����.
//���أ�TRUE:�ɹ�; FALSE:ʧ��.
//------------------------------------------------------------------------------
BOOL    OffsetWindow(HWND hwnd,int dx,int dy)
{
    if(HWND_Lock(hwnd))
    {
        _OffsetWindow(hwnd,dx,dy);
        HWND_Unlock(hwnd);
        return TRUE;
    }
    return FALSE;

}

//----�ƶ�����------------------------------------------------------------------
//����: �ƶ�һ������λ��,�����Ӵ���.�ú���Ϊ�ڲ�����.���̰߳�ȫ.
//������hwnd:���ھ��.
//      x,y: ����ڸ����ڿͻ�������λ��.
//���أ���.
//------------------------------------------------------------------------------
static void _MoveWindow(HWND hwnd,int x,int y)
{
    HWND hParent;
    POINT pt;

    if(hwnd == HWND_Desktop)
    {//���治�����ƶ�
        return;
    }

    pt.x =x;
    pt.y =y;

    hParent =_GetParent(hwnd);
    if(NULL!=hParent)
    {
        _ClientToScreen(hParent,&pt,1);
    }

    x = pt.x - hwnd->WinRect.left;
    y = pt.y - hwnd->WinRect.top;
    _OffsetWindow(hwnd,x,y);


}

//----�ƶ�����------------------------------------------------------------------
//����: �ƶ�һ������λ��,�����Ӵ���.
//������hwnd:���ھ��.
//      x,y: ����ڸ����ڿͻ�������λ��.
//���أ�TRUE:�ɹ�;FALSE:ʧ��.
//------------------------------------------------------------------------------
BOOL    MoveWindow(HWND hwnd,int x,int y)
{
    if(HWND_Lock(hwnd))
    {
        _MoveWindow(hwnd,x,y);
        HWND_Unlock(hwnd);
        return TRUE;
    }
    return FALSE;
}

//----�жϴ����Ƿ�ɼ�-----------------------------------------------------------
//����: �ú���Ϊ�ڲ�����,�����к����������,���̰߳�ȫ.
//������hwnd:���ھ��.
//���أ�TRUE:���ڿɼ�;FALSE:���ڲ��ɼ�.
//------------------------------------------------------------------------------
BOOL    _IsWindowVisible(HWND hwnd)
{
    while(hwnd)
    {
        if(!(hwnd->Style&WS_VISIBLE))
        {
            return FALSE;
        }
        hwnd=hwnd->Parent;
    }
    return TRUE;
}

//----�жϴ����Ƿ�ɼ�-----------------------------------------------------------
//����: ��
//������hwnd:���ھ��.
//���أ�TRUE:���ڿɼ�;FALSE:���ڲ��ɼ�.
//------------------------------------------------------------------------------
BOOL    IsWindowVisible(HWND hwnd)
{
    BOOL res=FALSE;

    if(HWND_Lock(hwnd))
    {
        res =_IsWindowVisible(hwnd);
        HWND_Unlock(hwnd);
    }
    return res;
}

//----���ô���Ϊ��Ч-----------------------------------------------------------
//����: ���ô���Ϊ��Ч����˼��:������Ҫ�ػ�.�����ػ���Ϣ���,�ú���Ϊ�ڲ�����.
//������hwnd:���ھ��.
//���أ���
//------------------------------------------------------------------------------
void    _InvalidateWindow(HWND hwnd)
{
    if(_IsWindowVisible(hwnd))
    {
        PostMessage(hwnd,MSG_NCPAINT,0,0);
        PostMessage(hwnd,MSG_PAINT,0,0);
    }
}

//----���ô���Ϊ��Ч-------------------------------------------------------------
//����: ��
//������hwnd:���ھ��.
//���أ�TRUE:�ɹ�; FALSE:ʧ��.
//------------------------------------------------------------------------------
BOOL    InvalidateWindow(HWND hwnd)
{
    if(HWND_Lock(hwnd))
    {
        _InvalidateWindow(hwnd);
        HWND_Unlock(hwnd);
        return TRUE;
    }
    return FALSE;
}

//----��ʾ/�����Ӵ���-------------------------------------------------------------
//����: ���������Ӵ���Ϊ��ʾ������,�ú���Ϊ�ڲ�����.
//������hwnd:���ھ��.
//      bShow: TURE:��ʾ����; FALSE:���ش���.
//���أ���.
//------------------------------------------------------------------------------
static void _ShowChild(HWND hwnd,BOOL bShow)
{

    if(hwnd->Child!=NULL)
    {
        _ShowChild(hwnd->Child,bShow);
    }

    hwnd =_GetWindow(hwnd,GW_HWNDLAST);
    while(hwnd)
    {
        if(bShow)
        {
            hwnd->Style |= WS_VISIBLE;
            PostMessage(hwnd,MSG_PAINT,0,0);
            PostMessage(hwnd,MSG_NCPAINT,0,0);
        }
        else
        {
            hwnd->Style &= ~WS_VISIBLE;
        }
        hwnd =_GetWindow(hwnd,GW_HWNDPREV);
    }
}

//----��ʾ/���ش���-------------------------------------------------------------
//����: ���ô���Ϊ��ʾ������(�����Ӵ���),�ú���Ϊ�ڲ�����.
//������hwnd:���ھ��.
//      bShow: TURE:��ʾ����; FALSE:���ش���.
//���أ���.
//------------------------------------------------------------------------------
static void _ShowWindow(HWND hwnd,BOOL bShow)
{
    UpdateDisplay();
    if(bShow)
    {
        //printf("show: %08XH\r\n",hwnd);

        GK_ApiSetPrio(hwnd->pGkWin,hwnd->Zorder,100*mS);

        hwnd->Style |= WS_VISIBLE;
        PostMessage(hwnd,MSG_PAINT,0,0);
        PostMessage(hwnd,MSG_NCPAINT,0,0);
        _ShowChild(hwnd,bShow);

    }
    else
    {
        //printf("hide:%08XH\r\n",hwnd);

    //  GK_ApiSetPrio(hwnd->pGkWin,1,100*mS);
        hwnd->Style &= ~WS_VISIBLE;
        _ShowChild(hwnd,bShow);

    }

}

//----��ʾ/���ش���-------------------------------------------------------------
//����: ���ô���Ϊ��ʾ������(�����Ӵ���),�ú���Ϊ�ڲ�����.
//������hwnd:���ھ��.
//      bShow: TURE:��ʾ����; FALSE:���ش���.
//���أ�TRUE:�ɹ�;FALSE:ʧ��.
//------------------------------------------------------------------------------
BOOL    ShowWindow(HWND hwnd,BOOL bShow)
{
    if(HWND_Lock(hwnd))
    {
        _ShowWindow(hwnd,bShow);
        HWND_Unlock(hwnd);
        return TRUE;
    }
    return FALSE;
}

#if 0
//----ʹ��/��ֹ����-------------------------------------------------------------
//����: ���ô���Ϊʹ�ܻ��ֹ.
//������hwnd:���ھ��.
//      bEnable: TURE:ʹ�ܴ���; FALSE:��ֹ����.
//���أ�TRUE:�ɹ�;FALSE:ʧ��.
//------------------------------------------------------------------------------
BOOL    EnableWindow(HWND hwnd,bool_t bEnable)
{
    return  FALSE;
}
#endif

//----��ô��ڻ�ͼ������(DC)-----------------------------------------------------
//����: �ú�����õ�DC,�������������ڷ�Χ�ڻ�ͼ���(�ǿͻ���+�ͻ���).
//������hwnd:���ھ��.
//���أ�DC���.
//------------------------------------------------------------------------------
HDC GetWindowDC(HWND hwnd)
{
    DC *pdc;

    pdc =malloc(sizeof(DC));
    if(pdc!=NULL)
    {
        _InitDC(pdc,hwnd->pGkWin,hwnd,DC_TYPE_WINDOW);
    }
    return DC2HDC(pdc);

}

//----��ô��ڿͻ�����ͼ������(DC)-----------------------------------------------
//����: �ú�����õ�DC,ֻ���ڴ��ڿͻ�����Χ�ڻ�ͼ���).
//������hwnd:���ھ��.
//���أ�DC���.
//------------------------------------------------------------------------------
HDC GetDC(HWND hwnd)
{
    DC *pdc;

    pdc =malloc(sizeof(DC));
    if(pdc!=NULL)
    {
        _InitDC(pdc,hwnd->pGkWin,hwnd,DC_TYPE_CLIENT);
    }
    return DC2HDC(pdc);

}

//----��ô��ڿͻ�����ͼ������(DC)-----------------------------------------------
//����: �ú�����õ�DC,ֻ���ڴ��ڿͻ�����Χ�ڻ�ͼ���).
//������hwnd:���ھ��.
//���أ�DC���.
//------------------------------------------------------------------------------
BOOL    ReleaseDC(HWND hwnd,HDC hdc)
{
    return  DeleteDC(hdc);
}

//----ָ�����ڿ�ʼ��ͼ-----------------------------------------------------------
//����: �ú���ֻ����MSG_PAINT�е���,������EndPaint�ɶ�ʹ��.
//������hwnd: ���ھ��.
//���أ�DC���.
//------------------------------------------------------------------------------
HDC BeginPaint(HWND hwnd)
{
    DC *pdc;

    pdc =malloc(sizeof(DC));
    if(pdc!=NULL)
    {
        _InitDC(pdc,hwnd->pGkWin,hwnd,DC_TYPE_PAINT);
    }
    return DC2HDC(pdc);
}

//----ָ�����ڽ�����ͼ-----------------------------------------------------------
//����: �ú���ֻ����MSG_PAINT�е���,������BeginPaint�ɶ�ʹ��.
//������hwnd: ���ھ��.
//      hdc: DC���,������BeginPaint���ص�DC���.
//���أ�TRUE:�ɹ�; FALSE:ʧ��.
//------------------------------------------------------------------------------
BOOL    EndPaint(HWND hwnd,HDC hdc)
{
    UpdateDisplay();

    return  DeleteDC(hdc);
}

//----Ĭ�ϵĴ��ڷǿͻ���������Ϣ������------------------------------------------
//����: �ú���Ϊ�ڲ�����,��DefWindowPorc��MSG_NCPAINT�б�����.
//������pMsg: ��Ϣָ��.
//���أ���.
//------------------------------------------------------------------------------
static void DefWindowProc_NCPAINT(MSG *pMsg)
{
    HWND hwnd=pMsg->hwnd;
    HDC hdc;
    RECT rc;

    if(HWND_Lock(hwnd))
    {
        hdc =GetWindowDC(hwnd);
        if(NULL!=hdc)
        {
            _GetWindowRect(hwnd,&rc);
            _ScreenToWindow(hwnd,(POINT*)&rc,2);

            if(hwnd->Style&WS_BORDER)
            {
                SetDrawColor(hdc,WINDOW_BORDER_COLOR);
                DrawRect(hdc,&rc);
                InflateRect(&rc,-1,-1);
            }

            if(hwnd->Style&WS_DLGFRAME)
            {
                SetDrawColor(hdc,WINDOW_DLGFRAME_COLOR1);
                DrawRect(hdc,&rc);
                InflateRect(&rc,-1,-1);

                SetDrawColor(hdc,WINDOW_DLGFRAME_COLOR2);
                DrawRect(hdc,&rc);
                InflateRect(&rc,-1,-1);

                SetDrawColor(hdc,WINDOW_DLGFRAME_COLOR3);
                DrawRect(hdc,&rc);
                InflateRect(&rc,-1,-1);

            }

            if(hwnd->Style&WS_CAPTION)
            {
                rc.bottom =rc.top+hwnd->CaptionSize;

                GradientFillRect(hdc,&rc,RGB(180,180,200),RGB(0,0,180),GFILL_U_D);

                SetTextColor(hdc,WINDOW_CAPTION_TEXT_COLOR);
                InflateRect(&rc,-1,-1);
                DrawText(hdc,hwnd->Text,-1,&rc,DT_LEFT|DT_VCENTER);
            }

            ReleaseDC(hwnd,hdc);
        }

        HWND_Unlock(hwnd);
    }


}

//----Ĭ�ϵĴ�����Ϣ������-----------------------------------------------------
//����: �û�������Ȥ����Ϣ,�ɸú�������(ϵͳ�����Ὣ������Ϣ������).
//������pMsg: ��Ϣָ��.
//���أ���Ϣ������.
//------------------------------------------------------------------------------
u32 DefWindowProc(MSG *pMsg)
{

    switch(pMsg->Code)
    {
        case    MSG_NCPAINT:
                DefWindowProc_NCPAINT(pMsg);
                break;
                ////
        case    MSG_CLOSE:
                DestroyWindow(pMsg->hwnd);
                return 1;
                ////
        case    MSG_DESTROY:
                PostQuitMessage(pMsg->hwnd,0);
                return 1;

        default:
                break;
    }

    return 0;
}

//----GDD����ģ���ʼ������------------------------------------------------------
//����: �ú���Ϊ�ڲ�����.
//��������
//���أ���
//------------------------------------------------------------------------------
void    GDD_WindowInit(void)
{
    HWND_Desktop =NULL;
    HWND_Focus   =NULL;
}

//----Ĭ�ϵ����洰�ڹ��̻ص�����------------------------------------------------------
//����: �ú���Ϊ�ڲ�����.
//��������Ϣָ��.
//���أ���Ϣ������.
//------------------------------------------------------------------------------

static HWND wnd=NULL;
static  u32 desktop_proc(MSG *msg)
{
	RECT rc;
    HWND hwnd=msg->hwnd;
    switch(msg->Code)
    {
        case    MSG_CREATE:
                wnd=NULL;
           //     printf("Desktop: MSG_CREATE: %08XH.\r\n",hwnd);
                GetClientRect(hwnd,&rc);
             //   GDD_CreateTimer(hwnd,1122,1000,TMR_START);  //ѭ����ʱ

                break;
                ////
        case    MSG_LBUTTON_DOWN:
         //       printf("Desktop: MSG_LBUTTON_DOWN: %d,%d\r\n",LO16(msg->Param2),HI16(msg->Param2));
                break;
                ////
        case    MSG_LBUTTON_UP:
          //      printf("Desktop: MSG_LBUTTON_UP: %d,%d.\r\n",LO16(msg->Param2),HI16(msg->Param2));
                break;
                ////
        case    MSG_MOUSE_MOVE:
          //      printf("Desktop: MSG_MOUSE_MOVE: %08XH,%d,%d.\r\n",msg->Param1,LO16(msg->Param2),HI16(msg->Param2));
                break;
                ////
        /*
        case    MSG_GET_POS:
                printf("get_pos:%d\r\n",msg->Param1);
                return 0x1234;
                break;
        */
                /////

        case    MSG_TIMER:
                {
                    if(msg->Param1==1)
                    {
                       // InvalidateWindow(hwnd);
                        //PostMessage(hwnd,MSG_PAINT,0,0);
                    }
                }
                break;
                /////

        case    MSG_PAINT:
                {


                    HDC hdc;
                    RECT rc;


                    hdc =BeginPaint(hwnd);

                    GetClientRect(hwnd,&rc);

                    //SetFillColor(hdc,RGB(50,100,200));
                    //FillRect(hdc,&rc);

                    GradientFillRect(hdc,&rc,RGB(120,120,255),RGB(20,20,80),GFILL_U_D);

                    EndPaint(hwnd,hdc);
                }
                break;
        default:
                return DefWindowProc(msg);
    }

    return  0;
}


//----GDD������-----------------------------------------------------------------
//����: GDD����ִ�к���,�ú������᷵��.
//������dev_name: ��ʾ�豸��
//���أ���
//------------------------------------------------------------------------------

void    GDD_TimerExecu(u32 tick_time_ms);

void    GDD_Execu(struct tagGkWinRsc *desktop)
{
    HWND hwnd;
    MSG msg;
    int w,h;

    pGkWinDesktop=desktop;

    w=pGkWinDesktop->right-pGkWinDesktop->left;
    h=pGkWinDesktop->bottom-pGkWinDesktop->top;


    hwnd =CreateDesktop(desktop_proc,"Desktop",
                        0,0,w,h,
                        NULL);

    ShowWindow(hwnd,TRUE);

    while(1)
    {

        if(PeekMessage(&msg,hwnd))
        {
            DispatchMessage(&msg);
        }
        else
        {
            Djy_EventDelay(mS*50);
            GDD_TimerExecu(GUI_GetTickMS());
          //  TouchScreenExecu();
        }
    }
}

//*============================================================================*/




