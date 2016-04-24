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

//����ģ��: gui
//����:  ������.
//�汾��V1.0.0
//�ļ�����: gui kernel���岿��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-11-21
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __GK_WIN_H__
#define __GK_WIN_H__
#include "errno.h"
#include "config-prj.h"
#include "ring.h"
#ifdef __cplusplus
extern "C" {
#endif

struct tagGkscParaCreateDesktop;
struct tagGkscParaCreateGkwin;
struct tagGkscParaSetRopCode;
struct tagGkscParaSetPatBuf;
struct tagGkscParaSetPrio;
struct tagGkscParaSetTransparentColor;
struct tagGkscParaSetBitmsk;
struct tagGkscParaDestroyWin;
struct tagGkscParaSetBoundMode;
struct tagGkscParaMoveWin;
struct tagGkscParaChangeWinArea;
struct tagGkscParaSetDirectScreen;
struct tagGkscParaUnsetDirectScreen;

struct tagDisplayRsc;

//��¼�����޸Ĳ���
#define CN_GKWIN_CHANGE_NONE    0   //û���޸�
#define CN_GKWIN_CHANGE_PART    1   //�����޸�
#define CN_GKWIN_CHANGE_ALL     2   //ȫ���޸�

struct tagGkChunnel       //�û�����(ͨ����gui windows) �� gui kernel��ͨ�Źܵ�
{
    struct tagRingBuf ring_syscall;         //gui ϵͳ���û��λ�����
    struct tagRingBuf ring_usercall;        //gui �û����û��ѻ�������
    u8 *syscall_buf;                        //Ϊϵͳ���û��λ����������ڴ�
    u8 *usercall_buf;                       //Ϊ�û����û��ѻ����������ڴ�
    struct tagMutexLCB *syscall_mutex;      //������,���ڷ�ֹ����д
    struct tagSemaphoreLCB *syscall_semp;   //�ź�����дʱ�������������Ⱥ�
//    struct tagMutexLCB *usercall_mutex;   //������,���ڷ�ֹ����д
                                            //usercall ringֻ��gkwin���߳�д
    struct tagSemaphoreLCB *usercall_semp;  //�ź�����дʱ�������������Ⱥ�
};

ptu32_t ModuleInstall_GK(ptu32_t para);
struct tagGkWinRsc *__GK_GetZsectionStart(struct tagGkWinRsc *gkwin);
struct tagGkWinRsc *__GK_GetZsectionEnd(struct tagGkWinRsc *gkwin);
struct tagGkWinRsc *__GK_GetZsectionAllStart(
                                                    struct tagGkWinRsc *gkwin);
struct tagGkWinRsc *__GK_GetZsectionAllEnd(
                                                    struct tagGkWinRsc *gkwin);
struct tagGkWinRsc *GK_CreateDesktop(struct tagGkscParaCreateDesktop *para);
struct tagGkWinRsc *GK_CreateWin(struct tagGkscParaCreateGkwin *para);
void GK_SetName(struct tagGkWinRsc *gkwin,char *name);
void GK_SetUnlock(struct tagGkWinRsc *gkwin);
void GK_SetLock(struct tagGkWinRsc *gkwin);
void GK_SetHide(struct tagGkWinRsc *gkwin);
void GK_SetShow(struct tagGkWinRsc *gkwin);
void GK_MoveWin(struct tagGkscParaMoveWin *para);
void __GK_SetBound(struct tagGkWinRsc *gkwin);
void GK_SetBoundMode(struct tagGkscParaSetBoundMode *para);
void GK_SetPrio(struct tagGkscParaSetPrio *para);
bool_t GK_SetRopCode(struct tagGkscParaSetRopCode *para);
bool_t GK_SetPatBuf(struct tagGkscParaSetPatBuf *para);
bool_t GK_SetTransparentColor(
                                struct tagGkscParaSetTransparentColor *para);
bool_t GK_SetBitMsk(struct tagGkscParaSetBitmsk *para);
void GK_SetDirectScreen(struct tagGkscParaSetDirectScreen *para);
void GK_UnSetDirectScreen(struct tagGkscParaUnsetDirectScreen *para);
void __GK_DestroyWinChild(struct tagGkWinRsc *gkwin);
void GK_DestroyWin(struct tagGkscParaDestroyWin *para);
bool_t GK_ChangeWinArea(struct tagGkscParaChangeWinArea *para);
void GK_SetCanvasColor(struct tagGkWinRsc *gkwin,u32 color);
void GK_GetCanvasColor(struct tagGkWinRsc *gkwin,u32 *color);
void __GK_OutputRedraw(struct tagDisplayRsc *display);
void __GK_OutputFrameBuffer(struct tagDisplayRsc *display);
u16 GK_SyscallChunnel(u16 command,u32 sync_time,void *param1,u16 size1,
                                                void *param2,u16 size2);
u16 GK_UsercallChunnel(u16 usercall_id,struct tagSemaphoreLCB *semp,
                        void **result,void *param,u16 size);
ptu32_t GK_Server(void);

#ifdef __cplusplus
}
#endif

#endif //__GK_WIN_H__

