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
#include "os_inc.h"

#if (cfg_gui_kernel == 1)

#include "gk_inc.h"
#include "stm32f10x.h"
#include "LCD_9325.h"

#define cn_coordinates      1       //0=正常坐标，1=翻转坐标，即原点=(239,319)
u8 u8g_frame_buffer[cn_frame_buffer_size];
struct display_rsc tg_lcd_display;

//以下是ili9325的操作函数

/*---------------------------------------------------------------------------
功能：	读lcd内部0x00寄存器
---------------------------------------------------------------------------*/
u32 __ili9325_read_reg(void)
{
    LCD_DATA=0x0000;
	return LCD_CMD;
}
/*---------------------------------------------------------------------------
功能：	写lcd内部寄存器
---------------------------------------------------------------------------*/
void __ili9325_write_reg(u32 reg_index, u32 data)
{
    LCD_DATA=reg_index;
    LCD_CMD=data;
}

/*---------------------------------------------------------------------------
功能：	写命令
---------------------------------------------------------------------------*/
void __ili9325_write_cmd(u32 cmd)
{
    LCD_DATA = cmd;
}

/*---------------------------------------------------------------------------
功能：	写数据
---------------------------------------------------------------------------*/
#define __ili9325_write_data(data) (LCD_CMD=data)

/*---------------------------------------------------------------------------
功能：	读数据
---------------------------------------------------------------------------*/
u32 __ili9325_read_data(void)
{
	return LCD_CMD;
}
/*---------------------------------------------------------------------------
功能：	设置一个像素
---------------------------------------------------------------------------*/
void __ili9325_set_pixel(u32 x, u32 y, u16 color)
{
	__ili9325_write_reg(0x0021, y);        //v垂直方向
	__ili9325_write_reg(0x0020, x);    //h水平方向

	__ili9325_write_cmd(0x0022);
	__ili9325_write_data(color);
}

/*---------------------------------------------------------------------------
功能：	读取一个像素	    
---------------------------------------------------------------------------*/
u16 __ili9325_get_pixel(u32 x, u32 y)
{
	u16 col = 0;//, rgb = 0
     
	__ili9325_write_reg(0x0021, y);        //v垂直方向
	__ili9325_write_reg(0x0020, x);    //h水平方向

	__ili9325_write_cmd(0x0022);
	__ili9325_read_data();//dummy read

    col	= __ili9325_read_data();//BGR 排列
//  rgb = (col & 0x001f)<<11;
//	rgb |= (col & 0xf800)>>11;
//	rgb |= col & 0x07e0;
	return col;
}

/*---------------------------------------------------------------------------
功能：	设置光标位置
    	row     :0--319
    	column	:0--239
---------------------------------------------------------------------------*/ 
void __ili9325_set_cursor(u32 x, u32 y)
{
	__ili9325_write_reg(0x0021, y);        //v垂直方向
	__ili9325_write_reg(0x0020, x);    //h水平方向	            
}

/*---------------------------------------------------------------------------
功能：	开窗
---------------------------------------------------------------------------*/ 
void __ili9325_set_window(u32 x,u32 y, u32 wide,u32 high)
{
	__ili9325_write_reg(0x0050, x);//水平起始地址

	if (x+wide <= 240)

	__ili9325_write_reg(0x0051, x+wide-1);//水平结束地址
	else
	__ili9325_write_reg(0x0051, 239);

	__ili9325_write_reg(0x0052, y);//垂直起始地址
	if (y+high <= 320)
	__ili9325_write_reg(0x0053, y+high-1);//垂直结束地址	
	else
	__ili9325_write_reg(0x0053, 319);

	__ili9325_set_cursor(x, y);//光标设在左上角	        
	
}

/*---------------------------------------------------------------------------
功能：	关窗
---------------------------------------------------------------------------*/ 
void __ili9325_close_window(void)
{
	__ili9325_write_reg(0x0050, 0);//水平起始地址
	__ili9325_write_reg(0x0051, 239);//水平结束地址

	__ili9325_write_reg(0x0052, 0);//垂直起始地址
	__ili9325_write_reg(0x0053, 319);//垂直结束地址
    
	__ili9325_set_cursor(0, 0);//光标设在左上角	            
}
void lcd_clear(u32 color)
{
    u32 i = 0;

	__ili9325_close_window();

	__ili9325_write_cmd(0x0022);									//写GRAM之前 要先写0x0022
	for (i = 0; i < 320*240 ; i++)
	{
	    __ili9325_write_data(color);	
	}
		
}

void lcd_reset(void)
{
	GPIO_ResetBits(GPIOG, LCD_RST_GPIOG8);
	djy_event_delay(5*mS);
	GPIO_SetBits(GPIOG, LCD_RST_GPIOG8);
}

void lcd_backlight_on(void)
{
	GPIO_ResetBits(GPIOF, LCD_ON_GPIOF10);
}

void lcd_backlight_off(void)
{
	GPIO_SetBits(GPIOF, LCD_ON_GPIOF10);
}

void LCD_FSMCConfig(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
      FSMC_NORSRAMTimingInitTypeDef  p;
    /*-- FSMC Configuration ------------------------------------------------------*/
    /*----------------------- SRAM Bank 3 ----------------------------------------*/
      /* FSMC_Bank1_NORSRAM3 configuration */
      p.FSMC_AddressSetupTime = 1;
      p.FSMC_AddressHoldTime = 0;
      p.FSMC_DataSetupTime = 2;
      p.FSMC_BusTurnAroundDuration = 0;
      p.FSMC_CLKDivision = 0;
      p.FSMC_DataLatency = 0;
      p.FSMC_AccessMode = FSMC_AccessMode_A;
      /* Color LCD configuration ------------------------------------
         LCD configured as follow:
            - Data/Address MUX = Disable
            - Memory Type = SRAM
            - Data Width = 16bit
            - Write Operation = Enable
            - Extended Mode = Enable
            - Asynchronous Wait = Disable */
      FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM3;
      FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
      FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
      FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
      FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
      FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
      FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
      FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
      FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
      FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
      FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
      FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
      FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
      FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
      FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;
      FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
      /* BANK 3 (of NOR/SRAM Bank) is enabled */
      FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3, ENABLE);
}

void LCD_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin = LCD_ON_GPIOF10;
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = LCD_RST_GPIOG8;
    GPIO_Init(GPIOG, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = LCD_CS_GPIOG10;
    GPIO_Init(GPIOG, &GPIO_InitStructure);

    lcd_backlight_off();
    LCD_FSMCConfig();
}



//以下是djygui要求的接口函数，许多情况下，大多数用空函数即可
/*---------------------------------------------------------------------------
功能：    lcd 初始化
---------------------------------------------------------------------------*/
void __lcd_ili9325_init(void)
{
	LCD_GPIO_Init();
    lcd_reset();

    djy_event_delay(100*mS);

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

    djy_event_delay(5*mS);                                                                   // Delay 200 MS , Dis-charge capacitor power voltage

    __ili9325_write_reg(0x0010, 0x1690);                                                            // SAP, BT[3:0], AP, DSTB, SLP, STB
    __ili9325_write_reg(0x0011, 0x0227);                                                            // R11H=0x0221 at VCI=3.3V, DC1[2:0], DC0[2:0], VC[2:0]

    djy_event_delay(5*mS);                                                                    // Delay 50ms

    __ili9325_write_reg(0x0012, 0x001d);                                                            // External reference voltage= Vci;001d

    djy_event_delay(5*mS);                                                                    // Delay 50ms

    __ili9325_write_reg(0x0013, 0x0800);                                                            // R13H=1D00 when R12H=009D;VDV[4:0] for VCOM amplitude
    __ili9325_write_reg(0x0029, 0x0014);                                                            // R29H=0013 when R12H=009D;VCM[5:0] for VCOMH
    __ili9325_write_reg(0x002b, 0x000B);                                                            // Frame Rate = 96Hz

    djy_event_delay(5*mS);                                                                      // Delay 50ms

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
    __ili9325_write_reg(0x0061, 0x0001);            // NDL,VLE（滚屏允许）, REV（灰度翻转）
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
功能：    display on
---------------------------------------------------------------------------*/ 
void lcd_display_on(void)
{
    __ili9325_write_reg(0x0007, 0x0173);            
}

/*---------------------------------------------------------------------------
功能：    display off
---------------------------------------------------------------------------*/ 
void lcd_display_off(void)
{
    __ili9325_write_reg(0x0007, 0x0000);        
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
    u32 x,y;
    u16 pixel;
    u16 *dst_offset;
    if(dst_bitmap->pf_type != cn_sys_pf_r5g6b5)
        return false;
    pixel = __gk_convert_color_to_pf(cn_sys_pf_r5g6b5,color);
    
    dst_offset = (u16*)((ptu32_t)dst_bitmap->bm_bits 
                              + dst_rect->top * dst_bitmap->linebytes);
    dst_offset += dst_rect->left;
    
    for(y = dst_rect->top; y < dst_rect->bottom; y++)
    {
        for(x = 0; x < dst_rect->right-dst_rect->left; x++)
        {
            dst_offset[x] = pixel;
        }
        dst_offset += dst_bitmap->linebytes >> 1;
    }
    return true;
}

//本函数用于在最通常的情况下，加速图形绘制。
//绝大多数位图传输操作，源和目标位图格式都是cn_sys_pf_r5g6b5，
//且rop=cn_r3_srccopy
bool_t __lcd_blt_bm_to_bm( struct st_rect_bitmap *dst_bitmap,
                            struct st_rect *dst_rect,
                            struct st_rect_bitmap *src_bitmap,s32 xsrc,s32 ysrc,
                            u8 *pat,struct st_rect_bitmap *mask,u32 rop_code,
                            u32 transparentcolor)
{
    u16 *src_offset,*dst_offset;    //源位图点阵缓冲区可能不对齐!!!
    u32 x,y;
    if((dst_bitmap->pf_type != cn_sys_pf_r5g6b5) 
             ||(src_bitmap->pf_type != cn_sys_pf_r5g6b5)
             ||(rop_code != cn_r3_srccopy))
    {
        return false;
    }
    else
    {
        if((ptu32_t)src_bitmap->bm_bits & 1)    //源位图缓冲区非对齐模型
        {
        }else
        {
            dst_offset = (u16*)((ptu32_t)dst_bitmap->bm_bits 
                                      + dst_rect->top * dst_bitmap->linebytes);
            dst_offset += dst_rect->left;
            
            src_offset = (u16*)((ptu32_t)src_bitmap->bm_bits 
                                      + ysrc * src_bitmap->linebytes);
            src_offset += xsrc;
            
            for(y = dst_rect->top; y < dst_rect->bottom; y++)
            {
                for(x = 0; x < dst_rect->right-dst_rect->left; x++)
                {
                    dst_offset[x] = src_offset[x];
                }
                dst_offset += dst_bitmap->linebytes >> 1;
                src_offset += src_bitmap->linebytes >> 1;
            }
        }
    }
    return true;
}
bool_t __lcd_get_rect_bm(struct st_rect_bitmap *src,struct st_rect *rect,
                        struct st_rect_bitmap *dest)
{
    return false;
}
#if (cn_coordinates == 0)   //正常坐标
bool_t __lcd_set_pixel_screen(s32 x,s32 y,u32 color,u32 r2_code)
{
    u16 dest,pixel;
    pixel = __gk_convert_color_to_pf(cn_sys_pf_r5g6b5,color);
    if(cn_r2_copypen == r2_code)
    {
        __ili9325_set_pixel(x,y,pixel);
    }else
    {
        dest = __ili9325_get_pixel(x,y);
        pixel = __gk_blend_rop2(dest, pixel, r2_code);
        __ili9325_set_pixel(x,y,pixel);
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
    u32 x,y,width,height;
    u16 pixel;
    pixel = __gk_convert_color_to_pf(cn_sys_pf_r5g6b5,color);
    width = dst_rect->right-dst_rect->left;
    height = dst_rect->bottom-dst_rect->top;
    __ili9325_set_window(dst_rect->left,dst_rect->top,width,height);
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

//从内存缓冲区到screen位块传送，只支持块拷贝，不支持rop操作。
//镜像显示器的driver须提供这个函数
bool_t __lcd_bm_to_screen(struct st_rect *dst_rect,
            struct st_rect_bitmap *src_bitmap,s32 xsrc,s32 ysrc)
{
    s32 x,y,width,height;
    u16 *lineoffset;
    if(src_bitmap->pf_type != cn_sys_pf_r5g6b5)
        return false;
    width = dst_rect->right-dst_rect->left;
    height = dst_rect->bottom-dst_rect->top;
    //正常坐标，
    lineoffset = (u16*)((u32)src_bitmap->bm_bits + ysrc*src_bitmap->linebytes);
    lineoffset +=xsrc;
    //正常坐标，
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

//从screen中取一像素，并转换成cn_sys_pf_e8r8g8b8
u32 __lcd_get_pixel_screen(s32 x,s32 y)
{
    return __gk_convert_color_to_24(cn_sys_pf_r5g6b5, 
                        __ili9325_get_pixel(x,y));
}
#else
bool_t __lcd_set_pixel_screen(s32 x,s32 y,u32 color,u32 r2_code)
{
    u16 dest,pixel;
    pixel = __gk_convert_color_to_pf(cn_sys_pf_r5g6b5,color);
    if(cn_r2_copypen == r2_code)
    {
        __ili9325_set_pixel(cn_lcd_xsize-x,cn_lcd_ysize-y,pixel);   //转换坐标
    }else
    {
        dest = __ili9325_get_pixel(cn_lcd_xsize-1-x,cn_lcd_ysize-1-y);  //转换坐标
        pixel = __gk_blend_rop2(dest, pixel, r2_code);
        __ili9325_set_pixel(cn_lcd_xsize-1-x,cn_lcd_ysize-1-y,pixel);   //转换坐标
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
    u32 x,y,width,height;
    u16 pixel;
    pixel = __gk_convert_color_to_pf(cn_sys_pf_r5g6b5,color);
    width = dst_rect->right-dst_rect->left;
    height = dst_rect->bottom-dst_rect->top;
    //转换坐标
    __ili9325_set_window(cn_lcd_xsize-dst_rect->right,
                         cn_lcd_ysize-dst_rect->bottom,width,height);
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

//从内存缓冲区到screen位块传送，只支持块拷贝，不支持rop操作。
//镜像显示器的driver须提供这个函数
bool_t __lcd_bm_to_screen(struct st_rect *dst_rect,
            struct st_rect_bitmap *src_bitmap,s32 xsrc,s32 ysrc)
{
    s32 x,y,width,height;
    u16 *lineoffset;
    if(src_bitmap->pf_type != cn_sys_pf_r5g6b5)
        return false;
    width = dst_rect->right-dst_rect->left;
    height = dst_rect->bottom-dst_rect->top;
    //转换坐标
    lineoffset = (u16*)((u32)src_bitmap->bm_bits 
                        + (ysrc+height-1)*src_bitmap->linebytes);
    lineoffset +=xsrc;
    //转换坐标
    __ili9325_set_window(cn_lcd_xsize-dst_rect->right,
                         cn_lcd_ysize-dst_rect->bottom,width,height);
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

//从screen中取一像素，并转换成cn_sys_pf_e8r8g8b8
u32 __lcd_get_pixel_screen(s32 x,s32 y)
{
    return __gk_convert_color_to_24(cn_sys_pf_r5g6b5, 
                        __ili9325_get_pixel(cn_lcd_xsize-1-x,cn_lcd_ysize-1-y));
}

#endif
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
    __lcd_ili9325_init( );
    frame_win.wm_bitmap.bm_bits = u8g_frame_buffer;
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
    tg_lcd_display.pixel_format = cn_sys_pf_r5g6b5;
    tg_lcd_display.framebuf_direct = false;
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

#endif //   for #if (cfg_gui_kernel == 1)

