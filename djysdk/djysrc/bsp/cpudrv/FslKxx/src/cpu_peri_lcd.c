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
#include "string.h"
#include "stdint.h"
#include "stdlib.h"

#include "gk_display.h"
#include "board-config.h"

#include "cpu_peri.h"
#include "cpu_peri_lcd.h"


static u32 cfg_LcdPf = CN_SYS_PF_ERGB8888;

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
struct tagDisplayRsc tg_lcd_display;
static u8 *pg_video_buf,*pg_frame_buffer;


void __lcd_gpio_init(void)
{
    PORT_MuxConfig(PORT_PORT_F,PORT_PIN(4),PORT_PINMUX_ALT7); // D[0]
    PORT_MuxConfig(PORT_PORT_F,PORT_PIN(5),PORT_PINMUX_ALT7); // D[1]
    PORT_MuxConfig(PORT_PORT_F,PORT_PIN(6),PORT_PINMUX_ALT7); // D[2]
    PORT_MuxConfig(PORT_PORT_F,PORT_PIN(7),PORT_PINMUX_ALT7); // D[3]
    PORT_MuxConfig(PORT_PORT_F,PORT_PIN(8),PORT_PINMUX_ALT7); // D[4]
    PORT_MuxConfig(PORT_PORT_F,PORT_PIN(9),PORT_PINMUX_ALT7); // D[5]
    PORT_MuxConfig(PORT_PORT_F,PORT_PIN(10),PORT_PINMUX_ALT7); // D[6]
    PORT_MuxConfig(PORT_PORT_F,PORT_PIN(11),PORT_PINMUX_ALT7); // D[7]
    PORT_MuxConfig(PORT_PORT_F,PORT_PIN(12),PORT_PINMUX_ALT7); // D[8]
    PORT_MuxConfig(PORT_PORT_F,PORT_PIN(13),PORT_PINMUX_ALT7); // D[9]
    PORT_MuxConfig(PORT_PORT_F,PORT_PIN(14),PORT_PINMUX_ALT7); // D[10]
    PORT_MuxConfig(PORT_PORT_F,PORT_PIN(15),PORT_PINMUX_ALT7); // D[11]

    PORT_MuxConfig(PORT_PORT_F,PORT_PIN(16),PORT_PINMUX_ALT5); // D[12]
    PORT_MuxConfig(PORT_PORT_F,PORT_PIN(17),PORT_PINMUX_ALT5); // D[13]
    PORT_MuxConfig(PORT_PORT_F,PORT_PIN(18),PORT_PINMUX_ALT5); // D[14]
    PORT_MuxConfig(PORT_PORT_F,PORT_PIN(19),PORT_PINMUX_ALT5); // D[15]
    PORT_MuxConfig(PORT_PORT_F,PORT_PIN(20),PORT_PINMUX_ALT5); // D[16]

    PORT_MuxConfig(PORT_PORT_F,PORT_PIN(21),PORT_PINMUX_ALT7); // D[17]
    PORT_MuxConfig(PORT_PORT_F,PORT_PIN(22),PORT_PINMUX_ALT7); // D[18]
    PORT_MuxConfig(PORT_PORT_F,PORT_PIN(23),PORT_PINMUX_ALT7); // D[19]
    PORT_MuxConfig(PORT_PORT_F,PORT_PIN(24),PORT_PINMUX_ALT7); // D[20]
    PORT_MuxConfig(PORT_PORT_F,PORT_PIN(25),PORT_PINMUX_ALT7); // D[21]
    PORT_MuxConfig(PORT_PORT_F,PORT_PIN(26),PORT_PINMUX_ALT7); // D[22]
    PORT_MuxConfig(PORT_PORT_F,PORT_PIN(27),PORT_PINMUX_ALT7); // D[23]

    PORT_MuxConfig(PORT_PORT_F,PORT_PIN(0),PORT_PINMUX_ALT7); // PCLK
    PORT_MuxConfig(PORT_PORT_F,PORT_PIN(1),PORT_PINMUX_ALT7); // DE
    PORT_MuxConfig(PORT_PORT_F,PORT_PIN(2),PORT_PINMUX_ALT7); // HSYNC
    PORT_MuxConfig(PORT_PORT_F,PORT_PIN(3),PORT_PINMUX_ALT7); // VSYNC
}

void __lcd_controller_init(void)
{
    // Disable MPU
    MPU->CESR &= ~MPU_CESR_VLD_MASK;

    // Enable LCDC and DDR Modules
    SIM->SCGC3 |= SIM_SCGC3_LCDC_MASK;

    // set LCD Screen Start Address
    LCDC->LSSAR = FRAME_START_ADDRESS;
    // set LCD Size. The XMAX bitfield is the screen x-size/16.
    LCDC->LSR = LCDC_LSR_XMAX( (LCD_XSIZE / 16) ) | LCDC_LSR_YMAX( LCD_YSIZE );
    // set LCD virtual page width
    LCDC->LVPWR = LCDC_LVPWR_VPW( LCD_XSIZE );
    // set LCD cursor positon & settings (turn off)
    LCDC->LCPR = 0;
    LCDC->LCWHB = 0;

    // set LCD panel configuration. Use endianess to work with TWR-LCD-RGB lines.
    LCDC->LPCR =
    LCDC_LPCR_TFT_MASK      |       //TFT Screen
    LCDC_LPCR_COLOR_MASK    |       //Color
    LCDC_LPCR_BPIX(0x7)     |       //24 bpp
    LCDC_LPCR_FLMPOL_MASK   |       //first line marker active low (VSYNC)
    LCDC_LPCR_LPPOL_MASK    |       //line pulse active low (HSYNC)
    LCDC_LPCR_END_SEL_MASK  |       //Use big-endian mode (0xFFAA5500 means R=AA,G=55,B=00).
//  LCDC_LPCR_SWAP_SEL_MASK |       //Set if needed for LCD data lines match up correctly with the LCD
    LCDC_LPCR_SCLKIDLE_MASK |       //Enalbe LSCLK when vsync is idle
    LCDC_LPCR_SCLKSEL_MASK  |       //Always enable clock
    LCDC_LPCR_PCD(11);             //Divide 120 PLL0 clock (default clock) by (11+1)=12 to get 10MHz clock

    #define REVE
    //If RevE or later TWR-LCD-RGB, need to adjust clock settings
    #ifdef REVE

    LCDC->LPCR |= LCDC_LPCR_CLKPOL_MASK;    //In TFT mode, active on negative edge of LSCLK.

    // set LCD horizontal configuration based on panel data (Figure 3-3 in Seiko datasheet)
    LCDC->LHCR =
    LCDC_LHCR_H_WIDTH(41)   |    //(41+1)=42 SCLK period for HSYNC activated
    LCDC_LHCR_H_WAIT_1(1)  |    //(1+1)=2 SCLK period between end of OE and beginning of HSYNC
    LCDC_LHCR_H_WAIT_2(0);     //(0+3)=3 SCLK periods between end of HSYNC and beginning of OE

    // set LCD vertical configuration based on panel data (Figure 3-3 in Seiko datasheet)
    LCDC->LVCR =
    LCDC_LVCR_V_WIDTH(2)  |   //2 lines period for VSYNC activated
    LCDC_LVCR_V_WAIT_1(1) |    //1 line period between end of OE and beginning of VSYNC
    LCDC_LVCR_V_WAIT_2(1);    //1 line periods between end of VSYNC and beginning of OE
    //Earlier TWR-LCD-RGB versions (B, C, and D)
    #else
    LCDC->LHCR =
    LCDC_LHCR_H_WIDTH(40)   |    //(40+1)=41 SCLK period for HSYNC activated
    LCDC_LHCR_H_WAIT_1(1)  |    //(1+1)=2 SCLK period between end of OE and beginning of HSYNC
    LCDC_LHCR_H_WAIT_2(0);     //(0+3)=3 SCLK periods between end of HSYNC and beginning of OE

    // set LCD vertical configuration based on panel data (Figure 3-3 in Seiko datasheet)
    LCDC->LVCR =
    LCDC_LVCR_V_WIDTH(10)  |   //10 lines period for VSYNC activated
    LCDC_LVCR_V_WAIT_1(2) |    //2 line period between end of OE and beginning of VSYNC
    LCDC_LVCR_V_WAIT_2(2);    //2 line periods between end of VSYNC and beginning of OE
    #endif

    // set the LCD panning offset (not used in 24bpp mode)
    LCDC->LPOR = 0;

    // set LCD interrupt configuration register
    LCDC->LICR = 0;

    //Disable LCDC interrupts
    LCDC->LIER = 0;

    //Disable the graphic window. See the "color" and "fsl" demos for examples of
    //  using the graphic window feature
    LCDC->LGWCR &=~LCDC_LGWCR_GWE_MASK;

    //Set background plane DMA to burst mode
    LCDC->LDCR&=~LCDC_LDCR_BURST_MASK;

    //Set graphic window DMA to burst mode
    LCDC->LGWDCR&=~LCDC_LGWDCR_GWBT_MASK;
}
//----��ʼ��lcdӲ��------------------------------------------------------------
//����: ��ʼ��lcdӲ����
//����: ��
//����: ��
//��ע: ����������ֲ�ؼ�����
//----------------------------------------------------------------------------
void __lcd_hard_init(void)
{
    pg_video_buf = (u8*)FRAME_START_ADDRESS;
    pg_frame_buffer = (u8*)FRAME_START_ADDRESS;

    __lcd_gpio_init();
    __lcd_controller_init();

}
//----LCD�����ź�------------------------------------------------------------
//����: LCD ��Ƶ������߼�ʹ��/��ֹ��1=������Ƶ����� LCD�����źţ�
//      0=��ֹ��Ƶ����� LCD�����źš�
//����: onoff�������ź�
//����: ��
//-----------------------------------------------------------------------------
void __lcd_envid_of(int onoff)
{
    if(onoff == 1)
        SIM->MCR |=SIM_MCR_LCDSTART_MASK;  //Start LCDC
    else
        SIM->MCR &= ~SIM_MCR_LCDSTART_MASK;  // End LCDC
}
//----����LCD��Դ--------------------------------------------------------------
//����: LCD��Դ��������ʹ�ܡ�
//����: invpwren������PWREN�źż���
//      pwren��LCD_PWREN����ź�ʹ��/��ֹ
//����: ��
//-----------------------------------------------------------------------------
void __lcd_power_enable(int invpwren,int pwren)
{

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
    u32 x,y;
    u32 pixel;
    u32 *dst,*p;
    if(Mode != CN_FILLRECT_MODE_N)
        return false;

    if(dst_bitmap->PixelFormat != CN_SYS_PF_ERGB8888)
        return false;

    pixel = GK_ConvertRGB24ToPF(CN_SYS_PF_ERGB8888,Color0);

    dst = (u32*)((ptu32_t)dst_bitmap->bm_bits
                              + Focus->top * dst_bitmap->linebytes);
    dst += Focus->left;

    for(y = Focus->top; y < Focus->bottom; y++)
    {
        p =dst;
        for(x=Focus->left; x<Focus->right;x++)
        {
            *p++ =pixel;
        }
        dst += dst_bitmap->linebytes >> 2;
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
bool_t __lcd_blt_bm_to_bm( struct tagRectBitmap *dst_bitmap,
                            struct tagRectangle *DstRect,
                            struct tagRectBitmap *src_bitmap,
                            struct tagRectangle *SrcRect,
                            u32 RopCode, u32 KeyColor)
{
    u32 *src_offset,*dst_offset;    //Դλͼ���󻺳������ܲ�����!!!
    u32 y;
    if((dst_bitmap->PixelFormat != CN_SYS_PF_ERGB8888)
             ||(src_bitmap->PixelFormat != CN_SYS_PF_ERGB8888)
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
            dst_offset = (u32*)((ptu32_t)dst_bitmap->bm_bits
                                      + DstRect->top * dst_bitmap->linebytes);
            dst_offset += DstRect->left;

            src_offset = (u32*)((ptu32_t)src_bitmap->bm_bits
                                      + SrcRect->top * src_bitmap->linebytes);
            src_offset += SrcRect->left;

            for(y = DstRect->top; y < DstRect->bottom; y++)
            {
                memcpy(dst_offset,src_offset,(DstRect->right-DstRect->left)<<2);
                dst_offset += dst_bitmap->linebytes >> 2;
                src_offset += src_bitmap->linebytes >> 2;
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
    byteoffset = y*LCD_XSIZE + x;
    pen = GK_ConvertRGB24ToPF(CN_SYS_PF_ERGB8888,color);
    dest = (u32)pg_video_buf[byteoffset];
    dest = GK_BlendRop2(dest,pen,rop2_code);
    pg_video_buf[byteoffset] = (u32)dest;
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
//      color����ͼ�õ���ɫ��CN_SYS_PF_ERGB8888��ʽ
//      r2_code����Ԫ��դ������
//����: true=�ɹ����ƣ�false=ʧ�ܣ���Ӳ�����ٻ�֧�ְ�r2_code����
//-----------------------------------------------------------------------------
bool_t __lcd_line_screen_ie(struct tagRectangle *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    return false;
}
//----screen��������-----------------------------------------------------------
//����: ��screen�еľ�����color��ɫ��䣬color�ĸ�ʽ��CN_SYS_PF_ERGB8888��
//����: dst_rect�������ľ���
//      color�������ɫ
//����: true=�ɹ����ƣ�false=ʧ��
//-----------------------------------------------------------------------------
bool_t __lcd_fill_rect_screen(struct tagRectangle *Target,
                              struct tagRectangle *Focus,
                              u32 Color0,u32 Color1,u32 Mode)
{
//  int x,y;
//    u32 pen;
//    u32 *p;
//
//    if(Mode != CN_FILLRECT_MODE_N)
//        return false;
//    pen = GK_ConvertRGB24ToPF(CN_SYS_PF_ERGB8888,Color0);
//
//    for(y=Focus->top;y<Focus->bottom;y++)
//    {
//      p =&pg_video_buf[y*cfg_LcdxSize];
//
//      for(x=Focus->left;x<Focus->right;x++)
//      {
//          *p++ =pen;
//      }
//    }
//    return true;
    return false;
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
    size = (dst_rect->right-dst_rect->left)*(cfg_LcdPf>>8)/8;
    //Դλͼ���Ͻ�������ֽ�ƫ����
    byteoffset_bitmap = ysrc*double_width_src + xsrc*2;
    //Ŀ��������Ͻ������˫�ֽ�ƫ��������Ϊpg_video_bufΪu16��ָ��
    //��Ϊ�Ǵ��͵�screen�ϣ�����ÿ��˫�ֽ���ȡLCD_XSIZE
    byteoffset_rect = dst_rect->top*LCD_XSIZE + dst_rect->left;

    //bitmap��screenλ�鴫��
    for(y = dst_rect->top;y < dst_rect->bottom;y++)
    {
        memcpy(&(pg_video_buf[byteoffset_rect]),
                    &src_bitmap->bm_bits[byteoffset_bitmap],size);
        byteoffset_bitmap += double_width_src;
        byteoffset_rect += LCD_XSIZE;
    }
    return true;
}
//----��screen��ȡ����---------------------------------------------------------
//����: ��screen��ȡһ���أ���ת����CN_SYS_PF_ERGB8888��cn_sys_pf_a8r8g8b8��ʽ��
//����: x��y������
//����: ������ɫֵ
//-----------------------------------------------------------------------------
u32 __lcd_get_pixel_screen(s32 x,s32 y)
{
    u32 color,offset;

    offset = LCDC->LSSAR;
    offset += y*LCD_XSIZE*2;
    offset += x*2;
    color = (u32)(*(u32*)offset);
    //R5G6B5
//    r = (color>>11) &0x1f;
//    g = (color>>5) & 0x3f;
//    b = color & 0x1f;
//    return (r<<19) | (g<<10) | (b<<3);
    //R8G8B8
    return color;
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
struct tagDisplayRsc* ModuleInstall_LCD(s32 width,s32 height,const char *DisplayName,const char* HeapName)
{

    static struct tagGkWinRsc frame_win;
    static struct tagRectBitmap FrameBitmap;

    __lcd_hard_init();
    __lcd_power_enable(0,1);
    __lcd_envid_of(1);

    FrameBitmap.bm_bits = (u8*)pg_frame_buffer;
    FrameBitmap.width = width;
    FrameBitmap.height = height;
    FrameBitmap.PixelFormat = cfg_LcdPf;
    FrameBitmap.linebytes = width*4;
    FrameBitmap.ExColor = 0;
    frame_win.wm_bitmap = &FrameBitmap;
    tg_lcd_display.frame_buffer = &frame_win;

    tg_lcd_display.xmm = 0;
    tg_lcd_display.ymm = 0;
    tg_lcd_display.width = width;
    tg_lcd_display.height = height;
    tg_lcd_display.pixel_format = CN_SYS_PF_ERGB8888;
    tg_lcd_display.reset_clip = false;
    tg_lcd_display.framebuf_direct = true;
    //�����ʼ��frame_buffer��desktop��z_topmost������Ա

    tg_lcd_display.draw.set_pixel_bm = __lcd_set_pixel_bm;
    tg_lcd_display.draw.fill_rect_bm = __lcd_fill_rect_bm;
    tg_lcd_display.draw.line_bm = __lcd_line_bm;
    tg_lcd_display.draw.line_bm_ie = __lcd_line_bm_ie;
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

    tg_lcd_display.DisplayHeap = (struct tagHeapCB *)M_FindHeap(HeapName);
    tg_lcd_display.disp_ctrl = __lcd_disp_ctrl;

    if(GK_InstallDisplay(&tg_lcd_display,(char*)DisplayName))
        return (ptu32_t)&tg_lcd_display;
    else
        return (ptu32_t)NULL;
}

