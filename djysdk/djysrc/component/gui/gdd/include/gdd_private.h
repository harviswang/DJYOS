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
//�ļ�����: GDD�ں�˽��ͷ�ļ�
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2014-11-10
//   ����:  LiuWei.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __GDD_PRIVATE_H__
#define __GDD_PRIVATE_H__

#if __cplusplus
extern "C" {
#endif

#include    "stddef.h"
#include    "stdio.h"
#include    "stdlib.h"
#include    "string.h"
#include 	"math.h"

#include    "systime.h"
#include    "lock.h"
#include    "djyos.h"
#include	"stddev.h"
#include 	"gkernel.h"

//#include "gdd.h"

#ifndef INLINE
#define INLINE  inline
#endif

/*============================================================================*/

#define DEF_BORDER_SIZE     1   //Ĭ�ϵı߿��С
#define DEF_DLGFRAME_SIZE   3   //Ĭ�ϵĶԻ��߿��С
#define DEF_CAPTION_SIZE    28  //Ĭ�ϵı�������С

#define WINDOW_BORDER_COLOR         RGB(50,50,50)       //���ڱ߿���ɫ
#define WINDOW_DLGFRAME_COLOR1      RGB(80,80,80)       //���ڶԻ��߿���ɫ1
#define WINDOW_DLGFRAME_COLOR2      RGB(120,120,120)    //���ڶԻ��߿���ɫ2
#define WINDOW_DLGFRAME_COLOR3      RGB(90,90,90)       //���ڶԻ��߿���ɫ3
#define WINDOW_CAPTION_TEXT_COLOR   RGB(240,240,240)    //���ڱ�����������ɫ


/*============================================================================*/

#define INVALD_TAG  0x00000000
#define DC_TAG      (('H'<<24)|('D'<<16)|('C'<<8)|('.'))
#define WIN_TAG     (('H'<<24)|('W'<<16)|('N'<<8)|('D'))
#define TMR_TAG     (('H'<<24)|('T'<<16)|('M'<<8)|('R'))
#define MSGQ_TAG    (('M'<<24)|('S'<<16)|('G'<<8)|('Q'))


typedef struct  tagDC       DC;
typedef struct  tagMSGQ     MSGQ;
typedef struct  tagWINDOW   WINDOW;

/*============================================================================*/
struct _list_node
{
    struct _list_node *next;
    struct _list_node *prev;
};
typedef struct _list_node list_t;

#define list_entry(node, type, member) \
    ((type *)((char *)(node) - (unsigned long)(&((type *)0)->member)))

void list_init(list_t *l);
void list_insert_after(list_t *l, list_t *n);
void list_insert_before(list_t *l, list_t *n);
void list_remove(list_t *n);
int list_isempty(const list_t *l);

#if 0
int     WDD_wsprintf(WCHAR *buf,const WCHAR *fmt, ...);
int     WDD_wstrlen(const WCHAR *src);
WCHAR*  WDD_wstrncpy(WCHAR *dst,const WCHAR *src,int n);
int     WDD_wstrncmp(const WCHAR *s1,const WCHAR *s2,int n);
#endif

/*============================================================================*/

#define WDD_LOCK()
#define WDD_UNLOCK()

#define HDC2DC(hdc) (struct tagDC*)(hdc)
#define DC2HDC(dc)  (HDC)dc


/*============================================================================*/

#define DC_TYPE_PAINT   0
#define DC_TYPE_CLIENT  1
#define DC_TYPE_WINDOW  2

struct  tagDC
{
    struct tagGkWinRsc *pGkWin;
    struct tagFontRsc *pFontDef;   //Ĭ������,���ֵ�ڴ���DCʱָ��,���������ɸ���
    struct tagFontRsc *pFont;      //��ǰ����,���ֵ��DCʹ�ù�����,���Ա�����
    struct tagCharset *pCharset;   //��ǰ�ַ���
    HWND hwnd;          //DC��������
    u32 DCType;         //DC����
    s32 CurX,CurY;      //��ǰλ��
    u32 DrawColor;      //������ɫ
    u32 FillColor;      //�����ɫ
    u32 TextColor;      //������ɫ
    u32 SyncTime;       //ͬ��ʱ��
    u32 RopCode;        //��դ��

};


struct  tagWINDOW
{
    u32     Tag;
    char    Text[256];
    char    Name[32];
    struct tagMutexLCB *mutex_lock;      //���ڻ�����
    struct  tagWINDOW *Parent;  //������
    struct  tagWINDOW *Child;   //������
    struct  tagWINDOW *Prev;    //ǰһ������
    struct  tagWINDOW *Next;    //��һ������
    WNDPROC *WndProc;           //���ڹ��̻ص�
    MSGQ    *pMsgQ;             //������Ϣ����
    struct tagGkWinRsc *pGkWin; //�̳�GkWin
    void	*PrivateData;		//����˽������
    void	*UserData;			//�����û�����

    u16     EventID;       //���������¼�ID
    u16     WinId;          //����ID
    u16     rsv1;           //...
    u32     Style;          //���ڷ��
    u8      BorderSize;     //�߿��С
    u8      DlgFrameSize;   //�Ի��߿��С
    u8      CaptionSize;    //��������С
    s8      Zorder;         //...
    RECT    WinRect;        //���ھ�����(ʹ����Ļ����)
    RECT    CliRect;        //���ڿͻ���(ʹ����Ļ����)

    list_t  list_timer;         //���ڶ�ʱ������
    list_t  node_msg_close;     //����CLOSE��Ϣ����ڵ�
    list_t  node_msg_ncpaint;   //����NCPAINT��Ϣ����ڵ�
    list_t  node_msg_paint;     //����PAINT��Ϣ����ڵ�


};

struct  tagTIMER
{
    u32 Tag ;
    HWND hwnd;              //��ʱ����������.
    u16 Id;                 //��ʱ��Id.
    u16 Flag;               //��ʱ�����.
    u32 Interval;           //��ʱ���(��λ:����).
    u32 HoldTime;           //��ʱ��ʼʱ��(��λ:����).
    list_t  node_sys;       //ϵͳ��ʱ������ڵ�.
    list_t  node_hwnd;      //�������ڵĶ�ʱ������ڵ�.
    list_t  node_msg_timer; //TIMER��Ϣ�ڵ�,������MSG_TIMER��Ϣʱ,�ýڵ���뵽
                            //�������ڵ���Ϣ�����е�list_msg_timer��.
};

/*============================================================================*/
void    GDD_Init(void);

BOOL    GDD_Lock(void);
void    GDD_Unlock(void);
u32     GUI_GetTickMS(void);

void    _InitDC(DC *pdc,struct tagGkWinRsc *gk_win,HWND hwnd,int dc_type);
void    _SetRect(RECT *prc,int x,int y,int w,int h);
void    _CopyRect(RECT *dst,const RECT *src);
void    _OffsetRect(RECT *prc,int dx,int dy);
void    _InflateRect(RECT *prc,int dx,int dy);
void    _InflateRectEx(RECT *prc,int l,int t,int r,int b);
BOOL    _PtInRect(const RECT *prc,const POINT *pt);

BOOL    HWND_Lock(HWND hwnd);
void    HWND_Unlock(HWND hwnd);

BOOL    _PostMessage(MSGQ *pMsgQ,HWND hwnd,u32 msg,u32 param1,ptu32_t param2);
void    _PostTimerMessage(TIMER *ptmr);

void    _ScreenToClient(HWND hwnd,POINT *pt,s32 count);
void    _ClientToScreen(HWND hwnd,POINT *pt,s32 count);

BOOL    _IsWindowVisible(HWND hwnd);
void    _InvalidateWindow(HWND hwnd);
MSGQ*   GUI_CreateMsgQ(u8 size);
void    GUI_DeleteMsgQ(MSGQ *pMsgQ);
void    _RemoveWindowTimer(HWND hwnd);
void    _DeleteMainWindowData(HWND hwnd);

MSGQ*       _GetWindowMsgQ(HWND hwnd);
WNDPROC*    _GetWindowProc(HWND hwnd);
u32         _GetWindowEvent(HWND hwnd);
HWND    _GetDlgItem(HWND hwnd,int id);

/*============================================================================*/


#if __cplusplus
}
#endif
#endif  /*__WDD_PRIVATE_H__*/
