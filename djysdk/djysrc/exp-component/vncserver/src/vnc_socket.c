// =======================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// ģ������: VNCServer
// ģ��汾: V1.00(��ʼ���汾)
// �ļ���     ��socket.c
// ������Ա: Administrator
// ����ʱ��: 2013-3-5
// =======================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <V1.00> <2013-3-5>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =======================================================================
/*
 * sockets.c - deal with TCP & UDP sockets.
 *
 * This code should be independent of any changes in the RFB protocol.  It just
 * deals with the X server scheduling stuff, calling rfbNewClientConnection and
 * rfbProcessClientMessage to actually deal with the protocol.  If a socket
 * needs to be closed for any reason then rfbCloseClient should be called, and
 * this in turn will call rfbClientConnectionGone.  To make an active
 * connection out, call rfbConnect - note that this does _not_ call
 * rfbNewClientConnection.
 *
 * This file is divided into two types of function.  Those beginning with
 * "rfb" are specific to sockets using the RFB protocol.  Those without the
 * "rfb" prefix are more general socket routines (which are used by the http
 * code).
 *
 * Thanks to Karl Hakimian for pointing out that some platforms return EAGAIN
 * not EWOULDBLOCK.
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
#include <WinSock2.h>
#include <Windows.h>
#include "rfb.h"
#include "debug.h"
//#include "misc.h"  //һЩ���Ӻ��ҵĶ���
//#include "rfbproto_processor.h"
#include "vnc_thread.h"  //����Դ
#include  "colormap.h"
#include  "color_remap.h"
#include "rfbproto_processor.h"

int rfbMaxClientWait = 2000;   /* time (ms) after which we decide client has
                                   gone away - needed to stop us hanging */
#pragma comment(lib, "WS2_32")

// =========================================================================
// �������ܣ��ƺ�ͻ��˹رպ���ڴ�����
// ������������رյĿͻ��˲���cl
// ���������
// ����ֵ    ��
// ˵��         ���жϴ��رյĿͻ���cl���������screen֮��Ĺ�ϵ
// =========================================================================
void rfbClientConnectionGone(rfbClientPtr cl)
{
//    int i;
 //   debug_printf("Client %s gone\n",cl->host);
    free(cl->host);//Ŀǰʹ�õ���windows��gethostname.

    /* Release the compression state structures if any. */
    /*
    if ( cl->compStreamInited == TRUE ) {
    deflateEnd( &(cl->compStream) );
    }

    for (i = 0; i < 4; i++) {
    if (cl->zsActive[i])
        deflateEnd(&cl->zsStruct[i]);
    }
    */
    if (cl->prev)
        cl->prev->next = cl->next;
    else
        cl->screen->rfbClientHead = cl->next;
    if (cl->next)
        cl->next->prev = cl->prev;

    cl->screen->cl_number--;
    /*
    if (cl->translateLookupTable)
        free(cl->translateLookupTable);
    */
    free(cl);
    debug_printf("@close socket completely!\n");
}

// =========================================================================
// �������ܣ��رտͻ���
// ������������رյĿͻ��˲���cl
// ���������
// ����ֵ    ��
// ˵��         ���ر��׽��֣������ڴ棬�жϺ�screen֮��Ĺ�ϵ
// =========================================================================
void rfbCloseClient(rfbClientPtr cl)
{
    debug_printf("begin to shutdown the client!\n");
//  ��screen��fds����ɾ��
    FD_CLR(cl->sock,&(cl->screen->allFds));
//  �ر�socket
    closesocket(cl->sock);
//    WSACleanup();
//  ����cl���ڴ�����
    rfbClientConnectionGone(cl);
}

/*
 * ReadExact reads an exact number of bytes from a client.  Returns 1 if
 * those bytes have been read, 0 if the other end has closed, or -1 if an error
 * occurred (errno is set to ETIMEDOUT if it timed out).
 */
// =========================================================================
// �������ܣ��ӿͻ��˶�ȡ����
// ����������ͻ��˲���cl���洢buf������len
// ���������
// ����ֵ  ��1�ɹ���-1ʧ��
// ˵��    ����ȡ�̶�Ϊlen���ֽ���
// =========================================================================
int ReadExact(rfbClientPtr cl, char *buf, int len)
{
    int sock = cl->sock;
    int n;
    fd_set fds;
    struct timeval tv;
    int totalTimeWaited = 0;
    while (len > 0)
    {
        n = recv(sock, buf, len,0);

        if (n > 0) {

            buf += n;
            len -= n;

        }  else {
            FD_ZERO(&fds);
            FD_SET(sock, &fds);
            //tv.tv_sec = rfbMaxClientWait / 1000;
            //tv.tv_usec = (rfbMaxClientWait % 1000) * 1000;
            tv.tv_sec = 0;
            tv.tv_usec =1000;

            n = select(sock+1, &fds, NULL, NULL, &tv);
            if (n < 0) {
                debug_printf("@ReadExact: select");
                return n;
            }
            if (n == 0) {

                  debug_printf("@read:select time out!\n");
                 totalTimeWaited += 1000;
                if (totalTimeWaited >= rfbMaxClientWait) {

                    return -1;
                }
            }
            else{
                totalTimeWaited=0;
            }
        }
    }
    return 1;
}

/*
 * WriteExact writes an exact number of bytes to a client.  Returns 1 if
 * those bytes have been written, or -1 if an error occurred (errno is set to
 * ETIMEDOUT if it timed out).
 */
// =========================================================================
// �������ܣ���ͻ���clд������
// ����������ͻ��˲���cl���洢buf������len
// ���������
// ����ֵ     ��1�ɹ���-1ʧ��
// ˵��          ��д��̶�Ϊlen���ֽ���
// =========================================================================
#define WAIT_TIMES  50000   //50����
int WriteExact(rfbClientPtr cl, char *buf, int len)
{
    int sock = cl->sock;
    int n;
    fd_set fds;
    struct timeval tv;
    int totalTimeWaited = 0;
    int write_len=0;
    int total_len=0;
    while (len > 0)
    {
        n = send(sock, buf, len,0);
        if (n > 0)
        {
            buf += n;
            len -= n;
            total_len+=n;
        }
        else
        {
            FD_ZERO(&fds);
            FD_SET(sock, &fds);
            tv.tv_sec = 1;
            tv.tv_usec = 0;
            n = select(sock+1, NULL, &fds, NULL, &tv);
            if (n < 0)
            {
                debug_printf("##WriteExact: select");
                return -1;
            }
            if (n == 0)
            {
                totalTimeWaited += 1000;
                debug_printf("##write:select time out");
                if (totalTimeWaited >= WAIT_TIMES) {
                    return -1;
                }
            }
            else
            {
                totalTimeWaited = 0;
            }
        }
    }
    return 1;
}
// ========================================================================
// ��������:����TCP��listen����
// �������:port������Ҫ������TCP�˿ںţ�-1����ʧ��
// ���������
// ����ֵ    :��Ӧ�ڴ˶˿ںŵ�socket�׽���
// ˵��         :ʹ�ó�����Ӧ���ܱ�֤���ṩ����port�ǿ��õģ�����᲻�ɹ�
// =========================================================================
int ListenOnTCPPort(int port)
{
    struct sockaddr_in addr;
    int sock;
    int one = 1024;

    int length=0;

    //windows���еģ���������
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2),&wsa);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    //addr.sin_addr.s_addr = interface.s_addr;
    addr.sin_addr.s_addr = INADDR_ANY;

    if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        debug_printf("socket failed!\n");
        return -1;
    }
    /**/
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,(char *)&one, sizeof(one)) < 0)
    {
        closesocket(sock);
        debug_printf("setsockopt failed!\n");
        return -1;
    }


    printf("----has reached!--------------!\n");

    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        closesocket(sock);
        debug_printf("bind failed!\n");
        return -1;
    }
    if (listen(sock, 5) < 0)
    {
        closesocket(sock);
        debug_printf("listen failed!\n");
        return -1;
    }

    getsockname(sock,&addr,&length);

    printf( "����IP��%s ", inet_ntoa(addr.sin_addr));
    printf( "����PORT��%d ", ntohs(addr.sin_port));

    return sock;
}

// =========================================================================
// �������ܣ���ʼ��screen������˿ڣ��ڴ�ֻ���TCP��������
// �������: �����ķ�������screen
// ���������
// ����ֵ  :
// ˵��    :  ���仰˵�˺�����Ҫʵ�����׽��ֺ�fds������
// =========================================================================
void  rfbInitSockets(rfbScreenInfoPtr rfbScreen)
{
    if (rfbScreen->socketInitDone)
    return;

    rfbScreen->socketInitDone = TRUE;
//  ����TCP�������ķ�ʽ���������Ӳ�������--TODO
    debug_printf("Begin to set TCP port %d at listen!\n", rfbScreen->rfbPort);

    if ((rfbScreen->rfbListenSock = ListenOnTCPPort(rfbScreen->rfbPort)) < 0)
    {
        debug_printf("fatal error!ListenOnTCPPort=%d\n",rfbScreen->rfbListenSock);
        exit(1);
    }

    FD_ZERO(&(rfbScreen->allFds));
    FD_SET(rfbScreen->rfbListenSock, &(rfbScreen->allFds));
    rfbScreen->maxFd = rfbScreen->rfbListenSock;

    debug_printf("Has  set TCP port %d at listen!\n", rfbScreen->rfbPort);
}
// =========================================================================
// ��������:����һ���µĿͻ�
// �������:����������Ļ��screen��accept�����׽���
// ���������
// ����ֵ    :�µĿͻ��˲���
// ˵��         :���½��Ĳ����Ѿ���ӵ�screen��cl�����У�ʧ�ܷ���NULL
// =========================================================================
rfbClientPtr rfbNewClient(rfbScreenInfoPtr rfbScreen,int sock)
{

    rfbProtocolVersionMsg pv;
    rfbClientPtr cl;
    struct sockaddr_in addr;
    int addrlen = sizeof(struct sockaddr_in);
    int i = 0;

    cl = (rfbClientPtr)M_MallocLc(sizeof(rfbClientRec),0);
    if(NULL == cl)
    {
        debug_printf("#error: no mem more for the newclient!\n");
        return cl;
    }
    rfbScreen->cl_number++;
    //���뵽screen������֮�У��½�����������������ͷ��˫��ѭ���б�
    cl->next = rfbScreen->rfbClientHead;
    cl->prev = NULL;
    if (rfbScreen->rfbClientHead)
        rfbScreen->rfbClientHead->prev = cl;
    rfbScreen->rfbClientHead = cl;
//��ʼ��ʼ��cl�ĳ�Ա����
    //����socket��
    cl->screen = rfbScreen;
    cl->sock = sock;
    getpeername(sock, (struct sockaddr *)&addr, &addrlen);
    cl->host = strdup(inet_ntoa(addr.sin_addr));


    //���ڽ������ӵĲ���
    cl->readyForSetColourMapEntries = FALSE;
    cl->InitDone=FALSE;
    cl->HasRequest = 0;
    cl->state = RFB_PROTOCOL_VERSION;
    cl->client_dirty = 0;

    //���ڼ��ܷ�ʽ
    cl->useCopyRect = FALSE;
    cl->preferredEncoding = rfbEncodingRaw;
    cl->correMaxWidth = 48;
    cl->correMaxHeight = 48;
    cl->modifiedregion.x=0;
    cl->modifiedregion.y=0;
    cl->modifiedregion.w=0;
    cl->modifiedregion.h=0;
    //����ѹ���㷨�Ĳ��ֳ�ʼ��
    cl->tightCompressLevel = TIGHT_DEFAULT_COMPRESSION;
    cl->tightQualityLevel = -1;
    for (i = 0; i < 4; i++)
        cl->zsActive[i] = FALSE;
    cl->enableCursorShapeUpdates = FALSE;
    cl->enableLastRectEncoding = FALSE;
    cl->compStreamInited = FALSE;
    cl->zlibCompressLevel = 5;




//����cl����ɫӳ�亯����������ɫӳ�����Ľ�����screen����ɫbits��أ������ڴ��趨
    cl->format = cl->screen->rfbServerFormat;
    set_client_colormap_function(cl);
//  �½��Ŀͻ�����ʼ��ͻ��˷��Ͱ汾�ţ���ʼ�������ֽ׶�
    sprintf(pv,rfbProtocolVersionFormat,rfbProtocolMajorVersion,
            rfbProtocolMinorVersion);
    if (WriteExact(cl, pv, sz_rfbProtocolVersionMsg) < 0) {
        debug_printf("rfbNewClient: write");
        rfbCloseClient(cl);
        return NULL;
    }

    return cl;
}

/*
 * rfbCheckFds is called from ProcessInputEvents to check for input on the RFB
 * socket(s).  If there is input to process, the appropriate function in the
 * RFB server code will be called (rfbNewClientConnection,
 * rfbProcessClientMessage, etc).
 */
// =========================================================================
// ��������:��ѭscreen�Ķ˿ڣ������������˿ںͿͻ��˶˿�
// �����������������Ļscreen�Լ���ʱʱ������
// ���������
// ����ֵ  :
// ˵��    :��ѭ�������Ķ˿���Ϊ�˼���Ƿ����µĿͻ���Ҫ���룬
//          ��ѭ�ͻ��˿���Ϊ�˿����ͻ����Ƿ����µ�ͨ����Ϣ��Ҫ����
// =========================================================================
void rfbCheckFds(rfbScreenInfoPtr rfbScreen,long usec)
{
    int nfds;
    fd_set fds;
    struct timeval tv;
    struct sockaddr_in addr;
    int addrlen = sizeof(addr);
//    char buf[6];
    const int one = 32*1024;
    int noblock=1;
    int sock;
    rfbClientPtr cl,cl_next=NULL;

    memcpy((char *)&fds, (char *)&(rfbScreen->allFds), sizeof(fd_set));
    tv.tv_sec = 0;
    tv.tv_usec = usec;
    nfds = select(rfbScreen->maxFd + 1, &fds, NULL, NULL, &tv);
    if (nfds == 0)
    {
//      debug_printf("--fds:select time out!\n");
        return;
    }
    if (nfds < 0)
    {
        debug_printf("#error��rfbCheckFds: select");
        return;
    }

    //��ʵlistensock���Ǵ�˵�е�TCPsock����Ĭ�ϵ�
    if ((rfbScreen->rfbListenSock != -1) && ( FD_ISSET(rfbScreen->rfbListenSock, &fds)))
    {
        if ((sock = accept(rfbScreen->rfbListenSock,(struct sockaddr *)&addr, &addrlen)) == INVALID_SOCKET)
        {
            debug_printf("rfbCheckFds: accept");
            goto check_sockfd;
        }
        ioctlsocket(sock,FIONBIO,&noblock);
        if (setsockopt(sock, IPPROTO_TCP, TCP_NODELAY,(char *)&one, sizeof(one)) != 0)
        {
            debug_printf("#error��rfbCheckFds: setsockopt failed");
            closesocket(sock);
            goto check_sockfd;
        }

        debug_printf("Got connection from client %s\n", inet_ntoa(addr.sin_addr));

//      �������ӵ�sock��ӵ�fds���б���
        FD_SET(sock, &(rfbScreen->allFds));
        rfbScreen->maxFd = max(sock,rfbScreen->maxFd);

        rfbNewClient(rfbScreen,sock);

        FD_CLR(rfbScreen->rfbListenSock, &fds);
    }
check_sockfd:
    cl= rfbScreen->rfbClientHead;
    do{
        if(cl)
        {
            cl_next=cl->next;
            if(cl->client_dirty == 1)//��clientΪ�࣬Ҫɾ���Ķ���
            {
                rfbCloseClient(cl);
            }
            else
            {
                if (FD_ISSET(cl->sock, &fds) && FD_ISSET(cl->sock, &(rfbScreen->allFds)))
                {
                    rfbProcessClientMessage(cl);//����client��server������ͨ��״̬
                }
                cl=cl_next;
            }
        }
        else
        {
          break;
        }
    }while(cl);
}









