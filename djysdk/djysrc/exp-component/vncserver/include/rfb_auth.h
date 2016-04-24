// =======================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// ģ������: VNCServer
// ģ��汾: V1.00(��ʼ���汾)
// �ļ���  : vnc_auth.h
// ������Ա: Administrator
// ����ʱ��: 2013-3-5
// =======================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <V1.00> <2013-3-5>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =======================================================================

#ifndef _VNC_AUTH_H_
#define _VNC_AUTH_H_

#include "rfb.h"

//������֤��Ϣ
void rfbAuthNewClient(rfbClientPtr cl);

void rfbVncAuthSendChallenge(rfbClientPtr cl);

void rfbClientSendString(rfbClientPtr cl, const char *reason);

#endif /* AUTH_H_ */
