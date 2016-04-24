// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ����ƣ�vnc_bmp.c
// ģ������:
// ģ��汾: V1.00
// ����ʱ��: 2013-7-1
// ԭʼ����: Administrator
// ����˵����
// =============================================================================
#include "rfb.h"
#include "vnc_bmp.h"
#include "debug.h"

#include <stdio.h>//djyfs�Ķ�д���У�Ϊʲô��--TODO

#define widthbytes(bits) (((bits)+31)/32*4)
//��õ�ǰͼ�����bytes����Ҫ��4�ֽڶ���
// ===========================================================================
// �������ܣ����Ժ���
// ���������
// ����ֵ    ��
// ˵��         ��
// ===========================================================================
void debug_test_show_fileheader(bitmap_fileheader *fileheader)
{
    debug_printf("file_type = %04x\n",fileheader->bfType);
    debug_printf("file_size = %08x\n",fileheader->bfSize);
    debug_printf("file_ofset = %08x\n",fileheader->bfOffBits);
}

// ===========================================================================
// �������ܣ�ʵ�ִ��ļ��ж�ȡbmpͼƬ��Ϣ����д�뵽�ڴ浱�У�
// ���������filename�ļ�����,bitmap,�洢bmpͼ����mapbits�ɴ˺�����̬����
// ���������
// ����ֵ    ��
// ˵��         ���ö��ڴ������¼ͼ��Ĵ�С��RGBֵ�Լ�����ɫ����Ϣ
//          ʵ���ϣ��ú��������ǽ�BMP��ɫ��λͼ���ļ��н�����RGB��ʽ�����ݴ�Ž�
//          �ڴ����
//          �������24λ��ɫ�ʽ���ת������
// ===========================================================================
bool_t vnc_get_image_from_bmp(const char *filename, \
        struct tagRectBitmap *bitmap)
{
    bool_t result = true;
    u16 read_size = 0;
    u16 width = 0;
    u16 height = 0;
    u16 i = 0;

    u8  *buf;//����
    u32  num;

    bitmap_fileheader  file_header;
    bitmap_infoheader  image_info;
    u32 linebytes_bmp;//4�ֽڶ����
    u32 linebytes_image;//ʵ��ͼ������
    u32 data_offset;

//
//��ȡͷ�ļ���Ϣ
//    fp = fopen(filename,"r");
    fp = fopen(filename,"r+");
    if(NULL == fp)
    {
        result = false;
        debug_printf("open the file =%s failed\n",filename);
        goto open_filefailed;
    }
    else
    {
        debug_printf("open the file =%s success\n",filename);
    }

//    fseek(fp,0,SEEK_SET);
//    num = fread(&(file_header.bfType),2,1,fp);
//    num = fread(&(file_header.bfSize),12,1,fp);
//    debug_printf("read block = %d \n",num);
//    debug_test_show_fileheader(&file_header);
//    fseek(fp,cn_bmp_fileheader_size,SEEK_SET);
//    num = fread(&image_info,40,1,fp);
//    debug_printf("read block = %d \n",num);
//    data_offset = 0;
    fseek(fp,0,SEEK_SET);
    num = fread(&(file_header.bfType),2,1,fp);
    num = fread(&(file_header.bfSize),12,1,fp);
    debug_printf("read block = %d \n",num);
    debug_test_show_fileheader(&file_header);
    fseek(fp,cn_bmp_fileheader_size,SEEK_SET);
    num = fread(&image_info,40,1,fp);
    debug_printf("read block = %d \n",num);

    data_offset = file_header.bfOffBits;
    if(image_info.biWidth > 0)
    {
        width = image_info.biWidth;
    }
    else
    {
        width  = (u16)(-image_info.biWidth);
    }
    if(image_info.biHeight > 0)
    {
        height = image_info.biHeight;

    }
    else
    {
        height = (u16)(-image_info.biHeight);
    }

    linebytes_bmp = widthbytes(width*image_info.biBitCount);
    linebytes_image = (width*image_info.biBitCount +7)/8;

    buf = M_MallocLc(linebytes_image * height, 0);
    debug_printf("malloc %d bytes  buf = %08x\n",linebytes_image * height,buf);
    bitmap->bm_bits = buf;
    if(NULL == buf)
    {
        result = false;
        debug_printf("#failed:no mem for keymap\n");
        goto malloc_failed;
    }

    bitmap->width = width;
    bitmap->height = height;
    bitmap->linebytes = linebytes_image;
    if(image_info.biBitCount == 24)//24λ��Ŀǰ��������888�����õ�ɫ��
    {
        bitmap->PixelFormat = CN_SYS_PF_RGB888;
        //һ��һ�еĶ�ȡ���ݣ��󲿷�������ͼ���ǵ�����
        if(image_info.biHeight > 0)//������bottom��top
        {
            data_offset = data_offset + (height-1)*linebytes_bmp;//��ָ���
            for(i = 0; i < height; i++)//��Ϊ��λ��ȡ����������
            {
//              fseek(fp,data_offset,SEEK_SET);
//              fread(buf,linebytes_image,1,fp);
                fseek(fp,data_offset,SEEK_SET);
                fread(buf,linebytes_image,1,fp);
                buf += linebytes_image;
                data_offset = data_offset - linebytes_bmp;
            }
        }
        else
        {
            for(i = 0; i < height; i++)//��Ϊ��λ��ȡ����������
            {
//              fseek(fp,data_offset,SEEK_SET);
//              fread(buf,linebytes_image,1,fp);
                fseek(fp,data_offset,SEEK_SET);
                fread(buf,linebytes_image,1,fp);
                buf += linebytes_image;
                data_offset = data_offset + linebytes_bmp;
            }
        }
    }

malloc_failed:
//    fclose(fp);
    fclose(fp);
open_filefailed:
    return result;
}


