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
//�ļ�����: GDD����ͷ�ļ�(��GDD�ں˼��û�ʹ��)
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2011-11-10
//   ����:  LiuWei.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __GDD_H__
#define __GDD_H__

#if __cplusplus
extern "C" {
#endif

#include "stddef.h"
#include "gkernel.h"

#ifndef FALSE
#define FALSE   0
#endif

#ifndef TRUE
#define TRUE    1
#endif

#ifndef MIN
#define MIN(a,b) (a)<(b)?(a):(b)
#endif

#ifndef MAX
#define MAX(a,b) (a)>(b)?(a):(b)
#endif

#ifndef ABS
#define ABS(a) (a)>0?(a):(-a)
#endif

#define MS2US(ms)   (ms*1000)
#define LO16(a)     (a&0xFFFF)
#define HI16(a)     ((a>>16)&0xFFFF)

#define RGB(r,g,b)  ((u8)r<<16)|((u8)g<<8)|((u8)b)

typedef bool_t  BOOL;
typedef wchar_t WCHAR;
typedef const WCHAR* LPCWSTR;

typedef struct  tagWINDOW*  HWND;
typedef struct  tagDC*      HDC;
typedef struct 	tagFontRsc*    	HFONT;
typedef struct  tagTIMER    TIMER;

typedef struct tagPointCdn      POINT;
typedef struct tagRectangle     RECT;
typedef struct tagRectBitmap    BITMAP;

/*============================================================================*/
//// ��Ϣ���ݽṹ
typedef struct  tagMSG
{
    HWND hwnd;      //��Ϣ��������.
    u32 Code;       //��Ϣ����.
    u32 Param1;     //��Ϣ����1,��ͬ��Ϣ����,���岻ͬ.
    ptu32_t Param2;     //��Ϣ����2,��ͬ��Ϣ����,���岻ͬ.
    const void* ExData;     //��Ϣ��չ����

}MSG;

typedef u32 WNDPROC(MSG *pMsg);
typedef u32 *PFN_WNDPROC(MSG *pMsg);

/*============================================================================*/
//������Flag
#define	PBF_ORG_LEFT	(0<<0)
#define	PBF_ORG_RIGHT	(1<<0)
#define	PBF_ORG_TOP		PBF_ORG_LEFT
#define	PBF_ORG_BOTTOM	PBF_ORG_RIGHT
#define	PBF_SHOWTEXT	(1<<1)

//���������ݽṹ
typedef struct
{
	u32 Flag;
	u32 Range;
	u32 Pos;
	u32 FGColor;
	u32 BGColor;
	u32 TextColor;
	u32 DrawTextFlag;

}PROGRESSBAR_DATA;

/*============================================================================*/
//// DrawText flag
#define DT_VCENTER      (0<<0)  //���Ĵ�ֱ����.
#define DT_TOP          (1<<0)  //���Ķ��˶���.
#define DT_BOTTOM       (2<<0)  //���ĵײ�����.
#define DT_ALIGN_V_MASK (3<<0)

#define DT_CENTER       (0<<2)  //ʹ�����ھ�����ˮƽ����.
#define DT_LEFT         (1<<2)  //���������.
#define DT_RIGHT        (2<<2)  //�����Ҷ���.
#define DT_ALIGN_H_MASK (3<<2)

#define DT_BORDER       (1<<4)  //���Ʊ߿�(ʹ��DrawColor)
#define DT_BKGND        (1<<5)  //��䱳��(ʹ��FillColor)

//// �������ģʽ
#define GFILL_L_R   0   //���������
#define GFILL_U_D   1   //���ϵ���
#define GFILL_LU_RD 2   //�����ϵ�����
#define GFILL_RU_LD 3   //�����ϵ�����

//// ϵͳ�����Ĵ�����
u32 button_proc(MSG *pMsg);
u32 checkbox_proc(MSG *pMsg);
u32 progressbar_proc(MSG *pMsg);
u32 listbox_proc(MSG *pMsg);

#define BUTTON      (WNDPROC*)(button_proc) //��ť
#define RADIOBOX    (WNDPROC*)(0x0002)  //��ѡ��
#define CHECKBOX    (WNDPROC*)(checkbox_proc)   //��ѡ��
#define GROUPBOX    (WNDPROC*)(0x0004)  //��Ͽ�
#define SLIDER      (WNDPROC*)(0x0005)  //������
#define SCROLL      (WNDPROC*)(0x0006)  //����
#define PROGRESSBAR (WNDPROC*)(progressbar_proc) //������
#define	LISTBOX		(WNDPROC*)(listbox_proc)	//�б��

//// ��갴��״̬
#define MK_RBUTTON  (1<<0)
#define MK_MBUTTON  (1<<1)
#define MK_LBUTTON  (1<<2)

//// �����¼�����
typedef enum{
	KEY_EVENT_DOWN =1,	//��������.
	KEY_EVENT_UP   =2,	//��������.
}EN_GDD_KEY_EVENT;

/*============================================================================*/
//// ͨ�ô�����Ϣ����
#define MSG_CREATE              0x0002  //���ڴ�����Ϣ; Param1:��WindowCreate��pdata����;Param2:����.
#define MSG_NCPAINT             0x0003  //���ڷǿͻ�������; Param1:����; Param2:����.
#define MSG_PAINT               0x0004  //���ڿͻ�������; Param1:����; Param2:����.
#define MSG_TIMER               0x0005  //��ʱ����Ϣ: Param1:��ʱId; Param2:��ʱ������.
#define MSG_SETTEXT             0x0006  //���ô�������
#define MSG_GETTEXT             0x0007  //��ô�������
#define MSG_GETTEXTFLENGTH      0x0008  //��ô������ֳ���
#define MSG_CLOSE               0x0009  //���ڹر���Ϣ
#define MSG_DESTROY             0x000A  //����������Ϣ
#define MSG_QUIT                0x000B  //�����˳���Ϣ
#define MSG_NOTIFY              0x000C  //֪ͨ��Ϣ; Param1:�ؼ�Id(L16),֪ͨ��(H16); Param2:�ؼ�HWND.
#define	MSG_SETFOCUS			0x000D	//���ڻ�ý���; Param1:����; Param2:����;
#define	MSG_KILLFOCUS			0x000E	//����ʧȥ����; Param1:����; Param2:����;
#define MSG_LBUTTON_DOWN        0x0100  //����������; Param1:����; Param2:x����(L16),y����(H16).
#define MSG_LBUTTON_UP          0x0101  //����������; Param1:����; Param2:x����(L16),y����(H16).
#define MSG_RBUTTON_DOWN        0x0102  //����Ҽ�����; Param1:����; Param2:x����(L16),y����(H16).
#define MSG_RBUTTON_UP          0x0103  //����Ҽ�����; Param1:����; Param2:x����(L16),y����(H16).
#define MSG_MBUTTON_DOWN        0x0104  //����м�����; Param1:����; Param2:x����(L16),y����(H16).
#define MSG_MBUTTON_UP          0x0105  //����м�����; Param1:����; Param2:x����(L16),y����(H16).
#define MSG_MOUSE_MOVE          0x0106  //����ƶ�; Param1:��갴��״̬; Param2:x����(L16),y����(H16).
#define MSG_NCLBUTTON_DOWN      0x0107  //�ǿͻ�������������; Param1:����; Param2:x����(L16),y����(H16).
#define MSG_NCLBUTTON_UP        0x0108  //�ǿͻ�������������; Param1:����; Param2:x����(L16),y����(H16).
#define MSG_NCRBUTTON_DOWN      0x0109  //�ǿͻ�������Ҽ�����; Param1:����; Param2:x����(L16),y����(H16).
#define MSG_NCRBUTTON_UP        0x010A  //�ǿͻ�������Ҽ�����; Param1:����; Param2:x����(L16),y����(H16).
#define MSG_NCMBUTTON_DOWN      0x010B  //�ǿͻ�������м�����; Param1:����; Param2:x����(L16),y����(H16).
#define MSG_NCMBUTTON_UP        0x010C  //�ǿͻ�������м�����; Param1:����; Param2:x����(L16),y����(H16).
#define MSG_NCMOUSE_MOVE        0x010D  //�ǿͻ�������ƶ�; Param1:��갴��״̬; Param2:x����(L16),y����(H16).

#define MSG_KEY_DOWN            0x0120  //���̰���; Param1:����ֵ(L16); Param2:�¼�������ʱ��(���뵥λ).
#define MSG_KEY_UP              0x0121  //���̵���; Param1:����ֵ(L16); Param2:�¼�������ʱ��(���뵥λ).

//#define   MSG_GET_POS         0xF000

//// ��������Ϣ
#define	PBM_SETDATA		0x2100
#define	PBM_GETDATA		0x2101
#define	PBM_SETRANGE	0x2102	//��������: Param1:����ֵ; Param2:����; ����:����.
#define	PBM_GETRANGE	0x2103	//�������: Param1:����; Parame2:����; ����:����ֵ.
#define	PBM_SETPOS		0x2104	//���õ�ǰλ��: Param1:��ǰλ��; Param2:����; ����:����.
#define	PBM_GETPOS		0x2105	//��õ�ǰλ��: Param1:����; Param1:����; ����:��ǰλ��.

//// ListBox��Ϣ.
#define	LBM_ADDSTRING		0x2200	//����һ���ַ���; Param1:��Ŀ����; Param2:�ַ�ָ��; ����:ʵ����Ŀ����.
#define	LBM_DELSTRING		0x2201	//ɾ��һ���ַ���; Param1:��Ŀ����; Param2:����; ����:����.
#define	LBM_SETCURSEL		0x2202	//���õ�ǰѡ����; Param1:��Ŀ����; Param2:����; ����:����.
#define	LBM_GETCURSEL		0x2203	//��õ�ǰѡ����; Param1:����; Param2:����; ����:��ǰѡ��������.
#define	LBM_SETTOPINDEX		0x2204	//���ö����׸��ɼ���; Param1:��Ŀ����; Param2:����; ����:����.
#define	LBM_GETTOPINDEX		0x2205	//��ö����׸��ɼ���; Param1:����; Param2:����; ����:�����׸��ɼ�������.
#define	LBM_GETCOUNT		0x2206	//��������б�����Ŀ����; Param1:����; Param2:����; ����:��Ŀ����.
#define	LBM_RESETCONTENT	0x2207	//ɾ���б��������Ŀ; Param1:����; Param2:����; ����:����.
#define LBM_GETTEXTLEN      0x2208	//���ָ����Ŀ�������ֽ���; Param1:��Ŀ����; Param2:����; ����:����.
#define LBM_GETTEXT         0x2209	//���ָ����Ŀ������; Param1:��Ŀ����; Param2:���������; ����:����.
#define	LBM_SETITEMHEIGHT	0x220A	//�����б����Ŀ�߶�; Param1:�߶�ֵ(���ص�λ); Param2:����; ����:����.
#define	LBM_GETITEMHEIGHT	0x220B	//����б����Ŀ�߶�; Param1:����; Param2:����; ����:�߶�ֵ(���ص�λ).
#define	LBM_SETITEMDATA		0x220C	//�����б����Ŀ����; Param1:��Ŀ����; Param2:��Ŀ��ֵ; ����:����.
#define	LBM_GETITEMDATA		0x220D	//����б����Ŀ����; Param1:��Ŀ����; Param2:����; ����:��Ŀ��ֵ.

/*============================================================================*/

//Button�ؼ�֪ͨ��
#define BTN_DOWN    1    //����
#define BTN_UP      2    //����

//Checkbox�ؼ�֪ͨ��
#define CBN_SELECTED    1    //��ѡ��
#define CBN_UNSELECTED  2    //δ��ѡ��

//Listbox�ؼ�֪ͨ��
#define	LBN_SELCHANGE	1	//��ǰѡ����Ŀ���ı�

/*============================================================================*/
//// ���ڹ������
#define WS_CHILD    (1<<16) //�Ӵ��ڱ�־,�ؼ����ڱ���ָ���ñ�־.
#define WS_VISIBLE  (1<<17) //�����Ƿ�ɼ�
#define WS_DISABLE  (1<<18) //�����Ƿ�Ϊ��ֹ״̬,���ָ���ñ�־,���ڽ�����Ӧ������Ϣ
#define WS_BORDER   (1<<19) //�����Ƿ��б߿�
#define WS_DLGFRAME (1<<20) //�����Ƿ��жԻ��߿�
#define WS_CAPTION  (1<<21) //�����Ƿ��б�����
#define WS_SYSMENU  (1<<22) //ָ���ñ�־ʱ,������������"�ر�"��ť,���ڱ���ָ���� WS_CAPTION��־ʱ,����Ч.

#define WS_MAIN_WINDOW  (WS_BORDER|WS_DLGFRAME|WS_CAPTION|WS_SYSMENU)

/*============================================================================*/
//// �ؼ�˽�з��

// ��ť���
#define BS_TYPE_MASK        (3<<0)  //��������
#define BS_NORMAL           (0<<0)  //���水ť
#define BS_HOLD             (1<<0)  //������ť
#define BS_RADIO            (2<<0)  //��ѡ��ť

#define BS_SURFACE_MASK     (3<<2)  //�������
#define BS_NICE             (0<<2)  //���۷��
#define BS_SIMPLE           (1<<2)  //���ӷ��
#define BS_FLAT             (2<<2)  //ƽ����

#define BS_PUSHED           (1<<4)  //��ť����

// ��ѡ����
#define CBS_SELECTED        (1<<1)  //��ѡ��ѡ��

// ���������
#define	PBS_HOR		(0<<0)	//ˮƽ������
#define	PBS_VER		(1<<0)	//��ֱ������
#define	PBS_FLAT 	(1<<1)	//ƽ����

// �б����
#define	LBS_FLAT	(1<<0)	//ƽ����

/*============================================================================*/
//// GetWindow nCmd����
#define GW_CHILD        0   //����Z�򶥶˵��Ӵ���.
#define GW_HWNDPREV     1   //����Z����һ��ͬ������.
#define GW_HWNDNEXT     2   //����Z����һ��ͬ������.
#define GW_HWNDFIRST    3   //����Z����߶�ͬ������.
#define GW_HWNDLAST     4   //����z����Ͷ�ͬ������.

//// ��ʱ�����
#define TMR_START       (1<<0)
#define TMR_SINGLE      (1<<1)

//// ������ģʽ
typedef enum{
    PBM_LEFT = 0,   //��������
    PBM_RIGHT = 1,  //������ұ�
    PBM_TOP = 2,    //������ϱ�
    PBM_BOTTOM = 3, //����ڵױ�

}EN_PB_MODE;

/*============================================================================*/


HDC     CreateDC(struct tagGkWinRsc *gk_win,const RECT *prc);
BOOL    DeleteDC(HDC hdc);

u32     SetRopCode(HDC hdc,u32 rop_code);
u32     GetRopCode(HDC hdc);
void    MoveTo(HDC hdc,int x,int y,POINT *old_pt);

u32 SetDrawColor(HDC hdc,u32 color);
u32 GetDrawColor(HDC hdc);
u32 SetFillColor(HDC hdc,u32 color);
u32 GetFillColor(HDC hdc);
u32 SetTextColor(HDC hdc,u32 color);
u32 GetTextColor(HDC hdc);
u32 SetSyncTime(HDC hdc,u32 sync_time);
u32 GetSyncTime(HDC hdc);

HFONT   SetFont(HDC hdc,HFONT hFont);
HFONT   GetFont(HDC hdc);


void    SetPixel(HDC hdc,int x,int y,u32 color);
void    DrawLine(HDC hdc,int x0,int y0,int x1,int y1);
void    DrawLineTo(HDC hdc,int x,int y);
BOOL    TextOut(HDC hdc,int x,int y,const char *text,int count);
BOOL    DrawText(HDC hdc,const char *text,int count,const RECT *prc,u32 flag);
void    DrawRect(HDC hdc,const RECT *prc);;
void    FillRect(HDC hdc,const RECT *prc);
void    GradientFillRect(HDC hdc,const RECT *prc,u32 Color1,u32 Color2,u32 mode);

void    DrawCircle(HDC hdc,int cx,int cy,int r);
void    FillCircle(HDC hdc,int cx,int cy,int r);
void    DrawPolyLine(HDC hdc,const POINT *pt,int count);
void    DrawGroupBox(HDC hdc,const RECT *prc,const char *Text);

u32     DispatchMessage(MSG *pMsg);
u32     SendMessage(HWND hwnd,u32 msg,u32 param1,ptu32_t param2);
BOOL    PostMessage(HWND hwnd,u32 msg,u32 param1,ptu32_t param2);
BOOL    PostQuitMessage(HWND hwnd,u32 exit_code);
BOOL    PeekMessage(MSG *pMsg,HWND hwnd);
BOOL    GetMessage(MSG *pMsg,HWND hwnd);

BOOL    ScreenToClient(HWND hwnd,POINT *pt,s32 count);
BOOL    ClientToScreen(HWND hwnd,POINT *pt,s32 count);
BOOL    ScreenToWindow(HWND hwnd,POINT *pt,s32 count);
BOOL    WindowToScreen(HWND hwnd,POINT *pt,s32 count);

HWND    GetDesktopWindow(void);
BOOL    GetWindowRect(HWND hwnd,RECT *prc);
BOOL    GetClientRect(HWND hwnd,RECT *prc);
BOOL    GetClientRectToScreen(HWND hwnd,RECT *prc);
u32		GetWindowStyle(HWND hwnd);
void* 	GetWindowPrivateData(HWND hwnd);
void 	SetWindowPrivateData(HWND hwnd,void *data);
void* 	GetWindowUserData(HWND hwnd);
void 	SetWindowUserData(HWND hwnd,void *data);

HDC     GetWindowDC(HWND hwnd);
HDC     GetDC(HWND hwnd);
BOOL    ReleaseDC(HWND hwnd,HDC hdc);
HDC     BeginPaint(HWND hwnd);
BOOL    EndPaint(HWND hwnd,HDC hdc);

HWND    CreateWindow(WNDPROC *pfWinProc,const char *Text,u32 Style,s32 x,s32 y,s32 w,s32 h,HWND Parent,u32 WinId,const void *pdata);
void    DestroyWindow(HWND hwnd);

BOOL    MoveWindow(HWND hwnd,int x,int y);
BOOL    OffsetWindow(HWND hwnd,int dx,int dy);
BOOL    IsWindowVisible(HWND hwnd);
BOOL    InvalidateWindow(HWND hwnd);
BOOL    ShowWindow(HWND hwnd,BOOL bShow);
BOOL    EnableWindow(HWND hwnd,BOOL bEnable);
HWND    GetParent(HWND hwnd);
HWND    GetWindow(HWND hwnd,int nCmd);
HWND    GetDlgItem(HWND hwnd,int id);
void	SetWindowText(HWND hwnd,const char *text,s32 max_len);
char* 	GetWindowText(HWND hwnd,char *buf,s32 max_len);

HWND    GetWindowFromPoint(POINT *pt);
HWND	SetFocusWindow(HWND hwnd);
HWND	GetFocusWindow(void);
BOOL	IsFocusWindow(HWND hwnd);

u32     DefWindowProc(MSG *pMsg);

TIMER*  GDD_CreateTimer(HWND hwnd,u16 Id,u32 IntervalMS,u16 Flag);
TIMER*  GDD_FindTimer(HWND hwnd,u16 Id);
BOOL    GDD_ResetTimer(TIMER *ptmr,u32 IntervalMS,u32 Flag);
BOOL    GDD_DeleteTimer(TIMER *ptmr);

int     RectW(const RECT *prc);
int     RectH(const RECT *prc);

void    SetRect(RECT *prc,int x,int y,int w,int h);
void    SetRectEmpty(RECT *prc);
BOOL    CopyRect(RECT *dst,const RECT *src);
BOOL    IsRectEmpty(const RECT *prc);
BOOL    OffsetRect(RECT *prc,int dx,int dy);
BOOL    InflateRect(RECT *prc,int dx,int dy);
BOOL    InflateRectEx(RECT *prc,int l,int t,int r,int b);
BOOL    PtInRect(const RECT *prc,const POINT *pt);
BOOL    MakeProgressRect(RECT *dst,const RECT *src,u32 Range,u32 Val,EN_PB_MODE mode);


/*========================================================================================*/
void    ModuleInstall_GDD(struct tagGkWinRsc *desktop,const char *InputDevName[]);
void    GDD_Execu(struct tagGkWinRsc *desktop);

void    MouseInput(s32 x,s32 y,u32 key_state);
void	KeyboardInput(u16 key_val,EN_GDD_KEY_EVENT key_event,u32 time);
u32     AlphaBlendColor(u32 bk_c,u32 fr_c,u8 alpha);
void    UpdateDisplay(void);

/*========================================================================================*/


#if __cplusplus
}
#endif
#endif  /*__GDD_H__*/
