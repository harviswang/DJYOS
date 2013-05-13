#ifndef __LCDDRV_H_
#define __LCDDRV_H_

#ifdef __cplusplus
extern "C" {
#endif

#define cn_lcd_xsize        320
#define cn_lcd_ysize        240
//#define cn_lcd_line_size    (cn_lcd_xsize*2)


#define cn_frame_buffer_size    (cn_lcd_xsize*cn_lcd_ysize*2)
//hjj
//#define cn_lcd_frame_rate       60
#define cn_lcd_pf       cn_sys_pf_r5g6b5

#define LCDCON4_MVAL        (13)
#define LCDCON1_MMODE       (0)         //0=每帧   1=由MVAL定义此频率
#define LCDCON5_INVVDEN     (1)         //0=正常       1=反转
#define LCDCON5_BSWP        (0)         //字节交换控制位 
#define LCDCON5_HWSWP       (1)         //半字节交换控制位 
#define LCDCON1_PNRMODE     (3)         // 设置为TFT屏
#define LCDCON1_BPPMODE     (12)        // 设置为16bpp模式

#define LCDCON2_VBPD        (12)        //垂直同步信号的后肩
#define LCDCON2_VFPD        (4)         //垂直同步信号的前肩
#define LCDCON2_VSPW        (5)         //垂直同步信号的脉宽

#define LCDCON3_HBPD        (22)        //水平同步信号的后肩
#define LCDCON3_HFPD        (33)        //水平同步信号的前肩
#define LCDCON4_HSPW        (44)        //水平同步信号的脉宽

#define LCDCON1_CLKVAL      (6) 

//hjj
#define HOZVAL_TFT          (cn_lcd_xsize-1)
#define LINEVAL_TFT         (cn_lcd_ysize-1)


// LCD CONTROLLER
struct lcd_reg            //0x4d000000
{
    u32 LCDCON1;            //0x00,LCD控制1寄存器 
    u32 LCDCON2;            //0x04,LCD控制2寄存器 
    u32 LCDCON3;            //0x08,LCD控制3寄存器
    u32 LCDCON4;            //0x0c,LCD控制4寄存器
    u32 LCDCON5;            //0x10,LCD控制5寄存器
    u32 LCDSADDR1;          //0x14,帧缓冲器开始地址1寄存器 
    u32 LCDSADDR2;          //0x18,帧缓冲器开始地址2寄存器 
    u32 LCDSADDR3;          //0x1c,虚拟屏地址设置 
    u32 REDLUT;             //0x20,红色查找表寄存器
    u32 GREENLUT;           //0x24,绿色查找表寄存器
    u32 BLUELUT;            //0x28,蓝色查找表寄存器
    u32 revs[8];
    u32 DITHMODE;           //0x4c,抖动模式寄存器
    u32 TPAL;               //0x50,临时调色板寄存器 
    u32 LCDINTPND;          //0x54,LCD中断挂起寄存器
    u32 LCDSRCPND;          //0x58,LCD源挂起寄存器
    u32 LCDINTMSK;          //0x5c,LCD中断屏蔽寄存器
    u32 TCONSEL;            //0x60,控制 LPC3600/LCC3600模式寄存器 
    
};
#define PALETTE         0x4d000400            //0x400,调色板开始地址

void __lcd_hard_init(void);

void __lcd_envid_of(int );

void __lcd_power_enable(int ,int );
bool_t __lcd_disp_ctrl(struct display_rsc *disp);
bool_t __lcd_format_pattern(u8 *pattern,struct st_rect_bitmap *bitmap);
bool_t __lcd_set_pixel_bm(struct st_rect_bitmap *bitmap,
                     s32 x,s32 y,u32 color,u32 r2_code);
bool_t __lcd_line_bm(struct st_rect_bitmap *bitmap,struct st_rect *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code);
bool_t __lcd_line_bm_ie(struct st_rect_bitmap *bitmap,struct st_rect *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code);
bool_t __lcd_fill_rect_bm(struct st_rect_bitmap *bitmap,
                      struct st_rect *dst_rect,u32 color);
bool_t __lcd_blt_bm_to_bm(struct st_rect_bitmap *dst_bitmap,
                        struct st_rect *dst_rect,
                        struct st_rect_bitmap *src_bitmap,s32 xsrc,s32 ysrc,
                        u8 *pat,struct st_rect_bitmap *bitmsk,u32 rop_code,
                        u32 transparentcolor);
u32 __lcd_get_pixel_bm(struct st_rect_bitmap *bitmap,s32 x,s32 y);
bool_t __lcd_get_rect_bm(struct st_rect_bitmap *src,struct st_rect *rect,
                     struct st_rect_bitmap *dest);
bool_t __lcd_set_pixel_screen(s32 x,s32 y,u32 color,u32 rop2_code);
bool_t __lcd_line_screen(struct st_rect *limit,
                    s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code);
bool_t __lcd_line_screen_ie(struct st_rect *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code);
bool_t __lcd_fill_rect_screen(struct st_rect *dst_rect,u32 color);
bool_t __lcd_bm_to_screen(struct st_rect *dst_rect,
                struct st_rect_bitmap *src_bitmap,s32 xsrc,s32 ysrc);
u32 __lcd_get_pixel_screen(s32 x,s32 y);
bool_t __lcd_get_rect_screen(struct st_rect *rect,struct st_rect_bitmap *dest);
ptu32_t module_init_lcd(ptu32_t para);

#ifdef __cplusplus
}
#endif

#endif   //__LCDDRV_H_
