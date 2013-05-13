#ifndef __LCDDRV_H_
#define __LCDDRV_H_

#ifdef __cplusplus
extern "C" {
#endif

#define CN_LCD_XSIZE            ((u32)320)
#define CN_LCD_YSIZE            ((u32)240)
#define CN_LCD_LINE_SIZE        ((CN_LCD_XSIZE)/8)
#define CN_FRAME_BUFFER_SIZE    (CN_LCD_LINE_SIZE*CN_LCD_YSIZE)
#define CN_LCD_PF               cn_ext_pf_gray1     //单色
#define CN_FRAME_BASE_ADDRESS   0xc1f00000
#define CN_PALETTE_SIZE         32

//以下是djyos风格的一些定义，这样可以直接使用djyos提供的初始化文件。
#define cn_lcd_xsize            CN_LCD_XSIZE
#define cn_lcd_ysize            CN_LCD_YSIZE
#define cn_lcd_line_size        CN_LCD_LINE_SIZE
#define cn_frame_buffer_size    CN_FRAME_BUFFER_SIZE
#define cn_lcd_pf               CN_LCD_PF

void Lcd_BL_On(void);
void Lcd_BL_Off(void);
void Lcd_Display_On(void);
void Lcd_Display_Off(void);

ptu32_t module_init_lcd(ptu32_t para);

void Lcd_ContrastSet(u16 u16Contrast);
#ifdef __cplusplus
}
#endif

#endif //__LCDDRV_H_
