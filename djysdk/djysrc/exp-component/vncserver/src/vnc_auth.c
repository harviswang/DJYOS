// =======================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// ģ������: VNCServer
// ģ��汾: V1.00(��ʼ���汾)
// �ļ���     ��auth.c
// ������Ա: Administrator
// ����ʱ��: 2013-3-6
// =======================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <V1.00> <2013-3-6>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =======================================================================

/*
 * auth.c - deal with authentication.
 *
 * This file implements the VNC authentication protocol when setting up an RFB
 * connection.
 */

/*
 *  OSXvnc Copyright (C) 2001 Dan McGuirk <mcguirk@incompleteness.net>.
 *  Original Xvnc code Copyright (C) 1999 AT&T Laboratories Cambridge.
 *  All Rights Reserved.
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

#include "rfb.h"
#include "vnc_socket.h"
#include "rfbproto.h"
#include  "debug.h"
#include  "authencode.h"
#include  "vnc_auth.h"

char *rfbAuthPasswdFile = NULL;


// =========================================================================
// �������ܣ�����ʧ��ԭ��
// ����������������Ŀͻ���cl��ʧ��ԭ��
// ���������
// ����ֵ  ��
// ˵��    ��������Ҫ����3.7�������ϵİ汾��Э������ʱ
// =========================================================================
void rfbClientSendString(rfbClientPtr cl, const char *reason)
{
    char *buf;
    int len = strlen(reason);

    debug_printf("rfbClientSendString(\"%s\")\n", reason);

    buf = (char *)M_MallocLc(4 + len, 0);
    ((int *)buf)[0] = Swap32IfLE(len);
    memcpy(buf + 4, reason, len);

    if (WriteExact(cl, buf, 4 + len) < 0)
        debug_printf("rfbClientSendString: write");
    free(buf);

    rfbCloseClient(cl);
}
/*
 * Send the authentication challenge.
 */
// =========================================================================
// �������ܣ�������ս����
// ����������������Ŀͻ���cl
// ���������
// ����ֵ    ��
// ˵��         �����ڲ�ͬ�İ汾������һ����
// =========================================================================
void rfbVncAuthSendChallenge(rfbClientPtr cl)
{

    int i=0;
    /* 4 byte header is alreay sent. Which is rfbSecTypeVncAuth
       (same as rfbVncAuth). Just send the challenge. */
    RandomData(cl->authChallenge);//���ڶ�������㷨����������des3������

    for(i=0;i<CHALLENGESIZE;i++)
    {
        debug_printf("%d",cl->authChallenge[i]);
    }
    debug_printf("\n");
    if (WriteExact(cl, (char *)cl->authChallenge, CHALLENGESIZE) < 0) {
        debug_printf("rfbAuthNewClient: write");
        rfbCloseClient(cl);
        return;
    }

    /* Dispatch client input to rfbVncAuthProcessResponse. */
    cl->state = RFB_AUTHENTICATION;
}
/*
 * Tell the client what security type will be used (protocol 3.3).
 */
// =========================================================================
// �������ܣ�����V3.7���°汾�ķ���security type
// ����������������Ŀͻ���cl
// ���������
// ����ֵ    ��
// ˵��         ��NoeAuth����init״̬��AUTHVNC����challenge״̬��
// =========================================================================
static void  rfbSendSecurityType(rfbClientPtr cl, u32 securityType)
{
    u32 value32;

    /* Send the value. */
    value32 = Swap32IfLE(securityType);
    if (WriteExact(cl, (char *)&value32, 4) < 0) {
    debug_printf("rfbSendSecurityType: write");
    rfbCloseClient(cl);
    return;
    }

    /* Decide what to do next. */
    switch (securityType) {
    case rfbNoAuth:
    /* Dispatch client input to rfbProcessClientInitMessage. */
    cl->state = RFB_INITIALISATION;
    break;
    case rfbVncAuth:
    /* Begin the standard VNC authentication procedure. */
    rfbVncAuthSendChallenge(cl);
    break;
    default:
    /* Impossible case (hopefully). */
        debug_printf("rfbSendSecurityType: assertion failed");
        rfbCloseClient(cl);
        break;
    }
}
//#define rfbConnFailed 0
//#define rfbNoAuth 1
//#define rfbVncAuth 2
// =========================================================================
// �������ܣ�����V3.7���°汾�ķ���security type
// ����������������Ŀͻ���cl
// ���������
// ����ֵ    ��
// ˵��         ��NoeAuth����init״̬��AUTHVNC����challenge״̬��
// =========================================================================
static void  rfbSendSecurityTypeList(rfbClientPtr cl, u32 securityType)
{
    u8 buffer[4];
    int  size=4;

    buffer[0]=3;//һ����3�ְ�ȫ���ͣ�invalid��none��vnc
    buffer[1]=rfbConnFailed;
    buffer[2]=rfbNoAuth;
    buffer[3]=rfbVncAuth;

    /* Send the list. */
    if (WriteExact(cl, (char *)buffer, size) < 0) {
    debug_printf("rfbSendSecurityTypeList: write");
    rfbCloseClient(cl);
    return;
    }

    /*
      * if count is 0, we need to send the reason and closesocket the connection.
      */
    if(size <= 1) {
    /* This means total count is Zero and so reason msg should be sent */
    /* The execution should never reach here */
    char* reason = "No authentication mode is registered!";

    rfbClientSendString(cl, reason);
    return;
    }

    /* Dispatch client input to rfbProcessClientSecurityType. */
    cl->state = RFB_SECURITY_TYPE;
}
/*
 * rfbAuthNewClient is called when we reach the point of authenticating
 * a new client.  If authentication isn't being used then we simply send
 * rfbNoAuth.  Otherwise we send rfbVncAuth plus the challenge.
 */
// =========================================================================
// �������ܣ�����ȫ���͵�
// ����������������Ŀͻ�cl
// ���������
// ����ֵ  ��
// ˵��    ���ں�clЭ�̹�rfb��version֮����е�һ�����̣���ͬ�汾����ʽ��һ��
//          v3.7 ���£�serverֱ�ӷ���security type��cl��server��������typeֻ��һ��
//          v3.7�������ϣ�server����type���cl����clѡ����Ӧ��type
//          ��ͬ��type����ʽ��һ����NoneAuthֱ�ӽ���init״̬,authvnc��Ҫ����
//           challenge״̬
// =========================================================================
void rfbAuthNewClient(rfbClientPtr cl)
{
//  u32 securityType = rfbNoAuth;
    u32 securityType = rfbVncAuth;
    if (cl->protocolMajorVersion==3 && cl->protocolMinorVersion < 7)
    {
    /* Make sure we use only RFB 3.3 compatible security types. */
        rfbSendSecurityType(cl, securityType);
    }
    else
    {
    /* Here it's ok when securityType is set to rfbSecTypeInvalid. */
        rfbSendSecurityTypeList(cl, securityType);
    }
}


