//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//版权所有 (C), 2004-2009,   lst.
//所属模块:图形
//作者：lst
//版本：V1.0.0
//文件描述: lil9325驱动程序
//其他说明:
//修订历史:
//1. 日期: 2009-03-10
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#if (cfg_gui_kernel == 1)
#ifndef __LCD_ILI9325_H
#define __LCD_ILI9325_H

#define LCD_ON_GPIOF10        	GPIO_Pin_10
#define LCD_RST_GPIOG8        	GPIO_Pin_8
#define LCD_CS_GPIOG10        	GPIO_Pin_10

//#define LCD_DATA       	(u16*)(u32)(0x68000000)
//#define LCD_CMD       		(u16*)(u32)(0x68000002)	//16位数据宽度A0=1
#define LCD_DATA    (*(volatile u16*)  0x68000000)
#define LCD_CMD     (*(volatile u16*)  0x68000002)

#define cn_lcd_xsize            240
#define cn_lcd_ysize            320
#define cn_lcd_line_size        (cn_lcd_xsize<<1)
#define cn_frame_buffer_size    (cn_lcd_line_size * cn_lcd_ysize)
#define cn_lcd_pf               cn_sys_pf_r5g6b5

ptu32_t module_init_lcd(ptu32_t para);
void lcd_display_on(void);
void lcd_display_off(void);


/*-------------------------------------------------------------------------*/
#endif//__LCD_ILI9325_H 防止重复包含

#endif //   for #if (cfg_gui_kernel == 1)

