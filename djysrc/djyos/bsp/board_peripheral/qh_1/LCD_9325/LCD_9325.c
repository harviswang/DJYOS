//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//��Ȩ���� (C), 2004-2009,   lst.
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
#include "os_inc.h"

#if (cfg_gui_kernel == 1)

#include "gk_inc.h"
#include "stm32f10x.h"
#include "LCD_9325.h"

#define cn_coordinates      1       //0=�������꣬1=��ת���꣬��ԭ��=(239,319)
u8 u8g_frame_buffer[cn_frame_buffer_size];
struct display_rsc tg_lcd_display;

//������ili9325�Ĳ�������

/*---------------------------------------------------------------------------
���ܣ�	��lcd�ڲ�0x00�Ĵ���
---------------------------------------------------------------------------*/
u32 __ili9325_read_reg(void)
{
    LCD_DATA=0x0000;
	return LCD_CMD;
}
/*---------------------------------------------------------------------------
���ܣ�	дlcd�ڲ��Ĵ���
---------------------------------------------------------------------------*/
void __ili9325_write_reg(u32 reg_index, u32 data)
{
    LCD_DATA=reg_index;
    LCD_CMD=data;
}

/*---------------------------------------------------------------------------
���ܣ�	д����
---------------------------------------------------------------------------*/
void __ili9325_write_cmd(u32 cmd)
{
    LCD_DATA = cmd;
}

/*---------------------------------------------------------------------------
���ܣ�	д����
---------------------------------------------------------------------------*/
#define __ili9325_write_data(data) (LCD_CMD=data)

/*---------------------------------------------------------------------------
���ܣ�	������
---------------------------------------------------------------------------*/
u32 __ili9325_read_data(void)
{
	return LCD_CMD;
}
/*---------------------------------------------------------------------------
���ܣ�	����һ������
---------------------------------------------------------------------------*/
void __ili9325_set_pixel(u32 x, u32 y, u16 color)
{
	__ili9325_write_reg(0x0021, y);        //v��ֱ����
	__ili9325_write_reg(0x0020, x);    //hˮƽ����

	__ili9325_write_cmd(0x0022);
	__ili9325_write_data(color);
}

/*---------------------------------------------------------------------------
���ܣ�	��ȡһ������	    
---------------------------------------------------------------------------*/
u16 __ili9325_get_pixel(u32 x, u32 y)
{
	u16 col = 0;//, rgb = 0
     
	__ili9325_write_reg(0x0021, y);        //v��ֱ����
	__ili9325_write_reg(0x0020, x);    //hˮƽ����

	__ili9325_write_cmd(0x0022);
	__ili9325_read_data();//dummy read

    col	= __ili9325_read_data();//BGR ����
//  rgb = (col & 0x001f)<<11;
//	rgb |= (col & 0xf800)>>11;
//	rgb |= col & 0x07e0;
	return col;
}

/*---------------------------------------------------------------------------
���ܣ�	���ù��λ��
    	row     :0--319
    	column	:0--239
---------------------------------------------------------------------------*/ 
void __ili9325_set_cursor(u32 x, u32 y)
{
	__ili9325_write_reg(0x0021, y);        //v��ֱ����
	__ili9325_write_reg(0x0020, x);    //hˮƽ����	            
}

/*---------------------------------------------------------------------------
���ܣ�	����
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
���ܣ�	�ش�
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

	__ili9325_write_cmd(0x0022);									//дGRAM֮ǰ Ҫ��д0x0022
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



//������djyguiҪ��Ľӿں������������£�������ÿպ�������
/*---------------------------------------------------------------------------
���ܣ�    lcd ��ʼ��
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

//��������������ͨ��������£�����ͼ�λ��ơ�
//�������λͼ���������Դ��Ŀ��λͼ��ʽ����cn_sys_pf_r5g6b5��
//��rop=cn_r3_srccopy
bool_t __lcd_blt_bm_to_bm( struct st_rect_bitmap *dst_bitmap,
                            struct st_rect *dst_rect,
                            struct st_rect_bitmap *src_bitmap,s32 xsrc,s32 ysrc,
                            u8 *pat,struct st_rect_bitmap *mask,u32 rop_code,
                            u32 transparentcolor)
{
    u16 *src_offset,*dst_offset;    //Դλͼ���󻺳������ܲ�����!!!
    u32 x,y;
    if((dst_bitmap->pf_type != cn_sys_pf_r5g6b5) 
             ||(src_bitmap->pf_type != cn_sys_pf_r5g6b5)
             ||(rop_code != cn_r3_srccopy))
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
#if (cn_coordinates == 0)   //��������
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
//��screen�л�һ������ֱ�ߣ������˵㣬��Ӳ�����ٲ�֧����screen�ϻ��ߣ�������
//frame_buffer��driver���Լ򻯣�ֱ�ӷ���false����
//������ʾ����driver���ṩ�������
bool_t __lcd_line_screen(struct st_rect *limit,
                    s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    return false;
}

//��screen�л�һ������ֱ�ߣ����˵㣬��Ӳ�����ٲ�֧����screen�ϻ��ߣ�������
//frame_buffer��driver���Լ򻯣�ֱ�ӷ���false����
//������ʾ����driver���ṩ�������
bool_t __lcd_line_screen_ie(struct st_rect *limit,
                       s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    return false;
}
//screen�о�����䣬��Ӳ�����ٲ�֧����screen�Ͼ�����䣬������frame_buffer��
//driver���Լ򻯣�ֱ�ӷ���false����
//������ʾ����driver���ṩ���??
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

//���ڴ滺������screenλ�鴫�ͣ�ֻ֧�ֿ鿽������֧��rop������
//������ʾ����driver���ṩ�������
bool_t __lcd_bm_to_screen(struct st_rect *dst_rect,
            struct st_rect_bitmap *src_bitmap,s32 xsrc,s32 ysrc)
{
    s32 x,y,width,height;
    u16 *lineoffset;
    if(src_bitmap->pf_type != cn_sys_pf_r5g6b5)
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
        __ili9325_set_pixel(cn_lcd_xsize-x,cn_lcd_ysize-y,pixel);   //ת������
    }else
    {
        dest = __ili9325_get_pixel(cn_lcd_xsize-1-x,cn_lcd_ysize-1-y);  //ת������
        pixel = __gk_blend_rop2(dest, pixel, r2_code);
        __ili9325_set_pixel(cn_lcd_xsize-1-x,cn_lcd_ysize-1-y,pixel);   //ת������
    }
    return true;
}
//��screen�л�һ������ֱ�ߣ������˵㣬��Ӳ�����ٲ�֧����screen�ϻ��ߣ�������
//frame_buffer��driver���Լ򻯣�ֱ�ӷ���false����
//������ʾ����driver���ṩ�������
bool_t __lcd_line_screen(struct st_rect *limit,
                    s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    return false;
}

//��screen�л�һ������ֱ�ߣ����˵㣬��Ӳ�����ٲ�֧����screen�ϻ��ߣ�������
//frame_buffer��driver���Լ򻯣�ֱ�ӷ���false����
//������ʾ����driver���ṩ�������
bool_t __lcd_line_screen_ie(struct st_rect *limit,
                       s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    return false;
}
//screen�о�����䣬��Ӳ�����ٲ�֧����screen�Ͼ�����䣬������frame_buffer��
//driver���Լ򻯣�ֱ�ӷ���false����
//������ʾ����driver���ṩ���??
bool_t __lcd_fill_rect_screen(struct st_rect *dst_rect,u32 color)
{
    u32 x,y,width,height;
    u16 pixel;
    pixel = __gk_convert_color_to_pf(cn_sys_pf_r5g6b5,color);
    width = dst_rect->right-dst_rect->left;
    height = dst_rect->bottom-dst_rect->top;
    //ת������
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

//���ڴ滺������screenλ�鴫�ͣ�ֻ֧�ֿ鿽������֧��rop������
//������ʾ����driver���ṩ�������
bool_t __lcd_bm_to_screen(struct st_rect *dst_rect,
            struct st_rect_bitmap *src_bitmap,s32 xsrc,s32 ysrc)
{
    s32 x,y,width,height;
    u16 *lineoffset;
    if(src_bitmap->pf_type != cn_sys_pf_r5g6b5)
        return false;
    width = dst_rect->right-dst_rect->left;
    height = dst_rect->bottom-dst_rect->top;
    //ת������
    lineoffset = (u16*)((u32)src_bitmap->bm_bits 
                        + (ysrc+height-1)*src_bitmap->linebytes);
    lineoffset +=xsrc;
    //ת������
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

//��screen��ȡһ���أ���ת����cn_sys_pf_e8r8g8b8
u32 __lcd_get_pixel_screen(s32 x,s32 y)
{
    return __gk_convert_color_to_24(cn_sys_pf_r5g6b5, 
                        __ili9325_get_pixel(cn_lcd_xsize-1-x,cn_lcd_ysize-1-y));
}

#endif
//��screen�ھ�����������ݸ��Ƶ�bitmap������ǰ�������ú�dest��pf_type
bool_t __lcd_get_rect_screen(struct st_rect *rect,struct st_rect_bitmap *dest)
{
    return false;
}

//----������ʾ��---------------------------------------------------------------
//����: ������driver�ṩ�̱�д�ģ�����������ʾ�����ֲ����ĺ���������Ӧ�ó����
//      ���룬�������豸���̶��塣��ut2416�У�������Ϊ�ա�
//����: disp����ʾ��ָ��
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t __lcd_disp_ctrl(struct display_rsc *disp)
{
    return true;
}

//----��ʽ��pattern------------------------------------------------------------
//����: ���û��ύ����Ϊpattern��bitmap��ʽ����driver��ʶ��ĸ�ʽ
//����: pattern����������ָ��
//      bitmap���û��ṩ��λͼ
//����: true=�ɹ�,false=ʧ�ܣ�����ʾ��������֧��pattern���ʧ��
//-----------------------------------------------------------------------------
bool_t __lcd_format_pattern(u8 *pattern,struct st_rect_bitmap *bitmap)
{
    return true;
}

//----��ʼ��lcd�豸------------------------------------------------------------
//����: ����
//����: ��
//����: ��ʾ����Դָ��
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
    tg_lcd_display.vmalloc = NULL;
    tg_lcd_display.vrmalloc = NULL;
    tg_lcd_display.vfree = NULL;
    tg_lcd_display.disp_ctrl = __lcd_disp_ctrl;
    tg_lcd_display.format_pattern = __lcd_format_pattern;

    gk_install_display(&tg_lcd_display,(char*)para);
    return (ptu32_t)&tg_lcd_display;
}

#endif //   for #if (cfg_gui_kernel == 1)

