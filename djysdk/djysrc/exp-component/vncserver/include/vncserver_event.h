// =======================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// ģ������: VNCServer
// ģ��汾: V1.00(��ʼ���汾)
// �ļ���     ��vncserverapi.h
// ������Ա: Administrator
// ����ʱ��: 2013-3-5
// =======================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <V1.00> <2013-3-5>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =======================================================================

#ifndef VNCSERVERAPI_H_
#define VNCSERVERAPI_H_

#include "rfb.h"
//Ϊscreen����ռ䲢���л����ĳ�ʼ��
void rfbDefaultScreenInit(rfbScreenInfoPtr rfbScreen);

void   ProcessMediaInterfaceThread();

void   ProcessFrameUpdateThread();

#endif /* VNCSERVERAPI_H_ */
