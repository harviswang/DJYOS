#ifndef __LCDDRV_H_
#define __LCDDRV_H_

#ifdef __cplusplus
extern "C" {
#endif

#define cn_lcd_xsize        320
#define cn_lcd_ysize        240
//#define cn_lcd_line_size    (cn_lcd_xsize*2)


#define cn_frame_buffer_size    (cn_lcd_xsize*cn_lcd_ysize*2)
//hjj,?
//#define cn_lcd_frame_rate       60
#define cn_lcd_pf       cn_sys_pf_r5g6b5

#define LCDCON4_MVAL		(13)
#define LCDCON1_MMODE 	    (0)			//0=each frame   1=rate by MVAL
#define LCDCON5_INVVDEN	(1)			//0=normal       1=inverted
#define LCDCON5_BSWP		(0)			//Byte swap control
#define LCDCON5_HWSWP	    (1)			//Half word swap control
#define LCDCON1_PNRMODE	(3)			// 设置为TFT屏
#define LCDCON1_BPPMODE	(12)		// 设置为16bpp模式

#define LCDCON2_VBPD 		(12)		//垂直同步信号的后肩
#define LCDCON2_VFPD 		(4)			//垂直同步信号的前肩
#define LCDCON2_VSPW 		(5)			//垂直同步信号的脉宽

#define LCDCON3_HBPD 		(22)		//水平同步信号的后肩
#define LCDCON3_HFPD 		(33)		//水平同步信号的前肩
#define LCDCON4_HSPW		(44)		//水平同步信号的脉宽

#define LCDCON1_CLKVAL 	(6) 

//hjj,?
#define HOZVAL_TFT	        (cn_lcd_xsize-1)
#define LINEVAL_TFT	    (cn_lcd_ysize-1)


// LCD CONTROLLER
struct lcd_reg            //0x4d000000
{
    u32 LCDCON1;            //0x00,LCD control 1
    u32 LCDCON2;            //0x04	,LCD control 2
    u32 LCDCON3;            //0x08,LCD control 3
    u32 LCDCON4;            //0x0c,LCD control 4
    u32 LCDCON5;            //0x10,LCD control 5
    u32 LCDSADDR1;          //0x14,STN/TFT Frame buffer start address 1
    u32 LCDSADDR2;          //0x18,STN/TFT Frame buffer start address 2
    u32 LCDSADDR3;          //0x1c,STN/TFT Virtual screen address set
    u32 REDLUT;             //0x20,STN Red lookup table
    u32 GREENLUT;           //0x24,STN Green lookup table 
    u32 BLUELUT;            //0x28,STN Blue lookup table
    u32 revs[8];
    u32 DITHMODE;           //0x4c,STN Dithering mode
    u32 TPAL;               //0x50,TFT Temporary palette
    u32 LCDINTPND;          //0x54,LCD Interrupt pending
    u32 LCDSRCPND;          //0x58,LCD Interrupt source
    u32 LCDINTMSK;          //0x5c	,LCD Interrupt mask
    u32 TCONSEL;            //0x60,LPC3600 Control --- edited by junon
    
};
#define PALETTE         0x4d000400            //0x400,Palette start address


void __lcd_envid_of(int );

void __lcd_power_enable(int ,int );

void __lcd_hard_init(void);


ptu32_t module_init_lcd(ptu32_t para);

#ifdef __cplusplus
}
#endif

#endif   //__LCDDRV_H_
