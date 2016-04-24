// =======================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// ģ������: VNCServer
// ģ��汾: V1.00(��ʼ���汾)
// �ļ���  ��vnc_thread.h
// ������Ա: Administrator
// ����ʱ��: 2013-3-8
// =======================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <V1.00> <2013-3-8>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =======================================================================

#ifndef __VNC_THREAD_H__
#define __VNC_THREAD_H__

int  create_thread(void *(*start_routine)(void *),void *arg);


void  XMutexInit(struct tagMutexLCB *mutex, char  *name);
int  XMutexDelete(struct tagMutexLCB *mutex);
void  XMutexWait(struct tagMutexLCB *mutex);
void  XMutexPost(struct tagMutexLCB *mutex);

#endif /* __VNC_THREAD_H__ */
