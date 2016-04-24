// =======================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// ģ������: VNCServer
// ģ��汾: V1.00(��ʼ���汾)
// �ļ���     ��rfbserver.c//��Ҫ���server�Ϳͻ���֮�����Ϣ����
// ������Ա: Administrator
// ����ʱ��: 2013-3-6
// =======================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <V1.00> <2013-3-6>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =======================================================================

#include "rfbproto_processor.h"
#include "vnc_socket.h"
#include "rfbproto.h"
#include "debug.h"
#include "rfb_auth.h"
#include "keysymdef.h"
#include "vnc_keyboard.h"
#include "colormap.h"
#include "color_remap.h"
#include "authencode.h"

#include "vnc_keyboard.h"

int
rfbSendSetColourMapEntries(rfbClientPtr cl,
                           int firstColour,
                           int nColours)
{
    char buf[sz_rfbSetColourMapEntriesMsg + 256 * 3 * 2];
    char *wbuf = buf;
    rfbSetColourMapEntriesMsg *scme;
    unsigned short *rgb;

    int i, len;

    if (nColours > 256) {
    /* some rare hardware has, e.g., 4096 colors cells: PseudoColor:12 */
        wbuf = (char *) malloc(sz_rfbSetColourMapEntriesMsg + nColours * 3 * 2);
    }

    scme = (rfbSetColourMapEntriesMsg *)wbuf;
    rgb = (u16 *)(&wbuf[sz_rfbSetColourMapEntriesMsg]);

    scme->type = rfbSetColourMapEntries;

    scme->firstColour = Swap16IfLE(firstColour);
    scme->nColours = Swap16IfLE(nColours);

    len = sz_rfbSetColourMapEntriesMsg;

    for (i = 0; i < nColours; i++) {
      rgb[i*3] = Swap16IfLE(i);
      rgb[i*3+1] = Swap16IfLE(i);
      rgb[i*3+2] = Swap16IfLE(i);

    }

    len += nColours * 3 * 2;

    if (WriteExact(cl, wbuf, len) < 0) {
    debug_printf("rfbSendSetColourMapEntries: write");
    rfbCloseClient(cl);
    if (wbuf != buf)
        free(wbuf);

    return FALSE;
    }
    if (wbuf != buf) free(wbuf);
    return TRUE;
}
// =========================================================================
// ��������:��������ʧ�ܵ��ƺ�����
// �������:�����ӵĿͻ�cl������ʧ�ܵ�ԭ��
// ���������
// ����ֵ    :
// ˵��         :����Э��汾Э�̻�������֤��ʱ��ʧ�ܣ���ô�������������������ƺ�
// =========================================================================
void  rfbClientConnFailed(rfbClientPtr cl,char *reason)
{
    char *buf;
    int len = strlen(reason);

    buf = (char *)malloc(8 + len);
    ((u32 *)buf)[0] = Swap32IfLE(rfbConnFailed);
    ((u32 *)buf)[1] = Swap32IfLE(len);
    memcpy(buf + 8, reason, len);

    if (WriteExact(cl, buf, 8 + len) < 0)
        debug_printf("rfbClientConnFailed: write");
    free(buf);
    rfbCloseClient(cl);
}
// =========================================================================
// ��������:����ͻ��˺��û�����Ϣ����:����Э��汾��Ϣ
// �������:
// ���������
// ����ֵ    :�������Ŀͻ�cl
// ˵��         :���ڽ����¿ͻ���ʱ�������Ѿ�����������ߵİ汾�ŷ��͸��˿ͻ���
// =========================================================================

static void  rfbProcessClientProtocolVersion(rfbClientPtr cl)
{
    rfbProtocolVersionMsg pv;
    int n, major, minor;
    char failureReason[256];

    if ((n = ReadExact(cl, pv, sz_rfbProtocolVersionMsg)) <= 0) {
        if (n == 0)
            debug_printf("rfbProcessClientProtocolVersion: client gone\n");
        else
            debug_printf("rfbProcessClientProtocolVersion: read");
        rfbCloseClient(cl);
        return;
    }

    pv[sz_rfbProtocolVersionMsg] = 0;
    if (sscanf(pv,rfbProtocolVersionFormat,&major,&minor) != 2) {
        debug_printf("rfbProcessClientProtocolVersion: not a valid RFB client\n");
        rfbCloseClient(cl);
        return;
    }
    debug_printf("Protocol version %d.%d\n", major, minor);

    if (major != rfbProtocolMajorVersion) {
        /* Major version mismatch - send a ConnFailed message */

        debug_printf("Major version mismatch\n");
        sprintf(failureReason,
                "RFB protocol version mismatch - server %d.%d, client %d.%d",
                rfbProtocolMajorVersion,rfbProtocolMinorVersion,major,minor);
        rfbClientConnFailed(cl, failureReason);
        return;
    }

    if (minor != rfbProtocolMinorVersion) {
        /* Minor version mismatch - warn but try to continue */
        debug_printf("Ignoring minor version mismatch\n");
    }
    //ÿ���ͻ��˶��ᷢ���Լ���proto
    cl->protocolMajorVersion=major;
    cl->protocolMinorVersion=minor;

    rfbAuthNewClient(cl);
}
/*
 * rfbAuthProcessClientMessage is called when the client sends its
 * authentication response.
 */
// =========================================================================
// �������ܣ�����ͻ��˶���ս��Ϣ����Ӧ
// ����������������Ŀͻ���cl
// ���������
// ����ֵ  ��
// ˵��    ������Ӧ��Ϣʧ��ʱ��3.7���ϵİ汾�ᷢ��ʧ�ܵ�ԭ�򣬶�3.7�������»�ֱ��
//          �رտͻ���
// =========================================================================
void rfbAuthProcessClientMessage(rfbClientPtr cl)
{
    char *passwd="123";
    int n;
    u8 response[CHALLENGESIZE];
    u32 authResult;

    if ((n = ReadExact(cl, (char *)response, CHALLENGESIZE)) <= 0) {
        if (n != 0)
            debug_printf("rfbAuthProcessClientMessage: read");
        rfbCloseClient(cl);
        return;
    }
     rfbEncryptBytes(cl->authChallenge, passwd);


    if (memcmp(cl->authChallenge, response, CHALLENGESIZE) != 0) {
        debug_printf("rfbAuthProcessClientMessage: authentication failed from %s\n",
               cl->host);

        authResult = Swap32IfLE(rfbVncAuthFailed);

        if (WriteExact(cl, (char *)&authResult, 4) < 0) {
            debug_printf("rfbAuthProcessClientMessage: write");
            rfbClientSendString(cl,"challengdge failed\n");
        }
        rfbCloseClient(cl);
        return;
    }
    else
    {
        debug_printf("challendge success ---cl=%s",cl->host);

    }

    authResult = Swap32IfLE(rfbVncAuthOK);

    if (WriteExact(cl, (char *)&authResult, 4) < 0) {
        debug_printf("rfbAuthProcessClientMessage: write");
        rfbCloseClient(cl);
        return;
    }

    cl->state = RFB_INITIALISATION;
}
// =========================================================================
// �������ܣ�����ͻ��˵İ�ȫ������Ϣ
// ����������������Ŀͻ���cl
// ���������
// ����ֵ    ��
// ˵��         �� ֻ��3.7�������ϰ汾��server�Ż���clѡ��ȫ����
// =========================================================================void
void rfbProcessClientSecurityType(rfbClientPtr cl)
{
    int n;
    u8 chosenType;
    u32 authResult;
        /* Read the security type. */
    n = ReadExact(cl, (char *)&chosenType, 1);
    if (n <= 0) {
    if (n == 0)
        debug_printf("rfbProcessClientSecurityType: client gone\n");
    else
        debug_printf("rfbProcessClientSecurityType: read");
    rfbCloseClient(cl);
    return;
    }

    /* Make sure it was present in the list sent by the server. */
    switch (chosenType)
    {
        case rfbConnFailed:
            debug_printf("The client %s connfailed!\n",cl->host);
            cl->state = RFB_INITIALISATION;
            break;

        case rfbNoAuth:
            debug_printf("The client %s need no auth!\n",cl->host);
            //����3.8�汾��Ȼ��Ҫ�ѽ��������ȥ
            if((cl->protocolMajorVersion==3)&&(cl->protocolMinorVersion>7))
            {
                authResult = Swap32IfLE(rfbVncAuthOK);
                if (WriteExact(cl, (char *)&authResult, 4) < 0) {
                    debug_printf("rfbAuthProcessClientMessage: write");
                    rfbCloseClient(cl);
                    break;
                }
            }
            cl->state = RFB_INITIALISATION;
            break;
        case rfbVncAuth:
            debug_printf("The client %s need vncauth!\n",cl->host);
            rfbVncAuthSendChallenge(cl);//������ս��Ϣ
            break;

        default:
            debug_printf("No supported security method--%d\n",chosenType);
            rfbCloseClient(cl);
            break;
    }

    return;
}

// =========================================================================
// ��������:����ͻ��˺ͷ�����֮��ĳ�ʼ����Ϣ
// �������:
// ���������
// ����ֵ    :�������Ŀͻ�cl
// ˵��         :�����в�����⣬�������ŵ������Լ������õ�
// =========================================================================

static void rfbProcessClientInitMessage(rfbClientPtr cl)
{
    rfbClientInitMsg ci;
    char buf[256];
    rfbServerInitMsg *si = (rfbServerInitMsg *)buf;
    int len, n;
    if ((n = ReadExact(cl, (char *)&ci,sz_rfbClientInitMsg)) <= 0) {
        if (n == 0)
            debug_printf("rfbProcessClientInitMessage: client gone\n");
        else
            debug_printf("rfbProcessClientInitMessage: read");
        rfbCloseClient(cl);
        return;
    }

    si->framebufferWidth = Swap16IfLE(cl->screen->width);
    si->framebufferHeight = Swap16IfLE(cl->screen->height);
    debug_printf("width=%d  hieght=%d\n",cl->screen->width,cl->screen->height);
    si->format = cl->screen->rfbServerFormat;
    si->format.redMax = Swap16IfLE(si->format.redMax);
    si->format.greenMax = Swap16IfLE(si->format.greenMax);
    si->format.blueMax = Swap16IfLE(si->format.blueMax);

    if (strlen(cl->screen->desktopName) > 128)      /* sanity check on desktop name len */
        cl->screen->desktopName[128] = 0;

    strcpy(buf + sz_rfbServerInitMsg, cl->screen->desktopName);
    len = strlen(buf + sz_rfbServerInitMsg);
    si->nameLength = Swap32IfLE(len);

    if (WriteExact(cl, buf, sz_rfbServerInitMsg + len) < 0) {
        debug_printf("rfbProcessClientInitMessage: write");
        rfbCloseClient(cl);
        return;
    }

    cl->state = RFB_NORMAL;
    if(ci.shared)
    {
        debug_printf("The client permit share!\n");
    }
    else
    {
        debug_printf("The client refused share!\n");
    }
    //�����Ƿ�������⣬�Ժ���̸
}
// =============================================================================
// ��������:����ͻ��˺ͷ�����֮���������ͨ����Ϣ
// �������:
// ���������
// ����ֵ  :�������Ŀͻ�cl
// ˵��    :�����в�����⣬�������ŵ������Լ������õ�
// =============================================================================

static void rfbProcessClientNormalMessage(rfbClientPtr cl)
{
    int n=0;
    rfbClientToServerMsg msg;
    char *str;
    int i;

//  unsigned char  keyvalue;
    u32 enc;

//   debug_printf("--In the NormalMessadge process!\n");
    if ((n = ReadExact(cl, (char *)&msg, 1)) <= 0)
    {
        debug_printf("rfbProcessClientNormalMessage: read--type-error,shutdown clien!\n");
//      delay(10);
        rfbCloseClient(cl);
        return;
    }
 //   debug_printf("--In the NormalMessadge process!--has read msg type=%d \n",msg.type);
    switch (msg.type) //�����յ�����Ϣ����Ӧ�Ĵ���������
    {

        case rfbSetPixelFormat://Ŀǰ�Ѿ������걸
            debug_printf("SetPixelFormat!\n");

            if ((n = ReadExact(cl, ((char *)&msg) + 1,
                               sz_rfbSetPixelFormatMsg - 1)) <= 0)
            {
                debug_printf("rfbProcessClientNormalMessage: read--setpixelmessage-error,shutdown client!\n");
//              delay(10000);
                rfbCloseClient(cl);
                return;
            }

            cl->format.bitsPerPixel = msg.spf.format.bitsPerPixel;
            cl->format.depth = msg.spf.format.depth;
            cl->format.bigEndian = (msg.spf.format.bigEndian ? 1 : 0);
            cl->format.trueColour = (msg.spf.format.trueColour ? 1 : 0);
            cl->format.redMax = Swap16IfLE(msg.spf.format.redMax);
            cl->format.greenMax = Swap16IfLE(msg.spf.format.greenMax);
            cl->format.blueMax = Swap16IfLE(msg.spf.format.blueMax);
            cl->format.redShift = msg.spf.format.redShift;
            cl->format.greenShift = msg.spf.format.greenShift;
            cl->format.blueShift = msg.spf.format.blueShift;

            cl->readyForSetColourMapEntries = TRUE;
            //����Ǳ���Ҫ����ģ���Ϊ���ͻ��˷��͵���Ϣ����ɫ�������ɫ�Ļ�������Ҫ���ͷ�������ӳ�䷽����
            debug_printf("got  the client set pixformat=%s\n",cl->host);
            showPixelFormat(&(cl->format));
    //      deug_printf("--set the client color translate begin!\n");
            rfbSetTranslateFunction(cl);
    //      debug_printf("--set the client color end!\n");

            return;

        case rfbFixColourMapEntries:

            debug_printf("rfbFixColourMapEntries!\n");

            if ((n = ReadExact(cl, ((char *)&msg) + 1,
                               sz_rfbFixColourMapEntriesMsg - 1)) <= 0) {
                debug_printf("rfbProcessClientNormalMessage: read--fixpixelmessage-error,shutdown client!\n");
                delay(10000);
                rfbCloseClient(cl);
                return;
            }
            debug_printf("FixColourMapEntries unsupported\n");
            rfbCloseClient(cl);
            return;


        case rfbSetEncodings://Ŀǰ�Ѿ������걸
        {
            //debug_printf("rfbSetEncodings!\n");


            if ((n = ReadExact(cl, ((char *)&msg) + 1,
                               sz_rfbSetEncodingsMsg - 1)) <= 0) {
                debug_printf("rfbProcessClientNormalMessage: read--rfbSetEncodings-error,shutdown client!\n");
                delay(10000);
                rfbCloseClient(cl);
                return;
            }

            msg.se.nEncodings = Swap16IfLE(msg.se.nEncodings);

            cl->preferredEncoding = -1;
            cl->useCopyRect = FALSE;
            cl->enableCursorShapeUpdates = FALSE;
            cl->enableLastRectEncoding = FALSE;

            for (i = 0; i < msg.se.nEncodings; i++) {
                if ((n = ReadExact(cl, (char *)&enc, 4)) <= 0) {
                    debug_printf("rfbProcessClientNormalMessage: read--rfbSetEncodings-error,shutdown client!\n");
                    delay(10000);
                    rfbCloseClient(cl);
                    return;
                }
                enc = Swap32IfLE(enc);

                switch (enc) {

                case rfbEncodingCopyRect:
                    debug_printf("client %s supported rfbEncodingCopyRect\n",cl->host);
                    cl->useCopyRect = TRUE;
                    break;
                case rfbEncodingRaw:
                    debug_printf("client %s supported rfbEncodingRaw\n",cl->host);
                    if (cl->preferredEncoding == -1) {
                        cl->preferredEncoding = enc;
                        debug_printf("Using raw encoding for client %s\n",
                               cl->host);
                    }
                    break;
                case rfbEncodingRRE:
                    debug_printf("client %s supported rfbEncodingRRE\n",cl->host);
                    if (cl->preferredEncoding == -1) {
                        cl->preferredEncoding = enc;
                        debug_printf("Using rre encoding for client %s\n",
                               cl->host);
                    }
                    break;
                case rfbEncodingCoRRE:
                    debug_printf("client %s supported rfbEncodingCoRRE\n",cl->host);
                    if (cl->preferredEncoding == -1) {
                        cl->preferredEncoding = enc;
                        debug_printf("Using CoRRE encoding for client %s\n",
                               cl->host);
                    }
                    break;
                case rfbEncodingHextile:
                    debug_printf("client %s supported rfbEncodingHextile\n",cl->host);
                    if (cl->preferredEncoding == -1) {
                        cl->preferredEncoding = enc;
                        debug_printf("Using hextile encoding for client %s\n",
                               cl->host);
                    }
                    break;
                case rfbEncodingZlib:

                    debug_printf("client %s supported rfbEncodingZlib\n",cl->host);
                    if (cl->preferredEncoding == -1) {
                        cl->preferredEncoding = enc;
                        debug_printf("Using zlib encoding for client %s\n",
                                cl->host);
                    }
                  break;
                case rfbEncodingTight:
                    debug_printf("client %s supported rfbEncodingHextile\n",cl->host);

                    if (cl->preferredEncoding == -1) {
                        cl->preferredEncoding = enc;
                        debug_printf("Using tight encoding for client %s\n",
                   cl->host);
                    }
                    break;
                case rfbEncodingXCursor:
                    debug_printf("Enabling X-style cursor updates for client %s\n",
                   cl->host);
                    cl->enableCursorShapeUpdates = TRUE;
                    cl->useRichCursorEncoding = FALSE;
                    cl->cursorWasChanged = TRUE;
                    break;
                case rfbEncodingRichCursor:
                    if (!cl->enableCursorShapeUpdates) {
                        debug_printf("Enabling full-color cursor updates for client "
                    "%s\n", cl->host);
                        cl->enableCursorShapeUpdates = TRUE;
                        cl->useRichCursorEncoding = TRUE;
                        cl->cursorWasChanged = TRUE;
                    }
                    break;
                case rfbEncodingLastRect:
                    debug_printf("client %s supported rfbEncodingLastRect\n",cl->host);
                    if (!cl->enableLastRectEncoding) {
                        debug_printf("Enabling LastRect protocol extension for client "
                   "%s\n", cl->host);
                        cl->enableLastRectEncoding = TRUE;
                    }
                    break;
                default:
                    if ( enc >= (u32)rfbEncodingCompressLevel0 &&
                            enc <= (u32)rfbEncodingCompressLevel9 ) {
                        cl->zlibCompressLevel = enc & 0x0F;
                        cl->tightCompressLevel = enc & 0x0F;
                        debug_printf("Using compression level %d for client %s\n",
                                cl->tightCompressLevel, cl->host);
                    } else if ( enc >= (u32)rfbEncodingQualityLevel0 &&
                    enc <= (u32)rfbEncodingQualityLevel9 ) {
                        cl->tightQualityLevel = enc & 0x0F;
                        debug_printf("Using image quality level %d for client %s\n",
                   cl->tightQualityLevel, cl->host);
                    } else
                        debug_printf("rfbProcessClientNormalMessage: ignoring unknown "
                           "encoding type %d\n", (int)enc);
                    }
                }

                if (cl->preferredEncoding == -1) {
                cl->preferredEncoding = rfbEncodingRaw;
                }

            return;
        }


        case rfbFramebufferUpdateRequest:
        {
            //debug_printf("rfbSetEncodings!\n");
            if ((n = ReadExact(cl, ((char *)&msg) + 1,
                    sz_rfbFramebufferUpdateRequestMsg-1)) <= 0) {
              if (n != 0)
                  debug_printf("rfbProcessClientNormalMessage: read");
              rfbCloseClient(cl);
            return;
            }
            if(msg.fur.incremental==FALSE)//�ͻ��������������ݣ�������ĳЩ���ݵĶ�ʧ
            {
                cl->InitDone=FALSE;
                cl->modifiedregion.x=Swap16IfLE(msg.fur.x);
                cl->modifiedregion.y=Swap16IfLE(msg.fur.y);
                cl->modifiedregion.w=Swap16IfLE(msg.fur.w);
                cl->modifiedregion.h=Swap16IfLE(msg.fur.h);
            }
            else
            {
               cl->InitDone=TRUE;
            }

            if (!cl->readyForSetColourMapEntries) {
                /* client hasn't sent a SetPixelFormat so is using server's */
                cl->readyForSetColourMapEntries = TRUE;
                if (!cl->format.trueColour) {
                if (!rfbSendSetColourMapEntries(cl, 0, 256)) {
                //    sraRgnDestroy(tmpRegion);
                //    TSIGNAL(cl->updateCond);
                //    UNLOCK(cl->updateMutex);
                    debug_printf("failed to send the color map!");
                    return;
                }
                }
            }
            cl->HasRequest = 1;
            return;
        }

        case rfbKeyEvent:
            debug_printf("rfbKeyEvent!\n");
            cl->rfbKeyEventsRcvd++;

            if ((n = ReadExact(cl, ((char *)&msg) + 1,
                               sz_rfbKeyEventMsg - 1)) <= 0) {
                if (n != 0)
                    debug_printf("rfbProcessClientNormalMessage: read");
                rfbCloseClient(cl);
                return;
            }
            //��ӡ���ܵ��ļ���ֵ
            msg.ke.key=Swap32IfLE(msg.ke.key);
#if CN_CFG_KEYBOARD == 1
            vncclient_keyevent(cl->screen, msg);
#endif
            return;


        case rfbPointerEvent:
            cl->rfbPointerEventsRcvd++;
            if ((n = ReadExact(cl, ((char *)&msg) + 1,
                               sz_rfbPointerEventMsg - 1)) <= 0)
            {
                if (n != 0)//����0Ӧ���ǿͻ��������ر���
                    debug_printf("rfbProcessClientNormalMessage: read");
                rfbCloseClient(cl);
                return;
            }
            //����touch message
            msg.pe.x = Swap16IfLE(msg.pe.x);
            msg.pe.y = Swap16IfLE(msg.pe.y);

//          debug_printf("rfbPointerEvent!----x=%d y=%d\n", msg.pe.x,msg.pe.y);
#if CN_CFG_KEYBOARD == 1
            vncclient_pointevent(cl->screen, msg);
#endif
            return;

        case rfbClientCutText:
             debug_printf("rfbClientCutText!\n");

            if ((n = ReadExact(cl, ((char *)&msg) + 1,
                               sz_rfbClientCutTextMsg - 1)) <= 0) {
                if (n != 0)
                    debug_printf("rfbProcessClientNormalMessage: read");
                rfbCloseClient(cl);
                return;
            }

            msg.cct.length = Swap32IfLE(msg.cct.length);

            str = (char *)M_MallocLc(msg.cct.length+1, 0);

          if ((n = ReadExact(cl, str, msg.cct.length)) <= 0) {
                if (n != 0)
                   debug_printf("rfbProcessClientNormalMessage: read");
                free(str);
               rfbCloseClient(cl);
               return;
           }
    //        cl->screen->setXCutText(str, msg.cct.length, cl);
            str[msg.cct.length]= 0;
            debug_printf(" the cut messaged=%s\n",str);
            free(str);
            return;
        default:
            debug_printf("rfbProcessClientNormalMessage: unknown message type %d\n",
                    msg.type);
            debug_printf(" ... closing connection\n");
            rfbCloseClient(cl);
            return;
        }
}
// =========================================================================
// ��������:����ͻ��˺��û�����Ϣ����
// �������:
// ���������
// ����ֵ     :�������Ŀͻ�cl
// ˵��          :�����в�����⣬�������ŵ������Լ������õ�
//         : ����ͻ��˵�״̬�����ӽ������ӵ���������Ϣ����
// =========================================================================
void rfbProcessClientMessage( rfbClientPtr cl)
{
    switch (cl->state) {

    case RFB_PROTOCOL_VERSION:
        debug_printf("STATE:RFB_PROTO_VERSION---%s",cl->host);
        rfbProcessClientProtocolVersion(cl);
        return;

    case RFB_SECURITY_TYPE:
        debug_printf("STATE:RFB_SECURITY_VERSION---%s",cl->host);
        rfbProcessClientSecurityType(cl);
        return;

    case RFB_AUTHENTICATION:
        debug_printf("STATE:RFB_AUTHENTICATION---%s",cl->host);
        rfbAuthProcessClientMessage(cl);
        return;

    case RFB_INITIALISATION:
        debug_printf("STATE:RFB_INITIALISATION---%s",cl->host);
        rfbProcessClientInitMessage(cl);
        return;

    default:
        rfbProcessClientNormalMessage(cl);
        return;
    }
}
