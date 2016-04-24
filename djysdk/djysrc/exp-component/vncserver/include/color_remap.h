// =======================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// ģ������: VNCServer
// ģ��汾: V1.00(��ʼ���汾)
// �ļ���     ��color_remap.h
// ������Ա: Administrator
// ����ʱ��: 2013-6-10
// =======================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <V1.00> <2013-3-5>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =======================================================================

#ifndef COLOR_REMAP_H_
#define COLOR_REMAP_H_

#include "rfbproto.h"

void set_client_colormap_function(rfbClientPtr cl);

void  rfbTranslate8toX(rfbPixelFormat *src,
                                    rfbPixelFormat *dst,
                                    char *iptr, char *optr,
                                    int bytesBetweenInputLines,
                                    int width, int height);

void  rfbTranslate16toX(rfbPixelFormat *src,
                                    rfbPixelFormat *dst,
                                    char *iptr, char *optr,
                                    int bytesBetweenInputLines,
                                    int width, int height);


void  rfbTranslate32toX(rfbPixelFormat *src,
                                    rfbPixelFormat *dst,
                                    char *iptr, char *optr,
                                    int bytesBetweenInputLines,
                                    int width, int height);

void  rfbColorTranslate(rfbPixelFormat *src,
                                    rfbPixelFormat *dst,
                                    char *iptr, char *optr,
                                    int bytesBetweenInputLines,
                                    int width, int height);
void rfbTranslateNone(rfbPixelFormat *in, rfbPixelFormat *out,
                 char *iptr, char *optr, int bytesBetweenInputLines,
                 int width, int height);

void showPixelFormat(rfbPixelFormat *pix);
#endif /* COLOR_REMAP_H_ */
