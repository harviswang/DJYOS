// =======================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// ģ������: VNCServer
// ģ��汾: V1.00(��ʼ���汾)
// �ļ���  ��vnc_socket.h
// ������Ա: Administrator
// ����ʱ��: 2013-3-5
// =======================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <V1.00> <2013-3-5>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =======================================================================

#ifndef VNC_SOCKET_H_
#define VNC_SOCKET_H_

#include "rfb.h"

//��ʼ���������Ķ˿ڣ���������listen״̬
void rfbInitSockets(rfbScreenInfoPtr rfbScreen);

//�Է������Ķ˿ڽ��м�⣬���Ƿ����µ�����
void rfbCheckFds(rfbScreenInfoPtr rfbScreen,long usec);

//����len���ֳ����ͻ��ˣ�һ�η�����෢����
int WriteExact(rfbClientPtr cl, char *buf, int len);

//�ӿͻ��˶�ȡn���ֳ���һ�ζ�����������
int ReadExact(rfbClientPtr cl, char *buf, int len);

//�رտͻ��ˣ���ɹرտͻ��˺��һЩ�ƺ���
void rfbCloseClient(rfbClientPtr cl);

#endif /* SOCKET_H_ */
