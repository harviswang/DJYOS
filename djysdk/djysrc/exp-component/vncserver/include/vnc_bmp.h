// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ����ƣ�vnc_bmp.h
// ģ������: ����bmpͼ���ļ��Ĵ���
// ģ��汾: V1.00
// ����ʱ��: 2013-7-1
// ԭʼ����: Administrator
// ����˵����  ��Ҫ��Ϊ�˶Լ��̽���Ĵ���
// =============================================================================
#ifndef __VNC_BMP_H__
#define __VNC_BMP_H__

#include "stdint.h"

#define   cn_bmp_fileheader_size 14
#define   cn_bmp_mapinfoheader_size 40
//bm�ļ�ͷ��Ϣ
typedef struct _bitmap_fileheader
{
    u16 bfType;
    u32 bfSize;
    u16 bfReserved1;
    u16 bfReserved2;
    u32 bfOffBits;
}bitmap_fileheader;
//bm�ļ�ͼ����Ϣ
typedef struct _bitmap_infoheader
{
    u32 biSize;
    s32 biWidth;
    s32 biHeight;
    u16 biPlanes;
    u16 biBitCount;
    u32 biCompression;
    u32 biSizeImag;
    s32 biXPelsPerMeter;
    s32 biYPelsPerMeter;
    u32 biCLrUsed;
    u32 biClrImportant;
}bitmap_infoheader;

bool_t vnc_get_image_from_bmp(const char *filename, \
        struct tagRectBitmap *bitmap);

#endif /* __VNC_BMP_H__ */
