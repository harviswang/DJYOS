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
#include "stdint.h"
#include "stdlib.h"
#include "stddef.h"
#include "string.h"
#include "gkernel.h"
#include "gk_display.h"
#include "cpu_peri.h"

//����ϵ˵��,�������Դ�ƫ�Ƶ�ַ���������ص������ӳ���ϵ,ע�ⵥɫ�ͻҶ���ʾ��
//ÿ�ֽڿ��ܰ�����ֹһ�����ص�,ͼ��û�м�������.
//��ͬ��ϵͳ�в�ͬ�Ķ�Ӧ��ϵ,���ֱ仯Ӱ�� ������ʾ�����ı�д����.
//Ӧ�ó����Լ�gui�������������������ӳ���ϵ.
//�������,���������������Ļֱ��ӳ��ķ�����ʾbmpͼ��,��Ҫ�˽�����ӳ���ϵ.
//----------------------------------------------------->x   ���Ͻ�
// |0x0000,0x0001,0x0002--------------------------xmax-1
// |xmax,xmax+1,xmax+2---------------------------2*max-1
// |----------------------------------------------------
// |----------------------------------------------------
// |----------------------------------------------------
// |(ymax-1)*xmax,(ymax-1)*xmax+1------------ymax*xmax-1    ���½�
//\/
#define M5D(n)          ((n) & 0x1fffff)    //��õ�21λ
struct tagDisplayRsc tg_lcd_display;
//�����Դ棬��������һ���Դ��һ��֡���壬16�Ƕ���Ŀ���
static u8 u8g_dsp_buffer[cn_frame_buffer_size + 16];
static u8 u8g_video_buffer[cn_frame_buffer_size + 16];
static u16 *pg_frame_buffer,*pg_video_buf;
//volatile unsigned short LCD_BUFFER[SCR_YSIZE_TFT][SCR_XSIZE_TFT];

u32 GK_GetPixelBm(struct tagRectBitmap *bitmap, s32 x,s32 y);

//----��ʼ��lcdӲ��------------------------------------------------------------
//����: ��ʼ��lcdӲ����
//����: ��
//����: ��
//��ע: ����������ֲ�ؼ�����
//----------------------------------------------------------------------------
void __lcd_hard_init(void)
{

    struct gpio_reg *gpio =(struct gpio_reg*)GPIO_REG_BASE;
    struct lcd_reg *lcd =(struct lcd_reg*)LCD_REG_BASE;

    //�˿� C ������ʹ�ܼĴ��� ,0,ʹ�ܸ����������ܵ���Ӧ�˿�����
    gpio->GPCUP = 0x00000000;
    //GPC4 ,VM;GPC3 VFRAME;GPC2  VLINE;GPC1 VLINE;GPC0 �����
    //Initialize VD[7:0]��16-31λ,VD[23:0] ,��LCD ������������˿ڣ�
    gpio->GPCCON = 0xaaaa02a9;
    gpio->GPDUP = 0x00000000;
    gpio->GPDCON = 0xaaaaaaaa; //Initialize VD[23:8]��0-31λ��
#if 1
  //  alignbuf = (u8*)(((u32)u8g_dsp_buffer +15)&0xfffffff0);

    /*
    pg_frame_buffer = (u16*)align_up(16,(ptu32_t)u8g_dsp_buffer);//�Դ�Ҫ��16�ֽڶ���
    pg_video_buf = (u16*)align_up(16,(ptu32_t)u8g_video_buffer); //�Դ�Ҫ��16�ֽڶ���
     */

    pg_frame_buffer = (u8*)(((u32)u8g_dsp_buffer +15)&0xfffffff0);
    pg_video_buf = (u8*)(((u32)u8g_video_buffer +15)&0xfffffff0);


#endif
    lcd->LCDCON1 = (LCDCON1_CLKVAL<<8)|(LCDCON1_MMODE<<7)|(3<<5)
                            |(12<<1)|0;
    lcd->LCDCON2 = (LCDCON2_VBPD<<24)|(LINEVAL_TFT<<14)|(LCDCON2_VFPD<<6)
                            |(LCDCON2_VSPW);
    lcd->LCDCON3 = (LCDCON3_HBPD<<19)|(HOZVAL_TFT<<8)|(LCDCON3_HFPD);
    lcd->LCDCON4 = (LCDCON4_MVAL<<8)|(LCDCON4_HSPW);
    lcd->LCDCON5 = (1<<11) | (1<<10) | (1<<9) | (1<<8) | (0<<7)
                 | (0<<6) | (1<<3)  |(LCDCON5_BSWP<<1) | (LCDCON5_HWSWP);
    lcd->LCDSADDR1 = (((u32)pg_video_buf>>22)<<21)
                                |M5D((uint32_t)pg_video_buf>>1);
    lcd->LCDSADDR2 = M5D(((u32)pg_video_buf
                            +(cn_lcd_xsize*cn_lcd_ysize*2))>>1);
    lcd->LCDSADDR3 = (((cn_lcd_xsize-cn_lcd_xsize)/1)<<11)
                                |(cn_lcd_xsize/1);
    lcd->LCDINTMSK |= (3);         //LCD�ж����μĴ���
    lcd->TCONSEL &= (~7) ;       //TCON���ƼĴ���
    lcd->TPAL = 0;                 //��ʱ��ɫ��Ĵ���
}

//----LCD�����ź�------------------------------------------------------------
//����: LCD ��Ƶ������߼�ʹ��/��ֹ��1=������Ƶ����� LCD�����źţ�
//      0=��ֹ��Ƶ����� LCD�����źš�
//����: onoff�������ź�
//����: ��
//-----------------------------------------------------------------------------
void __lcd_envid_of(int onoff)
{
	struct lcd_reg *lcd =(struct lcd_reg*)LCD_REG_BASE;

    if(onoff == 1)
    	lcd->LCDCON1 |= 1;   //������Ƶ����� LCD�����ź�
    else                        //��ֹ��Ƶ����� LCD�����ź�
    	lcd->LCDCON1 = lcd->LCDCON1 & 0x3fffe;
}
//----����LCD��Դ--------------------------------------------------------------
//����: LCD��Դ��������ʹ�ܡ�
//����: invpwren������PWREN�źż���
//      pwren��LCD_PWREN����ź�ʹ��/��ֹ
//����: ��
//-----------------------------------------------------------------------------
void __lcd_power_enable(int invpwren,int pwren)
{
	struct gpio_reg *gpio =(struct gpio_reg*)GPIO_REG_BASE;
	struct lcd_reg *lcd =(struct lcd_reg*)LCD_REG_BASE;

	gpio->GPGUP  = (gpio->GPGUP & (~(1<<4))) | (1<<4); //����ʹ��
	gpio->GPGCON = (gpio->GPGCON & (~(3<<8))) | (3<<8);
	gpio->GPGDAT = gpio->GPGDAT | (1<<4) ;

    lcd->LCDCON5 = (lcd->LCDCON5 & (~(1<<3))) | (pwren<<3);
    lcd->LCDCON5 = (lcd->LCDCON5 & (~(1<<5))) |(invpwren<<5);
}
//----������ʾ��---------------------------------------------------------------
//����: ������driver�ṩ�̱�д�ģ�����������ʾ�����ֲ����ĺ���������Ӧ�ó����
//      ���룬�������豸���̶��塣
//����: disp����ʾ��ָ��
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t __lcd_disp_ctrl(struct tagDisplayRsc *disp)
{
    return true;
}
//----��ʽ��pattern------------------------------------------------------------
//����: ���û��ύ����Ϊpattern��bitmap��ʽ����driver��ʶ��ĸ�ʽ��
//����: pattern����������ָ��
//      bitmap���û��ṩ��λͼ
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t __lcd_format_pattern(u8 *pattern,struct tagRectBitmap *bitmap)
{
    s32 x,y;
    u32 color,bytes;

    if(bitmap->PixelFormat == cn_lcd_pf)
        return true;
    bytes = 0;
    for(y = 0;y < bitmap->height;y++)
    {
        for(x = 0;x < bitmap->width;x++)
        {
            color = GK_GetPixelBm(bitmap,x,y);
            color = GK_ConvertPF2PF(cn_lcd_pf,bitmap->PixelFormat,color,NULL);
            pattern[bytes] = (u8)color;
            pattern[bytes+1] = (u8)(color>>8);
            bytes += 2;
        }
    }
    return true;
}
//----λͼ�л�����-------------------------------------------------------------
//����: ��λͼ�л�һ�����أ�ֻ����bitmap�����ظ�ʽΪcn_custom_pfʱ������Ҫ���ơ�
//      ����Կ�������֧���Զ����ʽ��������ֱ�ӷ��ء�
//����: bitmap��Ŀ��λͼ
//      limit�����ƾ��Σ�ֻ�����ڸþ����ڲ��Ĳ���
//      x��y������
//      color����ͼ�õ���ɫ��cn_sys_pf_e8r8g8b8��ʽ
//      r2_code����Ԫ��դ������
//����: ��
//-----------------------------------------------------------------------------
bool_t __lcd_set_pixel_bm(struct tagRectBitmap *bitmap,
                     s32 x,s32 y,u32 color,u32 r2_code)
{
    return false;
}
//----λͼ�л���---------------------------------------------------------------
//����: ��λͼ�л�һ��ֱ�ߣ�������(x2,y2)�㣬ֻ������limit�޶��������ڵĲ��֡�
//����: bitmap��Ŀ��λͼ
//      limit�����ƾ��Σ�ֻ�����ڸþ����ڲ��Ĳ���
//      x1��y1��x2��y2������յ�����
//      color����ͼ�õ���ɫ��cn_sys_pf_e8r8g8b8��ʽ
//      r2_code����Ԫ��դ������
//����: true=�ɹ����ƣ�false=ʧ�ܣ���Ӳ�����ٻ�֧�ְ�r2_code����
//-----------------------------------------------------------------------------
bool_t __lcd_line_bm(struct tagRectBitmap *bitmap,struct tagRectangle *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    return false;
}
//----λͼ�л���(���˵�)-------------------------------------------------------
//����: ��λͼ�л�һ��ֱ�ߣ�ֻ������limit�޶��������ڵĲ��֡�
//����: bitmap��Ŀ��λͼ
//      limit�����ƾ��Σ�ֻ�����ڸþ����ڲ��Ĳ���
//      x1��y1��x2��y2������յ�����
//      color����ͼ�õ���ɫ��cn_sys_pf_e8r8g8b8��ʽ
//      r2_code����Ԫ��դ������
//����: true=�ɹ����ƣ�false=ʧ�ܣ���Ӳ�����ٻ�֧�ְ�r2_code����
//-----------------------------------------------------------------------------
bool_t __lcd_line_bm_ie(struct tagRectBitmap *bitmap,struct tagRectangle *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    return false;
}
//----λͼ��������-----------------------------------------------------------
//����: ��λͼ�еľ�����color��ɫ��䡣
//����: bitmap��Ŀ��λͼ
//      dst_rect�������ľ���
//      color�������ɫ��cn_sys_pf_e8r8g8b8��ʽ
//����: true=�ɹ����ƣ�false=ʧ��
//-----------------------------------------------------------------------------
bool_t __lcd_fill_rect_bm(struct tagRectBitmap *dst_bitmap,
                          struct tagRectangle *Target,
                          struct tagRectangle *Focus,
                          u32 Color0,u32 Color1,u32 Mode)
{
    s32 x, y;
    u16 pixel;
    u16 *dst,*p;

    if(Mode != CN_FILLRECT_MODE_N)
        return false;

    if(dst_bitmap->PixelFormat != CN_SYS_PF_RGB565)
        return false;

    pixel = GK_ConvertRGB24ToPF(CN_SYS_PF_RGB565,Color0);

    dst = (u16*)((u32)dst_bitmap->bm_bits
                      + Focus->top * dst_bitmap->linebytes);
    dst += Focus->left;

    for(y = Focus->top; y < Focus->bottom; y++)
    {
        p =dst;
        for(x=Focus->left; x<Focus->right;x++)
        {
            *p++ =pixel;
        }
        dst += dst_bitmap->linebytes >> 1;
    }
    return true;
}
//----bitmap��λ�鴫��---------------------------------------------------------
//����: ��һ��λͼ�е�һ�����������͵���һ����������λͼ�����ظ�ʽ������ͬ��
//      Դ��Ŀ��λͼ������ͬ��
//����: dst_bitmap��Ŀ��λͼ
//      dst_rect��Ŀ�����
//      src_bitmap��Դλͼ
//      xsrc��ysrc��Դλͼ�б����͵��������Ͻ�����
//      pat��patternָ��
//      bitmsk��rop4������Ҫ������
//      rop_code����Ԫ��դ������
//      transparentcolor��͸��ɫ
//����: true=�ɹ�,false=ʧ��
//-----------------------------------------------------------------------------
bool_t __lcd_blt_bm_to_bm(struct tagRectBitmap *dst_bitmap,
                        struct tagRectangle *DstRect,
                        struct tagRectBitmap *src_bitmap,
						struct tagRectangle *SrcRect,
						u32 RopCode,u32 transparentcolor)
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
                                      + SrcRect->top * src_bitmap->linebytes);
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
//----��ȡλͼ�е�����---------------------------------------------------------
//����: ȡλͼ�е�һ�����أ�ֻ����bitmap�����ظ�ʽΪcn_custom_pfʱ������Ҫ��ȡ��
//      ����Կ�������֧���Զ����ʽ��������ֱ�ӷ���false��
//����: bitmap��Ŀ��λͼ
//      x��y������
//����: ��ȡ������ֵ��cn_sys_pf_e8r8g8b8��ʽ
//-----------------------------------------------------------------------------
u32 __lcd_get_pixel_bm(struct tagRectBitmap *bitmap,s32 x,s32 y)
{
    return 0;
}
//----��ȡbitmap�о��ο�-------------------------------------------------------
//����: ��һ��λͼ�еľ��ζ�ȡ����һ��λͼ�С�
//����: src��Դλͼ
//      rect������ȡ�ľ���
//      dest��������ε�λͼ���䳤�������rect��ͬ
//����: true=�ɹ���ȡ��false=ʧ�ܣ�ԭ��������ṩ�˲�֧�ֵ����ظ�ʽ
//-----------------------------------------------------------------------------
bool_t __lcd_get_rect_bm(struct tagRectBitmap *src,struct tagRectangle *rect,
                     struct tagRectBitmap *dest)
{
    return false;
}
//----screen�л�����-----------------------------------------------------------
//����: ��screen�л�һ������.
//����: x��y����������
//      color����ɫ
//      r2_code����Ԫ��դ������
//����: true=�ɹ����ƣ�false=û�л��ƣ����������
//-----------------------------------------------------------------------------
bool_t __lcd_set_pixel_screen(s32 x,s32 y,u32 color,u32 rop2_code)
{
    u32 dest,pen;
    u32 byteoffset;
    byteoffset = y*cn_lcd_xsize + x;
    pen = GK_ConvertRGB24ToPF(CN_SYS_PF_RGB565,color);
    dest = (u32)pg_video_buf[byteoffset];
    dest = GK_BlendRop2(dest,pen,rop2_code);
    pg_video_buf[byteoffset] = (u16)dest;
    return true;
}
//----screen�л���---------------------------------------------------------------
//����: ��screen�л�һ������ֱ�ߣ������˵㣬ֻ������limit�޶��������ڵĲ��֡���
//      ��֧�ֻ��ߵ�Ӳ�����٣�ֱ�ӷ���false��
//����: limit�����ƾ��Σ�ֻ�����ڸþ����ڲ��Ĳ���
//      x1��y1��x2��y2������յ�����
//      color����ͼ�õ���ɫ
//      r2_code����Ԫ��դ������
//����: true=�ɹ����ƣ�false=ʧ�ܣ���Ӳ�����ٻ�֧�ְ�r2_code����
//-----------------------------------------------------------------------------
bool_t __lcd_line_screen(struct tagRectangle *limit,
                    s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    return false;
}
//----screen�л���(���˵�)------------------------------------------------------
//����: ��screen�л�һ��ֱ�ߣ�ֻ������limit�޶��������ڵĲ��֡�
//����: limit�����ƾ��Σ�ֻ�����ڸþ����ڲ��Ĳ���
//      x1��y1��x2��y2������յ�����
//      color����ͼ�õ���ɫ��cn_sys_pf_e8r8g8b8��ʽ
//      r2_code����Ԫ��դ������
//����: true=�ɹ����ƣ�false=ʧ�ܣ���Ӳ�����ٻ�֧�ְ�r2_code����
//-----------------------------------------------------------------------------
bool_t __lcd_line_screen_ie(struct tagRectangle *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    return false;
}
//----screen��������-----------------------------------------------------------
//����: ��screen�еľ�����color��ɫ��䣬color�ĸ�ʽ��cn_sys_pf_e8r8g8b8��
//����: dst_rect�������ľ���
//      color�������ɫ
//����: true=�ɹ����ƣ�false=ʧ��
//-----------------------------------------------------------------------------
bool_t __lcd_fill_rect_screen(struct tagRectangle *Target,
                              struct tagRectangle *Focus,
                              u32 Color0,u32 Color1,u32 Mode)
{
    int x,y;
    u32 pen;
    u16 *p;
    return false;

    if(Mode != CN_FILLRECT_MODE_N)
        return false;
    pen = __GK_ConvertColorToPF(CN_SYS_PF_RGB565,Color0);

    for(y=Focus->top;y<Focus->bottom;y++)
    {
        p =&pg_video_buf[y*cn_lcd_xsize];

        for(x=Focus->left;x<Focus->right;x++)
        {
            *p++ =pen;
        }
    }
    return true;
}

//----bitmap��screenλ�鴫��---------------------------------------------------
//����: ��һ��λͼ�е�һ�����������͵���һ����������λͼ�����ظ�ʽ������ͬ��
//      Դ��Ŀ��λͼ������ͬ��
//����: dst_rect��Ŀ�����
//      src_bitmap��Դλͼ
//      xsrc��ysrc��Դλͼ�б����͵��������Ͻ�����(��Դλͼ���Ͻ�����Ϊԭ��)
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t __lcd_bm_to_screen(struct tagRectangle *dst_rect,
                struct tagRectBitmap *src_bitmap,s32 xsrc,s32 ysrc)
{
    u32 size,double_width_src;
    u32 byteoffset_rect,byteoffset_bitmap;
    s32 y;

    double_width_src = src_bitmap->width*2;     //Դλͼһ����ռ���ֽ���
    //ÿһ��Ҫ�����͵��ֽ���
    size = (dst_rect->right-dst_rect->left)*(cn_lcd_pf>>8)/8;
    //Դλͼ���Ͻ�������ֽ�ƫ����
    byteoffset_bitmap = ysrc*double_width_src + xsrc*2;
    //Ŀ��������Ͻ������˫�ֽ�ƫ��������Ϊpg_video_bufΪu16��ָ��
    //��Ϊ�Ǵ��͵�screen�ϣ�����ÿ��˫�ֽ���ȡcn_lcd_xsize
    byteoffset_rect = dst_rect->top*cn_lcd_xsize + dst_rect->left;

    //bitmap��screenλ�鴫��
    for(y = dst_rect->top;y < dst_rect->bottom;y++)
    {
        memcpy(&(pg_video_buf[byteoffset_rect]),
                    &src_bitmap->bm_bits[byteoffset_bitmap],size);
        byteoffset_bitmap += double_width_src;
        byteoffset_rect += cn_lcd_xsize;
    }
    return true;
}
//----��screen��ȡ����---------------------------------------------------------
//����: ��screen��ȡһ���أ���ת����cn_sys_pf_e8r8g8b8��cn_sys_pf_a8r8g8b8��ʽ��
//����: x��y������
//����: ������ɫֵ
//-----------------------------------------------------------------------------
u32 __lcd_get_pixel_screen(s32 x,s32 y)
{
    u32 r,g,b,color,offset;

    offset = pg_lcd_reg->LCDSADDR1;
    offset += y*cn_lcd_xsize*2;
    offset += x*2;
    color = (u32)(*(u16*)offset);
    r = (color>>11) &0x1f;
    g = (color>>5) & 0x3f;
    b = color & 0x1f;
    return (r<<19) | (g<<10) | (b<<3);
}

//----��ȡbitmap�о��ο�-------------------------------------------------------
//����: ��һ��λͼ�еľ��ζ�ȡ����һ��λͼ�С�
//����: rect������ȡ�ľ���
//      dest��������ε�λͼ���䳤�������rect��ͬ
//����: true=�ɹ���ȡ��false=ʧ�ܣ�ԭ��������ṩ�˲�֧�ֵ����ظ�ʽ
//-----------------------------------------------------------------------------
bool_t __lcd_get_rect_screen(struct tagRectangle *rect,struct tagRectBitmap *dest)
{
    return false;
 }
//----��ʼ��lcd�豸------------------------------------------------------------
//����: ��ʼ��lcd�豸��
//����: ��
//����: ��ʾ����Դָ��
//-----------------------------------------------------------------------------
static struct tagGkWinRsc frame_win;
static struct tagRectBitmap frame_bm;

struct tagDisplayRsc* ModuleInstall_LCD(s32 width,s32 height,const char *DisplayName,const char* HeapName)
{

    __lcd_hard_init();
    __lcd_power_enable(0,1);
    __lcd_envid_of(1);

    frame_bm.PixelFormat = cn_lcd_pf;
    frame_bm.width = width;
    frame_bm.height = height;
    frame_bm.linebytes = width*2;
    frame_bm.bm_bits = (u8*)pg_frame_buffer;

    frame_win.wm_bitmap =&frame_bm;

    tg_lcd_display.frame_buffer = &frame_win;

    tg_lcd_display.xmm = 0;
    tg_lcd_display.ymm = 0;
    tg_lcd_display.width = width;
    tg_lcd_display.height = height;
    tg_lcd_display.pixel_format = CN_SYS_PF_RGB565;
    tg_lcd_display.reset_clip = false;
    tg_lcd_display.framebuf_direct = false;
    //�����ʼ��frame_buffer��desktop��z_topmost������Ա

    tg_lcd_display.draw.set_pixel_bm = __lcd_set_pixel_bm;
    tg_lcd_display.draw.fill_rect_bm = __lcd_fill_rect_bm;
    tg_lcd_display.draw.get_pixel_bm = __lcd_get_pixel_bm;
    tg_lcd_display.draw.line_bm = __lcd_line_bm;
    tg_lcd_display.draw.line_bm_ie = __lcd_line_bm_ie;
    tg_lcd_display.draw.blt_bm_to_bm = __lcd_blt_bm_to_bm;
    tg_lcd_display.draw.get_rect_bm = __lcd_get_rect_bm;
//    tg_lcd_display.draw.check_raster = __lcd_check_raster;
    tg_lcd_display.draw.set_pixel_screen = __lcd_set_pixel_screen;
    tg_lcd_display.draw.line_screen = __lcd_line_screen;
    tg_lcd_display.draw.line_screen_ie = __lcd_line_screen_ie;
    tg_lcd_display.draw.fill_rect_screen = __lcd_fill_rect_screen;
    tg_lcd_display.draw.bm_to_screen = __lcd_bm_to_screen;
    tg_lcd_display.draw.get_pixel_screen = __lcd_get_pixel_screen;
    tg_lcd_display.draw.get_rect_screen = __lcd_get_rect_screen;

//    tg_lcd_display.bmmalloc = lcd_bmmalloc;     //hjj,2416
    tg_lcd_display.DisplayHeap =  M_FindHeap(HeapName);
    tg_lcd_display.disp_ctrl = __lcd_disp_ctrl;

    if(GK_InstallDisplay(&tg_lcd_display,(char*)DisplayName))
        return (ptu32_t)&tg_lcd_display;
    else
        return (ptu32_t)NULL;
}






