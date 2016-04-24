// =======================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// ģ������: VNCServer
// ģ��汾: V1.00(��ʼ���汾)
// �ļ���     ��colormap.c
// ������Ա: Administrator
// ����ʱ��: 2013-3-9
// =======================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <V1.00> <2013-3-9>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =======================================================================


#include "debug.h"
#include "rfbproto.h"
#include "colormap.h"
#include "color_remap.h"
#include "vnc_socket.h"

//RGB332�����ظ�ʽ����
static const rfbPixelFormat BGR233Format = {
    8, 8, 0, 1, 7, 7, 3, 5, 2, 0, 0, 0
};

int rfbEconomicTranslate = FALSE;
/*
 * Macro to compare pixel formats.
 */
//�궨���������ظ�ʽ�����Ƿ�һ��
#define PF_EQ(x,y)                                                      \
        ((x.bitsPerPixel == y.bitsPerPixel) &&                          \
         (x.depth == y.depth) &&                                        \
         ((x.bigEndian == y.bigEndian) || (x.bitsPerPixel == 8)) &&     \
         (x.trueColour == y.trueColour) &&                              \
         (!x.trueColour || ((x.redMax == y.redMax) &&                   \
                            (x.greenMax == y.greenMax) &&               \
                            (x.blueMax == y.blueMax) &&                 \
                            (x.redShift == y.redShift) &&               \
                            (x.greenShift == y.greenShift) &&           \
                            (x.blueShift == y.blueShift))))
/*
typedef void (*rfbInitTableFnType)(char **table, rfbPixelFormat *in,
			rfbPixelFormat *out);

*/


/*
 * rfbTranslateNone is used when no translation is required.
 */

/*
 * rfbSetClientColourMapBGR233 sets the client's colour map so that it's
 * just like an 8-bit BGR233 true colour clien*/
// =========================================================================
// ��������:���ÿͻ��˵�8λ��ɫΪ���ɫ��RGB332��ʽ��
// �������:�������Ŀͻ�cl
// ���������
// ����ֵ  :
// ˵��    :���ÿͻ��˵�8λ��ɫ��Ϣӳ�䣬����ӳ������ݷ��͹�ȥ
// =========================================================================
static int rfbSetClientColourMapBGR233(rfbClientPtr cl)
{
    char buf[sz_rfbSetColourMapEntriesMsg + 256 * 3 * 2];
    rfbSetColourMapEntriesMsg *scme = (rfbSetColourMapEntriesMsg *)buf;
    u16 *rgb = (u16 *)(&buf[sz_rfbSetColourMapEntriesMsg]);
    int i, len;
    int r, g, b;
//	unsigned char bellbuf[8];

    if (cl->format.bitsPerPixel != 8) {
        debug_printf("%s: client not 8 bits per pixel\n",
                "rfbSetClientColourMapBGR233");
        rfbCloseClient(cl);
        return FALSE;
    }

    scme->type = rfbSetColourMapEntries;

    scme->firstColour = Swap16IfLE(0);
    scme->nColours = Swap16IfLE(256);

    len = sz_rfbSetColourMapEntriesMsg;

    i = 0;

    for (b = 0; b < 4; b++) {
        for (g = 0; g < 8; g++) {
            for (r = 0; r < 8; r++)
            {
                rgb[i++] = Swap16IfLE(r * 65535 / 7);
                rgb[i++] = Swap16IfLE(g * 65535 / 7);
                rgb[i++] = Swap16IfLE(b * 65535 / 3);
                //8λӳ���RGB(332)ģʽ
            }
        }
    }

    len += 256 * 3 * 2;
    if (WriteExact(cl, buf, sz_rfbSetColourMapEntriesMsg) < 0) {
        debug_printf("rfbSetClientColourMapBGR233: write");
        rfbCloseClient(cl);
        return FALSE;
    }
	if (WriteExact(cl, buf+sz_rfbSetColourMapEntriesMsg, 256*3*2) < 0) {
        debug_printf("rfbSetClientColourMapBGR233: write");
        rfbCloseClient(cl);
        return FALSE;
    }

    return TRUE;
}
// =========================================================================
// ��������:ӳ��ͻ��˵�color
// �������:�������Ŀͻ�cl
// ���������
// ����ֵ    :
// ˵��         :���ͻ��˵ķ���setpixelformat��Ϣʱ��������õ�cl����ɫ�������ɫʱ��
//          ��������������������������ֿͻ��˵���ɫ���͹�ȥ���Ա�ͻ��˽���ӳ��
// =========================================================================
int rfbSetTranslateFunction(rfbClientPtr cl)
{
    debug_printf("u32 format for client %s:\n",cl->host);
   // PrintPixelFormat(&cl->format);

    /*
     * Check that bits per pixel values are valid
     */
//�ͻ��˺ͷ���������֧��24λģʽ�������
    if ((cl->screen->rfbServerFormat.bitsPerPixel != 8) &&
        (cl->screen->rfbServerFormat.bitsPerPixel != 16) &&
        (cl->screen->rfbServerFormat.bitsPerPixel != 32))
    {
        debug_printf("%s: server bits per pixel not 8, 16 or 32\n",
                "rfbSetTranslateFunction");
        rfbCloseClient(cl);
        return FALSE;
    }

    if ((cl->format.bitsPerPixel != 8) &&
        (cl->format.bitsPerPixel != 16) &&
        (cl->format.bitsPerPixel != 32))
    {
        debug_printf("%s: client bits per pixel not 8, 16 or 32\n",
                "rfbSetTranslateFunction");
        rfbCloseClient(cl);
        return FALSE;
    }

    if (!cl->format.trueColour && (cl->format.bitsPerPixel != 8)) {
        debug_printf("rfbSetTranslateFunction: client has colour map "
                "but %d-bit - can only cope with 8-bit colour maps\n",
                cl->format.bitsPerPixel);
        rfbCloseClient(cl);
        return FALSE;
    }

    /*
     * bpp is valid, now work out how to translate
     */
    if (!cl->format.trueColour) {
        /*
         * truecolour -> color map
         *
         * Set client's colour map to BGR233, then effectively it's
         * truecolour as well
         */
		debug_printf("--Begin to send the color entry to %s\n",cl->host);
        if (!rfbSetClientColourMapBGR233(cl))//��colorӳ����͸�client
            return FALSE;

        //cl->format = BGR233Format;
    }


//����cl��ӳ�亯��
    if (PF_EQ(cl->format,cl->screen->rfbServerFormat))
    {

        /* client & server the same */
        debug_printf("no translation needed\n");
        cl->translateFn = rfbTranslateNone;
        return TRUE;
    }
    else
    {
    	set_client_colormap_function(cl);
    }
   //���������ͺͿͻ��˵����ظ�ʽ��һ��ʱ������Ҫ����ת���������Ų�����Ϊ�������Ϳͻ��˵�
    //���ظ�ʽ��һ������������

	debug_printf("--set translate function end!\n");
    return TRUE;
}




