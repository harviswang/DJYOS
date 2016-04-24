// =======================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// ģ������: VNCServer
// ģ��汾: V1.00(��ʼ���汾)
// �ļ���  ��vncserverapi.c
// ������Ա: Administrator
// ����ʱ��: 2013-3-5
//           ���ļ�ʵ��VNCSERVER����ҪAPI����
// =======================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <V1.00> <2013-3-5>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =======================================================================
#include  "vncserver_event.h"

#include  "rfb.h"
#include  "debug.h"
#include  "vnc_socket.h"//��������Ĵ�����

#include "vnc_image_update.h"
#include "vnc_thread.h"

//Ĭ�ϵľ�����ʾ���Ĵ�С�����ڿ����ǲ���ʵ�ģ���Ϊ������ʾ��ֻ����Ϊ������ʾ��������ʾ������
#define  SCREENWIDTH   640
#define  SCREENHEIGHT  240

void rfbDefaultScreenInit(rfbScreenInfoPtr rfbserver)
{
   XMutexInit(&(rfbserver->cl_mutex), "ClientMutex");
   XMutexInit(&(rfbserver->frame_mutex), "ClipMutex");
   //Ŀǰֻ����TCP���������⣬����Ĳ�����ĵģ�UDP,HTTP�ȣ�
   rfbserver->rfbPort=5900;
   rfbserver->socketInitDone=FALSE;
   rfbserver->maxFd=0;
   rfbserver->rfbListenSock=-1;

   rfbserver->desktopName = "LibVNCServer";
   rfbserver->rfbAlwaysShared = FALSE;
   rfbserver->rfbNeverShared = FALSE;

    gethostname(rfbserver->rfbThisHost, 255);

   //���ͻ��˶��г�ʼ��Ϊ�գ�
   rfbserver->rfbClientHead=NULL;
   rfbserver->cl_number=0;

   //������ʾ��Ϊ��
   rfbserver->mirror_display = NULL;
   rfbserver->mirror_display_enable = 0;

   //��ʼ�����¼������¼���״̬
   rfbserver->mainevevent_tobedead = 0;
   rfbserver->updateevent_end = 0;
   rfbserver->netevent_end = 0;
}

// =========================================================================
// �������ܣ�����ͼ�����ݸ��¸��ͻ���
// �������������������rfbScreen
// ���������
// ����ֵ  ��
// ˵��    ��
// =========================================================================
void   ProcessFrameUpdateThread()
{
//  rfbserver->event_id_update = Djy_MyEventId();
//  rfbserver->event_update_prio = djy_get_event_prio();
    while(1)
    {
//     Djy_WaitEvttPop(rfbserver->event_id_update,NULL,CN_TIMEOUT_FOREVER);
//      Djy_SetEventPrio(rfbserver->event_update_prio);
       delay(50);
       XMutexWait(&rfbserver->cl_mutex);
       //ʵ��û��ʲô�����ģ���Ϊ�������ñ��˵�
       if((rfbserver->updateevent_end == 1)||(rfbserver->mainevevent_tobedead == 1))
       {
           XMutexPost(&rfbserver->cl_mutex);
           break;
       }
       UpdateFrame2Clist(rfbserver);

       XMutexPost(&rfbserver->cl_mutex);
    }
//��Ȼ��ʧ
    rfbserver->updateevent_end = 1;
}
// =========================================================================
// �������ܣ������¿ͻ��˵����Ӻ�����ͨ��
// �������������������rfbserver
// ���������
// ����ֵ  ��
// ˵��    ��
// =========================================================================
void   ProcessMediaInterfaceThread()
{
    rfbClientPtr cl,temp;
    rfbInitSockets(rfbserver);
//  rfbserver->event_id_net = Djy_MyEventId();
    printf("--the server socket Init end!\n");
      while(1)//ѭ�����
      {
          delay(10);
          XMutexWait(&rfbserver->cl_mutex);

          //���¼�����ͼ�θ����¼�����һ��ʧ�ܣ������˳�
          if((rfbserver->mainevevent_tobedead == 1)||(rfbserver->updateevent_end == 1))
          {
              //�ͻ����б��socket�Ĵ���
              cl = rfbserver->rfbClientHead;
              while(cl)
              {
                  temp = cl->next;
                  rfbCloseClient(cl);
                  cl = temp;
              }
              //��������socket����,��ʱ��������DJYOS�汾��TCPIP��̨

              //�ͷ������˳�ѭ��
              XMutexPost(&rfbserver->cl_mutex);
              break;
          }

          rfbCheckFds(rfbserver,4000);

          XMutexPost(&rfbserver->cl_mutex);

      }
//�¼�����
      rfbserver->netevent_end = 1;
}
