//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

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
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
//����ģ��:ͼ��
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: lil9325��������
//����˵��:
//�޶���ʷ:
//1. ����: 2009-03-10
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"
#include "string.h"
#include "cpu_peri.h"

#include "gkernel.h"
#include "gk_display.h"
#include "stm32f10x.h"
#include "ili9325.h"
#include "djyos.h"

#define cn_coordinates      1       //0=�������꣬1=��ת���꣬��ԭ��=(239,319)
u8 u8g_frame_buffer[cn_frame_buffer_size];
struct tagDisplayRsc tg_lcd_display;

//������ili9325�Ĳ�������

/*---------------------------------------------------------------------------
���ܣ�  ��lcd�ڲ�0x00�Ĵ���
---------------------------------------------------------------------------*/
u32 __ili9325_read_reg(void)
{
    LCD_DATA=0x0000;
    return LCD_CMD;
}
/*---------------------------------------------------------------------------
���ܣ�  дlcd�ڲ��Ĵ���
---------------------------------------------------------------------------*/
void __ili9325_write_reg(u32 reg_index, u32 data)
{
    LCD_DATA=reg_index;
    LCD_CMD=data;
}

/*---------------------------------------------------------------------------
���ܣ�  д����
---------------------------------------------------------------------------*/
void __ili9325_write_cmd(u32 cmd)
{
    LCD_DATA = cmd;
}

/*---------------------------------------------------------------------------
���ܣ�  д����
---------------------------------------------------------------------------*/
#define __ili9325_write_data(data) (LCD_CMD=data)

/*---------------------------------------------------------------------------
���ܣ�  ������
---------------------------------------------------------------------------*/
u32 __ili9325_read_data(void)
{
    return LCD_CMD;
}
/*---------------------------------------------------------------------------
���ܣ�  ����һ������
---------------------------------------------------------------------------*/
void __ili9325_set_pixel(u32 x, u32 y, u16 color)
{
    __ili9325_write_reg(0x0021, y);        //v��ֱ����
    __ili9325_write_reg(0x0020, x);    //hˮƽ����

    __ili9325_write_cmd(0x0022);
    __ili9325_write_data(color);
}

/*---------------------------------------------------------------------------
���ܣ�  ��ȡһ������
---------------------------------------------------------------------------*/
u16 __ili9325_get_pixel(u32 x, u32 y)
{
    u16 col = 0;//, rgb = 0

    __ili9325_write_reg(0x0021, y);        //v��ֱ����
    __ili9325_write_reg(0x0020, x);    //hˮƽ����

    __ili9325_write_cmd(0x0022);
    __ili9325_read_data();//dummy read

    col = __ili9325_read_data();//BGR ����
//  rgb = (col & 0x001f)<<11;
//  rgb |= (col & 0xf800)>>11;
//  rgb |= col & 0x07e0;
    return col;
}

/*---------------------------------------------------------------------------
���ܣ�  ���ù��λ��
        row     :0--319
        column  :0--239
---------------------------------------------------------------------------*/
void __ili9325_set_cursor(u32 x, u32 y)
{
    __ili9325_write_reg(0x0021, y);        //v��ֱ����
    __ili9325_write_reg(0x0020, x);    //hˮƽ����
}

/*---------------------------------------------------------------------------
���ܣ�  ����
---------------------------------------------------------------------------*/
void __ili9325_set_window(u32 x,u32 y, u32 wide,u32 high)
{
    __ili9325_write_reg(0x0050, x);//ˮƽ��ʼ��ַ

    if (x+wide <= 240)

    __ili9325_write_reg(0x0051, x+wide-1);//ˮƽ������ַ
    else
    __ili9325_write_reg(0x0051, 239);

    __ili9325_write_reg(0x0052, y);//��ֱ��ʼ��ַ
    if (y+high <= 320)
    __ili9325_write_reg(0x0053, y+high-1);//��ֱ������ַ
    else
    __ili9325_write_reg(0x0053, 319);

    __ili9325_set_cursor(x, y);//����������Ͻ�

}

/*---------------------------------------------------------------------------
���ܣ�  �ش�
---------------------------------------------------------------------------*/
void __ili9325_close_window(void)
{
    __ili9325_write_reg(0x0050, 0);//ˮƽ��ʼ��ַ
    __ili9325_write_reg(0x0051, 239);//ˮƽ������ַ

    __ili9325_write_reg(0x0052, 0);//��ֱ��ʼ��ַ
    __ili9325_write_reg(0x0053, 319);//��ֱ������ַ

    __ili9325_set_cursor(0, 0);//����������Ͻ�
}
void lcd_clear(u32 color)
{
    u32 i = 0;

    __ili9325_close_window();

    __ili9325_write_cmd(0x0022);                                    //дGRAM֮ǰ Ҫ��д0x0022
    for (i = 0; i < 320*240 ; i++)
    {
        __ili9325_write_data(color);
    }

}

void lcd_reset(void)
{
    GPIO_SettoLow(CN_GPIO_G,0x100);
    Djy_DelayUs(5000);
    GPIO_SettoHigh(CN_GPIO_G,0x100);
}

void lcd_backlight_on(void)
{
    GPIO_SettoLow(CN_GPIO_F,0x400);
}

void lcd_backlight_off(void)
{
    GPIO_SettoHigh(CN_GPIO_F,0x400);
}

//������djyguiҪ��Ľӿں������������£�������ÿպ�������
/*---------------------------------------------------------------------------
���ܣ�    lcd ��ʼ��
---------------------------------------------------------------------------*/
void __lcd_ili9325_init(void)
{
    GPIO_PowerOn(CN_GPIO_F);
    GPIO_PowerOn(CN_GPIO_G);
    GPIO_CfgPinFunc(CN_GPIO_F,10,CN_GPIO_MODE_GPIO_OUT_OD_2Mhz);
    GPIO_CfgPinFunc(CN_GPIO_G,8,CN_GPIO_MODE_GPIO_OUT_PP_2Mhz);
    lcd_backlight_off();
    lcd_reset();

    Djy_DelayUs(100000);

    if( __ili9325_read_reg() != 0x9325)
    {
        return;
    }

    __ili9325_write_reg(0x0000, 0x0001);            //Start internal OSC
    __ili9325_write_reg(0x0001, 0x0000);            // set SS=0 and SM=0  S1---S720
    __ili9325_write_reg(0x0002, 0x0700);            // set 1 line inversion
    __ili9325_write_reg(0x0003, 0x1030);            // set GRAM write direction and BGR=1.
    __ili9325_write_reg(0x0004, 0x0000);            // Resize register
    __ili9325_write_reg(0x0008, 0x0207);            // set the back porch and front porch
    __ili9325_write_reg(0x0009, 0x0000);            // set non-display area refresh cycle ISC[3:0]
    __ili9325_write_reg(0x000a, 0x0000);            // FMARK function
    __ili9325_write_reg(0x000c, 0x0000);            // RGB interface setting
    __ili9325_write_reg(0x000d, 0x0000);            // Frame marker Position
    __ili9325_write_reg(0x000f, 0x0000);            // RGB interface polarity
    //-------------power on sequence------------
    __ili9325_write_reg(0x0010, 0x0000);            // SAP, BT[3:0], AP, DSTB, SLP, STB
    __ili9325_write_reg(0x0011, 0x0007);            // DC1[2:0], DC0[2:0], VC[2:0]
    __ili9325_write_reg(0x0012, 0x0000);                                                            // VREG1OUT voltage
    __ili9325_write_reg(0x0013, 0x0000);                                                            // VDV[4:0] for VCOM amplitude

    Djy_DelayUs(5000);

    __ili9325_write_reg(0x0010, 0x1690);                                                            // SAP, BT[3:0], AP, DSTB, SLP, STB
    __ili9325_write_reg(0x0011, 0x0227);                                                            // R11H=0x0221 at VCI=3.3V, DC1[2:0], DC0[2:0], VC[2:0]

    Djy_DelayUs(5000);

    __ili9325_write_reg(0x0012, 0x001d);                                                            // External reference voltage= Vci;001d

    Djy_DelayUs(5000);

    __ili9325_write_reg(0x0013, 0x0800);                                                            // R13H=1D00 when R12H=009D;VDV[4:0] for VCOM amplitude
    __ili9325_write_reg(0x0029, 0x0014);                                                            // R29H=0013 when R12H=009D;VCM[5:0] for VCOMH
    __ili9325_write_reg(0x002b, 0x000B);                                                            // Frame Rate = 96Hz

    Djy_DelayUs(5000);

    __ili9325_write_reg(0x0020, 0x0000);                                                            // GRAM horizontal Address
    __ili9325_write_reg(0x0021, 0x0000);                                                            // GRAM Vertical Address
    // ----------- Adjust the Gamma Curve ----------
    __ili9325_write_reg(0x0030, 0x0007);
    __ili9325_write_reg(0x0031, 0x0707);
    __ili9325_write_reg(0x0032, 0x0006);
    __ili9325_write_reg(0x0035, 0x0704);
    __ili9325_write_reg(0x0036, 0x1F04);
    __ili9325_write_reg(0x0037, 0x0004);
    __ili9325_write_reg(0x0038, 0x0000);
    __ili9325_write_reg(0x0039, 0x0706);
    __ili9325_write_reg(0x003c, 0x0701);
    __ili9325_write_reg(0x003d, 0x000F);
    // ------------------ Set GRAM area ---------------
    __ili9325_write_reg(0x0050, 0x0000);            // Horizontal GRAM Start Address
    __ili9325_write_reg(0x0051,    239);                // Horizontal GRAM End Address
    __ili9325_write_reg(0x0052, 0x0000);            // Vertical GRAM Start Address
    __ili9325_write_reg(0x0053,    319);                // Vertical GRAM Start Address
    __ili9325_write_reg(0x0060, 0x2700);            // GS=0 320 line
    __ili9325_write_reg(0x0061, 0x0001);            // NDL,VLE����������, REV���Ҷȷ�ת��
    __ili9325_write_reg(0x006a, 0x0000);            // set scrolling line
    // -------------- Partial Display Control ---------
    __ili9325_write_reg(0x0080, 0x0000);
    __ili9325_write_reg(0x0081, 0x0000);
    __ili9325_write_reg(0x0082, 0x0000);
    __ili9325_write_reg(0x0083, 0x0000);
    __ili9325_write_reg(0x0084, 0x0000);
    __ili9325_write_reg(0x0085, 0x0000);
    // -------------- Panel Control -------------------
    __ili9325_write_reg(0x0090, 0x0010);
    __ili9325_write_reg(0x0092, 0x0000);
    __ili9325_write_reg(0x0093, 0x0003);
    __ili9325_write_reg(0x0095, 0x0110);
    __ili9325_write_reg(0x0097, 0x0000);
    __ili9325_write_reg(0x0098, 0x0000);

    // Set GRAM write direction and BGR = 1
    // I/D=11 (Horizontal : increment, Vertical : increment)
    // AM=0 (address is updated in Horizontal writing direction)
    __ili9325_write_reg(0x0003, 0x1030);

    __ili9325_write_reg(0x0007, 0x0173);                        // 262K color and display ON

    lcd_backlight_on();
}

/*---------------------------------------------------------------------------
���ܣ�    display on
---------------------------------------------------------------------------*/
void lcd_display_on(void)
{
    __ili9325_write_reg(0x0007, 0x0173);
}

/*---------------------------------------------------------------------------
���ܣ�    display off
---------------------------------------------------------------------------*/
void lcd_display_off(void)
{
    __ili9325_write_reg(0x0007, 0x0000);
}


bool_t __lcd_set_pixel_bm(struct tagRectBitmap *bitmap,
                         s32 x,s32 y,u32 color,u32 r2_code)
{
    return false;
}
bool_t __lcd_line_bm(struct tagRectBitmap *bitmap,struct tagRectangle *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    return false;
}
bool_t __lcd_line_bm_ie(struct tagRectBitmap *bitmap,struct tagRectangle *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    return false;
}
bool_t __lcd_fill_rect_bm(struct tagRectBitmap *dst_bitmap,
                          struct tagRectangle *Target,
                          struct tagRectangle *Focus,
                          u32 Color0,u32 Color1,u32 Mode)
{
    u32 y;
    u16 pixel;
    u16 *dst_offset;
    if(Mode != CN_FILLRECT_MODE_N)
        return false;
    if(dst_bitmap->PixelFormat != CN_SYS_PF_RGB565)
        return false;
    pixel = GK_ConvertRGB24ToPF(CN_SYS_PF_RGB565,Color0);

    dst_offset = (u16*)((ptu32_t)dst_bitmap->bm_bits
                              + Focus->top * dst_bitmap->linebytes);
    dst_offset += Focus->left;

    for(y = Focus->top; y < Focus->bottom; y++)
    {
        memset(dst_offset,pixel,(Focus->right-Focus->left)<<1);
        dst_offset += dst_bitmap->linebytes >> 1;
    }
    return true;
}

//��������������ͨ��������£�����ͼ�λ��ơ�
//�������λͼ���������Դ��Ŀ��λͼ��ʽ����cn_sys_pf_r5g6b5��
bool_t __lcd_blt_bm_to_bm( struct tagRectBitmap *dst_bitmap,
                            struct tagRectangle *DstRect,
                            struct tagRectBitmap *src_bitmap,
                            struct tagRectangle *SrcRect,
                            u32 RopCode, u32 KeyColor)
{
    u16 *src_offset,*dst_offset;    //Դλͼ���󻺳������ܲ�����!!!
    u32 y;
    if((dst_bitmap->PixelFormat != CN_SYS_PF_RGB565)
             ||(src_bitmap->PixelFormat != CN_SYS_PF_RGB565)
             ||(RopCode != 0))
    {
        return false;
    }
    else
    {
        if((ptu32_t)src_bitmap->bm_bits & 1)    //Դλͼ�������Ƕ���ģ��
        {
        }else
        {
            dst_offset = (u16*)((ptu32_t)dst_bitmap->bm_bits
                                      + DstRect->top * dst_bitmap->linebytes);
            dst_offset += DstRect->left;

            src_offset = (u16*)((ptu32_t)src_bitmap->bm_bits
                                      +SrcRect->top * src_bitmap->linebytes);
            src_offset += SrcRect->left;

            for(y = DstRect->top; y < DstRect->bottom; y++)
            {
                memcpy(dst_offset,src_offset,(DstRect->right-DstRect->left)<<1);
                dst_offset += dst_bitmap->linebytes >> 1;
                src_offset += src_bitmap->linebytes >> 1;
            }
        }
    }
    return true;
}
bool_t __lcd_get_rect_bm(struct tagRectBitmap *src,struct tagRectangle *rect,
                        struct tagRectBitmap *dest)
{
    return false;
}
#if (cn_coordinates == 0)   //��������
bool_t __lcd_set_pixel_screen(s32 x,s32 y,u32 color,u32 r2_code)
{
    u16 dest,pixel;
    pixel = GK_ConvertRGB24ToPF(CN_SYS_PF_RGB565,color);
    if(CN_R2_COPYPEN == r2_code)
    {
        __ili9325_set_pixel(x,y,pixel);
    }else
    {
        dest = __ili9325_get_pixel(x,y);
        pixel = GK_BlendRop2(dest, pixel, r2_code);
        __ili9325_set_pixel(x,y,pixel);
    }
    return true;
}
//��screen�л�һ������ֱ�ߣ������˵㣬��Ӳ�����ٲ�֧����screen�ϻ��ߣ�������
//frame_buffer��driver���Լ򻯣�ֱ�ӷ���false����
//������ʾ����driver���ṩ�������
bool_t __lcd_line_screen(struct tagRectangle *limit,
                    s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    return false;
}

//��screen�л�һ������ֱ�ߣ����˵㣬��Ӳ�����ٲ�֧����screen�ϻ��ߣ�������
//frame_buffer��driver���Լ򻯣�ֱ�ӷ���false����
//������ʾ����driver���ṩ�������
bool_t __lcd_line_screen_ie(struct tagRectangle *limit,
                       s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    return false;
}
//screen�о�����䣬��Ӳ�����ٲ�֧����screen�Ͼ�����䣬������frame_buffer��
//driver���Լ򻯣�ֱ�ӷ���false����
bool_t __lcd_fill_rect_screen(struct tagRectangle *Target,
                              struct tagRectangle *Focus,
                              u32 Color0,u32 Color1,u32 Mode)
{
    u32 x,y,width,height;
    u16 pixel;
    if(Mode != CN_FILLRECT_MODE_N)
        return false;
    pixel = GK_ConvertRGB24ToPF(CN_SYS_PF_RGB565,Color0);
    width = Focus->right-Focus->left;
    height = Focus->bottom-Focus->top;
    __ili9325_set_window(Focus->left,Focus->top,width,height);
    __ili9325_write_cmd(0x0022);
    for(y = 0; y < height; y++)
    {
        for(x = 0; x < width; x++)
        {
            __ili9325_write_data(pixel);
        }
    }
    __ili9325_close_window();
    return true;
}

//���ڴ滺������screenλ�鴫�ͣ�ֻ֧�ֿ鿽������֧��rop������
//������ʾ����driver���ṩ�������
bool_t __lcd_bm_to_screen(struct tagRectangle *dst_rect,
            struct tagRectBitmap *src_bitmap,s32 xsrc,s32 ysrc)
{
    s32 x,y,width,height;
    u16 *lineoffset;
    if(src_bitmap->PixelFormat != CN_SYS_PF_RGB565)
        return false;
    width = dst_rect->right-dst_rect->left;
    height = dst_rect->bottom-dst_rect->top;
    //�������꣬
    lineoffset = (u16*)((u32)src_bitmap->bm_bits + ysrc*src_bitmap->linebytes);
    lineoffset +=xsrc;
    //�������꣬
    __ili9325_set_window(dst_rect->left,dst_rect->top,width,height);
    __ili9325_write_cmd(0x0022);
    for(y = 0; y < height; y++)
    {
        for(x = 0; x < width; x++)
        {
            __ili9325_write_data(lineoffset[x]);
        }
        lineoffset += (src_bitmap->linebytes)>>1;
    }
    __ili9325_close_window();
    return true;
}

//��screen��ȡһ���أ���ת����cn_sys_pf_e8r8g8b8
u32 __lcd_get_pixel_screen(s32 x,s32 y)
{
    return GK_ConvertColorToRGB24(CN_SYS_PF_RGB565,
                        __ili9325_get_pixel(x,y),0);
}
#else
bool_t __lcd_set_pixel_screen(s32 x,s32 y,u32 color,u32 r2_code)
{
    u16 dest,pixel;
    pixel = GK_ConvertRGB24ToPF(CN_SYS_PF_RGB565,color);
    if(CN_R2_COPYPEN == r2_code)
    {
        __ili9325_set_pixel(LCD_XSIZE-x,LCD_YSIZE-y,pixel);   //ת������
    }else
    {
        dest = __ili9325_get_pixel(LCD_XSIZE-1-x,LCD_YSIZE-1-y);  //ת������
        pixel = GK_BlendRop2(dest, pixel, r2_code);
        __ili9325_set_pixel(LCD_XSIZE-1-x,LCD_YSIZE-1-y,pixel);   //ת������
    }
    return true;
}
//��screen�л�һ������ֱ�ߣ������˵㣬��Ӳ�����ٲ�֧����screen�ϻ��ߣ�������
//frame_buffer��driver���Լ򻯣�ֱ�ӷ���false����
//������ʾ����driver���ṩ�������
bool_t __lcd_line_screen(struct tagRectangle *limit,
                    s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    return false;
}

//��screen�л�һ������ֱ�ߣ����˵㣬��Ӳ�����ٲ�֧����screen�ϻ��ߣ�������
//frame_buffer��driver���Լ򻯣�ֱ�ӷ���false����
//������ʾ����driver���ṩ�������
bool_t __lcd_line_screen_ie(struct tagRectangle *limit,
                       s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    return false;
}
//screen�о�����䣬��Ӳ�����ٲ�֧����screen�Ͼ�����䣬������frame_buffer��
//driver���Լ򻯣�ֱ�ӷ���false����
bool_t __lcd_fill_rect_screen(struct tagRectangle *Target,
                              struct tagRectangle *Focus,
                              u32 Color0,u32 Color1,u32 Mode)
{
    u32 x,y,width,height;
    u16 pixel;
    if(Mode != CN_FILLRECT_MODE_N)
        return false;
    pixel = GK_ConvertRGB24ToPF(CN_SYS_PF_RGB565,Color0);
    width = Focus->right-Focus->left;
    height = Focus->bottom-Focus->top;
    //ת������
    __ili9325_set_window(LCD_XSIZE-Focus->right,
                         LCD_YSIZE-Focus->bottom,width,height);
    __ili9325_write_cmd(0x0022);
    for(y = 0; y < height; y++)
    {
        for(x = 0; x < width; x++)
        {
            __ili9325_write_data(pixel);
        }
    }
    __ili9325_close_window();
    return true;
}

//���ڴ滺������screenλ�鴫�ͣ�ֻ֧�ֿ鿽������֧��rop������
//������ʾ����driver���ṩ�������
bool_t __lcd_bm_to_screen(struct tagRectangle *dst_rect,
            struct tagRectBitmap *src_bitmap,s32 xsrc,s32 ysrc)
{
    s32 x,y,width,height;
    u16 *lineoffset;
    if(src_bitmap->PixelFormat != CN_SYS_PF_RGB565)
        return false;
    width = dst_rect->right-dst_rect->left;
    height = dst_rect->bottom-dst_rect->top;
    //ת������
    lineoffset = (u16*)((u32)src_bitmap->bm_bits
                        + (ysrc+height-1)*src_bitmap->linebytes);
    lineoffset +=xsrc;
    //ת������
    __ili9325_set_window(LCD_XSIZE-dst_rect->right,
                         LCD_YSIZE-dst_rect->bottom,width,height);
    __ili9325_write_cmd(0x0022);
    for(y = 0; y < height; y++)
    {
        for(x = width-1; x >= 0; x--)
        {
            __ili9325_write_data(lineoffset[x]);
        }
        lineoffset -= (src_bitmap->linebytes)>>1;
    }
    __ili9325_close_window();
    return true;
}

//��screen��ȡһ���أ���ת����cn_sys_pf_e8r8g8b8
u32 __lcd_get_pixel_screen(s32 x,s32 y)
{
    return GK_ConvertColorToRGB24(CN_SYS_PF_RGB565,
                    __ili9325_get_pixel(LCD_XSIZE-1-x,LCD_YSIZE-1-y),0);
}

#endif
//��screen�ھ�����������ݸ��Ƶ�bitmap������ǰ�������ú�dest��pf_type
bool_t __lcd_get_rect_screen(struct tagRectangle *rect,struct tagRectBitmap *dest)
{
    return false;
}

//----������ʾ��---------------------------------------------------------------
//����: ������driver�ṩ�̱�д�ģ�����������ʾ�����ֲ����ĺ���������Ӧ�ó����
//      ���룬�������豸���̶��塣��ut2416�У�������Ϊ�ա�
//����: disp����ʾ��ָ��
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t __lcd_disp_ctrl(struct tagDisplayRsc *disp)
{
    return true;
}

//----��ʼ��lcd�豸------------------------------------------------------------
//����: ����
//����: ��
//����: ��ʾ����Դָ��
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_ili9325(const char *DisplayName,const char* HeapName)
{
    static struct tagGkWinRsc frame_win;
    static struct tagRectBitmap FrameBitmap;
    __lcd_ili9325_init( );
    FrameBitmap.bm_bits = u8g_frame_buffer;
    FrameBitmap.width = LCD_XSIZE;
    FrameBitmap.height = LCD_YSIZE;
    FrameBitmap.PixelFormat = cn_lcd_pf;
    FrameBitmap.linebytes = cn_lcd_line_size;
    FrameBitmap.ExColor = 0;
    frame_win.wm_bitmap = &FrameBitmap;
    tg_lcd_display.frame_buffer = &frame_win;

    tg_lcd_display.xmm = 0;
    tg_lcd_display.ymm = 0;
    tg_lcd_display.width = LCD_XSIZE;
    tg_lcd_display.height = LCD_YSIZE;
    tg_lcd_display.pixel_format = CN_SYS_PF_RGB565;
    tg_lcd_display.reset_clip = false;
    tg_lcd_display.framebuf_direct = false;
    //�����ʼ��frame_buffer��desktop��z_topmost������Ա

    tg_lcd_display.draw.set_pixel_bm = __lcd_set_pixel_bm;
    tg_lcd_display.draw.line_bm = __lcd_line_bm;
    tg_lcd_display.draw.line_bm_ie = __lcd_line_bm_ie;
    tg_lcd_display.draw.fill_rect_bm = __lcd_fill_rect_bm;
    tg_lcd_display.draw.blt_bm_to_bm = __lcd_blt_bm_to_bm;
    tg_lcd_display.draw.get_pixel_bm = NULL;
    tg_lcd_display.draw.get_rect_bm = __lcd_get_rect_bm;
    tg_lcd_display.draw.set_pixel_screen = __lcd_set_pixel_screen;
    tg_lcd_display.draw.line_screen = __lcd_line_screen;
    tg_lcd_display.draw.line_screen_ie = __lcd_line_screen_ie;
    tg_lcd_display.draw.fill_rect_screen = __lcd_fill_rect_screen;
    tg_lcd_display.draw.bm_to_screen = __lcd_bm_to_screen;
    tg_lcd_display.draw.get_pixel_screen = __lcd_get_pixel_screen;
    tg_lcd_display.draw.get_rect_screen = __lcd_get_rect_screen;

//    tg_lcd_display.bmmalloc = lcd_bmmalloc;

    tg_lcd_display.DisplayHeap = M_FindHeap(HeapName);
    tg_lcd_display.disp_ctrl = __lcd_disp_ctrl;

    GK_InstallDisplay(&tg_lcd_display,DisplayName);
    return (ptu32_t)&tg_lcd_display;
}

