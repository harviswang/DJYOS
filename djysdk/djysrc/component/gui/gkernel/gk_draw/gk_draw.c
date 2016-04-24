//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. As a constituent part of djyos,do not transplant it to other software
//    without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������
// 3. �������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ�������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------

//����ģ��: gui
//����:  ������.
//�汾��V1.0.0
//�ļ�����: ����Ļ������صĲ���
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2010-03-30
//   ����:  ������.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "align.h"
#include "stdlib.h"
#include "string.h"
#include "gkernel.h"
#include "gk_display.h"
#include "..\include\gk_syscall.h"
#include "..\include\gk_win.h"
#include "..\include\gk_draw.h"
#include "..\include\gk_clip.h"
#include "font.h"

#define __gk_swap(a, b){a^=b; b^=a; a^=b;}

#define MAX(x,y)    (x>y?x:y)
#define MIN(x,y)    (x<y?x:y)
//----��դ�������1------------------------------------------------------------
//����: ���ָ���Ĺ�դ�������Ƿ���ҪĿ��λͼ�μ����㡣
//����: rop_code����Ԫ/��Ԫ��դ������
//����: true=��Ҫ��false=����Ҫ
//ע: �ж��߼����ʽ(((fore_code & CN_ROP_D)>>1) == (fore_code & ~CN_ROP_D))��
//    ���壬̫����ע���޷�д�£���ο��������׵��顣
//-----------------------------------------------------------------------------
bool_t __GK_RopIsNeedDestination(u32 RopCode)
{
    u8 rop2;
    if(RopCode & CN_ALPHA_MODE_NEED_DST)
        return true;
    else
    {
        rop2 = (RopCode & CN_ROP_ROP2_MSK) >> CN_ROP_ROP2_OFF;
        if((rop2 == 0) || (rop2 == 3) || (rop2 == 12) || (rop2 == 15) )
            return false;
        else
            return true;
    }
}

//----���ظ�ʽת����rgb24------------------------------------------------------
//����: ��ɫ��ʽת������һ�����ظ�ʽ��ɫת����ERGB8888�����λͼ��ʽ�������
//      alphaͨ�������ȼ���alpha��
//����: PixelFormat�����ظ�ʽ
//      color��Դɫ��
//      ExColor,˫���ܲ��������PixelFormat=CN_SYS_PF_GRAY1~8������PixelFormat=
//          CN_SYS_PF_ALPHA1~8����ExColor��ʾ��ɫ��
//          ���PixelFormat=CN_SYS_PF_PALETTE1,��ExColor��ʾ��ɫ��ָ�롣
//          ���PixelFormat����Ҫ�˲������ɸ�0��NULL.
//����: ���ɫ��ʽ
//-----------------------------------------------------------------------------
u32 GK_ConvertColorToRGB24(u16 PixelFormat,u32 color,ptu32_t ExColor)
{
    u32 a,r,g,b;
    switch(PixelFormat)
    {
        case CN_SYS_PF_RGB565:
        {
            r = (color>>8) & 0xf8;
            g = (color>>3) & 0xfc;
            b = (color<<3) & 0xf8;
        }break;
        case CN_SYS_PF_RGB332:
        {
            r = color & 0xe0;
            g = (color<<3) & 0xe0;
            b = (color<<6) & 0xc0;
        }break;
        case CN_SYS_PF_RGBE4444:
        {
            r = (color>>8) & 0xf0;
            g = (color>>4) & 0xf0;
            b = color & 0xf0;
        }break;
        case CN_SYS_PF_ERGB4444:
        {
            r = (color>>4) & 0xf0;
            g = color & 0xf0;
            b = (color<<4) & 0xf0;
        }break;
        //5551��ʽ�ܱ�Ť����ʱ���ε�������Ҫʱ�ٸ���ʵ�ʸ�ʽ���ӡ�
//        case CN_SYS_PF_RGBC5551:
//        {
//            u32 c = (color & 1)<<2;
//            r = ((color>>8) & 0xf8) + c;
//            g = ((color>>3) & 0xf8) + c;
//            b = ((color<<2) & 0xfc);
//        }break;
        case CN_SYS_PF_ERGB6666:
        {
            r = (color>>10) & 0xfc;
            g = (color>>4) & 0xfc;
            b = (color<<2) & 0xfc;
        }break;
        case CN_SYS_PF_E12RGB666:
        {
            r = (color>>10) & 0xfc;
            g = (color>>4) & 0xfc;
            b = (color<<2) & 0xfc;
        }break;
        case CN_SYS_PF_RGB101010:
        {
            r = (color>>22) & 0xff;
            g = (color>>12) & 0xff;
            b = (color>>2) & 0xff;
        }break;
        case CN_SYS_PF_RGB888:
        {
            return color;
        }break;
        case CN_SYS_PF_ERGB8888:
        {
            return color;
        }break;
        case CN_SYS_PF_ARGB1555:
        {
            if(color & 0x8000)
            {
                r = (color>>7) & 0xf8;
                g = (color>>2) & 0xf8;
                b = (color<<3) & 0xf8;
            }
            else
            {
                return 0;
            }
        }break;
        case CN_SYS_PF_RGBA5551:
        {
            if(color & 1)
            {
                r = (color>>8) & 0xf8;
                g = (color>>3) & 0xf8;
                b = (color<<2) & 0xf8;
            }
            else
            {
                return 0;
            }
        }break;
        case CN_SYS_PF_ARGB8888:
        {
            a = (color>>24) & 0xff;
            r = ((color>>16) & 0xff) *a / 255;
            g = ((color>>8) & 0xff) *a / 255;
            b = ((color>>0) & 0xff) *a / 255;
        }break;
        case CN_SYS_PF_GRAY1:
        case CN_SYS_PF_ALPHA1:
        {
            return color * ExColor;
        }break;
        case CN_SYS_PF_GRAY2:
        case CN_SYS_PF_ALPHA2:
        {
            r = (((ExColor>>16) & 0xff) * color) / 3;
            g = (((ExColor>>8) & 0xff) * color) / 3;
            b = ((ExColor & 0xff) * color) / 3;
        }break;
        case CN_SYS_PF_GRAY4:
        case CN_SYS_PF_ALPHA4:
        {
            r = (((ExColor>>16) & 0xff) * color) / 15;
            g = (((ExColor>>8) & 0xff) * color) / 15;
            b = ((ExColor & 0xff) * color) / 15;
        }break;
        case CN_SYS_PF_GRAY8:
        case CN_SYS_PF_ALPHA8:
        {
            r = (((ExColor>>16) & 0xff) * color) / 255;
            g = (((ExColor>>8) & 0xff) * color) / 255;
            b = ((ExColor & 0xff) * color) / 255;
        }break;
        case CN_SYS_PF_PALETTE1:
        {
            return ((u32*)ExColor)[color];
        }break;
        case CN_SYS_PF_PALETTE2:
        {
            return ((u32*)ExColor)[color];
        }break;
        case CN_SYS_PF_PALETTE4:
        {
            return ((u32*)ExColor)[color];
        }break;
        case CN_SYS_PF_PALETTE8:
        {
            return ((u32*)ExColor)[color];
        }break;
        default:
        {
            return color;
        }break;
    }
    return (r<<16) | (g<<8) | b;
}

//----rgb24ת��������----------------------------------------------------------
//����: ��һ��ERGB8888��ʽ����ɫת���ɱ��ظ�ʽ�����ظ�ʽ��������alphaͨ����Ҳ��
//      �����ǵ�ɫ��λͼ��
//����: PixelFormat��Ŀ���ʽ
//      color��Դɫ��
//����: PixelFormat��ʽ��ɫ��
//ע: ��֧�ֵ�ɫ���ʽ��ֱ�ӷ���color
//    �Ҷ�ͼ���㷨������color��HLSɫ��ģ�͵����ȡ�
//-----------------------------------------------------------------------------
u32 GK_ConvertRGB24ToPF(u16 PixelFormat,u32 color)
{
    u32 r,g,b;
    //ת�����̲���Ҫargb�ֽ����ӵ�ת�����ڴ�switch��ɣ������Ч��
    switch(PixelFormat)
    {
        case CN_SYS_PF_RGB888:
        case CN_SYS_PF_ERGB8888:
        case CN_SYS_PF_ARGB8888:
        case CN_SYS_PF_ARGB1555:
        case CN_SYS_PF_RGBA5551:
        //����4����ʽ���������������ڸ�ʽ��ʵ����Ҳ�������������ΪĿ��ı任
        case CN_SYS_PF_PALETTE1:
        case CN_SYS_PF_PALETTE2:
        case CN_SYS_PF_PALETTE4:
        case CN_SYS_PF_PALETTE8:
        {
            return color;
        }break;
        default: break;
    }
    r = (color>>16) &0xff;
    g = (color>>8) & 0xff;
    b = color & 0xff;
    switch(PixelFormat)
    {
        case CN_SYS_PF_RGB565:
        {
            return ((r>>3)<<11) | ((g>>2)<<5) | (b>>3);
        }break;
        case CN_SYS_PF_RGB332:
        {
            return ((r>>5)<<5) | ((g>>5)<<2) | (b>>6);
        }break;
        case CN_SYS_PF_RGBE4444:
        {
            return ((r>>4)<<12) | ((g>>4)<<8) | ((b>>4)<<4);
        }break;
        case CN_SYS_PF_ERGB4444:
        {
            return ((r>>4)<<8) | ((g>>4)<<4) | (b>>4);
        }break;
        //5551��ʽ�ܱ�Ť����ʱ���ε�������Ҫʱ�ٸ���ʵ�ʸ�ʽ���ӡ�
//        case CN_SYS_PF_RGBC5551:    //ʵ���϶��������λ
//        {
//            return ((r>>3)<<11) | ((g>>3)<<6) | ((b>>3)<<1);
//        }break;
        case CN_SYS_PF_ERGB6666:
        {
            return ((r>>2)<<12) | ((g>>2)<<6) | (b>>2);
        }break;
        case CN_SYS_PF_E12RGB666:
        {
            return ((r>>2)<<12) | ((g>>2)<<6) | (b>>2);
        }break;
        case CN_SYS_PF_RGB101010:
        {
            return (r<<20) | (g<<10) | b;
        }break;
        case CN_SYS_PF_GRAY1:
        case CN_SYS_PF_ALPHA1:
        {
            //����rgbֵ����hlsģ�͵�����
            return (((MAX(MAX(r,g),b)+MIN(MIN(r,g),b))*1) + 255 )/(2*255);
        }break;
        case CN_SYS_PF_GRAY2:
        case CN_SYS_PF_ALPHA2:
        {
            //����rgbֵ����hlsģ�͵�����
            return (((MAX(MAX(r,g),b)+MIN(MIN(r,g),b))*3) + 255 )/(2*255);
        }break;
        case CN_SYS_PF_GRAY4:
        case CN_SYS_PF_ALPHA4:
        {
            //����rgbֵ����hlsģ�͵�����
            return (((MAX(MAX(r,g),b)+MIN(MIN(r,g),b))*15) + 255 )/(2*255);
        }break;
        case CN_SYS_PF_GRAY8:
        case CN_SYS_PF_ALPHA8:
        {
            //����rgbֵ����hlsģ�͵�����
            return (((MAX(MAX(r,g),b)+MIN(MIN(r,g),b))*255) + 255 )/(2*255);
        }break;
        default:
        {
            return color;
        }
        break;
    }
    return color;
}
//----������ɫ��ʽ����໥ת��-------------------------------------------------
//����: ������ɫ��ʽ����໥ת����Ŀ���ʽ��������alphaͨ����Ҳ�������ǵ�ɫ��λ
//      ͼ�����Դ��ʽ��alphaͨ���Ļ�������ɫ��ֵ��˺���ת����
//����: dpf_type��Ŀ���ʽ
//      spf_type��Դ��ʽ
//      color��Դɫ�ʣ�Դpf_type��ʽ��
//      ExColor,Դɫ�ʵ�˫���ܲ��������spf_type=CN_SYS_PF_GRAY1~8������spf_type=
//          CN_SYS_PF_ALPHA1~8����ExColor��ʾ��ɫ��
//      ���spf_type=CN_SYS_PF_PALETTE1,��ExColor��ʾ��ɫ��ָ�롣
//����: dpf_type��ʽ��ɫ��
//-----------------------------------------------------------------------------
u32 GK_ConvertPF2PF(u16 dpf_type,u16 spf_type,u32 color,ptu32_t ExColor)
{
    u32 result;
    //Դ��ʽ��Ŀ���ʽ��ͬ��ֱ�ӷ���Դɫ��
    if(spf_type == dpf_type)
        return color;
    //��Դɫ��ת�������ɫ����alphaͨ���Ļ�������֮��
    result = GK_ConvertColorToRGB24(spf_type,color,ExColor);
    //�����ɫת����Ŀ���ʽ����ɫ
    result = GK_ConvertRGB24ToPF(dpf_type,result);
    return result;
}
//----rop2���-----------------------------------------------------------------
//����: ����rop2ɫ�ʻ�ϡ�
//����: dest��������ɫ
//      PenColor��������ɫ
//      rop2_code����Ԫ��դ������
//����: ��Ϻ��ɫ��
//-----------------------------------------------------------------------------
u32 GK_BlendRop2(u32 dest,u32 PenColor,u32 Rop2Code)
{
    switch(Rop2Code)
    {
        case CN_R2_BLACK:         // ��Ϻ����ɫΪ��ɫ(black)
        {
            return CN_COLOR_BLACK;
        }break;
        case CN_R2_NOTMERGEPEN:   //��Ϻ����ɫΪ~(dest | PenColor)
        {
            return ~(dest | PenColor);
        }break;
        case CN_R2_MASKNOTPEN:    //��Ϻ����ɫΪ~PenColor & dest
        {
            return ~PenColor & dest;
        }break;
        case CN_R2_NOTCOPYEN:    //��Ϻ����ɫΪ~PenColor
        {
            return ~PenColor;
        }break;
        case CN_R2_MASKPENNOT:    //��Ϻ����ɫΪ~dest & PenColor
        {
            return ~dest & PenColor;
        }break;
        case CN_R2_NOT:           //��Ϻ����ɫΪ~dest
        {
            return ~dest;
        }break;
        case CN_R2_XORPEN :        //��Ϻ����ɫΪdest ^ PenColor
        {
            return dest ^ PenColor;
        }break;
        case CN_R2_NOTMASKPEN:    //��Ϻ����ɫΪ~(dest & PenColor)
        {
            return ~(dest & PenColor);
        }break;
        case CN_R2_MASKPEN:       //��Ϻ����ɫΪdest & PenColor
        {
            return dest & PenColor;
        }break;
        case CN_R2_NOTXORPEN:     //��Ϻ����ɫΪ~(dest ^ PenColor)
        {
            return ~(dest ^ PenColor);
        }break;
        case CN_R2_NOP:           //��Ϻ����ɫΪdest��ԭɫ��
        {
            return dest;
        }break;
        case CN_R2_MERGENOTPEN:   //��Ϻ����ɫΪ~PenColor | dest
        {
            return ~PenColor | dest;
        }break;
        case CN_R2_COPYPEN:       //��Ϻ����ɫΪpen������ɫ��
        {
            return PenColor;
        }break;
        case CN_R2_MERGEPENNOT:   //��Ϻ����ɫΪ~dest | PenColor
        {
            return ~dest | PenColor;
        }break;
        case CN_R2_MERGEPEN:      //��Ϻ����ɫΪdest | PenColor
        {
            return dest | PenColor;
        }break;
        case CN_R2_WHITE:         //��Ϻ����ɫΪ��ɫ(white)
        {
            return CN_COLOR_WHITE ;
        }break;
        default:
        {
            return PenColor;
            break;
        }
    }
    return 0;
}
//----�ж�Ŀ������ڵ�����-----------------------------------------------------
//����: �ж�Ŀ������ڵ����򣬷ֳɾ���������ҡ��ϡ����Ĳ��֡�
//����: x��y��Ŀ���,��Դ��ڵ�����
//      limit��������,��Դ��ڵ�����
//����: ��־��c��c = 0��ʾĿ����ھ������ڣ������ھ�������
//-----------------------------------------------------------------------------
u32 __GK_PointSect(s32 x,s32 y,struct tagRectangle *limit)  //ȷ��
{
    s32 c;
    c = 0;

    if(x < limit->left)                 //�ж�Ŀ����Ƿ��ھ�������
        c |= 0x01;
    else
    {
        if(x > limit->right)            //�ж�Ŀ����Ƿ��ھ������ҷ�
        c |= 0x02;
    }
    if(y < limit->top)                  //�ж�Ŀ����Ƿ��ھ������Ϸ�
        c |= 0x04;
    else
    {
        if(y > limit->bottom)           //�ж�Ŀ����Ƿ��ھ������·�
        c |= 0x08;
    }
    return c;
}
//----��ȡֱ�߶��������Ľ���-------------------------------------------------
//����: ��ȡֱ�߶��������Ľ��㡣
//����: limit��������,��Դ��ڵ�����
//      x1��y1��x2��y2�����յ�,��Դ��ڵ�����
//����: ��־��flag��flag = 0��ʾб�߲���limit�ڣ�flag = 1��ʾб���е���limit��
//˵��: �˴���ֱ�߶������б�߶ε�
//-----------------------------------------------------------------------------
u32 __GK_OlineSectInter(struct tagRectangle *limit,s32 *x1,s32 *y1,s32 *x2,s32 *y2)//ȷ��
{
    s32 c1,c2,i=0;
    s32 flag = 1;

    c1 = __GK_PointSect(*x1,*y1,limit);
    c2 = __GK_PointSect(*x2,*y2,limit);

    if((c1 == 0)&&(c2 == 0))        //б����limit��
        return flag;
    else if((c1&c2) != 0)           //б����limit�⣬�����߶���limit��һ���߽���
    {
        flag = 0;
        return flag;
    }
    else
    {
        if(c1 == 0)                 //��(x1,y1)��limit�ڣ���(x2,y2)�϶�����
        {
            do
            {
                if((c2&0x01) != 0)
                {
                    *y2 = *y1+(*y1-*y1)*(limit->left-*x1)/(*x1-*x1);
                    *x2 = limit->left;
                    c2 = __GK_PointSect(*x2,*y2,limit);
                }
                else if((c2&0x02) != 0)
                {
                    *y2 = *y1+(*y2-*y1)*(limit->right-*x1)/(*x2-*x1);
                    *x2 = limit->right;
                    c2 = __GK_PointSect(*x2,*y2,limit);
                }
                else if((c2&0x04) != 0)
                {
                    *x2 = *x1+(*x2-*x1)*(limit->top-*y1)/(*y2-*y1);
                    *y2 = limit->top;
                    c2 = __GK_PointSect(*x2,*y2,limit);
                }
                else
                {
                    *x2 = *x1+(*x2-*x1)*(limit->bottom-*y1)/(*y2-*y1);
                    *y2 = limit->bottom;
                    c2 = __GK_PointSect(*x2,*y2,limit);
                }
            }while(c2 != 0);
            return flag;
        }
        else                    //��(x1,y1)����limit�ڣ���(x2,y2)������limit��
        {                       //Ҳ������limit��
            do{
                if((c1&0x01) != 0)
                {
                    *y1 = *y1+(*y2-*y1)*(limit->left-*x1)/(*x2-*x1);
                    *x1 = limit->left;
                    c1 = __GK_PointSect(*x1,*y1,limit);
                    i++;
                }
                else if((c1&0x02) != 0)
                {
                    *y1 = *y1+(*y2-*y1)*(limit->right-*x1)/(*x2-*x1);
                    *x1 = limit->right;
                    c1 = __GK_PointSect(*x1,*y1,limit);
                    i++;
                }
                else if((c1&0x04) != 0)
                {
                    *x1 = *x1+(*x2-*x1)*(limit->top-*y1)/(*y2-*y1);
                    *y1 = limit->top;
                    c1 = __GK_PointSect(*x1,*y1,limit);
                    i++;
                }
                else
                {
                    if((c1&0x08) != 0)
                    {
                        *x1 = *x1+(*x2-*x1)*(limit->bottom-*y1)/(*y2-*y1);
                        *y1 = limit->bottom;
                        c1 = __GK_PointSect(*x1,*y1,limit);
                        i++;
                    }
                }
                //ֱ�ߺ�limit�н���ʱ��һ���˵���������ν���
                if(i>2)
                {
                    flag = 0;
                    return flag;
                }
            }while(c1 != 0);
            if(c2 == 0)
                return flag;
            else
            {
                do
                {
                    if((c2&0x01) != 0)
                    {
                        *y2 = *y1+(*y2-*y1)*(limit->left-*x1)/(*x2-*x1);
                        *x2 = limit->left;
                        c2 = __GK_PointSect(*x2,*y2,limit);
                    }
                    else if((c2&0x02) != 0)
                    {
                        *y2 = *y1+(*y2-*y1)*(limit->right-*x1)/(*x2-*x1);
                        *x2 = limit->right;
                        c2 = __GK_PointSect(*x2,*y2,limit);
                    }
                    else if((c2&0x04) != 0)
                    {
                        *x2 = *x1+(*x2-*x1)*(limit->top-*y1)/(*y2-*y1);
                        *y2 = limit->top;
                        c2 = __GK_PointSect(*x2,*y2,limit);
                    }
                    else
                    {
                        *x2 = *x1+(*x2-*x1)*(limit->bottom-*y1)/(*y2-*y1);
                        *y2 = limit->bottom;
                        c2 = __GK_PointSect(*x2,*y2,limit);
                    }
                }while(c2 != 0);
                return flag;
            }
        }
    }
}

//----��ȡλͼ�е�����---------------------------------------------------------
//����: ��һ��bitmap��ȡ���أ���bitmap�����ظ�ʽ������δ���κ�ת����
//����: bitmap��Ŀ��λͼ
//      x��y��Ŀ�����ص�����
//����: ������ɫֵ,ֱ�Ӷ�ȡ��������ʽת������ʹ�Ҷ�ͼ���ɫ���ʽ��Ҳû��ת����
//      ��ʵɫ��
//-----------------------------------------------------------------------------
u32 GK_GetPixelBm(struct tagRectBitmap *bitmap, s32 x,s32 y)
{
    u8 color_bits,bit_offset;
    u32 color=0,byte_offset;
	if(bitmap == NULL)
	    return 0;
    color_bits = (bitmap->PixelFormat & CN_PF_BITWIDE_MSK) >> 8;
    //����λͼ��ɫ��ʽ��ȡ����
    switch(color_bits)
    {
        case 1:
        {
            //���������Ӧ���ֽ�ƫ����
            byte_offset = y*bitmap->linebytes + x/8;
            //������������Ӧ�ֽ��е�λƫ��
            bit_offset = 7-x%8;     //���λ��ʾ����ߣ����λ���ұ�
            //ȡ������ɫ
            color = (u32)(bitmap->bm_bits[byte_offset]>>bit_offset)&0x1;
        }break;
        case 2:
        {
            byte_offset = y*bitmap->linebytes + x/4;
            bit_offset = (3-x%4)<<1;     //���λ��ʾ����ߣ����λ���ұ�
            color = (u32)(bitmap->bm_bits[byte_offset]>>bit_offset)&0x3;
        }break;
        case 4:
        {
            byte_offset = y*bitmap->linebytes + x/2;
            bit_offset = (1-x%2)<<2;     //���λ��ʾ����ߣ����λ���ұ�
            color = (u32)(bitmap->bm_bits[byte_offset]>>bit_offset)&0xf;
        }break;
        case 8:
        {
            byte_offset = y*bitmap->linebytes + x;
            color = (u32)bitmap->bm_bits[byte_offset];
        }break;
        case 12:
        {
            byte_offset = y*bitmap->linebytes + x*3/2;
            bit_offset = (x*12)%8;
            //ע:��������⣬���ɰ�Ŀ���ַǿ��ת����16λָ�롣
            if(bit_offset == 0)
            {
                color = (u32)bitmap->bm_bits[byte_offset]+
                        (u32)((bitmap->bm_bits[byte_offset+1]&0xf)<<8);
            }
            else
            {
                color = (u32)((bitmap->bm_bits[byte_offset]&0xf0)>>4)+
                        (u32)(bitmap->bm_bits[byte_offset+1]<<4);
            }
        }break;
        case 16:
        {
            byte_offset = y*bitmap->linebytes + x*2;
            color = (u32)(bitmap->bm_bits[byte_offset]
                    |(bitmap->bm_bits[byte_offset+1]<<8));
        }break;
        case 24:
        {
            byte_offset = y*bitmap->linebytes + x*3;
            color = (u32)bitmap->bm_bits[byte_offset];
            color += (u32)bitmap->bm_bits[byte_offset+1]<<8;
            color += (u32)bitmap->bm_bits[byte_offset+2]<<16;
        }break;
        case 32:
        {
            byte_offset = y*bitmap->linebytes + x*4;
            color = (u32)(*(u32*)(&bitmap->bm_bits[byte_offset]));
        }break;
        default:
        break;
    }
    return color;
}
//----��ȡĿ����ɫ��argb����---------------------------------------------------
//����: ��ȡĿ����ɫ��argb�����������ظ�ʽ����Ҫ��λ������û��ת����ʡ�
//����: PixelFormat��Ŀ����ɫ��ɫ��ʽ
//      color��Ŀ����ɫ
//      a��r��g��b��Ŀ����ɫ��a��r��g��b����
//      ExColor,˫���ܲ��������PixelFormat=CN_SYS_PF_GRAY1~8������PixelFormat=
//          CN_SYS_PF_ALPHA1~8����ExColor��ʾ��ɫ��
//          ���PixelFormat=CN_SYS_PF_PALETTE1,��ExColor��ʾ��ɫ��ָ�롣
//          ���PixelFormat����Ҫ�˲������ɸ�0��NULL.
//����: �ޣ�
//ע: ����ǻҶȻ��ɫ���ʽ����ת��ΪRGB24��ȡ��������a(alpha)����Ϊ0
//-----------------------------------------------------------------------------
void __GK_GetRgbBm(u16 PixelFormat,u32 color,u32 *a,u32 *r,
                                            u32 *g,u32 *b,ptu32_t ExColor)
{
    u32 rgb32,lr,lg,lb;
    //����λͼ��ɫ��ʽ����Ŀ����ɫ�е�rgb����
    switch(PixelFormat)
    {
        case CN_SYS_PF_RGB565:
        {
            *r = (color>>11) & 0x1f;
            *g = (color>>5) & 0x3f;
            *b = color & 0x1f;
        }break;
        case CN_SYS_PF_RGB332:
        {
            *r = (color>>5) & 0x07;
            *g = (color<<2) & 0x07;
            *b = color & 0x03;
        }break;
        case CN_SYS_PF_RGBE4444:
        {
            *r = (color>>12) & 0x0f;
            *g = (color>>8) & 0x0f;
            *b = (color>>4) & 0x0f;
        }break;
        case CN_SYS_PF_ERGB4444:
        {
            *r = (color>>8) & 0x0f;
            *g = (color>>4) & 0x0f;
            *b = color & 0x0f;
        }break;
        //5551��ʽ�ܱ�Ť����ʱ���ε�������Ҫʱ�ٸ���ʵ�ʸ�ʽ���ӡ�
//        case CN_SYS_PF_RGBC5551:
//        {
//            u32 c;
//            c = (color & 1);
//            *r = (((color>>10) & 0x3e)+c);
//            *g = (((color>>5) & 0x3e)+c);
//            *b = color & 0x3f;
//        }break;
        case CN_SYS_PF_ERGB6666:
        {
            *r = (color>>12) & 0x3f;
            *g = (color>>6) & 0x3f;
            *b = color & 0x3f;
        }break;
        case CN_SYS_PF_E12RGB666:
        {
            *r = (color>>12) & 0x3f;
            *g = (color>>6) & 0x3f;
            *b = color & 0x3f;
        }break;
        case CN_SYS_PF_RGB101010:
        {
            *r = (color>>20) & 0x3ff;
            *g = (color>>10) & 0x3ff;
            *b = color & 0x3ff;
        }break;
        case CN_SYS_PF_RGB888:
        {
            *r = (color>>16) & 0xff;
            *g = (color>>8) & 0xff;
            *b = color & 0xff;
        }break;
        case CN_SYS_PF_ERGB8888:
        {
            *r = (color>>16) & 0xff;
            *g = (color>>8) & 0xff;
            *b = color & 0xff;
        }break;

        case CN_SYS_PF_ARGB1555:
        {
            *a = (color>>16) & 0x01;
            *r = (color>>10) & 0x1f;
            *g = (color>>5) & 0x1f;
            *b = color & 0x1f;
        }break;
        case CN_SYS_PF_RGBA5551:
        {
            *a = (color) & 0x01;
            *r = (color>>11) & 0x1f;
            *g = (color>>6) & 0x1f;
            *b = (color>>1) & 0x1f;
        }break;
        case CN_SYS_PF_ARGB8888:
        {
            *a = (color>>24) & 0xff;
            *r = (color>>16) & 0xff;
            *g = (color>>8) & 0xff;
            *b = color & 0xff;
        }break;
        case CN_SYS_PF_GRAY1:
        case CN_SYS_PF_ALPHA1:
        {
            rgb32 = color * ExColor;
            *r = (rgb32>>16) & 0xff;
            *g = (rgb32>>8) & 0xff;
            *b = rgb32 & 0xff;
        }break;
        case CN_SYS_PF_GRAY2:
        case CN_SYS_PF_ALPHA2:
        {
            lr = (ExColor>>16) & 0xff;
            lg = (ExColor>>8) & 0xff;
            lb = ExColor & 0xff;
            *r = (lr * color) / 3;
            *g = (lg * color) / 3;
            *b = (lb * color) / 3;
        }break;
        case CN_SYS_PF_GRAY4:
        case CN_SYS_PF_ALPHA4:
        {
            lr = (ExColor>>16) & 0xff;
            lg = (ExColor>>8) & 0xff;
            lb = ExColor & 0xff;
            *r = (lr * color) / 15;
            *g = (lg * color) / 15;
            *b = (lb * color) / 15;
        }break;
        case CN_SYS_PF_GRAY8:
        case CN_SYS_PF_ALPHA8:
        {
            lr = (ExColor>>16) & 0xff;
            lg = (ExColor>>8) & 0xff;
            lb = ExColor & 0xff;
            *r = (lr * color) / 255;
            *g = (lg * color) / 255;
            *b = (lb * color) / 255;
        }break;
        case CN_SYS_PF_PALETTE1:
        case CN_SYS_PF_PALETTE2:
        case CN_SYS_PF_PALETTE4:
        case CN_SYS_PF_PALETTE8:
        {
            lr = ((((u32*)ExColor)[color])>>16) & 0xff;
            lg = ((((u32*)ExColor)[color])>>8) & 0xff;
            lb = (((u32*)ExColor)[color]) & 0xff;
            *r = (lr * color) / 255;
            *g = (lg * color) / 255;
            *b = (lb * color) / 255;
        }break;
        default:
        {
            *a = 0;
            *r = 0;
            *g = 0;
            *b = 0;
        }
        break;
    }
}
//----����alpha���-------------------------------------------------------
//����: ������������alpha���ֵ,����color,RGB�ֱ����,ת����dst�����ظ�ʽ����.
//      dst�ĸ�ʽ�����ɫ�����ͣ���Ϻ�ķ���ֵ��RGB24��ʽ�ģ���Ϊ��Ϻ����ɫ��
//          �ܲ�����ɫ���С�
//      dst�ĸ�ʽ����ǻҶ�ͼ����Ϻ����ɫ�ټ������������
//����: SrcColor,DstColor������ϵ�ɫ��
//      PixelFormat, ɫ�ʵĸ�ʽ
//      ExColor,˫���ܲ��������PixelFormat=CN_SYS_PF_GRAY1~8������PixelFormat=
//          CN_SYS_PF_ALPHA1~8����ExColor��ʾ��ɫ��
//          ���PixelFormat=CN_SYS_PF_PALETTE1,��ExColor��ʾ��ɫ��ָ�롣
//          ���PixelFormat����Ҫ�˲������ɸ�0��NULL.
//      Asrc��Adst��src �� dst alpha,0~255
//      AlphaMode,alpha���ģʽ���μ� CN_ALPHA_MODE_MSK ����ض���
//����: ��Ϻ����ɫ
//-----------------------------------------------------------------------------
u32 __GK_AlphaBlendBitmapToWin(u32 SrcColor,u32 DstColor, u16 PixelFormat,
                  ptu32_t dst_ExColor,u32 Asrc,u32 Adst,u32 AlphaMode)
{
    u32 a,r,g,b;
    u32 a_src=0,r_src,g_src,b_src;
    u32 a_dst=0,r_dst,g_dst,b_dst;
    u32 result;
    //��ȡĿ��λͼ��ɫ��rgb����
    __GK_GetRgbBm(PixelFormat,SrcColor,&a_dst,&r_dst,&g_dst,&b_dst,dst_ExColor);
    //��ȡԴλͼ��ɫ��rgb����
    __GK_GetRgbBm(PixelFormat,DstColor,&a_src,&r_src,&g_src,&b_src,dst_ExColor);
    //����Ŀ��λͼ��ɫ��ʽ��ȡ��Ϻ����ɫ
    switch(AlphaMode)
    {
        case CN_ALPHA_MODE_AsAdN:
        {
            a = a_src*Asrc/255+a_dst*(255-Asrc)*Adst/255;
            r = r_src*Asrc/255+r_dst*(255-Asrc)*Adst/255;
            g = g_src*Asrc/255+g_dst*(255-Asrc)*Adst/255;
            b = b_src*Asrc/255+b_dst*(255-Asrc)*Adst/255;
        }break;
        case CN_ALPHA_MODE_AsAd:
        {
            a = a_src*Asrc/255+a_dst*Adst/255;
            r = r_src*Asrc/255+r_dst*Adst/255;
            g = g_src*Asrc/255+g_dst*Adst/255;
            b = b_src*Asrc/255+b_dst*Adst/255;
        }break;
        case CN_ALPHA_MODE_As:
        {
            a = a_src*Asrc/255;
            r = r_src*Asrc/255;
            g = g_src*Asrc/255;
            b = b_src*Asrc/255;
        }break;
        case CN_ALPHA_MODE_AsN:
        default:
        {
            a = a_src*Asrc/255+a_dst*(255-Asrc)/255;
            r = r_src*Asrc/255+r_dst*(255-Asrc)/255;
            g = g_src*Asrc/255+g_dst*(255-Asrc)/255;
            b = b_src*Asrc/255+b_dst*(255-Asrc)/255;
        }break;
    }
    switch(PixelFormat)
    {
        case CN_SYS_PF_RGB565:
        {
            result = (r<<11) | (g<<5) | b;
        }break;
        case CN_SYS_PF_RGB332:
        {
            result = (r<<5) | (g<<2) | b;
        }break;
        case CN_SYS_PF_RGBE4444:
        {
            result = (r<<16) | (g<<8) | (b<<4);
        }break;
        case CN_SYS_PF_ERGB4444:
        {
            result = (r<<8) | (g<<4) | b;
        }break;
        //5551��ʽ�ܱ�Ť����ʱ���ε�������Ҫʱ�ٸ���ʵ�ʸ�ʽ���ӡ�
//        case CN_SYS_PF_RGBC5551:
//        {
//            u32 c;
//            c = r & 1;
//            r = r>>1;
//            g = g>>1;
//            b = b>>1;
//            color = (r<<11) | (g<<6) | (b<<1) | c;
//            return color;
//        }break;
        case CN_SYS_PF_ERGB6666:
        {
            result = (r<<12) | (g<<6) | b;
        }break;
        case CN_SYS_PF_E12RGB666:
        {
            result = (r<<12) | (g<<6) | b;
        }break;
        case CN_SYS_PF_RGB101010:
        {
            result = (r<<20) | (g<<10) | b;
        }break;
        case CN_SYS_PF_RGB888:
        case CN_SYS_PF_ERGB8888:
        {
            result = (r<<16) | (g<<8) | b;
        }break;
        case CN_SYS_PF_ARGB1555:
        {
            result = (a<<15) | (r<<10) | (g<<5) | b;
        }break;
        case CN_SYS_PF_RGBA5551:
        {
            result = (a &1) | (r<<11) | (g<<6) | (b<<1);
        }break;
        case CN_SYS_PF_ARGB8888:
        {
            result = (a<<24) | (r<<16) | (g<<8) | b;
       }break;
        case CN_SYS_PF_GRAY1:
        {
            //����rgbֵ����hlsģ�͵�����
            result = (((MAX(MAX(r,g),b)+MIN(MIN(r,g),b))*1) + 255 )/(2*255);
        }break;
        case CN_SYS_PF_GRAY2:
        {
            //����rgbֵ����hlsģ�͵�����
            result = (((MAX(MAX(r,g),b)+MIN(MIN(r,g),b))*3) + 255 )/(2*255);
        }break;
        case CN_SYS_PF_GRAY4:
        {
            //����rgbֵ����hlsģ�͵�����
            result = (((MAX(MAX(r,g),b)+MIN(MIN(r,g),b))*15) + 255 )/(2*255);
        }break;
        case CN_SYS_PF_GRAY8:
        {
            //����rgbֵ����hlsģ�͵�����
            return (((MAX(MAX(r,g),b)+MIN(MIN(r,g),b))*255) + 255 )/(2*255);
        }break;
        case CN_SYS_PF_PALETTE1:
        case CN_SYS_PF_PALETTE2:
        case CN_SYS_PF_PALETTE4:
        case CN_SYS_PF_PALETTE8:
        {
            result = (r<<16) | (g<<8) | b;
        }break;
        default:
        {
            result = 0;
        }
        break;
    }

    return result;
}

//----���ڴ滭��(ִ�ж�Ԫ��դ����)---------------------------------------------
//����: ��һ��bitmap�ϻ��㣬ִ�ж�Ԫ��դ������Ҫ��color��bitmap�ĸ�ʽ��ͬ��
//����: bitmap��Ŀ��λͼ
//      x��y����ͼ����
//      color����ɫ��bitmap�ļ��ݸ�ʽ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __GK_SetPixelRop2Bm(struct tagRectBitmap *bitmap,
                         s32 x,s32 y,u32 color,u32 Rop2Code)
{
    u8 bit_offset;
    u8 color_bits;
    u32 byte_offset;
    u32 pf,alpha;
    color_bits = (bitmap->PixelFormat & CN_PF_BITWIDE_MSK) >>8;
    //����λͼ��ɫ��ʽ���ڴ��ϻ���
    switch(color_bits)
    {
        case 1:
        {
            byte_offset = y*bitmap->linebytes + x/8;//���Ӧ������ֽ�ƫ����
            //���Ӧ�������ֽ��е�λƫ������λ���ұߣ���λ����
            bit_offset = 7-x%8;
            //Ŀ��λͼ�϶�Ӧ�������ɫ
            pf = (u32)bitmap->bm_bits[byte_offset];
            //���ж�Ԫ��դ���������ɫ
            pf = GK_BlendRop2(pf,color,Rop2Code)
                    &(0x1<<bit_offset);
            //������Ŀ��λ�ģ�����λ��ɫ
            bitmap->bm_bits[byte_offset] &= ~(1<<bit_offset);
            //����Ϻ����ɫ��ӵ���Ӧλ
            bitmap->bm_bits[byte_offset] |= (u8)pf;
        }break;
        case 2:
        {
            byte_offset = y*bitmap->linebytes + x/4;//���Ӧ������ֽ�ƫ����
            //���Ӧ�������ֽ��е�λƫ������λ���ұߣ���λ����
            bit_offset = (3-x%4)<<1;
            //Ŀ��λͼ�϶�Ӧ�������ɫ
            pf = (u32)bitmap->bm_bits[byte_offset];
            //���ж�Ԫ��դ���������ɫ
            pf = GK_BlendRop2(pf,color,Rop2Code)
                    &(0x3<<bit_offset);
            //������Ŀ��λ�ģ�����λ��ɫ
            bitmap->bm_bits[byte_offset] &= ~(3<<bit_offset);
            //����Ϻ����ɫ��ӵ���Ӧλ
            bitmap->bm_bits[byte_offset] |= (u8)pf;
        }break;
        case 4:
        {
            byte_offset = y*bitmap->linebytes + x/2;//���Ӧ������ֽ�ƫ����
            //���Ӧ�������ֽ��е�λƫ������λ���ұߣ���λ����
            bit_offset = (1-x%2)<<2;
            //Ŀ��λͼ�϶�Ӧ�������ɫ
            pf = (u32)bitmap->bm_bits[byte_offset];
            //���ж�Ԫ��դ���������ɫ
            pf = GK_BlendRop2(pf,color,Rop2Code)
                    &(0xf<<bit_offset);
            //������Ŀ��λ�ģ�����λ��ɫ
            bitmap->bm_bits[byte_offset] &= ~(0xf<<bit_offset);
            //����Ϻ����ɫ��ӵ���Ӧλ
            bitmap->bm_bits[byte_offset] |= (u8)pf;
        }break;
        case 8:
        {
            byte_offset = y*bitmap->linebytes + x;//���Ӧ������ֽ�ƫ����
            //���ж�Ԫ��դ���������ɫ
            pf = GK_BlendRop2((u32)bitmap->bm_bits[byte_offset],
                    color,Rop2Code);
            bitmap->bm_bits[byte_offset] = (u8)pf;//����Ϻ����ɫ��ӵ���Ӧλ
        }break;
        case 12:
        {
            byte_offset = y*bitmap->linebytes + x*3/2;//���Ӧ������ֽ�ƫ����
            bit_offset = (x*12)%8;              //���Ӧ�������ֽ��е�λƫ����
            //ע:��������⣬���ɰ�Ŀ���ַǿ��ת����16λָ�롣
            pf = (u32)bitmap->bm_bits[byte_offset]
                  + (u32)(bitmap->bm_bits[byte_offset+1]<<8);
            pf = pf>>bit_offset;
            pf = GK_BlendRop2(pf,color,Rop2Code);//���ж�Ԫ��դ���������ɫ
            if(bit_offset == 0)
            {
                bitmap->bm_bits[byte_offset] = (u8)(pf&0xff);
                bitmap->bm_bits[byte_offset+1] &= 0xf0;
                bitmap->bm_bits[byte_offset+1] |= (u8)(pf>>8);
            }else
            {
                bitmap->bm_bits[byte_offset] &= 0xf;
                bitmap->bm_bits[byte_offset] |= (u8)((pf&0xf)<<4);
                bitmap->bm_bits[byte_offset+1] = (u8)(pf>>4);
            }
        }break;
        case 16:
        {
            byte_offset = y*bitmap->linebytes + x*2;//���Ӧ������ֽ�ƫ����
            //Ŀ��λͼ�϶�Ӧ�������ɫ
            pf = (u32)(*(u16*)(&bitmap->bm_bits[byte_offset]));
//            alpha = pf;
            //���ж�Ԫ��դ���������ɫ
            pf = GK_BlendRop2(pf,color,Rop2Code);
//            if(bitmap->PixelFormat == CN_SYS_PF_ARGB1555)
//            {
//                pf &= 0x7fff;
//                pf |= alpha & 0x8000;
//            }
            //ע:��������⣬���ɰ�Ŀ���ַǿ��ת����16λָ�롣
            bitmap->bm_bits[byte_offset] = (u8)pf;
            bitmap->bm_bits[byte_offset+1] = (u8)(pf>>8);
        }break;
        case 24:
        {
            byte_offset = y*bitmap->linebytes + x*3;//���Ӧ������ֽ�ƫ����
            pf = (u32)bitmap->bm_bits[byte_offset];
            pf += (u32)bitmap->bm_bits[byte_offset+1]<<8;
            pf += (u32)bitmap->bm_bits[byte_offset+2]<<16;
            //���ж�Ԫ��դ���������ɫ
            pf = GK_BlendRop2(pf,color,Rop2Code);
            bitmap->bm_bits[byte_offset] = (u8)pf;
            bitmap->bm_bits[byte_offset+1] = (u8)(pf >> 8);
            bitmap->bm_bits[byte_offset+2] = (u8)(pf >> 16);
        }break;
        case 32:
        {
            byte_offset = y*bitmap->linebytes + x*4;//���Ӧ������ֽ�ƫ����
            pf = (u32)(*(u32*)(&bitmap->bm_bits[byte_offset]));
            alpha = pf;
            //���ж�Ԫ��դ���������ɫ
            pf = GK_BlendRop2(pf,color,Rop2Code);
            if(bitmap->PixelFormat == CN_SYS_PF_ARGB8888)
            {
                pf &= 0x00ffffff;
                pf |= alpha & 0xff000000;
            }
            //ע:��������⣬���ɰ�Ŀ���ַǿ��ת����32λָ�롣
            bitmap->bm_bits[byte_offset] = (u8)pf;
            bitmap->bm_bits[byte_offset+1] = (u8)(pf >> 8);
            bitmap->bm_bits[byte_offset+2] = (u8)(pf >> 16);
            bitmap->bm_bits[byte_offset+3] = (u8)(pf >> 24);
        }break;
    }
}

//----RGB888��ʽAlpha���-------------------------------------------------------
//����: ������ɫ��ǰ��ɫ����Alpha���
//����: bk_c, ����ɫ
//     fr_c, ǰ��ɫ
//     alpha, Alphaֵ
//����:�µ���ɫֵ(RGB888��ʽ)
//-----------------------------------------------------------------------------
static  u32 __alpha_blend_color(u32 bk_c,u32 fr_c,u8 alpha)
{
    u8 bk_r,bk_g,bk_b;
    u8 fr_r,fr_g,fr_b;

    bk_r =bk_c>>16;
    bk_g =bk_c>>8;
    bk_b =bk_c;
    fr_r =fr_c>>16;
    fr_g =fr_c>>8;
    fr_b =fr_c;
    fr_r =(((fr_r-bk_r)*alpha)>>8) + bk_r;
    fr_g =(((fr_g-bk_g)*alpha)>>8) + bk_g;
    fr_b =(((fr_b-bk_b)*alpha)>>8) + bk_b;

    return  (fr_r<<16)|(fr_g<<8)|(fr_b);
}

//----λͼ��copy����(RopCode)----------------------------------------------
//����: ��src_bitmapλͼ��ָ����һ�����أ����Ƶ�dst_bitmap�ϣ�ִ����չrop������
//      ��չrop�����Ķ��壬�� CN_ROP_ALPHA_SRC_MSK ����ض��塣ע�⣬Ŀ��λͼ��
//      ����alphaͨ����Ҳ�������ǵ�ɫ��λͼ��
//����: dst_bitmap��Ŀ��λͼ
//      src_bitmap��Դλͼ
//      x_dst��y_dst��Ŀ��λͼ�е�Ŀ������
//      x_src��y_src��Դλͼ�е�Ŀ������
//      RopCode����դ�����룬�� CN_ROP_ALPHA_SRC_MSK ����ض���
//      KeyColor��͸��ɫ,ERGB8888��ʽ
//����: ��
//-----------------------------------------------------------------------------
void __GK_CopyPixelRopBm(struct tagRectBitmap *dst_bitmap,
                                struct tagRectBitmap *src_bitmap,
                                s32 x_dst,s32 y_dst,s32 x_src,s32 y_src,
                                u32 RopCode,u32 KeyColor)
{
    u32 src_alpha,dst_alpha;
    u32 color;
    u32 Rop2;
    u32 dst_color,src_color;

    //��ȡԴλͼ�е���ɫ��δת����ERGB8888
    src_color = GK_GetPixelBm(src_bitmap,x_src,y_src);
    //ת��ʱ���Ѿ�����Դλͼ��alphaͨ����(�����)
    src_color = GK_ConvertColorToRGB24(src_bitmap->PixelFormat,
                                       src_color,src_bitmap->ExColor);
    if(RopCode & CN_ROP_ALPHA_EN)       //alpha���
    {
        if(!(RopCode & CN_ROP_KEYCOLOR_EN) || (src_color != KeyColor) )
        {
            src_alpha = (RopCode & CN_ROP_ALPHA_SRC_MSK) >> CN_ROP_ALPHA_SRC_OFF;
            dst_alpha = (RopCode & CN_ROP_ALPHA_DST_MSK) >> CN_ROP_ALPHA_DST_OFF;
            //��Դλͼ��ɫ������ɫ��ʽת�����õ���Ŀ��λͼ��ɫ��ʽһ�µ���ɫ
            src_color = GK_ConvertRGB24ToPF(dst_bitmap->PixelFormat,src_color);
            //��ȡĿ��λͼ�е���ɫ��δת����ERGB8888
            dst_color = GK_GetPixelBm(dst_bitmap,x_dst,y_dst);
            color = __GK_AlphaBlendBitmapToWin(src_color,dst_color,
                            dst_bitmap->PixelFormat,
                            dst_bitmap->ExColor,src_alpha,dst_alpha,
                            (RopCode&CN_ROP_ALPHA_MODE_MSK)>>CN_ROP_ALPHA_MODE_OFF);
            //��������
            __GK_SetPixelRop2Bm(dst_bitmap,x_dst,y_dst,color,CN_R2_COPYPEN);
        }
    }
    else if(RopCode & CN_ROP_ROP2_EN)//���ͬʱʹ��alpha��Ϻ�rop2��rop2��������
    {
        if(!(RopCode & CN_ROP_KEYCOLOR_EN) || (src_color != KeyColor) )
        {
            Rop2 = (RopCode & CN_ROP_ROP2_MSK) >> CN_ROP_ROP2_OFF;
            //��Դλͼ��ɫ������ɫ��ʽת�����õ���Ŀ��λͼ��ɫ��ʽһ�µ���ɫ
            src_color = GK_ConvertRGB24ToPF(dst_bitmap->PixelFormat,src_color);
            //��������
            __GK_SetPixelRop2Bm(dst_bitmap,x_dst,y_dst,src_color,Rop2);
        }
    }
    else if(RopCode & CN_ROP_KEYCOLOR_EN)  //�ؼ�ɫ͸��
    {
        if(src_color != KeyColor)
        {
            //��Դλͼ��ɫ������ɫ��ʽת�����õ���Ŀ��λͼ��ɫ��ʽһ�µ���ɫ
            src_color = GK_ConvertRGB24ToPF(dst_bitmap->PixelFormat,src_color);
            //��������
            __GK_SetPixelRop2Bm(dst_bitmap,x_dst,y_dst,src_color,CN_R2_COPYPEN);
        }
    }
    else
    {
        switch(src_bitmap->PixelFormat)
        {
           case CN_SYS_PF_ALPHA1:
               if(GK_GetPixelBm(src_bitmap,x_src,y_src))
               {
                    //��Դλͼ��ɫ������ɫ��ʽת�����õ���Ŀ��λͼ��ɫ��ʽһ�µ���ɫ
                    src_color = GK_ConvertRGB24ToPF(dst_bitmap->PixelFormat,src_bitmap->ExColor);
                    //��������
                    __GK_SetPixelRop2Bm(dst_bitmap,x_dst,y_dst,src_color,CN_R2_COPYPEN);
               }
               break;

           case CN_SYS_PF_ALPHA2:
               src_alpha = GK_GetPixelBm(src_bitmap,x_src,y_src);
               src_alpha =(src_alpha<<6)&0xC0;

               dst_color = GK_GetPixelBm(dst_bitmap,x_dst,y_dst);
               dst_color = GK_ConvertColorToRGB24(dst_bitmap->PixelFormat,dst_color,dst_bitmap->ExColor);

               dst_color = __alpha_blend_color(dst_color,src_bitmap->ExColor,src_alpha);
               dst_color = GK_ConvertRGB24ToPF(dst_bitmap->PixelFormat,dst_color);
               __GK_SetPixelRop2Bm(dst_bitmap,x_dst,y_dst,dst_color,CN_R2_COPYPEN);
               break;

           case CN_SYS_PF_ALPHA4:
               src_alpha = GK_GetPixelBm(src_bitmap,x_src,y_src);
               src_alpha = (src_alpha<<4)&0xF0;

               dst_color = GK_GetPixelBm(dst_bitmap,x_dst,y_dst);
               dst_color = GK_ConvertColorToRGB24(dst_bitmap->PixelFormat,dst_color,dst_bitmap->ExColor);

               dst_color = __alpha_blend_color(dst_color,src_bitmap->ExColor,src_alpha);
               dst_color = GK_ConvertRGB24ToPF(dst_bitmap->PixelFormat,dst_color);
               __GK_SetPixelRop2Bm(dst_bitmap,x_dst,y_dst,dst_color,CN_R2_COPYPEN);
               break;

           case CN_SYS_PF_ALPHA8:
               src_alpha = GK_GetPixelBm(src_bitmap,x_src,y_src);
               src_alpha = src_alpha&0xFF;

               dst_color = GK_GetPixelBm(dst_bitmap,x_dst,y_dst);
               dst_color = GK_ConvertColorToRGB24(dst_bitmap->PixelFormat,dst_color,dst_bitmap->ExColor);

               dst_color = __alpha_blend_color(dst_color,src_bitmap->ExColor,src_alpha);
               dst_color = GK_ConvertRGB24ToPF(dst_bitmap->PixelFormat,dst_color);
               __GK_SetPixelRop2Bm(dst_bitmap,x_dst,y_dst,dst_color,CN_R2_COPYPEN);
               break;

           default:
               //��Դλͼ��ɫ������ɫ��ʽת�����õ���Ŀ��λͼ��ɫ��ʽһ�µ���ɫ
               src_color = GK_ConvertRGB24ToPF(dst_bitmap->PixelFormat,src_color);
               __GK_SetPixelRop2Bm(dst_bitmap,x_dst,y_dst,src_color,CN_R2_COPYPEN);
               break;
        }
        /*
        if(src_bitmap->PixelFormat == CN_SYS_PF_ALPHA1)
        {
            if(GK_GetPixelBm(src_bitmap,x_src,y_src))
            {
                //��Դλͼ��ɫ������ɫ��ʽת�����õ���Ŀ��λͼ��ɫ��ʽһ�µ���ɫ
                src_color = GK_ConvertRGB24ToPF(dst_bitmap->PixelFormat,src_bitmap->ExColor);
                //��������
                __GK_SetPixelRop2Bm(dst_bitmap,x_dst,y_dst,src_color,CN_R2_COPYPEN);
            }



        }
        else
        {
            //��Դλͼ��ɫ������ɫ��ʽת�����õ���Ŀ��λͼ��ɫ��ʽһ�µ���ɫ
            src_color = GK_ConvertRGB24ToPF(dst_bitmap->PixelFormat,src_color);
            __GK_SetPixelRop2Bm(dst_bitmap,x_dst,y_dst,src_color,CN_R2_COPYPEN);
        }
        */
    }

}

//----���ڴ滭��---------------------------------------------------------------
//����: ��һ��bitmap�ϵ�һ����copy����һ��bitmap�����Դλͼ��alphaͨ��������Ҫ
//      ����alpha
//����: dst_bitmap��Ŀ��λͼ
//      src_bitmap��Դλͼ
//      x_dst��y_dst��Ŀ��λͼ�е�Ŀ������
//      x_src��y_src��Դλͼ�е�Ŀ������
//����: ��
//-----------------------------------------------------------------------------
void __GK_CopyPixelBm(struct tagRectBitmap *dst_bitmap,
                      struct tagRectBitmap *src_bitmap,
                      s32 x_dst,s32 y_dst,s32 x_src,s32 y_src)
{
    u32 pf_src,cvt_src;
    //Ŀ��λͼ��ɫ��ʽ
//    color_bits = (dst_bitmap->PixelFormat & CN_PF_BITWIDE_MSK)>>8;
    //ȡԴλͼ��ɫ������Դλͼ��ɫ��ʽ������ֵ
    pf_src = GK_GetPixelBm(src_bitmap,x_src,y_src);
    //��ȡ����Դλͼ��ɫ������ɫ��ʽת�����õ���Ŀ��λͼ��ɫ��ʽһ�µ���ɫ
    cvt_src = GK_ConvertPF2PF(dst_bitmap->PixelFormat,
                            src_bitmap->PixelFormat,pf_src,src_bitmap->ExColor);
    __GK_SetPixelRop2Bm(dst_bitmap,x_dst,y_dst,cvt_src,CN_R2_COPYPEN);
}

//----����---------------------------------------------------------------------
//����: �ڴ����ڻ�һ�����ص㡣
//����: �����ǽṹpara�ĳ�Ա
//      gkwin��Ŀ�괰��ָ��
//      x��y����ͼ����
//      color������ʹ�õ���ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void GK_SetPixel(struct tagGkscParaSetPixel *para)
{
    struct tagClipRect *clip;
    struct tagGkWinRsc *fb_gkwin,*pixelwin;
    struct tagRectBitmap *bitmap;
    u32 pf_color;
    s32 offsetx,offsety;
    if((para->Rop2Code == CN_R2_NOP) || (para->Rop2Code > CN_R2_LAST))
        return;                                     //��ִ�в���
    pixelwin = para->gkwin;
    //˵����win buffer����ֱ��д������Ϊfalse
    if((pixelwin->wm_bitmap != NULL)
        && (pixelwin->direct_screen == false))
    {   //������:��win buffer�л�ͼ����־changed_msk
        bitmap = pixelwin->wm_bitmap;
        //����ʾ��ʹ��cn_custom_pf��ʽ��Ӧ����Ӳ��ʵ�ֻ�������
        if(bitmap->PixelFormat & CN_CUSTOM_PF)
        {
            pixelwin->disp->draw.set_pixel_bm(bitmap,para->x,para->y,
                                                 para->color,para->Rop2Code);
        }else//�����ʵ�ֻ�������
        {
            //�涨�����߸�������ɫ�������ɫ
            pf_color = GK_ConvertRGB24ToPF(bitmap->PixelFormat,para->color);
            __GK_SetPixelRop2Bm(bitmap,para->x,para->y,
                                pf_color,para->Rop2Code);      //��������
        }
        __GK_ShadingPixel(pixelwin,para->x,para->y);//��־���ص�changed_msk
    }else
    {
        clip = pixelwin->visible_clip;
        if(clip == NULL)        //������Ϊ�գ�ֱ�ӷ���
            return ;
        fb_gkwin = pixelwin->disp->frame_buffer;
        offsetx = pixelwin->absx0 +para->x;
        offsety = pixelwin->absy0 +para->y;
        //��frame buffer����ֱ��д������Ϊfalse
        if((fb_gkwin != NULL) && (pixelwin->direct_screen == false))
        {   //������:��frame buffer�л�ͼ����ֻ��gkwin�еĿ�������
            bitmap = fb_gkwin->wm_bitmap;
            do{
                //���ض�Ӧ�������ڿ������ڣ����������
                if(!((offsetx<clip->rect.left) || (offsetx>=clip->rect.right)
                    || (offsety<clip->rect.top) ||(offsety>=clip->rect.bottom)))
                {
                    //����ʾ��ʹ��cn_custom_pf��ʽ��Ӧ����Ӳ��driverʵ�ֻ�������
                    if(bitmap->PixelFormat & CN_CUSTOM_PF)
                    {
                        fb_gkwin->disp->draw.set_pixel_bm(bitmap,offsetx,
                                           offsety,para->color,para->Rop2Code);
                    }else//�����ʵ�ֻ�������
                    {
                        //�涨�����߸�������ɫ��ɫ��ʽ�������ɫ
                        pf_color = GK_ConvertRGB24ToPF(
                                                bitmap->PixelFormat,para->color);
                        __GK_SetPixelRop2Bm(bitmap,offsetx,offsety,
                                                pf_color,para->Rop2Code);
                    }
                    //��־���ص�changed_msk
                    __GK_ShadingPixel(fb_gkwin,offsetx,offsety);
                    break;      //�����Ѿ����ƣ��˳�ѭ��
                }else//���ض�Ӧ�����겻�ڿ������ڣ�ָ����һ��������
                    clip = clip->next;
            }while(clip != pixelwin->visible_clip);
        }
        else
        //��win buffer��Ҳ��frame buffer��ֱ�ӻ���screen��
        //ֱ��д������Ϊtrue���������޻���������ֱ�ӻ���screen��
        {
            do{
                //���ض�Ӧ�������ڿ������ڣ����������
                if(!((offsetx<clip->rect.left) || (offsetx>=clip->rect.right)
                    || (offsety<clip->rect.top) ||(offsety>=clip->rect.bottom)))
                {
                    __GK_SetPixelScreen(pixelwin->disp,offsetx,offsety,
                                          para->color,para->Rop2Code);
                    break;      //�����Ѿ����ƣ��˳�ѭ��
                }else//���ض�Ӧ�����겻�ڿ������ڣ�ָ����һ��������
                    clip = clip->next;
            }while(clip != pixelwin->visible_clip);
        }
    }
}

//----����changed_msk:����-----------------------------------------------------
//����: ��������ʱ�����ô��ڵ�changed_msk���Ѹ�д����������ɫ���ɵ�����ȷ������
//      �Ϸ���
//����: gkwin��Ŀ�괰��ָ��
//      x��y��Ŀ�����ض�Ӧ������
//����: ��
//-----------------------------------------------------------------------------
void __GK_ShadingPixel(struct tagGkWinRsc *gkwin,s32 x,s32 y)
{
    s32 msk_x,msk_y;
    if(gkwin->change_flag == CN_GKWIN_CHANGE_ALL)   //�Ѿ�ȫ����ɫ
    {
        return;
    }
    gkwin->change_flag = CN_GKWIN_CHANGE_PART;
    msk_x = x/8;       //����x��mskλͼ�еڼ���
    msk_y = y/8;       //����y��mskλͼ�еڼ���
    //���������ڵ�changed_msk�е�bit��Ϊ1
    __GK_SetPixelRop2Bm(&(gkwin->changed_msk),msk_x,msk_y,
                                CN_COLOR_WHITE,CN_R2_COPYPEN);
    return;
}

//----����changed_msk----------------------------------------------------------
//����: ��һ�����ڵ�rect�޶�����������Ϊ����д
//����: bitmap��Ŀ��bitmap
//      rect����д����
//����: ��
//˵��: �ڲ����������������Ϸ��ԣ��ɵ����߱�֤����ĺϷ���
//-----------------------------------------------------------------------------
void __GK_ShadingRect(struct tagGkWinRsc *gkwin,struct tagRectangle *rect)
{
    struct tagRectBitmap *bitmap;
    s32 x,y;
    s32 msk_x1,msk_x2,msk_y1,msk_y2;

    if(gkwin->change_flag == CN_GKWIN_CHANGE_ALL)   //�Ѿ�ȫ����ɫ
    {
        return;
    }
    bitmap = gkwin->wm_bitmap;
    if((rect->left <= 0) && (rect->top <= 0)
            && (rect->right >= bitmap->width)&&(rect->bottom >= bitmap->height))
    {
        gkwin->change_flag = CN_GKWIN_CHANGE_ALL;   //��msk�����򸲸���������
        return;
    }

    gkwin->change_flag = CN_GKWIN_CHANGE_PART;
    msk_x1 = rect->left/8;          //����x1��mskλͼ�еڼ���
    msk_x2 = (rect->right-1)/8;     //����x2��mskλͼ�еڼ��У��������յ㣬-1
    msk_y1 = rect->top/8;           //����y1��mskλͼ�еڼ���
    msk_y2 = (rect->bottom-1)/8;    //����y2��mskλͼ�еڼ��У��������յ㣬-1

    for(y = msk_y1;y <= msk_y2;y++)
    {
        for(x = msk_x1;x <= msk_x2;x++)
            __GK_SetPixelRop2Bm(&(gkwin->changed_msk),x,y,
                                CN_COLOR_WHITE,CN_R2_COPYPEN);
    }
}

//----����changed_msk----------------------------------------------------------
//����: �ڴ��ڻ�frame buffer����ֱ��ʱ�����ô��ڵ�changed_msk��
//      �Ѹ�д����������ɫ��
//����: gkwin��Ŀ�괰��
//      limit�������ͼ����������һ�����ε����򣬳����������ֱ�߲���
//      x1��y1��x2��y2�� ��㡢�յ�����
//����: ��
//-----------------------------------------------------------------------------
void __GK_ShadingLine(struct tagGkWinRsc *gkwin,struct tagRectangle *limit,
                     s32 x1,s32 y1,s32 x2,s32 y2)
{
    if(gkwin->change_flag == CN_GKWIN_CHANGE_ALL)   //�Ѿ�ȫ����ɫ
    {
        return;
    }
    gkwin->change_flag = CN_GKWIN_CHANGE_PART;

    if(y1 == y2)        //ˮƽֱ����ɫ
    {
        s32 x;
        s32 msk_x1,msk_x2,msk_y1;
        if(x1>x2)
            __gk_swap(x1,x2);
        if((y1 <= limit->top) || (y1 >= limit->bottom)
            ||(x2 <= limit->left) || (x1 >= limit->right))
            return;         //����ֱ����limit֮��
        if(x1 < limit->top)
            x1 = limit->top;
        if(x2 > limit->bottom)
            x2 = limit->bottom;
        msk_x1 = x1/8;     //����x1��mskλͼ�еڼ���
        msk_x2 = (x2-1)/8; //����(x2-1)��mskλͼ�еڼ��У�-1����Ϊ�յ㲻��������
        msk_y1 = y1/8;     //����y1��mskλͼ�еڼ���
        for(x = msk_x1;x <= msk_x2;x++)
        {
            //��changed_msk��Ӧ��bit��1
            __GK_SetPixelRop2Bm(&(gkwin->changed_msk),x,msk_y1,
                                    CN_COLOR_WHITE,CN_R2_COPYPEN);
        }

    }
    else if(x1 == x2)   //��ֱ����ɫ
    {
        s32 y;
        s32 msk_x1,msk_y1,msk_y2;
        if(y1>y2)
            __gk_swap(y1,y2);
        if((x1<limit->left) || (x1 >= limit->right)
            ||(y2 <= limit->top) || (y1 >= limit->bottom))
            return;         //����ֱ����limit֮��
        if(y1 < limit->top)
            y1 = limit->top;
        if(y2 > limit->bottom)
            y2 = limit->bottom;
        msk_x1 = x1/8;     //����x1��mskλͼ�еڼ���
        msk_y1 = y1/8;     //����y1��mskλͼ�еڼ���
        msk_y2 = (y2-1)/8; //����(y2-1)��mskλͼ�еڼ��У�-1����Ϊ�յ㲻��������
        for(y = msk_y1;y <= msk_y2;y++)
        {
            //��changed_msk��Ӧ��bit��1
            __GK_SetPixelRop2Bm(&(gkwin->changed_msk),msk_x1,y,
                                    CN_COLOR_WHITE,CN_R2_COPYPEN);
        }
    }
    else
    {
        s32 x,y;
        s32 msk_x,msk_y;
        s32 dx,dy;
        dx = abs(x2-x1);
        dy = abs(y2-y1);

        //x����仯����
        if(dx > dy)
        {
            if(((x1>x2)&&(y1>y2)) || ((x1<x2)&&(y1<y2)))
            {//x��y����������ͬ����ͬ������ͬ��
                //ѡȡx1��x2�н�С������Ϊx1
                if(x1 > x2)
                {
                    __gk_swap(x1,x2);
                    __gk_swap(y1,y2);
                }
                //���¶��룬ȡ8*8���أ�xÿ�仯8���غ�y��ֱ�ߵĽ��㣬
                //��б�������еĵ����ڵ�8*8���������Ӧ��
                //��changed_msk�е�bit��Ϊ1������˵���뿴djygui����
                for(x = align_down(8,x1); x <= x2; x+=8)
                {
                    //�Լ���Ľ�������������봦��x��y����������ͬ
                    //������0.5Ϊ��׼��
                    y = ((((x-x1)<<1)*(y2-y1)+(x2-x1))/((x2-x1)<<1))+y1;
                    msk_x = x/8;    //����x��mskλͼ�еڼ���
                    msk_y = y/8;    //����y��mskλͼ�еڼ���
                    //��changed_msk��Ӧ��bit��1
                    __GK_SetPixelRop2Bm(&(gkwin->changed_msk),msk_x,
                                         msk_y,CN_COLOR_WHITE,CN_R2_COPYPEN);
                    //��־(msk_x��msk_y)�����һ����
                    msk_y += 1;
                    //��changed_msk��Ӧ��bit��1
                    __GK_SetPixelRop2Bm(&(gkwin->changed_msk),msk_x,
                                         msk_y,CN_COLOR_WHITE,CN_R2_COPYPEN);
                }
            }
            else
            {//x��y���������෴����x����y����x����y��
                //ѡȡx1��x2�н�С������Ϊx1
                if(x1 > x2)
                {
                    __gk_swap(x1,x2);
                    __gk_swap(y1,y2);
                }
                //���¶��룬ȡ8*8���أ�xÿ�仯8���غ�y��ֱ�ߵĽ��㣬
                //��б�������еĵ����ڵ�8*8���������Ӧ��
                //��changed_msk�е�bit��Ϊ1������˵���뿴djygui����
                for(x = align_down(8,x1); x <= x2; x+=8)
                {
                    //�����ý�������������봦��yΪ������������-0.5Ϊ��׼��
                    y = ((((x-x1)<<1)*(y2-y1)-(x2-x1))/((x2-x1)<<1))+y1;
                    msk_x = x/8;    //����x��mskλͼ�еڼ���
                    msk_y = y/8;    //����y��mskλͼ�еڼ���
                    //��changed_msk��Ӧ��bit��1
                    __GK_SetPixelRop2Bm(&(gkwin->changed_msk),msk_x,
                                         msk_y,CN_COLOR_WHITE,CN_R2_COPYPEN);
                    //��־(msk_x��msk_y)�����һ����
                    msk_y -= 1;
                    //��changed_msk��Ӧ��bit��1
                    __GK_SetPixelRop2Bm(&(gkwin->changed_msk),msk_x,
                                         msk_y,CN_COLOR_WHITE,CN_R2_COPYPEN);
                }
            }
        }else
        {//y����仯���󣬻���x��y�仯�����
            if(((x1>x2)&&(y1>y2)) || ((x1<x2)&&(y1<y2)))
            {//x��y����������ͬ����ͬ������ͬ��
                //ѡȡy1��y2�н�С������Ϊy1
                if(y1 > y2)
                {
                    __gk_swap(x1,x2);
                    __gk_swap(y1,y2);
                }
                //���¶��룬ȡ8*8���أ�yÿ�仯8���غ�x��ֱ�ߵĽ��㣬
                //��б�������еĵ����ڵ�8*8���������Ӧ��
                //��changed_msk�е�bit��Ϊ1������˵���뿴djygui����
                for(y = align_down(8,y1); y <= y2; y+=8)
                {
                    x = ((((y-y1)<<1)*(x2-x1)+(y2-y1))/((y2-y1)<<1))+x1;
                    msk_x = x/8;    //����x��mskλͼ�еڼ���
                    msk_y = y/8;    //����y��mskλͼ�еڼ���
                    //��changed_msk��Ӧ��bit��1
                    __GK_SetPixelRop2Bm(&(gkwin->changed_msk),msk_x,
                                         msk_y,CN_COLOR_WHITE,CN_R2_COPYPEN);
                    msk_x += 1;
                    //��changed_msk��Ӧ��bit��1
                    __GK_SetPixelRop2Bm(&(gkwin->changed_msk),msk_x,
                                         msk_y,CN_COLOR_WHITE,CN_R2_COPYPEN);
                }
            }
            else
            {//x��y���������෴����x����y����x����y��
                //ѡȡy1��y2�н�С������Ϊy1
                if(y1 > y2)
                {
                    __gk_swap(x1,x2);
                    __gk_swap(y1,y2);
                }
                //���¶��룬ȡ8*8���أ�yÿ�仯8���غ�x��ֱ�ߵĽ��㣬
                //��б�������еĵ����ڵ�8*8���������Ӧ��
                //��changed_msk�е�bit��Ϊ1������˵���뿴djygui����
                for(y = align_down(8,y1); y <= y2; y+=8)
                {
                    x = ((((y-y1)<<1)*(x2-x1)-(y2-y1))/((y2-y1)<<1))+x1;
                    msk_x = x/8;    //����x��mskλͼ�еڼ���
                    msk_y = y/8;    //����y��mskλͼ�еڼ���
                    //��changed_msk��Ӧ��bit��1
                    __GK_SetPixelRop2Bm(&(gkwin->changed_msk),msk_x,
                                         msk_y,CN_COLOR_WHITE,CN_R2_COPYPEN);
                    msk_x -= 1;
                    //��changed_msk��Ӧ��bit��1
                    __GK_SetPixelRop2Bm(&(gkwin->changed_msk),msk_x,
                                             msk_y,CN_COLOR_WHITE,CN_R2_COPYPEN);
                }
            }
        }
    }
}

//----���changed_msk----------------------------------------------------------
//����: ���һ�����ڵĸ�д��־��
//����: gkwin��Ŀ�괰��ָ��
//����: ��
//-----------------------------------------------------------------------------
void __GK_ShadingClear(struct tagGkWinRsc *gkwin)
{
    u32 msk_size;
    if(gkwin->changed_msk.bm_bits)
    {
        msk_size =
            (gkwin->wm_bitmap->width +7)/8 /8*((gkwin->wm_bitmap->height +7)/8);
        memset(gkwin->changed_msk.bm_bits,0,msk_size);
    }
}

//----����б�߲��������-------------------------------------------------------
//����: ����б�߲���־λ��
//����: gkwin��Ŀ�괰��
//      x1��y1��x2��y2�����յ�
//      color����������ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//˵��: ���õ���Bresenham�㷨
//-----------------------------------------------------------------------------
void __GK_DrawOline(struct tagGkWinRsc *gkwin,s32 x1,s32 y1,
                                        s32 x2,s32 y2,u32 color,u32 Rop2Code)
{
    u32 pf_color;
    s32 x,y,dx,dy,s1,s2,p,interchange,i;

    struct tagRectBitmap *bitmap;
    bitmap = gkwin->wm_bitmap;

    x = x1;
    y = y1;
    dx = abs(x2-x1);
    dy = abs(y2-y1);
    //��־����б�߷���
    if(x2 > x1)
        s1 = 1;
    else
        s1 = -1;
    if(y2 > y1)
        s2 = 1;
    else
        s2 = -1;
    //��־���仯�ı�����interchangeΪ1��y�仯���Ϊ0��x�仯���
    if(dy > dx)
    {
        __gk_swap(dx, dy);
        interchange = 1;
    }
    else
        interchange = 0;
    p = (dy<<1)-dx;            //���ó�ʼ����б�ֵ
    //��ɫ��ʽΪ�Կ��Զ����ʽ
    if(bitmap->PixelFormat & CN_CUSTOM_PF)
    {
        for(i=1;i<=dx;i++)//hjj
        {
            gkwin->disp->draw.set_pixel_bm(bitmap,x,y,color,Rop2Code);
            //ȡ����ѧ��Ͻ�������
            if(p >= 0)
            {
                if(interchange == 0)
                    y = y+s2;
                else
                    x = x+s1;
                p = p-(dx<<1);
            }
            if(interchange == 0)
                x = x+s1;
            else
                y = y+s2;
            p = p+(dy<<1);
        }
    }
    else
    {//��ɫ��ʽΪ������ʽ
        pf_color = GK_ConvertRGB24ToPF(bitmap->PixelFormat,color);
        for(i=1;i<=dx;i++)
        {
            __GK_SetPixelRop2Bm(bitmap,x,y,pf_color,Rop2Code);
            //ȡ����ѧ��Ͻ�������
            if(p >= 0)
            {
                if(interchange == 0)
                    y = y+s2;
                else
                    x = x+s1;
                p = p-(dx<<1);
            }
            if(interchange == 0)
                x = x+s1;
            else
                y = y+s2;
            p = p+(dy<<1);
        }
    }
}

//----����ֱ��(�����˵�)-------------------------------------------------------
//����: �ڴ����ڴӵ�ǰ��괦��ʼ��һ����ֱ�ߡ�
//����: gkwin��Ŀ�괰��ָ��
//      limit�����Ƶ���������ֻ����ֱ����limit�����ڲ��Ĳ���
//      x1��y1����ʼ�������
//      y2���������������
//      color����ɫ��cn_sys_pf_e8r8g8b8��ʽ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __GK_VlinetoBm(struct tagGkWinRsc *gkwin,struct tagRectangle *limit,
                     s32 x1,s32 y1,s32 y2,u32 color,u32 Rop2Code)
{
    s32 y;
    s32 msk_x1,msk_y1,msk_y2;
    u32 pf_color;
    struct tagRectBitmap *bitmap;
    if(y1>y2)
        __gk_swap(y1,y2);
    if((x1<limit->left) || (x1 >= limit->right)
        ||(y2 <= limit->top) || (y1 >= limit->bottom))
        return;         //����ֱ����limit֮��
    bitmap = gkwin->wm_bitmap;
    //��ֱ�߳���limit�Ĳ��ֲ�����
    if(y1 < limit->top)
        y1 = limit->top;
    if(y2 > limit->bottom)
        y2 = limit->bottom;
    if(bitmap->PixelFormat & CN_CUSTOM_PF)
    {
        for(y = y1; y < y2;y++)
        {
            gkwin->disp->draw.set_pixel_bm(bitmap,x1,y,color,Rop2Code);
        }
    }else
    {
        //�û�������ɫ��ɫ��ʽΪrgb24
        pf_color = GK_ConvertRGB24ToPF(bitmap->PixelFormat,color);
        for(y = y1;y < y2;y++)
        {
            __GK_SetPixelRop2Bm(bitmap,x1,y,pf_color,Rop2Code);
        }
    }

    //���洦��changed_msk��־λͼ���Ѵ��������߶��޸ĵ�������ɫ
    if(gkwin->change_flag != CN_GKWIN_CHANGE_ALL)
    {
        gkwin->change_flag = CN_GKWIN_CHANGE_PART;  //��־���ڲ����޸�
        msk_x1 = x1/8;     //����x1��mskλͼ�еڼ���
        msk_y1 = y1/8;     //����y1��mskλͼ�еڼ���
        msk_y2 = (y2-1)/8; //����(y2-1)��mskλͼ�еڼ��У�-1����Ϊ�յ㲻��������
        for(y = msk_y1;y <= msk_y2;y++)
        {   //��changed_msk��Ӧ��bit��1
            __GK_SetPixelRop2Bm(&(gkwin->changed_msk),msk_x1,y,
                                    CN_COLOR_WHITE,CN_R2_COPYPEN);
        }
    }
}

//----��ˮƽֱ��(�����˵�)-------------------------------------------------------
//����: �ڴ����ڴӵ�ǰ��괦��ʼ��һ��ˮƽֱ�ߡ�
//����: gkwin��Ŀ�괰��ָ��
//      limit�����Ƶ���������ֻ����ֱ����limit�����ڲ��Ĳ���
//      x1��y1����ʼ�������
//      x2��������ĺ�����
//      color����ɫ��cn_sys_pf_e8r8g8b8��ʽ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __GK_HlinetoBm(struct tagGkWinRsc *gkwin,struct tagRectangle *limit,
                     s32 x1,s32 y1,s32 x2,u32 color,u32 Rop2Code)
{
    s32 x;
    s32 msk_x1,msk_x2,msk_y1;
    u32 pf_color;
    struct tagRectBitmap *bitmap;
    if(x1>x2)
        __gk_swap(x1,x2);
    if((y1<limit->top) || (y1 >= limit->bottom)
        ||(x2 <= limit->left) || (x1 >= limit->right))
        return;         //����ֱ����limit֮��
    bitmap = gkwin->wm_bitmap;
    //ˮƽ����limit�ⲿ�ֲ�����
    if(x1 < limit->left)
        x1 = limit->left;
    if(x2 > limit->right)
        x2 = limit->right;
    if(bitmap->PixelFormat & CN_CUSTOM_PF)
    {
        for(x = x1; x < x2;x++)
        {
            gkwin->disp->draw.set_pixel_bm(bitmap,x,y1,color,Rop2Code);
        }
    }else
    {
        //�û�������ɫ��ɫ��ʽΪrgb24
        pf_color = GK_ConvertRGB24ToPF(bitmap->PixelFormat,color);
        for(x = x1;x < x2;x++)
        {//��1��2��4λɫscreen�����ѭ���ܺ�cpu�����ǸĽ�--db
            __GK_SetPixelRop2Bm(bitmap,x,y1,pf_color,Rop2Code);
        }
    }

    //���洦��changed_msk��־λͼ���Ѵ��������߶��޸ĵ�������ɫ
    if(gkwin->change_flag != CN_GKWIN_CHANGE_ALL)
    {
        gkwin->change_flag = CN_GKWIN_CHANGE_PART;  //��־���ڲ����޸�
        msk_x1 = x1/8;     //����x1��mskλͼ�еڼ���
        msk_x2 = (x2-1)/8; //����(x2-1)��mskλͼ�еڼ��У�-1����Ϊ�յ㲻��������
        msk_y1 = y1/8;     //����y1��mskλͼ�еڼ���
        for(x = msk_x1;x <= msk_x2;x++)
        {   //��changed_msk��Ӧ��bit��1
            __GK_SetPixelRop2Bm(&(gkwin->changed_msk),x,msk_y1,
                                    CN_COLOR_WHITE,CN_R2_COPYPEN);
        }
    }
}
//----��б��(�����˵�)---------------------------------------------------------
//����: �ڴ����ڴӵ�ǰ��괦��ʼ��һ��б�ߡ�
//����: gkwin��Ŀ�괰��ָ��
//      limit�����Ƶ���������ֻ����ֱ����limit�����ڲ��Ĳ���
//      x1��y1����ʼ�������
//      x2��y2�������������
//      color����ɫ��cn_sys_pf_e8r8g8b8��ʽ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __GK_OlinetoBm(struct tagGkWinRsc *gkwin,struct tagRectangle *limit,//ȷ��
                       s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 Rop2Code)
{
    s32 x,y;
    s32 msk_x,msk_y;
    s32 dx,dy;

    if(__GK_OlineSectInter(limit,&x1,&y1,&x2,&y2))//ȡ��б����limit�Ľ���
    {
        __GK_DrawOline(gkwin,x1,y1,x2,y2,color,Rop2Code);//����б��

        //���洦��changed_msk��־λͼ���Ѵ��������߶��޸ĵ�������ɫ
        if(gkwin->change_flag != CN_GKWIN_CHANGE_ALL)
        {
            gkwin->change_flag = CN_GKWIN_CHANGE_PART;//��־���ڲ����޸�
            dx = abs(x2-x1);
            dy = abs(y2-y1);
            //x����仯����
            if(dx > dy)
            {
                if(((x1>x2)&&(y1>y2)) || ((x1<x2)&&(y1<y2)))
                {//x��y����������ͬ����ͬ������ͬ��
                    //ѡȡx1��x2�н�С������Ϊx1
                    if(x1 > x2)
                    {
                        __gk_swap(x1,x2);
                        __gk_swap(y1,y2);
                    }
                    //���¶��룬ȡ8*8���أ�xÿ�仯8���غ�y��ֱ�ߵĽ��㣬
                    //��б�������еĵ����ڵ�8*8���������Ӧ��
                    //��changed_msk�е�bit��Ϊ1������˵���뿴djygui����
                    for(x = align_down(8,x1); x <= x2; x+=8)
                    {
                        //�Լ���Ľ�������������봦��x��y����������ͬ
                        //������0.5Ϊ��׼��
                        y = ((((x-x1)<<1)*(y2-y1)+(x2-x1))/((x2-x1)<<1))+y1;
                        msk_x = x/8;    //����x��mskλͼ�еڼ���
                        msk_y = y/8;    //����y��mskλͼ�еڼ���
                        //��changed_msk��Ӧ��bit��1
                        __GK_SetPixelRop2Bm(&(gkwin->changed_msk),msk_x,
                                             msk_y,CN_COLOR_WHITE,CN_R2_COPYPEN);
                        //��־(msk_x��msk_y)�����һ����
                        msk_y += 1;
                        //��changed_msk��Ӧ��bit��1
                        __GK_SetPixelRop2Bm(&(gkwin->changed_msk),msk_x,
                                             msk_y,CN_COLOR_WHITE,CN_R2_COPYPEN);
                    }
                }
                else
                {//x��y���������෴����x����y����x����y��
                    //ѡȡx1��x2�н�С������Ϊx1
                    if(x1 > x2)
                    {
                        __gk_swap(x1,x2);
                        __gk_swap(y1,y2);
                    }
                    //���¶��룬ȡ8*8���أ�xÿ�仯8���غ�y��ֱ�ߵĽ��㣬
                    //��б�������еĵ����ڵ�8*8���������Ӧ��
                    //��changed_msk�е�bit��Ϊ1������˵���뿴djygui����
                    for(x = align_down(8,x1); x <= x2; x+=8)
                    {
                        //�����ý�������������봦��yΪ������������-0.5Ϊ��׼��
                        y = ((((x-x1)<<1)*(y2-y1)-(x2-x1))/((x2-x1)<<1))+y1;
                        msk_x = x/8;    //����x��mskλͼ�еڼ���
                        msk_y = y/8;    //����y��mskλͼ�еڼ���
                        //��changed_msk��Ӧ��bit��1
                        __GK_SetPixelRop2Bm(&(gkwin->changed_msk),msk_x,
                                             msk_y,CN_COLOR_WHITE,CN_R2_COPYPEN);
                        //��־(msk_x��msk_y)�����һ����
                        msk_y -= 1;
                        //��changed_msk��Ӧ��bit��1
                        __GK_SetPixelRop2Bm(&(gkwin->changed_msk),msk_x,
                                             msk_y,CN_COLOR_WHITE,CN_R2_COPYPEN);
                    }
                }
            }else
            {//y����仯���󣬻���x��y�仯�����
                if(((x1>x2)&&(y1>y2)) || ((x1<x2)&&(y1<y2)))
                {//x��y����������ͬ����ͬ������ͬ��
                    //ѡȡy1��y2�н�С������Ϊy1
                    if(y1 > y2)
                    {
                        __gk_swap(x1,x2);
                        __gk_swap(y1,y2);
                    }
                    //���¶��룬ȡ8*8���أ�yÿ�仯8���غ�x��ֱ�ߵĽ��㣬
                    //��б�������еĵ����ڵ�8*8���������Ӧ��
                    //��changed_msk�е�bit��Ϊ1������˵���뿴djygui����
                    for(y = align_down(8,y1); y <= y2; y+=8)
                    {
                        x = ((((y-y1)<<1)*(x2-x1)+(y2-y1))/((y2-y1)<<1))+x1;
                        msk_x = x/8;    //����x��mskλͼ�еڼ���
                        msk_y = y/8;    //����y��mskλͼ�еڼ���
                        //��changed_msk��Ӧ��bit��1
                        __GK_SetPixelRop2Bm(&(gkwin->changed_msk),msk_x,
                                             msk_y,CN_COLOR_WHITE,CN_R2_COPYPEN);
                        msk_x += 1;
                        //��changed_msk��Ӧ��bit��1
                        __GK_SetPixelRop2Bm(&(gkwin->changed_msk),msk_x,
                                             msk_y,CN_COLOR_WHITE,CN_R2_COPYPEN);
                    }
                }
                else
                {//x��y���������෴����x����y����x����y��
                    //ѡȡy1��y2�н�С������Ϊy1
                    if(y1 > y2)
                    {
                        __gk_swap(x1,x2);
                        __gk_swap(y1,y2);
                    }
                    //���¶��룬ȡ8*8���أ�yÿ�仯8���غ�x��ֱ�ߵĽ��㣬
                    //��б�������еĵ����ڵ�8*8���������Ӧ��
                    //��changed_msk�е�bit��Ϊ1������˵���뿴djygui����
                    for(y = align_down(8,y1); y <= y2; y+=8)
                    {
                        x = ((((y-y1)<<1)*(x2-x1)-(y2-y1))/((y2-y1)<<1))+x1;
                        msk_x = x/8;    //����x��mskλͼ�еڼ���
                        msk_y = y/8;    //����y��mskλͼ�еڼ���
                        //��changed_msk��Ӧ��bit��1
                        __GK_SetPixelRop2Bm(&(gkwin->changed_msk),msk_x,
                                             msk_y,CN_COLOR_WHITE,CN_R2_COPYPEN);
                        msk_x -= 1;
                        //��changed_msk��Ӧ��bit��1
                        __GK_SetPixelRop2Bm(&(gkwin->changed_msk),msk_x,
                                                 msk_y,CN_COLOR_WHITE,CN_R2_COPYPEN);
                    }
                }
            }
        }
    }
}
//----��ֱ��(�����˵�)---------------------------------------------------------
//����: �ڴ����ϻ�ֱ�ߣ��˵㲻�����봦��changed_msk����
//����: gkwin��Ŀ�괰��ָ��
//      limit�������ͼ����������һ�����ε�����,�����������ֱ�߲���
//      x1��y1��x2��y2�������յ�����
//      color����ͼ���õ���ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __GK_LinetoBm(struct tagGkWinRsc *gkwin,struct tagRectangle *limit,
                    s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 Rop2Code)
{
    if(y1 == y2)        //��ˮƽ��
        __GK_HlinetoBm(gkwin,limit,x1,y1,x2,color,Rop2Code);
    else if(x1 == x2)   //�洹ֱ��
        __GK_VlinetoBm(gkwin,limit,x1,y1,y2,color,Rop2Code);
    else                //��б��
        __GK_OlinetoBm(gkwin,limit,x1,y1,x2,y2,color,Rop2Code);
}

//----��screen�ϻ�����---------------------------------------------------------
//����: ��screen��ֱ�ӻ��㡣
//����: gkwin�����Ƶ�Ŀ�괰��
//      x��y�� ����
//      color����ͼ���õ���ɫ��cn_sys_pf_e8r8g8b8��ʽ
//      r2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __GK_SetPixelScreen(struct tagDisplayRsc *display,s32 x,s32 y,
                           u32 color,u32 Rop2Code)
{
    struct tagDisplayRsc *mirror,*current;

    display->draw.set_pixel_screen(x,y,color,Rop2Code);//��screen�ϻ���
    mirror = (struct tagDisplayRsc *)display->node.child;
    if(mirror == NULL)      //û�о�����ʾ����ֱ�ӷ���
        return;
    current = mirror;
    while(current != NULL)
    {
        current->draw.set_pixel_screen(x,y,color,Rop2Code);
        current = (struct tagDisplayRsc *)Rsc_TraveScion(&mirror->node,
                &current->node);
    }
}

//----��ֱ��(�����˵�)---------------------------------------------------------
//����: ��screen��ֱ�ӻ�ֱ�ߣ��˵㲻����
//����: display�����Ƶ�Ŀ����ʾ��
//      limit�������ͼ����������һ�����ε����򣬳����������ֱ�߲���
//      x1��y1��x2��y2�������յ�����
//      color����ͼ���õ���ɫ��cn_sys_pf_e8r8g8b8��ʽ
//      r2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __GK_LinetoScreen(struct tagDisplayRsc *display,struct tagRectangle *limit,
                   s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 Rop2Code)
{
    struct tagDisplayRsc *mirror,*current;
    s32 x,y,ax,ay,s1,s2,p,interchange,i;

    if(display->draw.line_screen(limit, x1, y1, x2, y2, color,Rop2Code))
    {
    }else   //Ӳ�����ٲ�֧��ֱ�߻��ƣ����ʵ��
    {
        if(y1 == y2)    //��ˮƽ��
        {
            if(x1 > x2)
                __gk_swap(x1,x2);
            if((y1 < limit->top) || (y1 >= limit->bottom)
                ||(x2 <= limit->left) || (x1 >= limit->right))
                return;         //����ֱ����limit֮��
            if(x1 < limit->left)
                x1 = limit->left;
            if(x2 > limit->right)
                x2 = limit->right;
            for(x = x1; x < x2;x++)
            {
                display->draw.set_pixel_screen(x,y1,color,Rop2Code);
            }
        }else if(x1 == x2)   //�洹ֱ��
        {
            if(y1 > y2)
                __gk_swap(y1,y2);
            if((x1 < limit->left) || (x1 >= limit->right)
                ||(y2 <= limit->top) || (y1 >= limit->bottom))
                return;         //����ֱ����limit֮��
            if(y1 < limit->top)
                y1 = limit->top;
            if(y2 > limit->bottom)
                y2 = limit->bottom;
            for(y = y1; y < y2;y++)
            {
                display->draw.set_pixel_screen(x1,y,color,Rop2Code);
            }
        }else            //��б��
        {
            if( __GK_OlineSectInter(limit,&x1,&y1,&x2,&y2))
            {
                x = x1;
                y = y1;
                ax = abs(x2-x1);
                ay = abs(y2-y1);
                if(x2 > x1)   //����б�߷���
                   s1 = 1;
                else
                   s1 = -1;
                if(y2 > y1)
                   s2 = 1;
                else
                   s2 = -1;
                if(ay > ax)
                {
                    __gk_swap(ax,ay);
                    interchange = 1;
                }
                else
                    interchange = 0;
                p = (ay<<1)-ax;   //���ó�ʼ����б�ֵ
                for(i = 1;i <= ax;i++)
                {
                    display->draw.set_pixel_screen(x,y,color,Rop2Code);

                    if(p >= 0)     //ȡ����ѧ��Ͻ�������
                    {
                        if(interchange == 0)
                           y = y+s2;
                        else
                           x = x+s1;
                        p = p-(ax<<1);
                    }
                    if(interchange == 0)
                        x = x+s1;
                    else
                        y = y+s2;
                    p = p+(ay<<1);
                }
            }
        }
    }
    mirror = (struct tagDisplayRsc *)display->node.child;
    if(mirror == NULL)      //û�о�����ʾ����ֱ�ӷ���
        return;
    current = mirror;
    while(current != NULL)
    {
        current->draw.line_screen(limit, x1, y1, x2, y2, color,Rop2Code);
        current = (struct tagDisplayRsc *)Rsc_TraveScion(&mirror->node,
                &current->node);
    }
}
//----����bitmap��screen-------------------------------------------------------
//����: ��bitmap�Ĳ��ִ��ڴ洫�͵�screen��
//����: gkwin�����Ƶ�Ŀ�괰��
//      rect����ͼ����һ�����ε����򣬳���������Ĳ��ֲ���
//      bitmap�����Ƶ�λͼ
//      x��y��bitmapҪ���Ͳ������Ͻ�����
//����: ��
//-----------------------------------------------------------------------------
void __GK_BltBmToScreen(struct tagDisplayRsc *display,struct tagRectangle *rect,
                            struct tagRectBitmap *bitmap,s32 x,s32 y)
{
    u32 color;
    s32 x_bmap,y_bmap,x_rect,y_rect;
    struct tagDisplayRsc *mirror,*current;

    if(display->draw.bm_to_screen(rect,bitmap,x,y))
    {
    }
    else//bitmap��ɫ��ʽ����ʾ����һ������Ӳ�����ٲ�֧�������ʵ��
    {
        //����bitmap��screen
        y_bmap = y;
        for(y_rect = rect->top;y_rect < rect->bottom;y_rect++)
        {
            x_bmap = x;
            for(x_rect = rect->left;x_rect < rect->right;x_rect++)
            {
                color = GK_GetPixelBm(bitmap,x_bmap,y_bmap);
                color = GK_ConvertColorToRGB24(bitmap->PixelFormat,color,
                                              bitmap->ExColor);
                display->draw.set_pixel_screen(x_rect,y_rect,
                                                color,CN_R2_COPYPEN);
                x_bmap++;
            }
            y_bmap++;
        }
    }
    mirror = (struct tagDisplayRsc *)display->node.child;
    if(mirror == NULL)      //û�о�����ʾ����ֱ�ӷ���
        return;
    current = mirror;
    while(current != NULL)
    {
        current->draw.bm_to_screen(rect,bitmap,x,y);
        current = (struct tagDisplayRsc *)Rsc_TraveScion(&mirror->node,
                                                        &current->node);
    }
}

//----������-----------------------------------------------------------------
//����: ��dst_bitmap��dst_rect��ռ�ݵ�λ���ϣ���color��ɫ��䡣color�ĸ�ʽ��
//      cn_sys_pf_e8r8g8b8��Ҫת����dst_bitmap->pf_typeƥ�����ɫ��䡣
//����: dst_bitmap��������λͼ
//      dst_rect������λ��
//      color��������ɫ����ʽ��cn_sys_pf_e8r8g8b8
//����: ��
//-----------------------------------------------------------------------------
void __GK_FillRect(struct tagRectBitmap *dst_bitmap,
                     struct tagRectangle *dst_rect,u32 color)
{
    s32 x,y,x1,y1,x2,y2;
    u8 bit_offset,i;
    u8 color_bits;
    s32 byte_offset,temp_bytes;
    u32 pf,pf_color;
    u8 bit_offset1,bit_offset2,start_byte,end_byte,middle_byte;
    s32 byte_offset1,byte_offset2;

    x1 = dst_rect->left;
    y1 = dst_rect->top;
    y2 = dst_rect->bottom;
    x2 = dst_rect->right;
    temp_bytes = dst_bitmap->linebytes;
    //color�ĸ�ʽ��cn_sys_pf_e8r8g8b8��ת����dst_bitmap->pf_typeƥ��ĸ�ʽ
    pf_color = GK_ConvertRGB24ToPF(dst_bitmap->PixelFormat,color);
    //������λͼ��ÿ����λ��
    color_bits = (dst_bitmap->PixelFormat & CN_PF_BITWIDE_MSK)>>8;
    start_byte = 0;
    end_byte = 0;
    switch(color_bits)
    {
        case 1:
        {
            pf = pf_color & 0x01;               //ȡ�����������ɫ
            byte_offset1 = y1*temp_bytes+x1/8;  //dst_rect������߽������ֽ�
            byte_offset2 = y1*temp_bytes+x2/8;  //dst_rect�����ұ߽������ֽ�
            bit_offset1 = x1%8;   //dst_rect��߽��������ֽڵ�λ��x=0��bit=7
            bit_offset2 = x2%8;     //7��ȥdst_rect�ұ߽��������ֽڵ�λ��7-bit
            if(pf == 1)
                middle_byte = 0xff;
            else
                middle_byte = 0;
            //dst_rect�����ֽ���β�ֽڲ���һ���ֽ���
            if(byte_offset1 != byte_offset2)
            {
                //dst_rectÿ�е����ֽڸ�λ����ɫ
                //���ֽ�ȡ��ɫ��λ����bit��ͬ���ұ��Ǹ�λ
//                for(i = 0;i <= bit_offset1;i++)
//                    start_byte |= pf<<i;
//                for(i = 0;i <= bit_offset2;i++) //dst_rectÿ�е�β�ֽڸ�λ����ɫ
//                    end_byte |= pf<<(7-i);      //���ֽ�ȡ��ɫ��λ����7-bit��ͬ
                //ȡÿ���м��ֽڵ���ɫ����ɫֻ��0��1�������
                start_byte = middle_byte << bit_offset1;    //����������ֽڸ�λ
                end_byte = middle_byte >> (8-bit_offset2);  //�Ҷ��������ֽڵ�bit
                for(y = y1;y < y2;y++)  //�����ɫ
                {
                    //���dst_rectÿ���м��ֽڵ���ɫ
                    for(x = byte_offset1+1; x < byte_offset2; x++)
                    {
                        dst_bitmap->bm_bits[x] = middle_byte;
                    }
                    dst_bitmap->bm_bits[byte_offset1] &= ~(0xff<<bit_offset1);
                    dst_bitmap->bm_bits[byte_offset1] |= start_byte;  //�ֽ�ͷ��ֵ
                    dst_bitmap->bm_bits[byte_offset2] &= 0xff<<bit_offset2;
                    dst_bitmap->bm_bits[byte_offset2] |= end_byte;    //�ֽ�β��ֵ
                    //�ֽ�������
                    byte_offset1 += temp_bytes;
                    byte_offset2 += temp_bytes;
                }
            }
            else//dst_rect���ֽ���β�ֽ���һ���ֽ���
            {   //bit1����bit2���ң��ұ�Ϊ��λ���˴���bit_offset2=7-bit2
                for(i = 7-bit_offset2;i <= bit_offset1;i++)
                {
                    end_byte |= 1<<i;
                    start_byte |= pf<<i;
                }
                start_byte = ((u8)0xff << bit_offset1)>>bit_offset1;
                start_byte = (start_byte >> (8-bit_offset2))<<(8-bit_offset2);
                end_byte = start_byte & middle_byte;
                for(y = y1;y < y2;y++)
                {   //����Ŀ���ֽ��ڲ���Ҫ����λ
                    dst_bitmap->bm_bits[byte_offset1] &= ~start_byte;
                    //�����Ӧ��λ
                    dst_bitmap->bm_bits[byte_offset1] |= end_byte;
                    byte_offset1 += temp_bytes; //�ֽ�������
                }
            }
        }break;
        case 2:
        {
            pf = pf_color & 0x03;               //ȡ�����������ɫ
            byte_offset1 = y1*temp_bytes+x1/4;  //dst_rect������߽������ֽ�
            byte_offset2 = y1*temp_bytes+x2/4;  //dst_rect�����ұ߽������ֽ�
            bit_offset1 = (x1%4)<<1;//dst_rect��߽��������ֽڵ�λ��x=0��bit=7
            bit_offset2 = (x2%4)<<1;//7��ȥdst_rect�ұ߽��������ֽڵ�λ��7-bit
            middle_byte = (u8)((pf<<6)|(pf<<4)|(pf<<2)|pf);//ÿ���м��ֽڵ���ɫ
            //dst_rect���е����ֽ���β�ֽڲ���һ���ֽ���
            if(byte_offset1 != byte_offset2)
            {   //dst_rectÿ�е����ֽڸ�λ����ɫ
                //���ֽ�ȡ��ɫ��λ����bit��ͬ���ұ��Ǹ�λ��λ����2�ı���
                start_byte = middle_byte << bit_offset1;    //����������ֽڸ�λ
                end_byte = middle_byte >> (8-bit_offset2);  //�Ҷ��������ֽڵ�bit
                for(y = y1;y < y2;y++)  //�����ɫ
                {
                    //���dst_rectÿ���м��ֽڵ���ɫ
                    for(x = byte_offset1+1; x < byte_offset2; x++)
                    {
                        dst_bitmap->bm_bits[x] = middle_byte;
                    }
                    dst_bitmap->bm_bits[byte_offset1] &= ~(0xff<<bit_offset1);
                    dst_bitmap->bm_bits[byte_offset1] |= start_byte;  //�ֽ�ͷ��ֵ
                    dst_bitmap->bm_bits[byte_offset2] &= 0xff<<bit_offset2;
                    dst_bitmap->bm_bits[byte_offset2] |= end_byte;    //�ֽ�β��ֵ
                    //�ֽ�������
                    byte_offset1 += temp_bytes;
                    byte_offset2 += temp_bytes;
                }
            }
            else//dst_rect���е����ֽ���β�ֽ���һ���ֽ���
            {   //bit1����bit2���ң��ұ�Ϊ��λ���˴���bit_offset2=7-bit2
                for(i = 6-bit_offset2;i <= bit_offset1;i = i+2)
                {
                    end_byte |= 0x3<<i;
                    start_byte |= pf<<i;
                }
                start_byte = ((u8)0xff << bit_offset1)>>bit_offset1;
                start_byte = (start_byte >> (8-bit_offset2))<<(8-bit_offset2);
                end_byte = start_byte & middle_byte;
                for(y = y1;y < y2;y++)
                {   //����Ŀ���ֽ��ڲ���Ҫ����λ
                    dst_bitmap->bm_bits[byte_offset1] &= ~start_byte;
                    //�����Ӧ��λ
                    dst_bitmap->bm_bits[byte_offset1] |= end_byte;
                    byte_offset1 += temp_bytes; //�ֽ�������
                }
            }
        }break;
        case 4:
        {
            pf = pf_color & 0xf;                //ȡ�����������ɫ
            byte_offset1 = y1*temp_bytes+x1/2;  //dst_rect������߽������ֽ�
            byte_offset2 = y1*temp_bytes+x2/2;  //dst_rect�����ұ߽������ֽ�
            bit_offset1 = (x1%2)<<2;//dst_rect��߽��������ֽڵ�λ��x=0��bit=7
            bit_offset2 = (x2%2)<<2;//7��ȥdst_rect�ұ߽��������ֽڵ�λ��7-bit
            middle_byte = (u8)((pf<<4)|pf);     //ÿ���м��ֽڵ���ɫ
            //dst_rect���е����ֽ���β�ֽڲ���һ���ֽ���
            if(byte_offset1 != byte_offset2)
            {
                start_byte = middle_byte << bit_offset1;    //����������ֽڸ�λ
                end_byte = middle_byte >> (8-bit_offset2);  //�Ҷ��������ֽڵ�bit
                for(y = y1;y < y2;y++)          //�����ɫ
                {
                    //���dst_rectÿ���м��ֽڵ���ɫ
                    for(x = byte_offset1+1; x < byte_offset2; x++)
                    {
                        dst_bitmap->bm_bits[x] = middle_byte;
                    }
                    dst_bitmap->bm_bits[byte_offset1] &= ~(0xff<<bit_offset1);
                    dst_bitmap->bm_bits[byte_offset1] |= start_byte;  //�ֽ�ͷ��ֵ
                    dst_bitmap->bm_bits[byte_offset2] &= 0xff<<bit_offset2;
                    dst_bitmap->bm_bits[byte_offset2] |= end_byte;    //�ֽ�β��ֵ
                    //�ֽ�������
                    byte_offset1 += temp_bytes;
                    byte_offset2 += temp_bytes;
                }
            }
            else//dst_rect���е����ֽ���β�ֽ���һ���ֽ���
            {   //bit1����bit2���ң��ұ�Ϊ��λ���˴���bit_offset2=7-bit2
                for(i = 4-bit_offset2;i <= bit_offset1;i = i+4)
                {
                    end_byte |= 0xf<<i;
                    start_byte |= pf<<i;
                }
                start_byte = ((u8)0xff << bit_offset1)>>bit_offset1;
                start_byte = (start_byte >> (8-bit_offset2))<<(8-bit_offset2);
                end_byte = start_byte & middle_byte;
                for(y = y1;y < y2;y++)
                {   //����Ŀ���ֽ��ڲ���Ҫ����λ
                    dst_bitmap->bm_bits[byte_offset1] &= ~start_byte;
                    //�����Ӧ��λ
                    dst_bitmap->bm_bits[byte_offset1] |= end_byte;
                    byte_offset1 += temp_bytes; //�ֽ�������
                }
            }
        }break;
        case 8:
        {
            pf = pf_color;                      //ȡ�����������ɫ
            byte_offset1 = y1*temp_bytes+x1;    //dst_rect������߽������ֽ�
            byte_offset2 = y1*temp_bytes+x2;    //dst_rect�����ұ߽������ֽ�
            for(y = y1;y < y2;y++)      //�����ɫ��ÿ�����һ������
            {
                //���dst_rectÿ���м��ֽڵ���ɫ
                for(x = byte_offset1;x <= byte_offset2;x++)
                {
                    dst_bitmap->bm_bits[x] = (u8)pf;
                }
                //�ֽ�������
                byte_offset1 += temp_bytes;
                byte_offset2 += temp_bytes;
             }
        }break;
        case 12:
        {
            pf = pf_color;           //ȡ�����������ɫ
            for(y = y1;y < y2;y++)  //�����ɫ
            {
                temp_bytes = y*temp_bytes;              //y�е��ֽ�ƫ����
                for(x = x1;x < x2;x++)
                {
                    byte_offset = temp_bytes + x*3/2;   //Ŀ�����������ֽ�
                    bit_offset = (x*12)%8;              //Ŀ�����ص�λƫ����
                    if(bit_offset == 0)
                    {
                        dst_bitmap->bm_bits[byte_offset] = (u8)(pf&0xff);
                        dst_bitmap->bm_bits[byte_offset+1] &= 0xf0;
                        dst_bitmap->bm_bits[byte_offset+1] |= (u8)(pf>>8);
                    }else
                    {
                        dst_bitmap->bm_bits[byte_offset] &= 0x0f;
                        dst_bitmap->bm_bits[byte_offset] |= (u8)pf<<4;
                        dst_bitmap->bm_bits[byte_offset+1] = (u8)(pf>>4);
                    }
                }
             }
        }break;
        case 16:
        {
            pf = pf_color;                       //ȡ�����������ɫ
            if((u32)dst_bitmap->bm_bits & 1 )      //�Ƕ����ַ
            {
                byte_offset1 = y1*temp_bytes+x1*2;  //dst_rect������߽������ֽ�
                byte_offset2 = y1*temp_bytes+x2*2;  //dst_rect�����ұ߽������ֽ�
                for(y = y1;y < y2;y++)              //�����ɫ��ÿ�������������
                {
                    for(x = byte_offset1;x <= byte_offset2;x = x+2)
                    {
    //                    alpha = pf;
    //                    if(dst_bitmap->PixelFormat == CN_SYS_PF_ARGB1555)
    //                    {
    //                        pf &= 0x7fff;
    //                        pf |= alpha & 0x8000;
    //                    }
                        //ע:��������⣬���ɰ�Ŀ���ַǿ��ת����16λָ�롣
                        dst_bitmap->bm_bits[x] = (u8)pf;
                        dst_bitmap->bm_bits[x+1] = (u8)(pf>>8);
                    }
                    //�ֽ�������
                    byte_offset1 += temp_bytes;
                    byte_offset2 += temp_bytes;
                }
            }
            else                        //����ĵ�ַ
            {
                u16 *vm;
                vm = (u16*)((u32)dst_bitmap->bm_bits+y1*dst_bitmap->linebytes);
                for(y = y1;y < y2;y++)              //�����ɫ��ÿ�������������
                {
                    for(x = x1;x < x2;x++)
                    {
    //                    alpha = pf;
    //                    if(dst_bitmap->PixelFormat == CN_SYS_PF_ARGB1555)
    //                    {
    //                        pf &= 0x7fff;
    //                        pf |= alpha & 0x8000;
    //                    }
                        vm[x] = pf;
                    }
                    vm += dst_bitmap->linebytes>>1;
                }

            }
        }break;
        case 24:
        {
            pf = pf_color;                      //ȡ�����������ɫ
            byte_offset1 = y1*temp_bytes+x1*3;  //dst_rect������߽������ֽ�
            byte_offset2 = y1*temp_bytes+x2*3;  //dst_rect�����ұ߽������ֽ�
            for(y = y1;y < y2;y++)              //�����ɫ��ÿ�����һ������
            {
                for(x = byte_offset1;x <= byte_offset2;x = x+3)
                {
                    dst_bitmap->bm_bits[x] = (u8)pf;
                    dst_bitmap->bm_bits[x+1] = (u8)(pf >> 8);
                    dst_bitmap->bm_bits[x+2] = (u8)(pf >> 16);
                }
                //�ֽ�������
                byte_offset1 += temp_bytes;
                byte_offset2 += temp_bytes;
             }
        }break;
        case 32:
        {
            pf = pf_color;                      //ȡ�����������ɫ
            byte_offset1 = y1*temp_bytes+x1*4;  //dst_rect������߽������ֽ�
            byte_offset2 = y1*temp_bytes+x2*4;  //dst_rect�����ұ߽������ֽ�
            for(y = y1;y < y2;y++)              //�����ɫ��ÿ�����һ������
            {
                for(x = byte_offset1;x <= byte_offset2;x = x+4)
                {
                    pf = pf_color;
//                    alpha = pf;
//                    if(dst_bitmap->PixelFormat == CN_SYS_PF_ARGB8888)
//                    {
//                        pf &= 0x00ffffff;
//                        pf |= alpha & 0xff000000;
//                    }
                    //ע:��������⣬���ɰ�Ŀ���ַǿ��ת����32λָ�롣
                    dst_bitmap->bm_bits[x] = (u8)pf;
                    dst_bitmap->bm_bits[x+1] = (u8)(pf >> 8);
                    dst_bitmap->bm_bits[x+2] = (u8)(pf >> 16);
                    dst_bitmap->bm_bits[x+3] = (u8)(pf >> 24);
                }
                //�ֽ�������
                byte_offset1 += temp_bytes;
                byte_offset2 += temp_bytes;
             }
        }break;
    }
}

//----���λͼ-----------------------------------------------------------------
//����: ��������__GK_FillRect���������������Ҫ���ľ���������bitmap��
//����: dst_bitmap,������λͼ
//      color��������ɫ����ʽ��cn_sys_pf_e8r8g8b8
//����: ��
//-----------------------------------------------------------------------------
void __GK_FillBm(struct tagRectBitmap *dst_bitmap,u32 color)
{
    u32 pf_color,color_bits;
    u32 byte_offset,total_bytes;

    //color�ĸ�ʽ��cn_sys_pf_e8r8g8b8��ת����dst_bitmap->pf_typeƥ��ĸ�ʽ
    pf_color = GK_ConvertRGB24ToPF(dst_bitmap->PixelFormat,color);
    //������λͼ��ÿ����λ��
    color_bits = (dst_bitmap->PixelFormat & CN_PF_BITWIDE_MSK)>>8;
    switch(color_bits)
    {
        case 1:
        {//�����ɫ��ֱ�ӽ���Ӧ��λ����
            if(1 == pf_color)
                memset( dst_bitmap->bm_bits,0xff,
                        dst_bitmap->linebytes*dst_bitmap->height);
            else
                memset( dst_bitmap->bm_bits,0,
                        dst_bitmap->linebytes*dst_bitmap->height);
        }break;
        case 2:
        {//�����ɫ��ֱ�ӽ���Ӧ��λ����
            memset( dst_bitmap->bm_bits,
                    pf_color + (pf_color<<2) + (pf_color<<4) + (pf_color<<6),
                    dst_bitmap->linebytes*dst_bitmap->height);
        }break;
        case 4:
        {//�����ɫ��ֱ�ӽ���Ӧ��λ����
            memset( dst_bitmap->bm_bits,(u8)(pf_color|(pf_color<<4)),
                    dst_bitmap->linebytes*dst_bitmap->height);
        }break;
        case 8:
        {//ֱ�ӽ���Ӧ��λ����
            memset( dst_bitmap->bm_bits,(u8)pf_color,
                    dst_bitmap->linebytes*dst_bitmap->height);
        }break;
        case 12:
        {   //�ܹ���Ҫ�����ֽ���
            total_bytes = dst_bitmap->linebytes*dst_bitmap->height;
            //12λ����ɫ��ʽ��һ�ο����3���ֽڣ�����������
            for(byte_offset = 0;byte_offset < total_bytes;)
            {
                dst_bitmap->bm_bits[byte_offset] = (u8)(pf_color&0xff);
                dst_bitmap->bm_bits[byte_offset+1] = (u8)((pf_color>>8)&0xf);
                dst_bitmap->bm_bits[byte_offset+1] |= (u8)((pf_color&0xf)<<4);
                dst_bitmap->bm_bits[byte_offset+2] = (u8)(pf_color>>4);
                byte_offset += 3;
            }
        }break;
        case 16:
        {   //�ܹ���Ҫ�����ֽ���
            total_bytes = dst_bitmap->linebytes*dst_bitmap->height;
            if((u32)dst_bitmap->bm_bits & 1 )
            {
                for(byte_offset = 0;byte_offset < total_bytes;byte_offset += 2)
                {
    //                alpha = pf_color;
    //                if(dst_bitmap->PixelFormat == CN_SYS_PF_ARGB1555)
    //                {
    //                    pf_color &= 0x7fff;
    //                    pf_color |= alpha & 0x8000;
    //                }
                    //ע:��������⣬���ɰ�Ŀ���ַǿ��ת����16λָ�롣
                    //һ��дһ�����أ������ֽ�
                    dst_bitmap->bm_bits[byte_offset] = (u8)pf_color;
                    dst_bitmap->bm_bits[byte_offset+1] = (u8)(pf_color>>8);
                }
            }
            else
            {
                u16 *vm;
                vm = (u16*)(u32)dst_bitmap->bm_bits;
                for(byte_offset = 0;byte_offset < total_bytes>>1;byte_offset++)
                {
                    vm[byte_offset] = pf_color;
                }
            }
        }break;
        case 24:
        {   //�ܹ���Ҫ�����ֽ���
            total_bytes = dst_bitmap->linebytes*dst_bitmap->height;
            for(byte_offset = 0;byte_offset < total_bytes;)
            {
                dst_bitmap->bm_bits[byte_offset] = (u8)pf_color;
                dst_bitmap->bm_bits[byte_offset+1] = (u8)(pf_color >> 8);
                dst_bitmap->bm_bits[byte_offset+2] = (u8)(pf_color >> 16);
                byte_offset += 3;   //һ��дһ�����أ������ֽ�
             }
        }break;
        case 32:
        {   //�ܹ���Ҫ�����ֽ���
            total_bytes = dst_bitmap->linebytes*dst_bitmap->height;
            for(byte_offset = 0;byte_offset < total_bytes;)
            {
//                alpha = pf_color;
//                if(dst_bitmap->PixelFormat == CN_SYS_PF_ARGB8888)
//                {
//                    pf_color &= 0x00ffffff;
//                    pf_color |= alpha & 0xff000000;
//                }
                //ע:��������⣬���ɰ�Ŀ���ַǿ��ת����32λָ�롣
                dst_bitmap->bm_bits[byte_offset] = (u8)pf_color;
                dst_bitmap->bm_bits[byte_offset+1] = (u8)(pf_color >> 8);
                dst_bitmap->bm_bits[byte_offset+2] = (u8)(pf_color >> 16);
                dst_bitmap->bm_bits[byte_offset+3] = (u8)(pf_color >> 24);
                byte_offset += 4;   //һ��дһ�����أ��ĸ��ֽ�
             }
        }break;
    }
}
//----����8�����ص�------------------------------------------------------------
//����: ����8�����ص㣬�ڻ�Բ������µ��ã�������Բ�İ˷����ԣ�
//      ͬʱ��־changed_msk��
//����: gkwin��Ŀ�괰��
//      bitmap��Ŀ��λͼ
//      x0��y0��Բ������
//      x��y��Ҫ���Ƶĵ�����꣬�����Բ��
//      color����������ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __gk_set_all_pixels_circle(struct tagGkWinRsc *gkwin,
                                struct tagRectBitmap *bitmap,s32 x0,s32 y0,
                                s32 x,s32 y,u32 color,u32 Rop2Code)
{
    //����8�����ص�
    __GK_SetPixelRop2Bm(bitmap,x0+x,y0+y,color,Rop2Code);
    __GK_SetPixelRop2Bm(bitmap,x0+x,y0-y,color,Rop2Code);
    __GK_SetPixelRop2Bm(bitmap,x0-x,y0+y,color,Rop2Code);
    __GK_SetPixelRop2Bm(bitmap,x0-x,y0-y,color,Rop2Code);
    __GK_SetPixelRop2Bm(bitmap,x0+y,y0+x,color,Rop2Code);
    __GK_SetPixelRop2Bm(bitmap,x0+y,y0-x,color,Rop2Code);
    __GK_SetPixelRop2Bm(bitmap,x0-y,y0+x,color,Rop2Code);
    __GK_SetPixelRop2Bm(bitmap,x0-y,y0-x,color,Rop2Code);
    //��8�����ص�ֱ����changed_msk��־
    __GK_SetPixelRop2Bm(&(gkwin->changed_msk),(x0+x)/8,(y0+y)/8,
                                CN_COLOR_WHITE,CN_R2_COPYPEN);
    __GK_SetPixelRop2Bm(&(gkwin->changed_msk),(x0+x)/8,(y0-y)/8,
                                CN_COLOR_WHITE,CN_R2_COPYPEN);
    __GK_SetPixelRop2Bm(&(gkwin->changed_msk),(x0-x)/8,(y0+y)/8,
                                CN_COLOR_WHITE,CN_R2_COPYPEN);
    __GK_SetPixelRop2Bm(&(gkwin->changed_msk),(x0-x)/8,(y0-y)/8,
                                CN_COLOR_WHITE,CN_R2_COPYPEN);
    __GK_SetPixelRop2Bm(&(gkwin->changed_msk),(x0+y)/8,(y0+x)/8,
                                CN_COLOR_WHITE,CN_R2_COPYPEN);
    __GK_SetPixelRop2Bm(&(gkwin->changed_msk),(x0+y)/8,(y0-x)/8,
                                CN_COLOR_WHITE,CN_R2_COPYPEN);
    __GK_SetPixelRop2Bm(&(gkwin->changed_msk),(x0-y)/8,(y0+x)/8,
                                CN_COLOR_WHITE,CN_R2_COPYPEN);
    __GK_SetPixelRop2Bm(&(gkwin->changed_msk),(x0-y)/8,(y0-x)/8,
                                CN_COLOR_WHITE,CN_R2_COPYPEN);
}
//----����Ŀ��㲢��ɫ---------------------------------------------------------
//����: ����Ŀ��㣬ͬʱ��־changed_msk��
//����: gkwin��Ŀ�괰��
//      limit�����Ƶ���������ֻ����ֱ����limit�����ڲ��Ĳ���
//      bitmap��Ŀ��λͼ
//      x��y��Ҫ���Ƶĵ������
//      color����������ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __gk_pixel_sect_inter(struct tagGkWinRsc *gkwin,struct tagRectangle *limit,
                                struct tagRectBitmap *bitmap,s32 x,s32 y,
                                u32 color,u32 Rop2Code)
{
    if((x >= limit->left)&&(x < limit->right)
        &&(y >= limit->top)&&(y < limit->bottom))
    {//�����Ƶ�Ŀ������������ڣ����㲢��־changed_msk
        __GK_SetPixelRop2Bm(bitmap,x,y,color,Rop2Code);
        __GK_SetPixelRop2Bm(&(gkwin->changed_msk),x/8,y/8,
                                CN_COLOR_WHITE,CN_R2_COPYPEN);
    }
    else    //�����Ƶ�Ŀ������������⣬ֱ�ӷ���
        return;
}
//----���ƶ��Ŀ��㲢��־-----------------------------------------------------
//����: ���ƶ��Ŀ��㣬ͬʱ��־changed_msk��
//����: gkwin��Ŀ�괰��
//      limit�����Ƶ���������ֻ����ֱ����limit�����ڲ��Ĳ���
//      bitmap��Ŀ��λͼ
//      x0��y0��Բ������
//      x��y��Ҫ���Ƶĵ������
//      color����������ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __gk_set_part_pixels_circle(struct tagGkWinRsc *gkwin,struct tagRectangle *limit,
                                struct tagRectBitmap *bitmap,s32 x0,s32 y0,
                                s32 x,s32 y,u32 color,u32 Rop2Code)
{
    __gk_pixel_sect_inter(gkwin,limit,bitmap,x0+x,y0+y,color,Rop2Code);
    __gk_pixel_sect_inter(gkwin,limit,bitmap,x0+x,y0-y,color,Rop2Code);
    __gk_pixel_sect_inter(gkwin,limit,bitmap,x0-x,y0+y,color,Rop2Code);
    __gk_pixel_sect_inter(gkwin,limit,bitmap,x0-x,y0-y,color,Rop2Code);
    __gk_pixel_sect_inter(gkwin,limit,bitmap,x0+y,y0+x,color,Rop2Code);
    __gk_pixel_sect_inter(gkwin,limit,bitmap,x0+y,y0-x,color,Rop2Code);
    __gk_pixel_sect_inter(gkwin,limit,bitmap,x0-y,y0+x,color,Rop2Code);
    __gk_pixel_sect_inter(gkwin,limit,bitmap,x0-y,y0-x,color,Rop2Code);
}
//----��Բ---------------------------------------------------------------------
//����: �ڴ����ڻ�һ��Բ��
//����: gkwin��Ŀ�괰��
//      limit�����Ƶ���������ֻ����ֱ����limit�����ڲ��Ĳ���
//      x0��y0��Բ������
//      r��Բ�İ뾶
//      color����������ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//˵��: ʹ���е㻭Բ�㷨
//-----------------------------------------------------------------------------
void __GK_DrawCircleBm(struct tagGkWinRsc *gkwin,struct tagRectangle *limit,
                        s32 x0,s32 y0,s32 r,u32 color,u32 Rop2Code)
{
    s32 x,y,e;
    s32 left,right,top,bottom;
    u32 pf_color,flag;
    struct tagRectBitmap *bitmap;

    //ѡȡ(x,y)Ϊ(0,r)��Ϊ��ʼ����(����ʵ�ʵ�Բ��ƽ�Ƶ�ԭ���Բ)��Բ�ϵ������
    x = 0;
    y = r;
    e = 1-r;
    left = limit->left-x0;      //������߽絽Բ��x����ľ���
    top = limit->top-y0;        //������߽絽Բ��y����ľ���
    right = limit->right-x0;    //�����ұ߽絽Բ��x����ľ���
    bottom = limit->bottom-y0;  //�����ұ߽絽Բ��y����ľ���
    bitmap = gkwin->wm_bitmap;
    //Ҫ���������ɫ����ɫ��ʽΪ24λ
    //��������ǰ�轫24λ����Ϊ����ʾ��һ�µ���ɫ��ʽ
    pf_color = GK_ConvertRGB24ToPF(gkwin->disp->pixel_format,color);

    //����Բ����limit��
    if((x0-limit->left >= r)&&(limit->right-x0 > r)
        &&(y0-limit->top >= r)&&(limit->bottom-y0 > r))
        flag = 1;
    //Բ�ϵĵ㲻����limit�ڣ�������Բ�ڴ�����Ͳ����ڴ������������
    else if((left*left+top*top < r*r)
        &&(right*right+top*top < r*r)
        &&(left*left+bottom*bottom < r*r)
        &&(right*right+bottom*bottom < r*r))
        return;
    //Բ�ϵĵ㲿����limit��
    else
        flag = 0;
    if(flag)
    {//����Բ����limit�ڣ�������ķ�֮һ��Բ������
     //���ݶԳ��ԣ��ɵ�Բ���������е������
        while(x <= y)
        {
            //�����жϣ�����8�����أ�ͬʱ��־���ص�changed_msk
            __gk_set_all_pixels_circle(gkwin,bitmap,x0,y0,x,y,
                                                pf_color,Rop2Code);
            if(e < 0)
                e += x*2+3;
            else
            {
                e += 2*(x-y)+5;
                y--;
            }
            x++;
        }
    }
    else
    {//Բ�ϲ��ֵ���limit�ڣ���Ҫ��Բ�ϸ������Ƿ���limit�������ж�
        while(x <= y)
        {
            //�ж�Ҫ���Ƶ��λ�ú󣬾����Ƿ���ƣ����Ƶ�����ͬʱ��־changed_msk
            __gk_set_part_pixels_circle(gkwin,limit,bitmap,x0,y0,x,y,
                                                pf_color,Rop2Code);
            if(e < 0)
                e += x*2+3;
            else
            {
                e += 2*(x-y)+5;
                y--;
            }
            x++;
        }
    }
    if(gkwin->change_flag != CN_GKWIN_CHANGE_ALL)
        //��һ��Բֻ�ı��˴��ڵĲ�������
        gkwin->change_flag = CN_GKWIN_CHANGE_PART;
}
//----Ŀ���Ļ���-------------------------------------------------------------
//����: ��Ŀ������ڵ�λ�ã������Ƿ���Ҫ���ơ�
//����: gkwin��Ŀ�괰��
//      limit�����Ƶ���������ֻ����ֱ����limit�����ڲ��Ĳ���
//      x��y��Ŀ��������
//      color����Բʹ�õ���ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __Gk_PixelSectInterScreen(struct tagDisplayRsc *display,
                                    struct tagRectangle *limit,
                                    s32 x,s32 y,u32 color,u32 Rop2Code)
{
    if((x >= limit->left)&&(x < limit->right)
        &&(y >= limit->top)&&(y < limit->bottom))
    //Ŀ�����limit�ڣ���Ҫ����
        display->draw.set_pixel_screen(x,y,color,Rop2Code);
    else
        return;
}

//----��screen�ϻ�Բ-----------------------------------------------------------
//����: ��screen�ϻ�һ��Բ��
//����: gkwin��Ŀ�괰��
//      limit�����Ƶ���������ֻ����ֱ����limit�����ڲ��Ĳ���
//      x0��y0��Բ������
//      r��Բ�İ뾶
//      color����Բʹ�õ���ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//ʹ���е㻭Բ�㷨
//-----------------------------------------------------------------------------
void __GK_DrawCircleScreen(struct tagDisplayRsc *display,struct tagRectangle *limit,
                        s32 x0,s32 y0,s32 r,u32 color,u32 Rop2Code)//ȷ��
{
    s32 x,y,e;
    s32 left,right,top,bottom;
    u32 flag;
    struct tagDisplayRsc *mirror,*current;
    struct tagDispDraw *mydraw;

    mydraw = &display->draw;
    //ѡȡ(x,y)Ϊ(0,r)��Ϊ��ʼ����(����ʵ�ʵ�Բ��ƽ�Ƶ�ԭ���Բ)��Բ�ϵ������
    x = 0;
    y = r;
    e = 1-r;
    left = x0 - limit->left;
    top = y0 - limit->top;
    right = limit->right-x0;
    bottom = limit->bottom-y0;
//    pf_color = GK_ConvertRGB24ToPF(display->pixel_format,color);

    //����Բ����limit��
    if((x0-limit->left >= r)&&(limit->right-x0 > r)
        &&(y0-limit->top >= r)&&(limit->bottom-y0 > r))
        flag = 1;

    //Բ�ϵĵ㶼����limit��
    else if((left*left+top*top < r*r)
        &&(right*right+top*top < r*r)
        &&(left*left+bottom*bottom < r*r)
        &&(right*right+bottom*bottom < r*r))
        return;
    //Բ�ϵĵ㲿����limit��
    else
        flag = 0;

    if(flag)
    {//����Բ����limit�ڣ�����Բ�İ˷����ԣ�ֻ��Ҫ����˷�֮һ��Բ������
        while(x <= y)
        {
            mydraw->set_pixel_screen(x0+x,y0+y,color,Rop2Code);
            mydraw->set_pixel_screen(x0+x,y0-y,color,Rop2Code);
            mydraw->set_pixel_screen(x0-x,y0+y,color,Rop2Code);
            mydraw->set_pixel_screen(x0-x,y0-y,color,Rop2Code);
            mydraw->set_pixel_screen(x0+y,y0+x,color,Rop2Code);
            mydraw->set_pixel_screen(x0+y,y0-x,color,Rop2Code);
            mydraw->set_pixel_screen(x0-y,y0+x,color,Rop2Code);
            mydraw->set_pixel_screen(x0-y,y0-x,color,Rop2Code);
            if(e < 0)
                e += x*2+3;
            else
            {
                e += 2*(x-y)+5;
                y--;
            }
            x++;
        }
    }
    else
    {//Բ�ϵĵ㲿����limit�ڣ���Ҫ����Բ�ϸ������Ƿ���limit��
        while(x <= y)
        {
            __Gk_PixelSectInterScreen(display,limit,
                                x0+x,y0+y,color,Rop2Code);
            __Gk_PixelSectInterScreen(display,limit,
                                x0+x,y0-y,color,Rop2Code);
            __Gk_PixelSectInterScreen(display,limit,
                                x0-x,y0+y,color,Rop2Code);
            __Gk_PixelSectInterScreen(display,limit,
                                x0-x,y0-y,color,Rop2Code);
            __Gk_PixelSectInterScreen(display,limit,
                                x0+y,y0+x,color,Rop2Code);
            __Gk_PixelSectInterScreen(display,limit,
                                x0+y,y0-x,color,Rop2Code);
            __Gk_PixelSectInterScreen(display,limit,
                                x0-y,y0+x,color,Rop2Code);
            __Gk_PixelSectInterScreen(display,limit,
                                x0-y,y0-x,color,Rop2Code);
            if(e < 0)
                e += x*2+3;
            else
            {
                e += 2*(x-y)+5;
                y--;
            }
            x++;
        }
    }
    //������ʾ
    mirror = (struct tagDisplayRsc *)display->node.child;
    if(mirror == NULL)      //û�о�����ʾ����ֱ�ӷ���
        return;
    current = mirror;
    while(current != NULL)
    {//���ھ�����ʾ��
        x = 0;
        y = r;
        if(flag)
        {//����Բ����limit�ڣ�����Բ�İ˷����ԣ�ֻ��Ҫ����˷�֮һ��Բ������
            while(x <= y)
            {
                current->draw.set_pixel_screen(x0+x,y0+y,color,Rop2Code);
                current->draw.set_pixel_screen(x0+x,y0-y,color,Rop2Code);
                current->draw.set_pixel_screen(x0-x,y0+y,color,Rop2Code);
                current->draw.set_pixel_screen(x0-x,y0-y,color,Rop2Code);
                current->draw.set_pixel_screen(x0+y,y0+x,color,Rop2Code);
                current->draw.set_pixel_screen(x0+y,y0-x,color,Rop2Code);
                current->draw.set_pixel_screen(x0-y,y0+x,color,Rop2Code);
                current->draw.set_pixel_screen(x0-y,y0-x,color,Rop2Code);
                if(e < 0)
                    e += x*2+3;
                else
                {
                    e += 2*(x-y)+5;
                    y--;
                }
                x++;
            }
        }
        else
        {//Բ�ϵĵ㲿����limit�ڣ���Ҫ����Բ�ϸ������Ƿ���limit��
            while(x <= y)
            {
                //ֻ������limit�ڵ�����
                if((x0+x >= limit->left)&&(x0+x < limit->right)
                        &&(y0+y >= limit->top)&&(y0+y < limit->bottom))
                    current->draw.set_pixel_screen(x0+x,y0+y,color,Rop2Code);
                if((x0+x >= limit->left)&&(x0+x < limit->right)
                        &&(y0-y >= limit->top)&&(y0+y < limit->bottom))
                    current->draw.set_pixel_screen(x0+x,y0-y,color,Rop2Code);
                if((x0-x >= limit->left)&&(x0-x < limit->right)
                        &&(y0+y >= limit->top)&&(y0+y < limit->bottom))
                    current->draw.set_pixel_screen(x0-x,y0+y,color,Rop2Code);
                if((x0-x >= limit->left)&&(x0-x < limit->right)
                        &&(y0-y >= limit->top)&&(y0-y < limit->bottom))
                    current->draw.set_pixel_screen(x0-x,y0-y,color,Rop2Code);
                if((x0+y >= limit->left)&&(x0+y < limit->right)
                        &&(y0+x >= limit->top)&&(y0+x < limit->bottom))
                    current->draw.set_pixel_screen(x0+y,y0+x,color,Rop2Code);
                if((x0+y >= limit->left)&&(x0+y < limit->right)
                        &&(y0-x >= limit->top)&&(y0-x < limit->bottom))
                    current->draw.set_pixel_screen(x0+y,y0-x,color,Rop2Code);
                if((x0-y >= limit->left)&&(x0-y < limit->right)
                        &&(y0+x >= limit->top)&&(y0+x < limit->bottom))
                    current->draw.set_pixel_screen(x0-y,y0+x,color,Rop2Code);
                if((x0-y >= limit->left)&&(x0-y < limit->right)
                        &&(y0-x >= limit->top)&&(y0-x < limit->bottom))
                    current->draw.set_pixel_screen(x0-y,y0-x,color,Rop2Code);
                if(e < 0)
                    e += x*2+3;
                else
                {
                    e += 2*(x-y)+5;
                    y--;
                }
                x++;
            }
        }
        current = (struct tagDisplayRsc *)Rsc_TraveScion(&mirror->node,
                        &current->node);
    }
}
//----��Բ---------------------------------------------------------------------
//����: �ڴ����ڻ�һ��Բ��
//����: gkwin��Ŀ�괰��
//      x0��y0��Բ�����꣬����ڴ���
//      r��Բ�İ뾶
//      color����Բʹ�õ���ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void GK_DrawCircle(struct tagGkscParaDrawCircle *para)//ȷ��
{
    struct tagRectangle limit;
    struct tagClipRect *clip;
    struct tagGkWinRsc *fb_gkwin,*cirwin;
//    struct tagDispDraw *my_draw_fun;
    s32 offsetx,offsety;
    if((para->Rop2Code == CN_R2_NOP) || (para->Rop2Code > CN_R2_LAST))
        return;                                         //��ִ�в���
    if(para->r <= 0)                                         //�������
        return;
    cirwin = para->gkwin;
//    my_draw_fun = &cirwin->disp->draw;
    //˵����win buffer����ֱ��д������Ϊfalse
    if((cirwin->wm_bitmap->bm_bits != NULL)
        && (cirwin->direct_screen ==false))
    {   //������:��win buffer�л�ͼ����־changed_msk
        limit.left = 0;
        limit.top = 0;
        limit.right = cirwin->wm_bitmap->width;
        limit.bottom = cirwin->wm_bitmap->height;
        //��Ӳ�����ٲ�֧��Բ�Ļ��ƿ��ǣ������ʵ��
        __GK_DrawCircleBm(cirwin,&limit,para->x0,para->y0,para->r,
                                    para->color,para->Rop2Code);
    }else
    {
        clip = cirwin->visible_clip;
        if(clip == NULL)
            return ;
        fb_gkwin = cirwin->disp->frame_buffer;
        offsetx = cirwin->absx0;
        offsety = cirwin->absy0;
        //��frame buffer����ֱ��д������Ϊfalse
        if((fb_gkwin != NULL) && (cirwin->direct_screen == false))
        {   //������:��frame buffer�л�ͼ����ֻ��gkwin�еĿ�������
            do{
                //��Ӳ�����ٲ�֧��Բ�Ļ��ƿ��ǣ������ʵ��
                __GK_DrawCircleBm(fb_gkwin,&clip->rect,
                                    para->x0+offsetx,para->y0+offsety,
                                    para->r,para->color,para->Rop2Code);
                clip = clip->next;
            }while(clip != cirwin->visible_clip);
        }else
        //��win buffer��Ҳ��frame buffer��ֱ�ӻ���screen��
        //ֱ��д������Ϊtrue���������޻���������ֱ�ӻ���screen��
        {
            do{
                __GK_DrawCircleScreen(cirwin->disp,&clip->rect,
                            para->x0+offsetx,para->y0+offsety,para->r,
                            para->color,para->Rop2Code);
                clip = clip->next;
            }while(clip != cirwin->visible_clip);
        }
    }
}

//----������������-------------------------------------------------------------
//����: �ڴ����ϻ����������ߡ�
//����: gkwin��Ŀ�괰��
//      limit�������ͼ����������һ�����ε����򣬳����������ֱ�߲���
//      x1��y1��x2��y2��x3��y3��x4��y4���˵�����
//      color����ͼ���õ���ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __GK_BezierBm(struct tagGkWinRsc *gkwin,struct tagRectangle *limit,
                    float x1,float y1,float x2,float y2,float x3,float y3,
                    float x4,float y4,u32 color,u32 Rop2Code)
{
    float a,b,c;
    float x12,y12,x23,y23,x34,y34;
    float x1223,y1223,x2334,y2334;
    float x,y,ab;

    //a��b��c��ֱ��(x1,y1)��(x4,y4)��һ��ʽax+by+c=0ʱ��Ӧ�ĸ�������
    a = y4-y1;
    b = x1-x4;
    c = y1*(x4-x1)-x1*(y4-y1);
    ab = a*a+b*b;

    //��(x2,y2)��(x3,y3)��ֱ��(x1,y1)��(x4,y4)�ľ���С��1(������)��
    //����ֱ�ߴ�������
    if(((a*x2+b*y2+c)*(a*x2+b*y2+c) < ab) &&((a*x3+b*y3+c)*(a*x3+b*y3+c) < ab))
    {
        __GK_LinetoBm(gkwin,limit,(s32)x1,(s32)y1,(s32)x4,(s32)y4,
                                color,Rop2Code);
        return;
    }
    x12 = x1+x2;
    y12 = y1+y2;
    x23 = x2+x3;
    y23 = y2+y3;
    x34 = x3+x4;
    y34 = y3+y4;

    x1223 = x12+x23;
    y1223 = y12+y23;
    x2334 = x23+x34;
    y2334 = y23+y34;

    x = x1223+x2334;
    y = y1223+y2334;

    //�������м��������Ƶ㵽�Զ˵�Ϊ���յ��ֱ�ߵľ���С��1(������)ʱ��
    //�����ߵ��е㴦�ָ�����
    __GK_BezierBm(gkwin,limit,x1,y1,x12/2,y12/2,
                    x1223/4,y1223/4,x/8,y/8,color,Rop2Code);
    __GK_BezierBm(gkwin,limit,x/8,y/8,x2334/4,y2334/4,
                    x34/2,y34/2,x4,y4,color,Rop2Code);
}

//----��screen�ϻ�����������---------------------------------------------------
//����: ��screen�ϻ����������ߡ�
//����: gkwin��Ŀ�괰��
//      limit�������ͼ����������һ�����ε����򣬳����������ֱ�߲���
//      x1��y1��x2��y2��x3��y3��x4��y4���˵�����
//      color����ͼ���õ���ɫ
//      rop2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void __GK_BezierScreen(struct tagDisplayRsc *display,struct tagRectangle *limit,
                    float x1,float y1,float x2,float y2,float x3,float y3,
                    float x4,float y4,u32 color,u32 Rop2Code)
{
    float a,b,c;
    float x12,y12,x23,y23,x34,y34;
    float x1223,y1223,x2334,y2334;
    float x,y,ab;

    //a��b��c��ֱ��(x1,y1)��(x4,y4)��һ��ʽax+by+c=0ʱ��Ӧ�ĸ�������
    a = y4-y1;
    b = x1-x4;
    c = y1*(x4-x1)-x1*(y4-y1);
    ab = a*a+b*b;

    //��(x2,y2)��(x3,y3)��ֱ��(x1,y1)��(x4,y4)�ľ���С��1(������)��
    //����ֱ�ߴ�������
    if(((a*x2+b*y2+c)*(a*x2+b*y2+c) < ab) &&((a*x3+b*y3+c)*(a*x3+b*y3+c) < ab))
    {
        __GK_LinetoScreen(display,limit,(s32)x1,(s32)y1,(s32)x4,(s32)y4,
                           color,Rop2Code);
        return;
    }
    x12 = x1+x2;
    y12 = y1+y2;
    x23 = x2+x3;
    y23 = y2+y3;
    x34 = x3+x4;
    y34 = y3+y4;

    x1223 = x12+x23;
    y1223 = y12+y23;
    x2334 = x23+x34;
    y2334 = y23+y34;

    x = x1223+x2334;
    y = y1223+y2334;

    //�������м��������Ƶ㵽�Զ˵�Ϊ���յ��ֱ�ߵľ���С��1(������)ʱ��
    //�����ߵ��е㴦�ָ�����
    __GK_BezierScreen(display,limit,x1,y1,x12/2,y12/2,
                    x1223/4,y1223/4,x/8,y/8,color,Rop2Code);
    __GK_BezierScreen(display,limit,x/8,y/8,x2334/4,y2334/4,
                    x34/2,y34/2,x4,y4,color,Rop2Code);
}
//----������������-------------------------------------------------------------
//����: �ڴ����ڻ�һ�����������ߡ�
//����: �����ǽṹpara�ĳ�Ա
//      gkwin��Ŀ�괰��ָ��
//      x1��y1��x2��y2��x3��y3��x4��y4������Bezier���ߵ��ĸ����Ƶ�
//      color������ʹ�õ���ɫ
//      r2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void GK_Bezier(struct tagGkscParaBezier *para)
{
    s32 offsetx,offsety;
    struct tagRectangle limit;
    struct tagClipRect *clip;
    struct tagGkWinRsc *fb_gkwin,*bzrwin;
//    struct tagDispDraw *my_draw_fun;
    if((para->Rop2Code == CN_R2_NOP) || (para->Rop2Code > CN_R2_LAST))
        return;                                         //��ִ�в���
    bzrwin = para->gkwin;
//    my_draw_fun = &bzrwin->disp->draw;
    //˵����win buffer����ֱ��д������Ϊfalse
    if((bzrwin->wm_bitmap != NULL)
        && (bzrwin->direct_screen == false))
    {   //������:��win buffer�л�ͼ����־changed_msk
        limit.left = 0;
        limit.top = 0;
        limit.right = bzrwin->wm_bitmap->width;
        limit.bottom = bzrwin->wm_bitmap->height;
        //��Ӳ�����ٲ�֧�ֱ��������ߵĻ��ƿ��ǣ������ʵ��
        __GK_BezierBm(bzrwin,&limit,para->x1,para->y1,para->x2,para->y2,
                            para->x3,para->y3,para->x4,para->y4,
                            para->color,para->Rop2Code);
    }else
    {
        clip = bzrwin->visible_clip;
        if(clip == NULL)
            return ;
        fb_gkwin = bzrwin->disp->frame_buffer;
        offsetx = bzrwin->absx0;
        offsety = bzrwin->absy0;
        //��frame buffer����ֱ��д������Ϊfalse
        if((fb_gkwin != NULL) && (bzrwin->direct_screen == false))
        {   //������:��frame buffer�л�ͼ����ֻ��gkwin�еĿ�������
            do{
                //��Ӳ�����ٲ�֧�ֱ��������ߵĻ��ƿ��ǣ������ʵ��
                __GK_BezierBm(fb_gkwin,&clip->rect,para->x1+offsetx,
                            para->y1+offsety,para->x2+offsetx,para->y2+offsety,
                            para->x3+offsetx,para->y3+offsety,para->x4+offsetx,
                            para->y4+offsety,para->color,para->Rop2Code);
                clip = clip->next;
            }while(clip != bzrwin->visible_clip);
        }else
        //��win buffer��Ҳ��frame buffer��ֱ�ӻ���screen��
        //ֱ��д������Ϊtrue���������޻�������ֱ�ӻ���screen��
        {
            do{
                __GK_BezierScreen(bzrwin->disp,&clip->rect,para->x1+offsetx,
                            para->y1+offsety,para->x2+offsetx,para->y2+offsety,
                            para->x3+offsetx,para->y3+offsety,para->x4+offsetx,
                            para->y4+offsety,para->color,para->Rop2Code);
                clip = clip->next;
            }while(clip != bzrwin->visible_clip);
        }
    }
}
//----��ֱ��-------------------------------------------------------------------
//����: �ڴ����ڻ�һ��ֱ�ߡ�
//����: �����ǽṹpara�ĳ�Ա
//      gkwin��Ŀ�괰��ָ��
//      x1��y1�������������
//      x2��y2�������յ�����(������)
//      color������ʹ�õ���ɫ
//      r2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void GK_Lineto(struct tagGkscParaLineto *para)
{
    struct tagRectangle limit;
    struct tagClipRect *clip;
    struct tagGkWinRsc *fb_gkwin,*lintowin;
    struct tagDispDraw *my_draw_fun;
    s32 offsetx,offsety;
    if((para->Rop2Code == CN_R2_NOP) || (para->Rop2Code > CN_R2_LAST))
        return;                                         //��ִ�в���
    if((para->x1==para->x2) && (para->y1==para->y2))    //�������
        return;
    lintowin = para->gkwin;
    my_draw_fun = &lintowin->disp->draw;
    //˵����win buffer����ֱ��д������Ϊfalse
    if((lintowin->wm_bitmap != NULL)
        && (lintowin->direct_screen == false))
    {   //������:��win buffer�л�ͼ����־changed_msk
        limit.left = 0;
        limit.top = 0;
        limit.right = lintowin->wm_bitmap->width;
        limit.bottom = lintowin->wm_bitmap->height;
        if(!my_draw_fun->line_bm(lintowin->wm_bitmap,&limit,para->x1,
                    para->y1,para->x2,para->y2,para->color,para->Rop2Code))
        {    //Ӳ�����ٲ�֧��ֱ�߻��ƣ��������ʵ�֣���������㷨��
            //ͬʱ������changed_msk
            __GK_LinetoBm(lintowin,&limit,para->x1,para->y1,para->x2,
                                    para->y2,para->color,para->Rop2Code);
        }
        else    //Ӳ�����ٻ�ͼʱ��δ����changed_msk������֮
        {
            __GK_ShadingLine(lintowin,&limit,
                                para->x1,para->y1,para->x2,para->y2);
        }
    }
    else
    {
        clip = lintowin->visible_clip;
        if(clip == NULL)
            return ;
        fb_gkwin = lintowin->disp->frame_buffer;
        offsetx = lintowin->absx0;
        offsety = lintowin->absy0;
        //��frame buffer,��ֱ��д������Ϊfalse
        if((fb_gkwin != NULL) && (lintowin->direct_screen == false))
        {   //������:��frame buffer�л�ͼ����ֻ��gkwin�еĿ�������
            do{
                if(!my_draw_fun->line_bm(
                            fb_gkwin->wm_bitmap,&clip->rect,
                            para->x1+offsetx,para->y1+offsety,para->x2+offsetx,
                            para->y2+offsety,para->color,para->Rop2Code))
                {    //Ӳ�����ٲ�֧��ֱ�߻��ƣ��������ʵ�֣���������㷨��
                    //ͬʱ������changed_msk
                    __GK_LinetoBm(fb_gkwin,&clip->rect,
                                    para->x1+offsetx,para->y1+offsety,
                                    para->x2+offsetx,para->y2+offsety,
                                    para->color,para->Rop2Code);
                }
                else    //Ӳ�����ٻ�ͼʱ��δ����changed_msk������֮
                {
                    __GK_ShadingLine(fb_gkwin,&clip->rect,para->x1+offsetx,
                                      para->y1+offsety,para->x2+offsetx,
                                      para->y2+offsety);
                }
                clip = clip->next;
            }while(clip != lintowin->visible_clip);
        }
        else
        //��win buffer��Ҳ��frame buffer��ֱ�ӻ���screen��
        //ֱ��д������Ϊtrue���������޻���������ֱ�ӻ���screen��
        {
            do{//��screen��ֱ�ӻ�ֱ�ߣ������յ�
//              if(!my_draw_fun->line_screen(&clip->rect,para->x1+offsetx,
//                           para->y1+offsety,para->x2+offsetx,para->y2+offsety,
//                           para->color,para->Rop2Code))
                {
                    __GK_LinetoScreen(lintowin->disp,&clip->rect,
                            para->x1+offsetx,para->y1+offsety,para->x2+offsetx,
                            para->y2+offsety,para->color,para->Rop2Code);
                }
                clip = clip->next;
            }while(clip != lintowin->visible_clip);
        }
    }
}
//----��ֱ��-------------------------------------------------------------------
//����: �ڴ����ڻ�һ��ֱ�ߡ�
//����: gkwin��Ŀ�괰��ָ��
//      gkwin��Ŀ�괰��ָ��
//      x1��y1�������������
//      x2��y2�������յ�����(����)
//      color������ʹ�õ���ɫ
//      r2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
void GK_LinetoIe(struct tagGkscParaLineto *para)
{
    s32 dx,dy;
    struct tagRectangle limit;
    struct tagClipRect *clip;
    struct tagGkWinRsc *fb_gkwin,*lintoiewin;
    struct tagDispDraw *my_draw_fun;
    struct tagDisplayRsc *display;
    s32 offsetx,offsety;
    if((para->Rop2Code == CN_R2_NOP) || (para->Rop2Code > CN_R2_LAST))
        return;                                         //��ִ�в���
    if((para->x1==para->x2) && (para->y1==para->y2))    //�������
        return;
    lintoiewin = para->gkwin;
    display = lintoiewin->disp;
    my_draw_fun = &display->draw;
    if(para->y1 == para->y2)    //����ˮƽ��
    {
        //˵����win buffer����ֱ��д������Ϊfalse
        if((lintoiewin->wm_bitmap != NULL)
            && (lintoiewin->direct_screen == false))
        {   //������:��win buffer�л�ͼ����־changed_msk
            limit.left = 0;
            limit.top = 0;
            limit.right = lintoiewin->wm_bitmap->width;
            limit.bottom = lintoiewin->wm_bitmap->height;
            if(!my_draw_fun->line_bm_ie(lintoiewin->wm_bitmap,&limit,para->x1,
                        para->y1,para->x2,para->y2,para->color,para->Rop2Code))
            {    //Ӳ�����ٲ�֧��ֱ�߻��ƣ��������ʵ�֣���������㷨��
                //ͬʱ������changed_msk
                __GK_HlinetoBm(lintoiewin,&limit,para->x1,para->y1,
                                para->x2+1,para->color,para->Rop2Code);
            }
            else
            {   //Ӳ������֧��ֱ�߻��ƣ�δ����changed_msk������֮
                __GK_ShadingLine(lintoiewin,&limit,
                                para->x1,para->y1,para->x2+1,para->y2);
            }

        }
        else
        {
            clip = lintoiewin->visible_clip;
            if(clip == NULL)
                return ;
            fb_gkwin = display->frame_buffer;
            offsetx = lintoiewin->absx0;
            offsety = lintoiewin->absy0;
            //��frame buffer����ֱ��д������Ϊfalse
            if((fb_gkwin != NULL) && (lintoiewin->direct_screen == false))
            {   //������:��frame buffer�л�ͼ����ֻ��gkwin�еĿ�������
                do
                {
                    if(!my_draw_fun->line_bm_ie(
                                fb_gkwin->wm_bitmap,&clip->rect,
                                para->x1+offsetx,para->y1+offsety,
                                para->x2+offsetx,para->y2+offsety,
                                para->color,para->Rop2Code))
                    {    //Ӳ�����ٲ�֧��ֱ�߻��ƣ��������ʵ�֣���������㷨��
                        //ͬʱ������changed_msk
                        __GK_HlinetoBm(fb_gkwin,&clip->rect,
                                    para->x1+offsetx,para->y1+offsety,
                                    para->x2+offsetx+1,
                                    para->color,para->Rop2Code);
                    }
                    else
                    {   //Ӳ�����ٻ�ͼʱ��δ����changed_msk������֮
                        __GK_ShadingLine(fb_gkwin,&clip->rect,
                                        para->x1+offsetx,para->y1+offsety,
                                        para->x2+offsetx+1,para->y2+offsety);
                    }
                    clip = clip->next;
                }while(clip != lintoiewin->visible_clip);
            }
            else
            //��win buffer��Ҳ��frame buffer��ֱ�ӻ���screen��
            //ֱ��д������Ϊtrue���������޻���������ֱ�ӻ���screen��
            {
                do{//��screen��ֱ�ӻ�ֱ�ߣ����յ�
//                  if(!my_draw_fun->line_screen_ie(&clip->rect,
//                                para->x1+offsetx,para->y1+offsety,
//                                para->x2+offsetx,para->y2+offsety,
//                               para->color,para->Rop2Code))
                    {
                        __GK_LinetoScreen(display,&clip->rect,
                                        para->x1+offsetx,para->y1+offsety,
                                        para->x2+offsetx+1,para->y2+offsety,
                                        para->color,para->Rop2Code);
                    }
                    clip = clip->next;
                }while(clip != lintoiewin->visible_clip);
            }
        }
    }
    else if(para->x1 == para->x2)    //���ƴ�ֱ��
    {
        //˵����win buffer����ֱ��д������Ϊfalse
        if((lintoiewin->wm_bitmap != NULL)
            && (lintoiewin->direct_screen == false))
        {   //������:��win buffer�л�ͼ����־changed_msk
            limit.left = 0;
            limit.top = 0;
            limit.right = lintoiewin->wm_bitmap->width;
            limit.bottom = lintoiewin->wm_bitmap->height;
            if(!my_draw_fun->line_bm_ie(lintoiewin->wm_bitmap,&limit,para->x1,
                        para->y1,para->x2,para->y2,para->color,para->Rop2Code))
            {    //Ӳ�����ٲ�֧��ֱ�߻��ƣ��������ʵ�֣���������㷨��
                //ͬʱ������changed_msk
                __GK_VlinetoBm(lintoiewin,&limit,para->x1,
                            para->y1,para->y2+1,para->color,para->Rop2Code);
            }
            else
            {   //Ӳ������֧��ֱ�߻��ƣ�δ����changed_msk������֮
                __GK_ShadingLine(lintoiewin,&limit,
                                para->x1,para->y1,para->x2,para->y2+1);
            }
        }
        else
        {
            clip = lintoiewin->visible_clip;
            if(clip == NULL)
                return ;
            fb_gkwin = display->frame_buffer;
            offsetx = lintoiewin->absx0;
            offsety = lintoiewin->absy0;
            //��frame buffer����ֱ��д������Ϊfalse
            if((fb_gkwin != NULL) && (lintoiewin->direct_screen == false))
            {   //������:��frame buffer�л�ͼ����ֻ��gkwin�еĿ�������
                do
                {
                    if(!my_draw_fun->line_bm_ie(
                                fb_gkwin->wm_bitmap,&clip->rect,
                                para->x1+offsetx,para->y1+offsety,
                                para->x2+offsetx,para->y2+offsety,
                                para->color,para->Rop2Code))
                    {    //Ӳ�����ٲ�֧��ֱ�߻��ƣ��������ʵ�֣���������㷨��
                        //ͬʱ������changed_msk
                        __GK_VlinetoBm(fb_gkwin,&clip->rect,
                                    para->x1+offsetx,para->y1+offsety,
                                    para->y2+offsety+1,
                                    para->color,para->Rop2Code);
                    }
                    else
                    {   //Ӳ�����ٻ�ͼʱ��δ����changed_msk������֮
                        __GK_ShadingLine(fb_gkwin,&clip->rect,
                                        para->x1+offsetx,para->y1+offsety,
                                        para->x2+offsetx,para->y2+offsety+1);
                    }
                    clip = clip->next;
                }while(clip != lintoiewin->visible_clip);
            }
            else
            //��win buffer��Ҳ��frame buffer��ֱ�ӻ���screen��
            //ֱ��д������Ϊtrue���������޻���������ֱ�ӻ���screen��
            {
                do{//��screen��ֱ�ӻ�ֱ�ߣ����յ�
//                  if(!my_draw_fun->line_screen_ie(&clip->rect,
//                                para->x1+offsetx,para->y1+offsety,
//                                para->x2+offsetx,para->y2+offsety,
//                               para->color,para->Rop2Code))
                    {
                        __GK_LinetoScreen(display,&clip->rect,
                                        para->x1+offsetx,para->y1+offsety,
                                        para->x2+offsetx,para->y2+offsety+1,
                                        para->color,para->Rop2Code);
                    }
                    clip = clip->next;
                }while(clip != lintoiewin->visible_clip);
            }
        }
    }
    else    //����б��
    {
        dx = abs(para->x2-para->x1);
        dy = abs(para->y2-para->y1);
        //˵����win buffer����ֱ��д������Ϊfalse
        if((lintoiewin->wm_bitmap != NULL)
            && (lintoiewin->direct_screen == false))
        {   //������:��win buffer�л�ͼ����־changed_msk
            limit.left = 0;
            limit.top = 0;
            limit.right = lintoiewin->wm_bitmap->width;
            limit.bottom = lintoiewin->wm_bitmap->height;
            if(!my_draw_fun->line_bm_ie(lintoiewin->wm_bitmap,&limit,para->x1,
                        para->y1,para->x2,para->y2,para->color,para->Rop2Code))
            {    //Ӳ�����ٲ�֧��ֱ�߻��ƣ��������ʵ�֣���������㷨��
                //ͬʱ������changed_msk
                if(dy > dx)
                {
                    __GK_OlinetoBm(lintoiewin,&limit,para->x1,para->y1,
                                    para->x2,para->y2+1,
                                    para->color,para->Rop2Code);
                }
                else
                {
                    __GK_OlinetoBm(lintoiewin,&limit,para->x1,para->y1,
                                    para->x2+1,para->y2,
                                    para->color,para->Rop2Code);
                }
            }
            else
            {   //Ӳ������֧��ֱ�߻��ƣ�δ����changed_msk������֮
                __GK_ShadingLine(lintoiewin,&limit,
                                para->x1,para->y1,para->x2,para->y2);
            }

        }
        else
        {
            clip = lintoiewin->visible_clip;
            if(clip == NULL)
                return ;
            fb_gkwin = display->frame_buffer;
            offsetx = lintoiewin->absx0;
            offsety = lintoiewin->absy0;
            //��frame buffer����ֱ��д������Ϊfalse
            if((fb_gkwin != NULL) && (lintoiewin->direct_screen == false))
            {   //������:��frame buffer�л�ͼ����ֻ��gkwin�еĿ�������
                do
                {
                    if(!my_draw_fun->line_bm_ie(
                                fb_gkwin->wm_bitmap,&clip->rect,
                                para->x1+offsetx,para->y1+offsety,
                                para->x2+offsetx,para->y2+offsety,
                                para->color,para->Rop2Code))
                    {    //Ӳ�����ٲ�֧��ֱ�߻��ƣ��������ʵ�֣���������㷨��
                        //ͬʱ������changed_msk
                            __GK_OlinetoBm(fb_gkwin,&clip->rect,
                                            para->x1+offsetx,para->y1+offsety,
                                            para->x2+offsetx,para->y2+offsety,
                                            para->color,para->Rop2Code);
                    }
                    else
                    {   //Ӳ�����ٻ�ͼʱ��δ����changed_msk������֮
                            __GK_ShadingLine(fb_gkwin,&clip->rect,
                                            para->x1+offsetx,para->y1+offsety,
                                            para->x2+offsetx,para->y2+offsety);
                    }
                    clip = clip->next;
                }while(clip != lintoiewin->visible_clip);
            }
            else
            //��win buffer��Ҳ��frame buffer��ֱ�ӻ���screen��
            //ֱ��д������Ϊtrue���������޻���������ֱ�ӻ���screen��
            {
                do{//��screen��ֱ�ӻ�ֱ�ߣ����յ�
//                  if(!my_draw_fun->line_screen_ie(&clip->rect,para->x1+offsetx,
//                               para->y1+offsety,para->x2+offsetx,para->y2+offsety,
//                               para->color,para->Rop2Code))
                    {
                            __GK_LinetoScreen(display,&clip->rect,
                                            para->x1+offsetx,para->y1+offsety,
                                            para->x2+offsetx,para->y2+offsety,
                                            para->color,para->Rop2Code);
                    }
                    clip = clip->next;
                }while(clip != lintoiewin->visible_clip);
            }
        }
    }
}

//----��ʾ�ı�-----------------------------------------------------------------
//����: ��Ĭ�������Ĭ���ַ�����ʾһ���ı�����
//����: gkwin����ʾ��Ŀ�괰��
//      x��y����ʾλ�ã������gkwin
//      text������ʾ���ַ���
//      count���ַ������ȣ�����������
//����: ��
//-----------------------------------------------------------------------------
void GK_DrawText(struct tagGkscParaDrawText *para,const char *text)
{
    struct tagGkscParaDrawBitmapRop bitmap_para;
    struct tagFontRsc* cur_font;
    struct tagCharset* cur_enc;
    s32 len, src_len,size,size_bak;
    u32 wc;
    u8 buf[128],*dbuf;  //����һ���㹻��32�����ֵĻ������������Ҫ��ʾ���ַ�
                        //�����˵��󣬾���Ҫ��̬����

    if(para->Rop2Code == CN_R2_NOP)
        return;

    cur_font = para->pFont;
    if(cur_font == NULL)
        cur_font = Font_GetCurFont();
    if(cur_font == NULL)
        return;

    cur_enc = para->pCharset;
    if(cur_enc == NULL)
        cur_enc = Charset_NlsGetCurCharset();
    if(cur_enc == NULL)
        return;

    // �����ֽ���
    src_len = para->count;

//    bitmap_para.command =CN_GKSC_DRAW_BITMAP;
    bitmap_para.gkwin = para->gkwin;
    bitmap_para.KeyColor = 0;
    bitmap_para.x = para->x;
    bitmap_para.y = para->y;
    bitmap_para.RopCode = CN_ROP_KEYCOLOR_EN;
    if(para->Rop2Code != CN_R2_COPYPEN)
    {
        bitmap_para.RopCode |= CN_ROP_ROP2_EN;
        bitmap_para.RopCode |= (para->Rop2Code << CN_ROP_ROP2_OFF)
                                    & CN_ROP_ROP2_MSK;
    }
    bitmap_para.bitmap.ExColor = para->color;
    dbuf = NULL;
    size_bak = 0;
    for(; src_len > 0;)
    {
        len= cur_enc->mb_to_ucs4(&wc, text, src_len);
        if(len == -1)
        { // ��Ч�ַ�
            src_len--;
            text++;
        }
        else if(len == 0)
        {
            break;
        }
        else
        { // ��Ч�ַ�
            text += len;
            src_len -= len;
            if(wc==L'\r')
            {
                bitmap_para.x =  para->x;
                continue;
            }

            if(wc==L'\n')
            {
                bitmap_para.y += cur_font->MaxHeight;
                continue;
            }

            // ȡ����
            bitmap_para.bitmap.bm_bits = NULL;      //NULL=��ѯbitmap.
            cur_font->GetBitmap(wc,0,0,&(bitmap_para.bitmap));
            size = bitmap_para.bitmap.linebytes * bitmap_para.bitmap.height;
            if(size < 128)
                bitmap_para.bitmap.bm_bits = buf;
            else
            {
                if(size > size_bak)
                {
                    free(dbuf);
                    dbuf = M_MallocLc(size,0);
                    if(dbuf == NULL)
                        return;
                    size_bak = size;
                }
                bitmap_para.bitmap.bm_bits = dbuf;
            }
            cur_font->GetBitmap(wc,0,0,&(bitmap_para.bitmap));
            GK_DrawBitMap(&bitmap_para);

            bitmap_para.x += bitmap_para.bitmap.width;
        }
    }
    free(dbuf);
}
//----���ƹ�դλͼ-------------------------------------------------------------
//����: �ڴ����ڻ���դλͼ��λͼ�������ڵĲ��ֽ�������ơ�������һһ��Ӧcopy��
//      ���������ѹ��
//����: �����ǽṹpara�ĳ�Ա
//      gkwin��Ŀ�괰��ָ��
//      bitmap�������Ƶ�λͼ
//      pattern��Ҫ���õ�ͼ��λͼ�����ظ�ʽ�� bitmap һ��
//      bitmsk��rop4��������Ҫ������λͼ��һ����ɫλͼ
//      transparentcolor��Ҫ���õ�͸��ɫ
//      x��y�����Ƶ�Ŀ��λ��
//      rop_code����դ������
//����: ��
//-----------------------------------------------------------------------------
void GK_DrawBitMap(struct tagGkscParaDrawBitmapRop *para)
{
//    u32 KeyColor;
    s32 x_src,y_src,x_dst,y_dst;
    struct tagPointCdn InsOffset;
    struct tagRectangle DstRect,SrcRect,InsRect;
    struct tagClipRect *clip;
    struct tagGkWinRsc *fb_gkwin,*DstGkwin;
    struct tagRectBitmap *DstBitmap;
    struct tagRectBitmap *SrcBitmap;
    struct tagDisplayRsc *display;
    DstGkwin = para->gkwin;
    display = DstGkwin->disp;

//    KeyColor = para->KeyColor;
    DstBitmap = DstGkwin->wm_bitmap;
    SrcBitmap = &para->bitmap;
    //�����Ƶ�λͼҪ���Ƶ�λ�������Ŀ�괰�ڵ�����
    SrcRect.left = 0;
    SrcRect.top = 0;
    SrcRect.right = SrcBitmap->width;
    SrcRect.bottom = SrcBitmap->height;
    DstRect.left = para->x;
    DstRect.top = para->y;
    DstRect.right = para->x+SrcRect.right;
    DstRect.bottom = para->y+SrcRect.bottom;
    //����������λͼҪ���Ƶ�λ�ã������ڴ�����
    if((DstRect.right <= 0)
            || (DstRect.left >= (DstGkwin->limit_right - DstGkwin->limit_left))
            ||(DstRect.bottom <= 0)
            || (DstRect.top >= (DstGkwin->limit_bottom - DstGkwin->limit_top)))
        return;                             //��ͼ�����ڴ�������
    if(DstRect.left < 0)                    //������߽��ڴ����ڲ�
    {
        SrcRect.left = -DstRect.left;
        DstRect.left = 0;
    }
    if(DstRect.top < 0)                     //�����ϱ߽��ڴ����ڲ�
    {
        SrcRect.top = -DstRect.top;
        DstRect.top = 0;
    }
    if(DstRect.right > (DstGkwin->limit_right - DstGkwin->limit_left))        //�����ұ߽��ڴ����ڲ�
    {
        SrcRect.right = DstRect.right - (DstGkwin->limit_right - DstGkwin->limit_left);
        DstRect.right = (DstGkwin->limit_right - DstGkwin->limit_left);
    }
    if(DstRect.bottom > (DstGkwin->limit_bottom - DstGkwin->limit_top))      //�����±߽��ڴ����ڲ�
    {
        SrcRect.bottom = DstRect.bottom - (DstGkwin->limit_bottom - DstGkwin->limit_top);
        DstRect.bottom = (DstGkwin->limit_bottom - DstGkwin->limit_top);
    }

    //˵����win buffer����ֱ��д������Ϊfalse
    if((DstBitmap != NULL) && (DstGkwin->direct_screen == false))
    {   //������:��win buffer�л�ͼ����־changed_msk
        if(!display->draw.blt_bm_to_bm(DstBitmap,&DstRect,
                                       SrcBitmap,&SrcRect,
                                       para->RopCode,para->KeyColor))
        {
            //������֧�� blt_bm_to_bm ���ƣ����ɴ���ʵ��
            y_dst= DstRect.top;
            for(y_src= 0;
                y_src < DstRect.bottom-DstRect.top;
                y_src++)
            {
                x_dst = DstRect.left;
                for(x_src = 0;
                    x_src < DstRect.right-DstRect.left;
                    x_src++)
                {
                    __GK_CopyPixelRopBm(DstBitmap,SrcBitmap,
                                            x_dst,y_dst,SrcRect.left+x_src,
                                            SrcRect.top+y_src,
                                            para->RopCode,para->KeyColor);
                    x_dst++;
                }
                y_dst++;
            }
        }
        __GK_ShadingRect(DstGkwin,&DstRect);    //���û��Ʋ��ֵ�changed_msk
    }
    else        //��win buffer����ֱ��д������Ϊtrue
    {
        clip = DstGkwin->visible_clip;
        if(clip == NULL)                            //���ڿ�����Ϊ�գ�ֱ�ӷ���
            return ;
        //��clip��Ӧ��SrcRectλ�û��Ƶ�framebuffer�ϣ�����clip�϶���framebuffer
        //���ڣ���ֻ��Ҫ�������꣬�����ж��Ƿ�Խframebuffer�߽硣
        fb_gkwin = display->frame_buffer;
        DstBitmap = fb_gkwin->wm_bitmap;
        //��frame buffer����ֱ��д������Ϊfalse���ѿ�������Ƶ�framebuffer��
        if((fb_gkwin != NULL) && (DstGkwin->direct_screen == false))
        {   //������:��frame_buffer�л�ͼ����ֻ��gkwin�еĿ�������
            //Ϊ����������걣��һ�£�����λͼҪ���Ƶ�λ�õ�
            //�����������screenԭ��Ϊԭ������꣬ע�����������꣬���ܳ���
            //framebuffer�ĳߴ���ޣ���û�й�ϵ�ģ�ȡ�����򽻼���ʱ��ᴦ�����
            DstRect.left += DstGkwin->absx0;
            DstRect.right += DstGkwin->absx0;
            DstRect.top += DstGkwin->absy0;
            DstRect.bottom += DstGkwin->absy0;
            //SrcRect���ڱ�������Ƶ�bitmap���framebuffer��ƫ�ƣ����ڼ���clip
            //��framebuffer�ж�Ӧ��λ��
            InsOffset.x = DstGkwin->absx0 + para->x;
            InsOffset.y = DstGkwin->absy0 + para->y;
            do{
                if(__GK_GetRectInts(&clip->rect,&DstRect,&InsRect))
                {
                    SrcRect.left = InsRect.left-InsOffset.x;
                    SrcRect.top = InsRect.top-InsOffset.y;
                    SrcRect.right = InsRect.left-InsOffset.x;
                    SrcRect.bottom = InsRect.top-InsOffset.y;
                    if(!fb_gkwin->disp->draw.blt_bm_to_bm(
                                        DstBitmap,&InsRect,
                                        SrcBitmap,&SrcRect,
                                        para->RopCode,para->KeyColor))
                    {
                        SrcBitmap = &para->bitmap;

                        y_dst= InsRect.top;
                        //�����Ĺ�դ������Ϊ��չ�Ĺ�դ������
                        if(para->RopCode == 0)
                        {
                            for(y_src= InsRect.top-DstRect.top;
                                y_src < InsRect.bottom-DstRect.top;
                                y_src++)
                            {
                                x_dst = InsRect.left;
                                for(x_src = InsRect.left-DstRect.left;
                                    x_src < InsRect.right-DstRect.left;
                                    x_src++)
                                {
                                    __GK_CopyPixelBm(DstBitmap,SrcBitmap,
                                                     x_dst,y_dst,x_src, y_src);
                                    x_dst++;
                                }
                                y_dst++;
                            }
                        }
                        else
                        {
                            for(y_src= InsRect.top-DstRect.top;
                                y_src < InsRect.bottom-DstRect.top;
                                y_src++)
                            {
                                x_dst = InsRect.left;
                                for(x_src = InsRect.left-DstRect.left;
                                    x_src < InsRect.right-DstRect.left;
                                    x_src++)
                                {
                                    __GK_CopyPixelRopBm(DstBitmap,SrcBitmap,
                                                        x_dst,y_dst,x_src,
                                                        y_src,para->RopCode,
                                                        para->KeyColor);
                                    x_dst++;
                                }
                                y_dst++;
                            }
                        }
                    }
                    //���û��Ʋ��ֵ�changed_msk
                    __GK_ShadingRect(fb_gkwin,&InsRect);
                }
                clip = clip->next;
            }while(clip != DstGkwin->visible_clip);
        }
        else
        //��rop������λͼ��֧��ֱ��д������win buffer��Ҳ��frame buffer��
        //����ֱ��д������Ϊtrue����������˻�ΪGK_DrawBitMap�Ĺ���
        {
            //Ϊ����������걣��һ�£�����λͼҪ���Ƶ�λ�õ�
            //�����������screenԭ��Ϊԭ�������
            DstRect.left += DstGkwin->absx0;
            DstRect.right += DstGkwin->absx0;
            DstRect.top += DstGkwin->absy0;
            DstRect.bottom += DstGkwin->absy0;
            do{
                //Ҫ���Ƶ�λͼĿ��λ���ڴ��ڿ������ڲ�ִ�л��Ƴ���
                if(__GK_GetRectInts(&clip->rect,&DstRect,&InsRect))
                {
                    //����bitmap��screen
                    __GK_BltBmToScreen(display,&InsRect,
                                        &para->bitmap,
                                        InsRect.left-DstRect.left,
                                        InsRect.top-DstRect.top);
                }
                clip = clip->next;
            }while(clip != DstGkwin->visible_clip);
        }
    }
}

//----����ɫ���λͼ---------------------------------------------------------
//����: �ý���ɫ�����ʾ���е�һ����������
//����: Draw,Ŀ����ʾ���Ļ��ƺ�����ָ�롣
//      Target: ������̸��ǵ�����
//      Focus�������ļ������������Target�ڲ�
//      Color0,��ɫ1�������CN_FILLRECT_MODE_H�峣������
//      Color1,��ɫ2�������CN_FILLRECT_MODE_H�峣������
//      Mode,����ģʽ
//����: ��
//ע: б�߽���ʹ�õ��ǽ���ģʽ���ϸ�ģʽӦ����������������붥��ľ��룬������
//    ������漰���������㡣���۶���ɫ�仯�ǲ����еģ����Ƽ���Ҳûɶ�����á�
//-----------------------------------------------------------------------------
void __GK_GradientFillRect(     struct tagRectBitmap *bitmap,
                                struct tagRectangle *Target,
                                struct tagRectangle *Focus,
                                u32 Color0,u32 Color1,u32 Mode)
{
    u32 a0,r0,g0,b0,a1,r1,g1,b1,Color;
    u32 ax,rx,gx,bx,ay,ry,gy,by;
    s32 L0,L1,x,y,CrossX,CrossY;
    a0 = Color0>>24;
    r0 = (Color0>>16)&0xff;
    g0 = (Color0>>8)&0xff;
    b0 = Color0&0xff;
    a1 = Color1>>24;
    r1 = (Color1>>16)&0xff;
    g1 = (Color1>>8)&0xff;
    b1 = Color1&0xff;
    CrossY = Target->bottom - Target->top;
    CrossX = Target->right - Target->left;
    switch(Mode)
    {
        case CN_FILLRECT_MODE_H:
        {
            for(x = Focus->left; x<Focus->right; x++)
            {
                L0 = x-Target->left;
                L1 = Target->right - x;
                ax = ( ( (L0*a1 + L1*a0)<<8) / CrossX>>8) & 0xff;
                rx = ( ( (L0*r1 + L1*r0)<<8) / CrossX>>8) & 0xff;
                gx = ( ( (L0*g1 + L1*g0)<<8) / CrossX>>8) & 0xff;
                bx = ( ( (L0*b1 + L1*b0)<<8) / CrossX>>8) & 0xff;
                Color = ( ax << 24) +( rx << 16) +( gx << 8) +( bx << 0);
                Color = GK_ConvertRGB24ToPF(bitmap->PixelFormat,Color);
                for(y = Focus->top; y<Focus->bottom; y++)
                {
                    __GK_SetPixelRop2Bm(bitmap,x,y,Color,CN_R2_COPYPEN);
                }
            }
        } break;
        case CN_FILLRECT_MODE_V:
        {
            for(y = Focus->top; y<Focus->bottom; y++)
            {
                L0 = y-Target->top;
                L1 = Target->bottom- y;
                ay = ( ( (L0*a1 + L1*a0)<<8) / CrossY>>8) & 0xff;
                ry = ( ( (L0*r1 + L1*r0)<<8) / CrossY>>8) & 0xff;
                gy = ( ( (L0*g1 + L1*g0)<<8) / CrossY>>8) & 0xff;
                by = ( ( (L0*b1 + L1*b0)<<8) / CrossY>>8) & 0xff;
                Color = ( ay << 24) +( ry << 16) +( gy << 8) +( by << 0);
                Color = GK_ConvertRGB24ToPF(bitmap->PixelFormat,Color);
                for(x = Focus->left; x<Focus->right; x++)
                {
                    __GK_SetPixelRop2Bm(bitmap,x,y,Color,CN_R2_COPYPEN);
                }
            }

        } break;
        case CN_FILLRECT_MODE_SP:
        {
            for(x = Focus->left; x<Focus->right; x++)
            {
                L0 = x-Target->left;
                L1 = Target->right - x;
                ax = ( ( (L0*a1 + L1*a0)<<8) / CrossX>>8) & 0xff;
                rx = ( ( (L0*r1 + L1*r0)<<8) / CrossX>>8) & 0xff;
                gx = ( ( (L0*g1 + L1*g0)<<8) / CrossX>>8) & 0xff;
                bx = ( ( (L0*b1 + L1*b0)<<8) / CrossX>>8) & 0xff;
                for(y = Focus->top; y<Focus->bottom; y++)
                {
                    L0 = y-Target->top;
                    L1 = Target->bottom- y;
                    ay = ( ( (L0*a1 + L1*a0)<<8) / CrossY>>8) & 0xff;
                    ry = ( ( (L0*r1 + L1*r0)<<8) / CrossY>>8) & 0xff;
                    gy = ( ( (L0*g1 + L1*g0)<<8) / CrossY>>8) & 0xff;
                    by = ( ( (L0*b1 + L1*b0)<<8) / CrossY>>8) & 0xff;
                    Color = ( ((ax+ay)>>1) << 24)
                            +( ((rx+ry)>>1) << 16)
                            +( ((gx+gy)>>1) << 8)
                            +( ((bx+by)>>1) << 0);
                    Color = GK_ConvertRGB24ToPF(bitmap->PixelFormat,Color);
                    __GK_SetPixelRop2Bm(bitmap,x,y,Color,CN_R2_COPYPEN);
                }
            }
        } break;
        case CN_FILLRECT_MODE_SN:
        {
            for(x = Focus->left; x<Focus->right; x++)
            {
                L1 = x-Target->left;
                L0 = Target->right - x;
                ax = ( ( (L0*a1 + L1*a0)<<8) / CrossX>>8) & 0xff;
                rx = ( ( (L0*r1 + L1*r0)<<8) / CrossX>>8) & 0xff;
                gx = ( ( (L0*g1 + L1*g0)<<8) / CrossX>>8) & 0xff;
                bx = ( ( (L0*b1 + L1*b0)<<8) / CrossX>>8) & 0xff;
                for(y = Focus->top; y<Focus->bottom; y++)
                {
                    L0 = y-Target->top;
                    L1 = Target->bottom- y;
                    ay = ( ( (L0*a1 + L1*a0)<<8) / CrossY>>8) & 0xff;
                    ry = ( ( (L0*r1 + L1*r0)<<8) / CrossY>>8) & 0xff;
                    gy = ( ( (L0*g1 + L1*g0)<<8) / CrossY>>8) & 0xff;
                    by = ( ( (L0*b1 + L1*b0)<<8) / CrossY>>8) & 0xff;
                    Color = ( ((ax+ay)>>1) << 24)
                            +( ((rx+ry)>>1) << 16)
                            +( ((gx+gy)>>1) << 8)
                            +( ((bx+by)>>1) << 0);
                    Color = GK_ConvertRGB24ToPF(bitmap->PixelFormat,Color);
                    __GK_SetPixelRop2Bm(bitmap,x,y,Color,CN_R2_COPYPEN);
                }
            }

        } break;
    }
}


//----����ɫ�����ʾ��---------------------------------------------------------
//����: �ý���ɫ�����ʾ���е�һ����������
//����: Draw,Ŀ����ʾ���Ļ��ƺ�����ָ�롣
//      Target: ������̸��ǵ�����
//      Focus�������ļ������������Target�ڲ�
//      Color0,��ɫ1�������CN_FILLRECT_MODE_H�峣������
//      Color1,��ɫ2�������CN_FILLRECT_MODE_H�峣������
//      Mode,����ģʽ
void __GK_GradientFillScreenRect(struct tagDispDraw *Draw,
                                 struct tagRectangle *Target,
                                 struct tagRectangle *Focus,
                                 u32 Color0,u32 Color1,u32 Mode)
{
    u32 a0,r0,g0,b0,a1,r1,g1,b1,Color;
    u32 ax,rx,gx,bx,ay,ry,gy,by;
    s32 L0,L1,x,y,CrossX,CrossY;
    a0 = Color0>>24;
    r0 = (Color0>>16)&0xff;
    g0 = (Color0>>8)&0xff;
    b0 = Color0&0xff;
    a1 = Color1>>24;
    r1 = (Color1>>16)&0xff;
    g1 = (Color1>>8)&0xff;
    b1 = Color1&0xff;
    CrossY = Target->bottom - Target->top;
    CrossX = Target->right - Target->left;
    switch(Mode)
    {
        case CN_FILLRECT_MODE_N:
        {
            for(x = Focus->left; x<Focus->right; x++)
            {
                for(y = Focus->top; y<Focus->bottom; y++)
                {
                    Draw->set_pixel_screen(x,y,Color0,CN_R2_COPYPEN);
                }
            }
        } break;
        case CN_FILLRECT_MODE_H:
        {
            for(x = Focus->left; x<Focus->right; x++)
            {
                L0 = x-Target->left;
                L1 = Target->right - x;
                ax = ( ( (L0*a1 + L1*a0)<<8) / CrossX>>8) & 0xff;
                rx = ( ( (L0*r1 + L1*r0)<<8) / CrossX>>8) & 0xff;
                gx = ( ( (L0*g1 + L1*g0)<<8) / CrossX>>8) & 0xff;
                bx = ( ( (L0*b1 + L1*b0)<<8) / CrossX>>8) & 0xff;
                Color = ( ax << 24) +( rx << 16) +( gx << 8) +( bx << 0);
                for(y = Focus->top; y<Focus->bottom; y++)
                {
                    Draw->set_pixel_screen(x,y,Color,CN_R2_COPYPEN);
                }
            }
        } break;
        case CN_FILLRECT_MODE_V:
        {
            for(y = Focus->top; y<Focus->bottom; y++)
            {
                L0 = y-Target->top;
                L1 = Target->bottom- y;
                ay = ( ( (L0*a1 + L1*a0)<<8) / CrossY>>8) & 0xff;
                ry = ( ( (L0*r1 + L1*r0)<<8) / CrossY>>8) & 0xff;
                gy = ( ( (L0*g1 + L1*g0)<<8) / CrossY>>8) & 0xff;
                by = ( ( (L0*b1 + L1*b0)<<8) / CrossY>>8) & 0xff;
                Color = ( ay << 24) +( ry << 16) +( gy << 8) +( by << 0);
                for(x = Focus->left; x<Focus->right; x++)
                {
                    Draw->set_pixel_screen(x,y,Color,CN_R2_COPYPEN);
                }
            }

        } break;
        case CN_FILLRECT_MODE_SP:
        {
            for(x = Focus->left; x<Focus->right; x++)
            {
                L0 = x-Target->left;
                L1 = Target->right - x;
                ax = ( ( (L0*a1 + L1*a0)<<8) / CrossX>>8) & 0xff;
                rx = ( ( (L0*r1 + L1*r0)<<8) / CrossX>>8) & 0xff;
                gx = ( ( (L0*g1 + L1*g0)<<8) / CrossX>>8) & 0xff;
                bx = ( ( (L0*b1 + L1*b0)<<8) / CrossX>>8) & 0xff;
                for(y = Focus->top; y<Focus->bottom; y++)
                {
                    L0 = y-Target->top;
                    L1 = Target->bottom- y;
                    ay = ( ( (L0*a1 + L1*a0)<<8) / CrossY>>8) & 0xff;
                    ry = ( ( (L0*r1 + L1*r0)<<8) / CrossY>>8) & 0xff;
                    gy = ( ( (L0*g1 + L1*g0)<<8) / CrossY>>8) & 0xff;
                    by = ( ( (L0*b1 + L1*b0)<<8) / CrossY>>8) & 0xff;
                    Color = ( ((ax+ay)>>1) << 24)
                            +( ((rx+ry)>>1) << 16)
                            +( ((gx+gy)>>1) << 8)
                            +( ((bx+by)>>1) << 0);
                    Draw->set_pixel_screen(x,y,Color,CN_R2_COPYPEN);
                }
            }
        } break;
        case CN_FILLRECT_MODE_SN:
        {
            for(x = Focus->left; x<Focus->right; x++)
            {
                L1 = x-Target->left;
                L0 = Target->right - x;
                ax = ( ( (L0*a1 + L1*a0)<<8) / CrossX>>8) & 0xff;
                rx = ( ( (L0*r1 + L1*r0)<<8) / CrossX>>8) & 0xff;
                gx = ( ( (L0*g1 + L1*g0)<<8) / CrossX>>8) & 0xff;
                bx = ( ( (L0*b1 + L1*b0)<<8) / CrossX>>8) & 0xff;
                for(y = Focus->top; y<Focus->bottom; y++)
                {
                    L0 = y-Target->top;
                    L1 = Target->bottom- y;
                    ay = ( ( (L0*a1 + L1*a0)<<8) / CrossY>>8) & 0xff;
                    ry = ( ( (L0*r1 + L1*r0)<<8) / CrossY>>8) & 0xff;
                    gy = ( ( (L0*g1 + L1*g0)<<8) / CrossY>>8) & 0xff;
                    by = ( ( (L0*b1 + L1*b0)<<8) / CrossY>>8) & 0xff;
                    Color = ( ((ax+ay)>>1) << 24)
                            +( ((rx+ry)>>1) << 16)
                            +( ((gx+gy)>>1) << 8)
                            +( ((bx+by)>>1) << 0);
                    Draw->set_pixel_screen(x,y,Color,CN_R2_COPYPEN);
                }
            }

        } break;
    }
}


//----��䴰�ڵ�һ����---------------------------------------------------------
//����: ��ָ������ɫ��䴰�ڵ�һ���֡�
//����: �����ǽṹpara�ĳ�Ա
//      gkwin��Ŀ�괰��
//      rect�������ľ���
//      color�����ʹ�õ���ɫ
//����: ��
//��ע: ���������ܻ��޸�Rect�ĳ�Ա�����Բ������ⲿ���á�
//-----------------------------------------------------------------------------
void __GK_FillPartWin(struct tagGkWinRsc *Gkwin,struct tagRectangle *Rect,u32 Color)
{
    struct tagRectBitmap *bitmap;
    struct tagRectangle ins_rect;
    struct tagDispDraw *my_draw_fun;
    struct tagClipRect *clip;
    struct tagGkWinRsc *fb_gkwin;

    bitmap = Gkwin->wm_bitmap;
    //��������Ҫ���Ƶ�λ�õ����꣬�����Ŀ�괰��

    my_draw_fun = &Gkwin->disp->draw;
    //˵����win buffer����ֱ��д������Ϊfalse
    if((Gkwin->wm_bitmap != NULL)
        && (Gkwin->direct_screen == false))
    {
        //������:��win buffer�л�ͼ����ɫchanged_msk
        if(!my_draw_fun->fill_rect_bm(bitmap,Rect,Rect,Color,
                                        0,CN_FILLRECT_MODE_N))
        {
            //Ӳ�����ٲ�֧�����λͼ���������ʵ��
            __GK_FillRect(bitmap,Rect,Color);
        }
        __GK_ShadingRect(Gkwin,Rect);//��ɫ��������changed_msk
    }else
    {
        clip = Gkwin->visible_clip;
        if(clip == NULL)                //���ڿ�����Ϊ�գ�ֱ�ӷ���
            return ;
        fb_gkwin = Gkwin->disp->frame_buffer;
        //�������Ļ��ƣ�����֡�������screen�ϻ��ƣ�ʹ�þ������꣬�任֮
        Rect->left += Gkwin->absx0;
        Rect->right += Gkwin->absx0;
        Rect->top += Gkwin->absy0;
        Rect->bottom += Gkwin->absy0;
        //��frame buffer,��ֱ��д������Ϊfalse
        if((fb_gkwin != NULL) && (Gkwin->direct_screen == false))
        {   //������:��frame buffer�л�ͼ����ֻ��gkwin�еĿ�������
            do{
                //ֻ��䴰�ڿ�������Ҫ���Ƶ�����Ľ���
                if(__GK_GetRectInts(&clip->rect,Rect,&ins_rect))
                {
                    //֡��������꼴�������꣬����������任
                    if(!my_draw_fun->fill_rect_bm(fb_gkwin->wm_bitmap,
                                                  Rect,&ins_rect,Color,
                                                  0,CN_FILLRECT_MODE_N))
                    {
                        //Ӳ�����ٲ�֧�����λͼ���������ʵ��
                        __GK_FillRect(fb_gkwin->wm_bitmap,
                                            &ins_rect,Color);
                    }
                    //��־��������changed_msk
                    __GK_ShadingRect(fb_gkwin,&ins_rect);
                }
                clip = clip->next;
            }while(clip != Gkwin->visible_clip);
        }
        else
        //��win buffer��Ҳ��frame buffer��ֱ�ӻ���screen��
        //ֱ��д������Ϊtrue���������޻���������ֱ�ӻ���screen��
        {
            do{
                struct tagDisplayRsc *mirror,*current;
                //ֻ��䴰�ڿ�������Ҫ���Ƶ�����Ľ���
                if(__GK_GetRectInts(&clip->rect,Rect,&ins_rect))
                {
                    //Ӳ�����ٲ�֧�����λͼ���������ʵ��
                    if(!my_draw_fun->fill_rect_screen(Rect,&ins_rect,
                                             Color,0,CN_FILLRECT_MODE_N))
                    {
                        __GK_GradientFillScreenRect(my_draw_fun,Rect,
                                    &ins_rect,Color,0,CN_FILLRECT_MODE_N);
                    }
                    mirror = (struct tagDisplayRsc *)&Gkwin->disp->node;
                    if(mirror == NULL)      //û�о�����ʾ����ֱ�ӷ���
                        return;
                    current = mirror;
                    while(current != NULL)
                    {
                        //Ӳ�����ٲ�֧�����λͼ���������ʵ��
                        if(!current->draw.fill_rect_screen(Rect,&ins_rect,
                                              Color,0,CN_FILLRECT_MODE_N))
                        {
                            __GK_GradientFillScreenRect(&current->draw,Rect,
                                    &ins_rect,Color,0,CN_FILLRECT_MODE_N);
                        }
                        current = (struct tagDisplayRsc *)Rsc_TraveScion(
                                                        &mirror->node,
                                                        &current->node);
                    }
                }
                clip = clip->next;
            }while(clip != Gkwin->visible_clip);
        }
    }
}

//----����ɫ��䴰�ڵ�һ����---------------------------------------------------
//����: ��ָ������ɫ��䴰�ڵ�һ���֡�
//����: �����ǽṹpara�ĳ�Ա
//      gkwin��Ŀ�괰��
//      rect�������ľ���
//      color�����ʹ�õ���ɫ
//����: ��
//-----------------------------------------------------------------------------
void GK_GradientFillRect(struct tagGkscParaGradientFillWin *para)
{
    struct tagRectBitmap *bitmap;
    struct tagRectangle target,ins_rect;
    struct tagDispDraw *my_draw_fun;
    struct tagClipRect *clip;
    struct tagGkWinRsc *fb_gkwin,*fpwwin;
    u32 Color0,Color1,Mode;

    fpwwin = para->gkwin;
    bitmap = fpwwin->wm_bitmap;
    //��������Ҫ���Ƶ�λ�õ����꣬�����Ŀ�괰��
    target = para->rect;
    if((target.right <= 0) || (target.left >= bitmap->width)
        ||(target.bottom <= 0) || (target.top >= bitmap->height))
        return;                             //��ͼ�����ڴ�������

    if(target.left < 0) target.left = 0;    //������߽��ڴ����ڲ�
    if(target.top < 0) target.top = 0;      //�����ϱ߽��ڴ����ڲ�
    if(target.right > bitmap->width)        //�����ұ߽��ڴ����ڲ�
        target.right = bitmap->width;
    if(target.bottom > bitmap->height)      //�����±߽��ڴ����ڲ�
        target.bottom = bitmap->height;

    Mode = para->Mode;
    if(Mode== CN_FILLRECT_MODE_N)
        __GK_FillPartWin(fpwwin,&target,para->Color0);

    Color0 = para->Color0;
    Color1 = para->Color1;
    my_draw_fun = &fpwwin->disp->draw;
    //˵����win buffer����ֱ��д������Ϊfalse
    if((fpwwin->wm_bitmap != NULL)
        && (fpwwin->direct_screen == false))
    {
        //������:��win buffer�л�ͼ����ɫchanged_msk
        if(!my_draw_fun->fill_rect_bm(bitmap,&target,&target,Color0,Color1,Mode))
        {
            //Ӳ�����ٲ�֧�����λͼ���������ʵ��
            __GK_GradientFillRect(bitmap,&target,&target,Color0,Color1,Mode);
        }
        __GK_ShadingRect(fpwwin,&para->rect);//��ɫ��������changed_msk
    }else
    {
        clip = fpwwin->visible_clip;
        if(clip == NULL)                //���ڿ�����Ϊ�գ�ֱ�ӷ���
            return ;
        fb_gkwin = fpwwin->disp->frame_buffer;
        //�������Ļ��ƣ�����֡�������screen�ϻ��ƣ�ʹ�þ������꣬�任֮
        target.left += fpwwin->absx0;
        target.right += fpwwin->absx0;
        target.top += fpwwin->absy0;
        target.bottom += fpwwin->absy0;
        //��frame buffer,��ֱ��д������Ϊfalse
        if((fb_gkwin != NULL) && (fpwwin->direct_screen == false))
        {   //������:��frame buffer�л�ͼ����ֻ��gkwin�еĿ�������
            do{
                //ֻ��䴰�ڿ�������Ҫ���Ƶ�����Ľ���
                if(__GK_GetRectInts(&clip->rect,&target,&ins_rect))
                {
                    //֡��������꼴�������꣬����������任
                    if(!my_draw_fun->fill_rect_bm(fb_gkwin->wm_bitmap,
                                         &target,&ins_rect,Color0,Color1,Mode))
                    {
                        //Ӳ�����ٲ�֧�����λͼ���������ʵ��
                        __GK_GradientFillRect(fb_gkwin->wm_bitmap,&target,
                                                &ins_rect,Color0,Color1,Mode);
                    }
                    //��־��������changed_msk
                    __GK_ShadingRect(fb_gkwin,&ins_rect);
                }
                clip = clip->next;
            }while(clip != fpwwin->visible_clip);
        }
        else
        //��win buffer��Ҳ��frame buffer��ֱ�ӻ���screen��
        //ֱ��д������Ϊtrue���������޻���������ֱ�ӻ���screen��
        {
            do{
                struct tagDisplayRsc *mirror,*current;
                //ֻ��䴰�ڿ�������Ҫ���Ƶ�����Ľ���
                if(__GK_GetRectInts(&clip->rect,&target,&ins_rect))
                {
                    //Ӳ�����ٲ�֧�����λͼ���������ʵ��
                    if(!my_draw_fun->fill_rect_screen(&target,&ins_rect,
                                                      Color0,Color1,Mode))
                    {
                        __GK_GradientFillScreenRect(my_draw_fun,&target,
                                                &ins_rect,Color0,Color1,Mode);
                    }
                    mirror = (struct tagDisplayRsc *)&fpwwin->disp->node;
                    if(mirror == NULL)      //û�о�����ʾ����ֱ�ӷ���
                        return;
                    current = mirror;
                    while(current != NULL)
                    {
                        if(!current->draw.fill_rect_screen(&target,&ins_rect,
                                                       Color0,Color1,Mode) );
                        {
                            __GK_GradientFillScreenRect(&current->draw,&target,
                                                &ins_rect,Color0,Color1,Mode);
                        }
                        current =(struct tagDisplayRsc *)Rsc_TraveScion(
                                                &mirror->node,&current->node);
                    }
                }
                clip = clip->next;
            }while(clip != fpwwin->visible_clip);
        }
    }
}

//----��䴰��-----------------------------------------------------------------
//����: ��ָ������ɫ��䴰�ڡ�
//����: �����ǽṹpara�ĳ�Ա
//      gkwin��Ŀ�괰��ָ��
//      color�����ʹ�õ���ɫ
//����: ��
//-----------------------------------------------------------------------------
void GK_FillWin(struct tagGkscParaFillWin *para)
{
    struct tagRectangle rect;
    struct tagClipRect *clip;
    struct tagGkWinRsc *fb_gkwin,*fpwin;
    struct tagRectBitmap *bitmap;

    struct tagDispDraw *my_draw_fun;

    fpwin = para->gkwin;
    bitmap = fpwin->wm_bitmap;
    my_draw_fun = &fpwin->disp->draw;
    rect.left = 0;
    rect.top = 0;
    rect.right = bitmap->width;
    rect.bottom = bitmap->height;
    //˵����win buffer����ֱ��д������Ϊfalse
    if((fpwin->wm_bitmap != NULL)
        && (fpwin->direct_screen == false))
    {   //������:��win buffer�л�ͼ����־changed_msk
        bitmap = fpwin->wm_bitmap;
        if(!my_draw_fun->fill_rect_bm(bitmap,&rect,&rect,para->color,
                                        0,CN_FILLRECT_MODE_N))
        {//Ӳ�����ٲ�֧�����λͼ���������ʵ��
            __GK_FillBm(bitmap,para->color);
        }
        fpwin->change_flag = CN_GKWIN_CHANGE_ALL;
    }else
    {
        clip = fpwin->visible_clip;
        if(clip == NULL)
            return ;
        fb_gkwin = fpwin->disp->frame_buffer;
        //��frame buffer����ֱ��д������Ϊfalse
        if((fb_gkwin != NULL) && (fpwin->direct_screen == false))
        {   //������:��frame buffer�л�ͼ����ֻ��gkwin�еĿ�������
            bitmap = fb_gkwin->wm_bitmap;
            do{
                //֡����������clip�е����궼�Ǿ������꣬����任
                if(!my_draw_fun->fill_rect_bm(bitmap,&rect,&clip->rect,
                                              para->color,0,CN_FILLRECT_MODE_N))
                {//Ӳ�����ٲ�֧�����λͼ���������ʵ��
                    __GK_FillRect(bitmap,&clip->rect,para->color);
                }
                //��־��������changed_msk
                __GK_ShadingRect(fb_gkwin,&clip->rect);
                clip = clip->next;
            }while(clip != fpwin->visible_clip);
        }else
        //��win buffer��Ҳ��frame buffer��ֱ�ӻ���screen��
        //ֱ��д������Ϊtrue���������޻���������ֱ�ӻ���screen��
        {
            do{
                struct tagDisplayRsc *mirror,*current;
                if(!my_draw_fun->fill_rect_screen(&rect,&clip->rect,
                                            para->color,0,CN_FILLRECT_MODE_N))
                {
                    //Ӳ�����ٲ�֧�����screen�ϵľ������������ʵ��
                    __GK_GradientFillScreenRect(my_draw_fun,&rect,&clip->rect,
                                            para->color,0,CN_FILLRECT_MODE_N);
                }
                mirror = (struct tagDisplayRsc *)&fpwin->disp->node;
                if(mirror == NULL)      //û�о�����ʾ����ֱ�ӷ���
                    continue;
                current = mirror;
                while(current != NULL)
                {
                    if(!current->draw.fill_rect_screen(&rect,&clip->rect,
                                            para->color,0,CN_FILLRECT_MODE_N))
                    {
                        //Ӳ�����ٲ�֧�����screen�ϵľ������������ʵ��
                        __GK_GradientFillScreenRect(&current->draw,&rect,
                                                    &clip->rect,
                                                    para->color,
                                                    0,CN_FILLRECT_MODE_N);
                    }
                    current = (struct tagDisplayRsc *)Rsc_TraveScion(
                                                        &mirror->node,
                                                        &current->node);
                }
                clip = clip->next;
            }while(clip != fpwin->visible_clip);
        }
    }
}


