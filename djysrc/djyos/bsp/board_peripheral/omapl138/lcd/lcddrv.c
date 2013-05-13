#include "os_inc.h"
#include "gk_inc.h"
#include "lcddrv.h"
#include "gpio_api.h"
#include "cpu_peri.h"

//坐标系说明,描述了显存偏移地址与屏上像素点的坐标映射关系,注意单色和灰度显示器
//每字节可能包含不止一个像素点,图中没有加以区分.
//不同的系统有不同的对应关系,这种变化影响 部分显示函数的编写方法.
//应用程序以及gui驱动程序无需关心这种映射关系.
//例外的是,如果你用数组与屏幕直接映射的方法显示bmp图形,需要了解这种映射关系.
//----------------------------------------------------->x   右上角
// |0x0000,0x0001,0x0002--------------------------xmax-1
// |xmax,xmax+1,xmax+2---------------------------2*max-1
// |----------------------------------------------------
// |----------------------------------------------------
// |----------------------------------------------------
// |(ymax-1)*xmax,(ymax-1)*xmax+1------------ymax*xmax-1    右下角
//\/
//y

bool_t __lcd_set_pixel_bm(struct st_rect_bitmap *bitmap,
                         s32 x,s32 y,u32 color,u32 r2_code);
bool_t __lcd_line_bm(struct st_rect_bitmap *bitmap,struct st_rect *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code);
bool_t __lcd_line_bm_ie(struct st_rect_bitmap *bitmap,struct st_rect *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code);
bool_t __lcd_fill_rect_bm(struct st_rect_bitmap *dst_bitmap,
                         struct st_rect *dst_rect,u32 color);
bool_t __lcd_blt_bm_to_bm( struct st_rect_bitmap *dst_bitmap,
                            struct st_rect *dst_rect,
                            struct st_rect_bitmap *src_bitmap,s32 xsrc,s32 ysrc,
                            u8 *pat,struct st_rect_bitmap *mask,u32 rop_code,
                            u32 transparentcolor);
bool_t __lcd_get_rect_bm(struct st_rect_bitmap *src,struct st_rect *rect,
                        struct st_rect_bitmap *dest);
bool_t __lcd_set_pixel_screen(s32 x,s32 y,u32 color,u32 r2_code);
bool_t __lcd_line_screen(struct st_rect *limit,
                    s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code);
bool_t __lcd_line_screen_ie(struct st_rect *limit,
                       s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code);
bool_t __lcd_fill_rect_screen(struct st_rect *dst_rect,u32 color);
bool_t __lcd_bm_to_screen(struct st_rect *dst_rect,
            struct st_rect_bitmap *src_bitmap,s32 xsrc,s32 ysrc);
u32 __lcd_get_pixel_screen(s32 x,s32 y);
bool_t __lcd_get_rect_screen(struct st_rect *rect,struct st_rect_bitmap *dest);
bool_t __lcd_disp_ctrl(struct display_rsc *disp);
bool_t __lcd_format_pattern(u8 *pattern,struct st_rect_bitmap *bitmap);


struct display_rsc tg_lcd_display;

//size = [CN_PALETTE_SIZE + cn_frame_buffer_size]
u8 *u8g_dsp_buffer = (u8*)(CN_FRAME_BASE_ADDRESS +CN_PALETTE_SIZE);
u8 u8g_frame_buffer[cn_frame_buffer_size];
static u16 s_u16Contrast = 75;
#define M_GuiSwap(a, b){a^=b; b^=a; a^=b;}

void __lcd_hard_init(void)
{
    u32 x,y,reg;
    u16 *pdata;
    // enable power and setup lcdc.
    Cpucfg_EnableLPSC(cn_PSC1_LCDC);
    
        /* Palette */
    pdata = (u16*)(u8g_dsp_buffer - CN_PALETTE_SIZE);
#if CN_LCD_TYPE == CN_MODE_GREY_MONO
    *pdata++ = 0;
    *pdata++ = 15;
#elif CN_LCD_TYPE == CN_MODE_GREY_04
    *pdata++ = 0;
    *pdata++ = 5;
    *pdata++ = 10;
    *pdata++ = 15;
#elif CN_LCD_TYPE == CN_MODE_GREY_16
    *pdata++ = 0;
    *pdata++ = 1;
    *pdata++ = 2;
    *pdata++ = 3;
    *pdata++ = 4;
    *pdata++ = 5;
    *pdata++ = 6;
    *pdata++ = 7;
    *pdata++ = 8;
    *pdata++ = 9;
    *pdata++ = 10;
    *pdata++ = 11;
    *pdata++ = 12;
    *pdata++ = 13;
    *pdata++ = 14;
    *pdata++ = 15;
#endif

#if ((CN_LCD_TYPE == CN_MODE_GREY_MONO) || (CN_LCD_TYPE == CN_MODE_GREY_04) || (CN_LCD_TYPE == CN_MODE_GREY_16))

    //以下分配LCD控制器所需的引脚
    gpio_cfg_pin_func(7,8,cn_p7_8_lcd_d0);
    gpio_cfg_pin_func(7,9,cn_p7_9_lcd_d1);
    gpio_cfg_pin_func(7,10,cn_p7_10_lcd_d2);
    gpio_cfg_pin_func(7,11,cn_p7_11_lcd_d3);
    gpio_cfg_pin_func(8,10,cn_p8_10_lcd_mclk);
    gpio_cfg_pin_func(8,11,cn_p8_11_lcd_pclk);
    gpio_cfg_pin_func(6,0,cn_p6_0_nlcd_ac_enb_cs);
    gpio_cfg_pin_func(8,8,cn_p8_8_lcd_vsync);
    gpio_cfg_pin_func(8,9,cn_p8_9_lcd_hsync);
    
    g_ptSysconfig0Reg->MSTPRI[2] &= 0x0fffffff;      //设置LCD数据传送次高优先级
    g_ptSysconfig0Reg->MSTPRI[2] |= 0x10000000;      //设置LCD数据传送次高优先级
    // Turn raster controller off
    g_ptLcdcReg->RASTER_CTRL       &= 0xfffffffe;         
    
    // Clear status bits
    g_ptLcdcReg->LCD_STAT           = 0x000003ff;
    
    // PCLK = 1.27MHz, 帧频66，raster mode
    reg = CN_LCD_CLKVAL /(CN_LCD_SIZE_X * CN_LCD_SIZE_Y * 66) * 4;
    g_ptLcdcReg->LCD_CTRL &= ~CN_LCDC_LCD_CTRL_CLKDIV_MASK;
    g_ptLcdcReg->LCD_CTRL |= reg << CN_LCDC_LCD_CTRL_CLKDIV_SHIFT;
    g_ptLcdcReg->LCD_CTRL |= CN_LCDC_LCD_CTRL_MODESEL_MASK;
    
    g_ptLcdcReg->RASTER_CTRL =
                       (CN_LCDC_RASTER_CTRL_NIB_MODE_ENABLE << CN_LCDC_RASTER_CTRL_NIB_MODE_SHIFT)
                       +(CN_LCDC_RASTER_CTRL_MONO_COLOR_MONOCHROME << CN_LCDC_RASTER_CTRL_MONO_COLOR_SHIFT);

    
    // HBP = 0; HFP = 0; HSW = 0x3; PPL = 0x13
    g_ptLcdcReg->RASTER_TIMING_0 =
                        (4 << CN_LCDC_RASTER_TIMING_0_HBP_SHIFT)
                       +(4 << CN_LCDC_RASTER_TIMING_0_HFP_SHIFT)
                       +(4 << CN_LCDC_RASTER_TIMING_0_HSW_SHIFT)
                       +((CN_LCD_SIZE_X/16 -1)<< CN_LCDC_RASTER_TIMING_0_PPL_SHIFT);
    // VBP = 0x3; VFP = 0x2; VSW = 0xA: LPP = 0xef; 
    g_ptLcdcReg->RASTER_TIMING_1    = 
                         (10 << CN_LCDC_RASTER_TIMING_1_VBP_SHIFT)
                        +(9  << CN_LCDC_RASTER_TIMING_1_VFP_SHIFT)
                        +(5  << CN_LCDC_RASTER_TIMING_1_VSW_SHIFT)
                        +((CN_LCD_SIZE_Y -1)<< CN_LCDC_RASTER_TIMING_1_LPP_SHIFT);
    g_ptLcdcReg->RASTER_TIMING_2 =
                         (CN_LCDC_RASTER_TIMING_2_IPC_RISING << CN_LCDC_RASTER_TIMING_2_IPC_SHIFT);

    g_ptLcdcReg->LCDDMA_CTRL        = 0x00000540;
    // Frame buffer start
    g_ptLcdcReg->LCDDMA_FB0_BASE    = (u32)(u8g_dsp_buffer - CN_PALETTE_SIZE);
    // Frame buffer end,不知为何要减1
    g_ptLcdcReg->LCDDMA_FB0_CEILING = (u32)(u8g_dsp_buffer + cn_frame_buffer_size - 1);
    g_ptLcdcReg->RASTER_CTRL       |= 0x00000001;         // Enable controller
#endif

    return;
}

void Lcd_ClearScreen(void)
{
    memset(u8g_dsp_buffer,0,cn_frame_buffer_size);
}
void lcd_back_light_on(void)
{
}

void lcd_back_light_off(void)
{
}
void Lcd_Display_On(void)
{
    Cpucfg_EnableLPSC(cn_PSC1_LCDC);
}

void Lcd_Display_Off(void)
{
    Cpucfg_DisableLPSC(cn_PSC1_LCDC);
}

//========================================================
void Lcd_ContrastSet(u16 u16Contrast)
{
}
// 函数功能：控制液晶对比度增加
// 输入参数：无
// 输出参数：无
// 返回值  ：无
//========================================================
void Lcd_ContrastUp(void)
{
    s_u16Contrast++;
}

//========================================================
// 函数功能：控制液晶对比度下降
// 输入参数：无
// 输出参数：无
// 返回值  ：无
//========================================================
void Lcd_ContrastDown(void)
{
    s_u16Contrast--;
}

bool_t __lcd_set_pixel_bm(struct st_rect_bitmap *bitmap,
                         s32 x,s32 y,u32 color,u32 r2_code)
{
    return false;
}
bool_t __lcd_line_bm(struct st_rect_bitmap *bitmap,struct st_rect *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    return false;
}
bool_t __lcd_line_bm_ie(struct st_rect_bitmap *bitmap,struct st_rect *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    return false;
}
bool_t __lcd_fill_rect_bm(struct st_rect_bitmap *dst_bitmap,
                         struct st_rect *dst_rect,u32 color)
{
    return false;
}
bool_t __lcd_blt_bm_to_bm( struct st_rect_bitmap *dst_bitmap,
                            struct st_rect *dst_rect,
                            struct st_rect_bitmap *src_bitmap,s32 xsrc,s32 ysrc,
                            u8 *pat,struct st_rect_bitmap *mask,u32 rop_code,
                            u32 transparentcolor)
{
    return false;
}
bool_t __lcd_get_rect_bm(struct st_rect_bitmap *src,struct st_rect *rect,
                        struct st_rect_bitmap *dest)
{
    return false;
}

bool_t __lcd_set_pixel_screen(s32 x,s32 y,u32 color,u32 r2_code)
{
    u32 byteoffset,bitoffset;
    u8 dest,pixel;
    u8g_dsp_buffer = (u8*)(CN_FRAME_BASE_ADDRESS +CN_PALETTE_SIZE);     //db
    byteoffset = y*cn_lcd_line_size + x/8;
    bitoffset =(7- x%8);              //每像素1bit，字节高bit显示在左边
#if 0
    switch(x%8)                         //液晶模块数据线反了。
    {
        case 0:bitoffset = 4;break;
        case 1:bitoffset = 5;break;
        case 2:bitoffset = 6;break;
        case 3:bitoffset = 7;break;
        case 4:bitoffset = 0;break;
        case 5:bitoffset = 1;break;
        case 6:bitoffset = 2;break;
        case 7:bitoffset = 3;break;
    }
#endif
    pixel = (u8)__gk_convert_color_to_pf(cn_ext_pf_gray1,color);
    if(cn_r2_copypen == r2_code)
    {
        u8g_dsp_buffer[byteoffset] = (pixel<<bitoffset)
                      + (u8g_dsp_buffer[byteoffset] & (~(0x01 << bitoffset)));
    }else
    {
        dest = u8g_dsp_buffer[byteoffset] << bitoffset;
        pixel = __gk_blend_rop2(dest, pixel<<bitoffset, r2_code);
        u8g_dsp_buffer[byteoffset] = ((pixel & 0x1)<<bitoffset)
                      + (u8g_dsp_buffer[byteoffset] & (~(0x01 << bitoffset)));
    }
    return true;
}
//在screen中画一条任意直线，不含端点，如硬件加速不支持在screen上画线，或者有
//frame_buffer，driver可以简化，直接返回false即可
//镜像显示器的driver须提供这个函数
bool_t __lcd_line_screen(struct st_rect *limit,
                    s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    return false;
}

//在screen中画一条任意直线，含端点，如硬件加速不支持在screen上画线，或者有
//frame_buffer，driver可以简化，直接返回false即可
//镜像显示器的driver须提供这个函数
bool_t __lcd_line_screen_ie(struct st_rect *limit,
                       s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    return false;
}
//screen中矩形填充，如硬件加速不支持在screen上矩形填充，或者有frame_buffer，
//driver可以简化，直接返回false即可
//镜像显示器的driver须提供这个??
bool_t __lcd_fill_rect_screen(struct st_rect *dst_rect,u32 color)
{
    return false;
}
//从内存缓冲区到screen位块传送，只支持块拷贝，不支持rop操作。
//镜像显示器的driver须提供这个函数
bool_t __lcd_bm_to_screen(struct st_rect *dst_rect,
            struct st_rect_bitmap *src_bitmap,s32 xsrc,s32 ysrc)
{
    return false;
}

//从screen中取一像素，并转换成cn_sys_pf_e8r8g8b8
u32 __lcd_get_pixel_screen(s32 x,s32 y)
{
    u32 byteoffset,bitoffset;
    byteoffset = y*cn_lcd_line_size + x/8;
    //    bitoffset =(7- x%8);              //每像素1bit，字节高bit显示在左边
    switch(x%8)                         //液晶模块数据线反了。
    {
        case 0:bitoffset = 4;break;
        case 1:bitoffset = 5;break;
        case 2:bitoffset = 6;break;
        case 3:bitoffset = 7;break;
        case 4:bitoffset = 0;break;
        case 5:bitoffset = 1;break;
        case 6:bitoffset = 2;break;
        case 7:bitoffset = 3;break;
    }
    return __gk_convert_color_to_24(cn_ext_pf_gray1,
                (u8g_dsp_buffer[byteoffset]>>bitoffset)&0x1);
}
//把screen内矩形区域的内容复制到bitmap，调用前，先设置好dest的pf_type
bool_t __lcd_get_rect_screen(struct st_rect *rect,struct st_rect_bitmap *dest)
{
    return false;
}

//----控制显示器---------------------------------------------------------------
//功能: 这是由driver提供商编写的，用于设置显示器各种参数的函数，属于应用程序的
//      范畴，功能由设备厂商定义。在ut2416中，本函数为空。
//参数: disp，显示器指针
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t __lcd_disp_ctrl(struct display_rsc *disp)
{
    return true;
}

//----格式化pattern------------------------------------------------------------
//功能: 把用户提交的作为pattern的bitmap格式化成driver能识别的格式
//参数: pattern，保存结果的指针
//      bitmap，用户提供的位图
//返回: true=成功,false=失败，若显示器根本不支持pattern则会失败
//-----------------------------------------------------------------------------
bool_t __lcd_format_pattern(u8 *pattern,struct st_rect_bitmap *bitmap)
{
    return true;
}

//----初始化lcd设备------------------------------------------------------------
//功能: 如名
//参数: 无
//返回: 显示器资源指针
//-----------------------------------------------------------------------------
ptu32_t module_init_lcd(ptu32_t para)
{
    static struct gkwin_rsc frame_win;
    
    __lcd_hard_init( );
    frame_win.wm_bitmap.bm_bits = u8g_dsp_buffer;
//    frame_win.wm_bitmap.bm_bits = u8g_frame_buffer;
    frame_win.wm_bitmap.width = cn_lcd_xsize;
    frame_win.wm_bitmap.height = cn_lcd_ysize;
    frame_win.wm_bitmap.pf_type = cn_lcd_pf;
    frame_win.wm_bitmap.linebytes = cn_lcd_line_size;
    tg_lcd_display.frame_buffer = &frame_win;

    tg_lcd_display.xmm = 0;
    tg_lcd_display.ymm = 0;
    tg_lcd_display.width = cn_lcd_xsize;
    tg_lcd_display.height = cn_lcd_ysize;
    tg_lcd_display.patx = 8;
    tg_lcd_display.paty = 8;
    tg_lcd_display.pixel_format = cn_ext_pf_gray1;
    tg_lcd_display.framebuf_direct = true;
    //无须初始化frame_buffer和desktop，z_topmost三个成员

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
    tg_lcd_display.vmalloc = NULL;
    tg_lcd_display.vrmalloc = NULL;
    tg_lcd_display.vfree = NULL;
    tg_lcd_display.disp_ctrl = __lcd_disp_ctrl;
    tg_lcd_display.format_pattern = __lcd_format_pattern;

    gk_install_display(&tg_lcd_display,(char*)para);
    return (ptu32_t)&tg_lcd_display;
}


