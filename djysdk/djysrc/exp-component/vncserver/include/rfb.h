// =======================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// ģ������: VNCServer//����server��client�Ľṹ
// ģ��汾: V1.00(��ʼ���汾)
// �ļ���     ��rfb.h
// ������Ա: Administrator
// ����ʱ��: 2013-3-5
// =======================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <V1.00> <2013-3-5>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =======================================================================

#ifndef _RFB_H_
#define _RFB_H_

//#include "misc.h"//�������ݽṹ���ض���
#include "rfbproto.h"  //����RFBЭ���һЩ����

/*
 *  Copyright (C) 2000, 2001 Const Kaplinsky.  All Rights Reserved.
 *  Copyright (C) 2000 Tridia Corporation.  All Rights Reserved.
 *  Copyright (C) 1999 AT&T Laboratories Cambridge.  All Rights Reserved.
 *
 *  This is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this software; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307,
 *  USA.
 */

/*
 * rfbproto.h - header file for the RFB protocol version 3.3
 *
 * Uses types CARD<n> for an n-bit unsigned integer, INT<n> for an n-bit signed
 * integer (for n = 8, 16 and 32).
 *
 * All multiple byte integers are in big endian (network) order (most
 * significant byte first).  Unless noted otherwise there is no special
 * alignment of protocol structures.
 *
 *
 * Once the initial handshaking is done, all messages start with a type byte,
 * (usually) followed by message-specific data.  The order of definitions in
 * this file is as follows:
 *
 *  (1) Structures used in several types of message.
 *  (2) Structures used in the initial handshaking.
 *  (3) Message types.
 *  (4) Encoding types.
 *  (5) For each message type, the form of the data following the type byte.
 *      Sometimes this is defined by a single structure but the more complex
 *      messages have to be explained by comments.
 */

 //DJY��һЩ����
#include "stdint.h"
#include "lock.h"
#include "vnc_define.h"

typedef struct _rect
{
  u16 x;
  u16 y;
  u16 w;
  u16 h;
}rect;

typedef void (*KbdAddEventProcPtr)(int down, KeySym keySym,\
        struct rfbClientRec* cl);
typedef void (*KbdReleaseAllKeysProcPtr)(struct rfbClientRec* cl);
typedef void (*PtrAddEventProcPtr)(int buttonMask, int x, int y, \
        struct rfbClientRec* cl);
typedef void (*SetXCutTextProcPtr)(char* str,int len, struct rfbClientRec* cl);
typedef void (*rfbTranslateFnType)(rfbPixelFormat *in,
                                   rfbPixelFormat *out,
                                   char *iptr, char *optr,
                                   int bytesBetweenInputLines,
                                   int width, int height);

//������screen�Ķ��壬������ÿ��������ֻ��һ����
#define CL_BUFF_SIZE   1024
#define cn_vnc_keyboard_height  0x64  //�߶�32����
#define cn_vnc_keyboard_lines   0x01  //һ��
#define cn_vnc_keyboard_visual_enable 0x01


typedef struct
{
    struct tagMutexLCB  cl_mutex;//��client�б�ı���
    struct tagMutexLCB  frame_mutex;//��VNC�ľ���buffer�ı���
//    u16  event_id_update;
//    u16  event_id_net;
//    u8   event_update_prio;
// ������Ϣ
    u16 width;
    u16 height;
    u32 paddedWidthInBytes;
//    u16 depth;
//    u16 bitsPerPixel;
    rfbPixelFormat rfbServerFormat;
//    u16 oscolortype_server;
// ���粿��
    char* desktopName;
    char rfbThisHost[255];
    int rfbPort;
    int rfbListenSock;
    int socketInitDone;
    int rfbMaxClientWait;
//    char* rfbAuthPasswdFile;          //Ŀǰʹ�õ��Ǿ�̬����123
    int rfbDeferUpdateTime;         //���ڶ˿ں�  ͼ��ĸ��¼��
    int rfbAlwaysShared;           //�����Ƿ��������
    int rfbNeverShared;
    struct rfbClientRec* rfbClientHead;//���еĿͻ��˱�

//���еľ�����ʾ��
    struct tagDisplayRsc *mirror_display;
    u8    mirror_display_enable;//����û��ʹ�ܵ�ʱ���ǲ���������ʾ��дͼ���
//���ڿͻ��˶��¼
    int cl_number;//��¼cl����Ŀ
    fd_set allFds;
    int maxFd;
// �������ݵĴ洢
    char* frameBuffer;
    char cl_buffer[CL_BUFF_SIZE];//���ڸ��ͻ��˷�������ʹ�õ�buffer
    u16  ublen;//����ָʾcl_buffer�е����ݳ���
    struct tagClipRect *clip_head; //��Ǹı������
//  ����server�˳�ʱʹ�ã�ͬʱҲ���������¼���״̬
    u8  mainevevent_tobedead;
    u8  updateevent_end;
    u8  netevent_end;
//vnc�����豸�Ĵ���
#if CN_CFG_KEYBOARD == 1
    u32   key_state;//����״̬
    u32   key_update_list;//����ˢ�±�־
    struct tagKeyBoardPrivate vnc_keyboard;//vnc�Ŀͻ��˼�������
    struct tagSingleTouchPrivate vnc_lcdtouch;//vnc�Ĵ���������
#endif
    KbdAddEventProcPtr kbdAddEvent;
    KbdReleaseAllKeysProcPtr kbdReleaseAllKeys;
    PtrAddEventProcPtr ptrAddEvent;
    SetXCutTextProcPtr setXCutText;

} rfbScreenInfo, *rfbScreenInfoPtr;


//�ͻ��˶��壬ÿ���ͻ��˶�ӵ��һ�������Ľṹ���������������Ϳͻ��˽�����һЩ��������

#define CHALLENGESIZE 16  //��ս��֤����
#define MAX_ENCODINGS 10  //���ܵ�����
#define TIGHT_DEFAULT_COMPRESSION  6 //tight��ʽѹ������


typedef struct rfbClientRec
{
    rfbScreenInfoPtr screen;//������VNCserver

    int sock; //socket�׽���
    char *host;//�ͻ��˵�����
    int  protocolMajorVersion;//�ͻ������汾��
    int  protocolMinorVersion;//�ͻ��˴ΰ汾��

    int readyForSetColourMapEntries;//׼���÷�����ɫӳ���

    /* Possible client states: */
    enum {
        RFB_PROTOCOL_VERSION,   /* establishing protocol version */
        RFB_SECURITY_TYPE,      //��ȫ����
        RFB_AUTHENTICATION,     /* authenticating */
        RFB_INITIALISATION,     /* sending initialisation messages */
        RFB_NORMAL              /* normal protocol messages */
    } state;
    u8 HasRequest;//��׼���ã����Խ���ͼ�����ݵĴ��䡣
    rect modifiedregion;//�ͻ��˸���Ȥ������
    u8  InitDone;  //��������Ƿ�Ҫ������������Ȥ����
    //������Ǹ�client�Ƿ��ǿ��õģ���update���Ϊdirty���ɶ˿ڼ���ɾ��
    u8  client_dirty;
//�洢��֤��ս
    u8 authChallenge[CHALLENGESIZE];
// ������Ϣ
    rfbTranslateFnType translateFn;//��ɫת������
    rfbPixelFormat format;//���ظ�ʽ
    u16   oscolortype_client;

//��������ѹ��
    /* statistics */

    int useCopyRect;
    int preferredEncoding;
    int correMaxWidth, correMaxHeight;
    int rfbBytesSent[MAX_ENCODINGS];
    int rfbRectanglesSent[MAX_ENCODINGS];
    int rfbLastRectMarkersSent;
    int rfbLastRectBytesSent;
    int rfbCursorBytesSent;
    int rfbCursorUpdatesSent;

    int rfbFramebufferUpdateMessagesSent;
    int rfbRawBytesEquivalent;
    int rfbKeyEventsRcvd;
    int rfbPointerEventsRcvd;
    int compStreamInited;
    u32 zlibCompressLevel;
    int zsActive[4];
    int zsLevel[4];
    int tightCompressLevel;
    int tightQualityLevel;

    int enableLastRectEncoding;   /* client supports LastRect encoding */
    int enableCursorShapeUpdates; /* client supports cursor shape updates */
    int useRichCursorEncoding;    /* rfbEncodingRichCursor is preferred */
    int cursorWasChanged;         /* cursor shape update should be sent */

    //����˫��ѭ���б�
    struct rfbClientRec *prev;
    struct rfbClientRec *next;

} rfbClientRec, *rfbClientPtr;
rfbScreenInfoPtr rfbserver;
#endif /* RFB_H_ */
