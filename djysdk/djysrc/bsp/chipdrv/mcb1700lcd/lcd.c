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
//����ģ��:ͼ��ģ��
//���ߣ�HM
//�汾��V1.0.0
//�ļ�����: MCB1700����LCD��������,DJYGUIͼ��ģ�����������
//-----------------------------------------------------------------------------
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "cpu_peri.h"
#include "djyos.h"
#include "gkernel.h"
#include "gk_display.h"
#include "lcd.h"
#include "lcd_spidrv.h"
#include "heap.h"

// =============================================================================
#define cn_coordinates      0       //0=�������꣬1=��ת���꣬��ԭ��=(239,319)
struct tagDisplayRsc tg_lcd_display;

#define LANDSCAPE   0                   /* 1 for landscape, 0 for portrait    */
#define ROTATE180   0                   /* 1 to rotate the screen for 180 deg */

static unsigned char Himax;
// =============================================================================

#define DELAY_2N 18
static void delay (int cnt)     //TODO ������������õ�,����OS�ṩ�ĺ���.
{
    cnt <<= DELAY_2N;
    while (cnt--);
}

//������mcbqvga�Ĳ�������
/*******************************************************************************
* Write a command the LCD controller                                           *
*   Parameter:    cmd:    command to be written                                *
*   Return:                                                                    *
*******************************************************************************/
static __inline void wr_cmd (unsigned char cmd)
{
//    LCD_CS(0);
//    spi_tran(SPI_START | SPI_WR | SPI_INDEX);   /* Write : RS = 0, RW = 0       */
//    spi_tran(0);
//    spi_tran(cmd);
//    LCD_CS(1);
    u8 data[3];
    data[0] = (u8)(SPI_START | SPI_WR | SPI_INDEX);
    data[1] = 0;
    data[2] = cmd;
    LCD_TxRx(data,3,NULL,0);
}

/*******************************************************************************
* Write data to the LCD controller                                             *
*   Parameter:    dat:    data to be written                                   *
*   Return:                                                                    *
*******************************************************************************/
static __inline void wr_dat (unsigned short dat)
{
//    LCD_CS(0);
//    spi_tran(SPI_START | SPI_WR | SPI_DATA);    /* Write : RS = 1, RW = 0       */
//    spi_tran((dat >>   8));                     /* Write D8..D15                */
//    spi_tran((dat & 0xFF));                     /* Write D0..D7                 */
//    LCD_CS(1);
    u8 data[3];
    data[0] = (u8)(SPI_START | SPI_WR | SPI_DATA);
    data[1] = (u8)(dat >>   8);
    data[2] = (u8)(dat & 0xFF);
    LCD_TxRx(data,3,NULL,0);
}

/*---------------------------------------------------------------------------
���ܣ�  дlcd�ڲ��Ĵ���
---------------------------------------------------------------------------*/
void __mcbqvga_write_reg(u32 reg_index, u32 data)
{
      wr_cmd(reg_index);
      wr_dat(data);
}

/*---------------------------------------------------------------------------
���ܣ�  д����
---------------------------------------------------------------------------*/
void __mcbqvga_write_cmd(u32 cmd)
{
    wr_cmd(cmd);
}

/*---------------------------------------------------------------------------
���ܣ�  д����
---------------------------------------------------------------------------*/
//#define __mcbqvga_write_data(data) (LCD_CMD=data)
void __mcbqvga_write_data(u32 data)
{
    wr_dat((u16)data);
}

/*---------------------------------------------------------------------------
���ܣ�  ������
---------------------------------------------------------------------------*/
u32 __mcbqvga_read_data(void)
{
//  return LCD_CMD;
//      unsigned short val = 0;

//      LCD_CS(0);
//      spi_tran(SPI_START | SPI_RD | SPI_DATA);    /* Read: RS = 1, RW = 1         */
//      spi_tran(0);                                /* Dummy read 1                 */
//      val   = spi_tran(0);                        /* Read D8..D15                 */
//      val <<= 8;
//      val  |= spi_tran(0);                        /* Read D0..D7                  */
//      LCD_CS(1);
//      return (val);

    u8 dout[2],din[2];
    dout[0] = (u8)(SPI_START | SPI_RD | SPI_DATA);
    dout[1] = (u8)(0);
    LCD_TxRx(dout,2,din,2);
    return (u16)((din[0]<<8) + din[1]);
}

void __mcbqvga_set_pos(u32 x,u32 y)
{
    __mcbqvga_write_reg(0x02, x >>    8);            /* Column address start MSB           */
    __mcbqvga_write_reg(0x03, x &  0xFF);            /* Column address start LSB           */
    __mcbqvga_write_reg(0x04, x >>    8);            /* Column address end MSB             */
    __mcbqvga_write_reg(0x05, x &  0xFF);            /* Column address end LSB             */

    __mcbqvga_write_reg(0x06, y >>    8);            /* Row address start MSB              */
    __mcbqvga_write_reg(0x07, y &  0xFF);            /* Row address start LSB              */
    __mcbqvga_write_reg(0x08, y >>    8);            /* Row address end MSB                */
    __mcbqvga_write_reg(0x09, y &  0xFF);            /* Row address end LSB                */
}
/*---------------------------------------------------------------------------
���ܣ�  ����һ������
---------------------------------------------------------------------------*/
void __mcbqvga_set_pixel(u32 x, u32 y, u16 color)
{
    __mcbqvga_set_pos(x,y);

    __mcbqvga_write_cmd(0x0022);
    __mcbqvga_write_data(color);
}

/*---------------------------------------------------------------------------
���ܣ�  ��ȡһ������
---------------------------------------------------------------------------*/
u16 __mcbqvga_get_pixel(u32 x, u32 y)
{
    u16 col = 0;//, rgb = 0

    __mcbqvga_set_pos(x,y);

    __mcbqvga_write_cmd(0x0022);
    __mcbqvga_read_data();//dummy read

    col = __mcbqvga_read_data();//BGR ����
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
void __mcbqvga_set_cursor(u32 x, u32 y)
{
    __mcbqvga_set_pos(x,y);
}

/*---------------------------------------------------------------------------
���ܣ�  ����
---------------------------------------------------------------------------*/
void __mcbqvga_set_window(u32 x,u32 y, u32 wide,u32 high)
{
    unsigned int xe, ye;
    xe = x+wide-1;
    ye = y+high-1;

    __mcbqvga_write_reg(0x02, x  >>    8);           /* Column address start MSB           */
    __mcbqvga_write_reg(0x03, x  &  0xFF);           /* Column address start LSB           */
    __mcbqvga_write_reg(0x04, xe >>    8);           /* Column address end MSB             */
    __mcbqvga_write_reg(0x05, xe &  0xFF);           /* Column address end LSB             */

    __mcbqvga_write_reg(0x06, y  >>    8);           /* Row address start MSB              */
    __mcbqvga_write_reg(0x07, y  &  0xFF);           /* Row address start LSB              */
    __mcbqvga_write_reg(0x08, ye >>    8);           /* Row address end MSB                */
    __mcbqvga_write_reg(0x09, ye &  0xFF);           /* Row address end LSB                */

}

/*---------------------------------------------------------------------------
���ܣ�  �ش�
---------------------------------------------------------------------------*/
void __mcbqvga_close_window(void)
{
    __mcbqvga_write_reg(0x0050, 0);//ˮƽ��ʼ��ַ
    __mcbqvga_write_reg(0x0051, 239);//ˮƽ������ַ

    __mcbqvga_write_reg(0x0052, 0);//��ֱ��ʼ��ַ
    __mcbqvga_write_reg(0x0053, 319);//��ֱ������ַ

    __mcbqvga_set_cursor(0, 0);//����������Ͻ�
}
void lcd_clear(u32 color)
{
    u32 i = 0;

    __mcbqvga_close_window();

    __mcbqvga_write_cmd(0x0022);                                    //дGRAM֮ǰ Ҫ��д0x0022
    for (i = 0; i < 320*240 ; i++)
    {
        __mcbqvga_write_data(color);
    }

}

void lcd_reset(void)
{
}

void lcd_backlight_on(void)
{
}

void lcd_backlight_off(void)
{
}

//������djyguiҪ��Ľӿں������������£�������ÿպ�������
/*---------------------------------------------------------------------------
���ܣ�    lcd ��ʼ��
---------------------------------------------------------------------------*/
void __lcd_mcbqvga_init(void)
{
//    SPI_HardDrvInit();

    /* LCD with HX8347-D LCD Controller   */
    Himax = 1;  /* Set Himax LCD controller flag      */
    /* Driving ability settings ----------------------------------------------*/
    __mcbqvga_write_reg(0xEA, 0x00);                 /* Power control internal used (1)    */
    __mcbqvga_write_reg(0xEB, 0x20);                 /* Power control internal used (2)    */
    __mcbqvga_write_reg(0xEC, 0x0C);                 /* Source control internal used (1)   */
    __mcbqvga_write_reg(0xED, 0xC7);                 /* Source control internal used (2)   */
    __mcbqvga_write_reg(0xE8, 0x38);                 /* Source output period Normal mode   */
    __mcbqvga_write_reg(0xE9, 0x10);                 /* Source output period Idle mode     */
    __mcbqvga_write_reg(0xF1, 0x01);                 /* RGB 18-bit interface ;0x0110       */
    __mcbqvga_write_reg(0xF2, 0x10);

    /* Adjust the Gamma Curve ------------------------------------------------*/
    __mcbqvga_write_reg(0x40, 0x01);
    __mcbqvga_write_reg(0x41, 0x00);
    __mcbqvga_write_reg(0x42, 0x00);
    __mcbqvga_write_reg(0x43, 0x10);
    __mcbqvga_write_reg(0x44, 0x0E);
    __mcbqvga_write_reg(0x45, 0x24);
    __mcbqvga_write_reg(0x46, 0x04);
    __mcbqvga_write_reg(0x47, 0x50);
    __mcbqvga_write_reg(0x48, 0x02);
    __mcbqvga_write_reg(0x49, 0x13);
    __mcbqvga_write_reg(0x4A, 0x19);
    __mcbqvga_write_reg(0x4B, 0x19);
    __mcbqvga_write_reg(0x4C, 0x16);

    __mcbqvga_write_reg(0x50, 0x1B);
    __mcbqvga_write_reg(0x51, 0x31);
    __mcbqvga_write_reg(0x52, 0x2F);
    __mcbqvga_write_reg(0x53, 0x3F);
    __mcbqvga_write_reg(0x54, 0x3F);
    __mcbqvga_write_reg(0x55, 0x3E);
    __mcbqvga_write_reg(0x56, 0x2F);
    __mcbqvga_write_reg(0x57, 0x7B);
    __mcbqvga_write_reg(0x58, 0x09);
    __mcbqvga_write_reg(0x59, 0x06);
    __mcbqvga_write_reg(0x5A, 0x06);
    __mcbqvga_write_reg(0x5B, 0x0C);
    __mcbqvga_write_reg(0x5C, 0x1D);
    __mcbqvga_write_reg(0x5D, 0xCC);

    /* Power voltage setting -------------------------------------------------*/
    __mcbqvga_write_reg(0x1B, 0x1B);
    __mcbqvga_write_reg(0x1A, 0x01);
    __mcbqvga_write_reg(0x24, 0x2F);
    __mcbqvga_write_reg(0x25, 0x57);
    __mcbqvga_write_reg(0x23, 0x88);

    /* Power on setting ------------------------------------------------------*/
    __mcbqvga_write_reg(0x18, 0x36);                 /* Internal oscillator frequency adj  */
    __mcbqvga_write_reg(0x19, 0x01);                 /* Enable internal oscillator         */
    __mcbqvga_write_reg(0x01, 0x00);                 /* Normal mode, no scrool             */
    __mcbqvga_write_reg(0x1F, 0x88);                 /* Power control 6 - DDVDH Off        */
    delay(20);
    __mcbqvga_write_reg(0x1F, 0x82);                 /* Power control 6 - Step-up: 3 x VCI */
    delay(5);
    __mcbqvga_write_reg(0x1F, 0x92);                 /* Power control 6 - Step-up: On      */
    delay(5);
    __mcbqvga_write_reg(0x1F, 0xD2);                 /* Power control 6 - VCOML active     */
    delay(5);

    /* Color selection -------------------------------------------------------*/
    __mcbqvga_write_reg(0x17, 0x55);                 /* RGB, System interface: 16 Bit/Pixel*/
    __mcbqvga_write_reg(0x00, 0x00);                 /* Scrolling off, no standby          */

    /* Interface config ------------------------------------------------------*/
    __mcbqvga_write_reg(0x2F, 0x11);                 /* LCD Drive: 1-line inversion        */
    __mcbqvga_write_reg(0x31, 0x00);
    __mcbqvga_write_reg(0x32, 0x00);                 /* DPL=0, HSPL=0, VSPL=0, EPL=0       */

    /* Display on setting ----------------------------------------------------*/
    __mcbqvga_write_reg(0x28, 0x38);                 /* PT(0,0) active, VGL/VGL            */
    delay(20);
    __mcbqvga_write_reg(0x28, 0x3C);                 /* Display active, VGL/VGL            */

    #if (LANDSCAPE == 1)
    #if (ROTATE180 == 0)
     __mcbqvga_write_reg (0x16, 0xA8);
    #else
     __mcbqvga_write_reg (0x16, 0x68);
    #endif
    #else
    #if (ROTATE180 == 0)
     __mcbqvga_write_reg (0x16, 0x08);
    #else
     __mcbqvga_write_reg (0x16, 0xC8);
    #endif
    #endif

    /* Display scrolling settings --------------------------------------------*/
    __mcbqvga_write_reg(0x0E, 0x00);                 /* TFA MSB                            */
    __mcbqvga_write_reg(0x0F, 0x00);                 /* TFA LSB                            */
    __mcbqvga_write_reg(0x10, 320 >> 8);             /* VSA MSB                            */
    __mcbqvga_write_reg(0x11, 320 &  0xFF);          /* VSA LSB                            */
    __mcbqvga_write_reg(0x12, 0x00);                 /* BFA MSB                            */
    __mcbqvga_write_reg(0x13, 0x00);                 /* BFA LSB                            */

//  lcd_backlight_on();
      LPC_GPIO4->FIOSET = 0x10000000;
}

/*---------------------------------------------------------------------------
���ܣ�    display on
---------------------------------------------------------------------------*/
void lcd_display_on(void)
{
    __mcbqvga_write_reg(0x0007, 0x0173);
}

/*---------------------------------------------------------------------------
���ܣ�    display off
---------------------------------------------------------------------------*/
void lcd_display_off(void)
{
    __mcbqvga_write_reg(0x0007, 0x0000);
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
        __mcbqvga_set_pixel(x,y,pixel);
    }else
    {
        dest = __mcbqvga_get_pixel(x,y);
        pixel = GK_BlendRop2(dest, pixel, r2_code);
        __mcbqvga_set_pixel(x,y,pixel);
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
    __mcbqvga_set_window(Focus->left,Focus->top,width,height);
    __mcbqvga_write_cmd(0x0022);
    for(y = 0; y < height; y++)
    {
        for(x = 0; x < width; x++)
        {
            __mcbqvga_write_data(pixel);
        }
    }
    __mcbqvga_close_window();
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
    __mcbqvga_set_window(dst_rect->left,dst_rect->top,width,height);
    __mcbqvga_write_cmd(0x0022);
    for(y = 0; y < height; y++)
    {
        for(x = 0; x < width; x++)
        {
            __mcbqvga_write_data(lineoffset[x]);
        }
        lineoffset += (src_bitmap->linebytes)>>1;
    }
    __mcbqvga_close_window();
    return true;
}

//��screen��ȡһ���أ���ת����cn_sys_pf_e8r8g8b8
u32 __lcd_get_pixel_screen(s32 x,s32 y)
{
    return GK_ConvertColorToRGB24(CN_SYS_PF_RGB565,
                        __mcbqvga_get_pixel(x,y),0);
}
#else
bool_t __lcd_set_pixel_screen(s32 x,s32 y,u32 color,u32 r2_code)
{
    u16 dest,pixel;
    pixel = GK_ConvertRGB24ToPF(CN_SYS_PF_RGB565,color);
    if(CN_R2_COPYPEN == r2_code)
    {
        __mcbqvga_set_pixel(LCD_XSIZE-x,LCD_YSIZE-y,pixel);   //ת������
    }else
    {
        dest = __mcbqvga_get_pixel(LCD_XSIZE-1-x,LCD_YSIZE-1-y);  //ת������
        pixel = GK_BlendRop2(dest, pixel, r2_code);
        __mcbqvga_set_pixel(LCD_XSIZE-1-x,LCD_YSIZE-1-y,pixel);   //ת������
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
    __mcbqvga_set_window(LCD_XSIZE-Focus->right,
                         LCD_YSIZE-Focus->bottom,width,height);
    __mcbqvga_write_cmd(0x0022);
    for(y = 0; y < height; y++)
    {
        for(x = 0; x < width; x++)
        {
            __mcbqvga_write_data(pixel);
        }
    }
    __mcbqvga_close_window();
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
    __mcbqvga_set_window(LCD_XSIZE-dst_rect->right,
                         LCD_YSIZE-dst_rect->bottom,width,height);
    __mcbqvga_write_cmd(0x0022);
    for(y = 0; y < height; y++)
    {
        for(x = width-1; x >= 0; x--)
        {
            __mcbqvga_write_data(lineoffset[x]);
        }
        lineoffset -= (src_bitmap->linebytes)>>1;
    }
    __mcbqvga_close_window();
    return true;
}

//��screen��ȡһ���أ���ת����cn_sys_pf_e8r8g8b8
u32 __lcd_get_pixel_screen(s32 x,s32 y)
{
    return GK_ConvertColorToRGB24(CN_SYS_PF_RGB565,
                    __mcbqvga_get_pixel(LCD_XSIZE-1-x,LCD_YSIZE-1-y),0);
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
ptu32_t LCD_ModuleInit(ptu32_t para)
{
    LCD_HardInit();
    __lcd_mcbqvga_init( );
    tg_lcd_display.frame_buffer = NULL;

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

    tg_lcd_display.DisplayHeap = (struct tagHeapCB *)M_FindHeap("sys");
    tg_lcd_display.disp_ctrl = __lcd_disp_ctrl;

    GK_InstallDisplay(&tg_lcd_display,(char*)para);
    return (ptu32_t)&tg_lcd_display;
}

