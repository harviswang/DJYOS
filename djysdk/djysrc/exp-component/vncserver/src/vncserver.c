// =======================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// ģ������: VNCServer
// ģ��汾: V1.00(��ʼ���汾)
// �ļ���     ��VNCSERVER.c
// ������Ա: Administrator
// ����ʱ��: 2013-3-8
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <V1.00> <2013-3-8>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
#include "rfb.h"
#include "vncserver_event.h"
#include "vnc_mirror_display.h"
#include "vnc_thread.h"//�������߳�
#include "debug.h"//���Ժ���
#include "vncserver.h"
#include "vnc_keyboard.h"
// =============================================================================
// �������ܣ�VNC������
// ���������
// ���������
// ����ֵ  ��
// ˵��    �����ö��̵߳���ʽʵ�ַ������Ĺ��ܣ�һ���������߳�
//          1,accept�߳�����ר�Ŵ����¿ͻ��˵�����
//          2,messsadge �߳�����ר�Ŵ���ͻ��˺ͷ���������Ϣ����
//          3,update�߳����ڴ���ͼ����¸��ͻ���
// =============================================================================

//Ӧ����ʽ�ĵ���malloc��free������Ӧ�ò���Ҵ�ţ���Ȼ�����׳����ڴ�й¶����

int vncserver_start()
{
    bool_t  ret;
  //���б�Ҫ�ĳ�ʼ��
    rfbserver=M_MallocLc(sizeof(rfbScreenInfo),0);
    if(NULL==rfbserver)
    {
        debug_printf("the main failed--rfbDefaultScreenInit\n");
        return -1;
    }
    rfbDefaultScreenInit(rfbserver);
  //��װ������ʾ��
    ret= install_vncserver_mirrordisplay(rfbserver);
    if(false==ret)
    {
        free(rfbserver);
        debug_printf("##failed to install the vnc mirror screen!\n");
        return -1;
    }
#if CN_CFG_KEYBOARD == 1
    vnc_keyboard_visual_init(rfbserver);
    install_vncserver_keyboard(rfbserver);
    install_vncserver_touchlcd(rfbserver);
#endif

    debug_printf("screen width=%d  height=%d\n",rfbserver->width,rfbserver->height);
//������������ͼ������
    ret=create_thread(ProcessFrameUpdateThread,rfbserver);
    if(ret!=0)
    {
      debug_printf("failed to create the frameUpdateThread thread!\n");
      goto pop_update_event_failed;
    }
  //�����������˿���?

    ret=create_thread(ProcessMediaInterfaceThread,rfbserver);
    if(ret!=0)
    {
        debug_printf("failed to create the event thread!\n");
        goto pop_net_event_failed;
    }

    //ʹ�ܾ�����ʾ��
    rfbserver->mirror_display_enable = 1;
    debug_printf("---------------server main ok--------------!\n");
    return  0;
//ʧ���Ժ�Ӧ�õȴ����߳̽���
pop_net_event_failed:
    rfbserver->netevent_end = 1;
pop_update_event_failed:
    rfbserver->mainevevent_tobedead = 1;

    vncserver_exit(rfbserver);
    debug_printf("---------------server main failed--------------!\n");
    return -1;
}

// =============================================================================
// �������ܣ�vnc�������˳�����
// ���������
// ���������
// ����ֵ  ��
// ˵��:��vncserver��ռ�õ���Դ�����ͷŹ���
// =============================================================================
void vncserver_exit(rfbScreenInfoPtr rfbserver)
{
    rfbserver->mainevevent_tobedead = 1;
//�ȴ����¼�������
    while(1)
    {
        delay(1000);
        if((rfbserver->updateevent_end == 1)&&(rfbserver->netevent_end == 1))
        {
            break;
        }
    }
//�����¼����Ѿ���������ʵ��û�����¼�����˵��,���linux������˵����
//ж����ʾ��
    uninstall_vncserver_mirrrordisplay(rfbserver);
//ж�������豸
#if CN_CFG_KEYBOARD == 1
    uninstall_vnc_keyboard();
    uninstall_vnc_touchboard();
#endif
//ɾ��������
    XMutexDelete(&(rfbserver->frame_mutex));
    XMutexDelete(&(rfbserver->cl_mutex));
//�ͷ�server�Ŀռ�
    free(rfbserver);
    rfbserver = NULL;
    debug_printf("The vncserver exit normally!\n");
}
