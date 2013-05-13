#include "os_inc.h"
#include "gk_inc.h"
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
struct display_rsc tg_lcd_display;
//�����Դ棬��������һ���Դ��һ��֡���壬16�Ƕ���Ŀ���
static u8 u8g_dsp_buffer[cn_frame_buffer_size + 16];  
static u8 u8g_video_buffer[cn_frame_buffer_size + 16];  
static u16 *pg_frame_buffer,*pg_video_buf;
//volatile unsigned short LCD_BUFFER[SCR_YSIZE_TFT][SCR_XSIZE_TFT];
bool_t __lcd_line_bm_ie(struct st_rect_bitmap *bitmap,struct st_rect *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code);
bool_t __lcd_blt_bm_to_bm(struct st_rect_bitmap *dst_bitmap,
                        struct st_rect *dst_rect,
                        struct st_rect_bitmap *src_bitmap,s32 xsrc,s32 ysrc,
                        u8 *pat,struct st_rect_bitmap *bitmsk,u32 rop_code,
                        u32 transparentcolor);
//----��ʼ��lcdӲ��------------------------------------------------------------
//����: ��ʼ��lcdӲ����
//����: ��
//����: ��
//��ע: ����������ֲ�ؼ�����
//----------------------------------------------------------------------------
void __lcd_hard_init(void)
{
    u8 *alignbuf;
    //�˿� C ������ʹ�ܼĴ��� ,0,ʹ�ܸ����������ܵ���Ӧ�˿�����
    pg_gpio_reg->GPCUP = 0x00000000;   
    //GPC4 ,VM;GPC3 VFRAME;GPC2  VLINE;GPC1 VLINE;GPC0 �����
    //Initialize VD[7:0]��16-31λ,VD[23:0] ,��LCD ������������˿ڣ�
    pg_gpio_reg->GPCCON = 0xaaaa02a9;
    pg_gpio_reg->GPDUP = 0x00000000;
    pg_gpio_reg->GPDCON = 0xaaaaaaaa; //Initialize VD[23:8]��0-31λ��
#if 1
    alignbuf = (u8*)(((u32)u8g_dsp_buffer +15)&0xfffffff0);  
    //2440Ҫ���Դ治�ܿ�4M�߽�
//    if(((u32)alignbuf&0xffc00000)
//                    != (((u32)alignbuf+cn_frame_buffer_size)&0xffc0000))
//    {//�Դ��ǰ�벿��δ��4M�߽磬ǰ�벿�������Դ棬��벿������frame buffer
        pg_frame_buffer = (u16*)align_up(16,(ptu32_t)u8g_dsp_buffer);//�Դ�Ҫ��16�ֽڶ���
        pg_video_buf = (u16*)align_up(16,(ptu32_t)u8g_video_buffer); //�Դ�Ҫ��16�ֽڶ���
//    }else
//    {//�Դ��ǰ�벿�ֿ�4M�߽磬��벿�������Դ棬ǰ�벿������frame buffer
//        pg_video_buf = (u16*)alignbuf;
//        pg_frame_buffer = (u16*)(alignbuf + cn_frame_buffer_size);
//    }
#endif
    pg_lcd_reg->LCDCON1 = (LCDCON1_CLKVAL<<8)|(LCDCON1_MMODE<<7)|(3<<5)
                            |(12<<1)|0;
    pg_lcd_reg->LCDCON2 = (LCDCON2_VBPD<<24)|(LINEVAL_TFT<<14)|(LCDCON2_VFPD<<6)
                            |(LCDCON2_VSPW);
    pg_lcd_reg->LCDCON3 = (LCDCON3_HBPD<<19)|(HOZVAL_TFT<<8)|(LCDCON3_HFPD);
    pg_lcd_reg->LCDCON4 = (LCDCON4_MVAL<<8)|(LCDCON4_HSPW);
    pg_lcd_reg->LCDCON5 = (1<<11) | (0<<10) | (1<<9) | (1<<8) | (0<<7) 
                 | (0<<6) | (1<<3)  |(LCDCON5_BSWP<<1) | (LCDCON5_HWSWP);
    pg_lcd_reg->LCDSADDR1 = (((u32)pg_video_buf>>22)<<21)
                                |M5D((uint32_t)pg_video_buf>>1);
    pg_lcd_reg->LCDSADDR2 = M5D(((u32)pg_video_buf
                            +(cn_lcd_xsize*cn_lcd_ysize*2))>>1);
    pg_lcd_reg->LCDSADDR3 = (((cn_lcd_xsize-cn_lcd_xsize)/1)<<11)
                                |(cn_lcd_xsize/1);
    pg_lcd_reg->LCDINTMSK |= (3);         //LCD�ж����μĴ���
    pg_lcd_reg->TCONSEL &= (~7) ;       //TCON���ƼĴ��� 
    pg_lcd_reg->TPAL = 0;                 //��ʱ��ɫ��Ĵ��� 
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
        pg_lcd_reg->LCDCON1 |= 1;   //������Ƶ����� LCD�����ź�
    else                        //��ֹ��Ƶ����� LCD�����ź�
        pg_lcd_reg->LCDCON1 = pg_lcd_reg->LCDCON1 & 0x3fffe; 
}
//----����LCD��Դ--------------------------------------------------------------
//����: LCD��Դ��������ʹ�ܡ�
//����: invpwren������PWREN�źż���
//      pwren��LCD_PWREN����ź�ʹ��/��ֹ 
//����: ��
//-----------------------------------------------------------------------------
void __lcd_power_enable(int invpwren,int pwren)
{
    pg_gpio_reg->GPGUP = pg_gpio_reg->GPGUP&(~(1<<4))|(1<<4); //����ʹ��
    pg_gpio_reg->GPGCON = pg_gpio_reg->GPGCON&(~(3<<8))|(3<<8); 
    pg_gpio_reg->GPGDAT = pg_gpio_reg->GPGDAT | (1<<4) ;
    pg_lcd_reg->LCDCON5 = pg_lcd_reg->LCDCON5&(~(1<<3))|(pwren<<3);   
    pg_lcd_reg->LCDCON5 = pg_lcd_reg->LCDCON5&(~(1<<5))|(invpwren<<5); 
}
//----������ʾ��---------------------------------------------------------------
//����: ������driver�ṩ�̱�д�ģ�����������ʾ�����ֲ����ĺ���������Ӧ�ó����
//      ���룬�������豸���̶��塣
//����: disp����ʾ��ָ��
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t __lcd_disp_ctrl(struct display_rsc *disp)
{
    return true;
}
//----��ʽ��pattern------------------------------------------------------------
//����: ���û��ύ����Ϊpattern��bitmap��ʽ����driver��ʶ��ĸ�ʽ��
//����: pattern����������ָ��
//      bitmap���û��ṩ��λͼ
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t __lcd_format_pattern(u8 *pattern,struct st_rect_bitmap *bitmap)
{
    s32 x,y;
    u32 color,bytes;

    if(bitmap->pf_type == cn_lcd_pf)
        return true;
    bytes = 0;     
    for(y = 0;y < bitmap->height;y++)
    {
        for(x = 0;x < bitmap->width;x++)
        {
            color = __gk_get_pixel_bm(bitmap,x,y);
            color = __gk_convert_part_pf_to_pf(cn_lcd_pf,bitmap->pf_type,color);
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
bool_t __lcd_set_pixel_bm(struct st_rect_bitmap *bitmap,
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
bool_t __lcd_line_bm(struct st_rect_bitmap *bitmap,struct st_rect *limit,
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
bool_t __lcd_line_bm_ie(struct st_rect_bitmap *bitmap,struct st_rect *limit,
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
bool_t __lcd_fill_rect_bm(struct st_rect_bitmap *bitmap,
                      struct st_rect *dst_rect,u32 color)
{
    return false;
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
bool_t __lcd_blt_bm_to_bm(struct st_rect_bitmap *dst_bitmap,
                        struct st_rect *dst_rect,
                        struct st_rect_bitmap *src_bitmap,s32 xsrc,s32 ysrc,
                        u8 *pat,struct st_rect_bitmap *bitmsk,u32 rop_code,
                        u32 transparentcolor)
{
    return false;
}
//----��ȡλͼ�е�����---------------------------------------------------------
//����: ȡλͼ�е�һ�����أ�ֻ����bitmap�����ظ�ʽΪcn_custom_pfʱ������Ҫ��ȡ��
//      ����Կ�������֧���Զ����ʽ��������ֱ�ӷ���false��
//����: bitmap��Ŀ��λͼ
//      x��y������
//����: ��ȡ������ֵ��cn_sys_pf_e8r8g8b8��ʽ
//-----------------------------------------------------------------------------
u32 __lcd_get_pixel_bm(struct st_rect_bitmap *bitmap,s32 x,s32 y)
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
bool_t __lcd_get_rect_bm(struct st_rect_bitmap *src,struct st_rect *rect,
                     struct st_rect_bitmap *dest)
{
    return false;
}
//----screen�л�����-----------------------------------------------------------
//����: ��screen�л�һ�����أ���frame buffer��driver��ֱ�ӷ��ؼ��ɡ�
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
    pen = __gk_convert_color_to_pf(cn_sys_pf_r5g6b5,color);
    dest = (u32)pg_video_buf[byteoffset];
    dest = __gk_blend_rop2(dest,pen,rop2_code);
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
bool_t __lcd_line_screen(struct st_rect *limit,
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
bool_t __lcd_line_screen_ie(struct st_rect *limit,
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
bool_t __lcd_fill_rect_screen(struct st_rect *dst_rect,u32 color)
{
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
bool_t __lcd_bm_to_screen(struct st_rect *dst_rect,
                struct st_rect_bitmap *src_bitmap,s32 xsrc,s32 ysrc)
{
    u32 size,width_rect,double_width_src,height_rect,height;
    u32 byteoffset_rect,byteoffset_bitmap;
    s32 y;

    width_rect = dst_rect->right-dst_rect->left;//Ŀ����εĿ��
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
bool_t __lcd_get_rect_screen(struct st_rect *rect,struct st_rect_bitmap *dest)
{
    return false;
 }
//----��ʼ��lcd�豸------------------------------------------------------------
//����: ��ʼ��lcd�豸��
//����: ��
//����: ��ʾ����Դָ��
//-----------------------------------------------------------------------------
ptu32_t module_init_lcd(ptu32_t para)
{
    static struct gkwin_rsc frame_win;
    __lcd_hard_init();
    __lcd_power_enable(0,1);
    __lcd_envid_of(1);    

    frame_win.wm_bitmap.bm_bits = (u8*)pg_frame_buffer;
    frame_win.wm_bitmap.width = cn_lcd_xsize;
    frame_win.wm_bitmap.height = cn_lcd_ysize;
    frame_win.wm_bitmap.pf_type = cn_lcd_pf;
    frame_win.wm_bitmap.linebytes = cn_lcd_xsize*2;
    tg_lcd_display.frame_buffer = &frame_win;

        
    tg_lcd_display.xmm = 0;
    tg_lcd_display.ymm = 0;
    tg_lcd_display.width = cn_lcd_xsize;
    tg_lcd_display.height = cn_lcd_ysize;
    tg_lcd_display.patx = 8;
    tg_lcd_display.paty = 8;
    tg_lcd_display.pixel_format = cn_sys_pf_r5g6b5;
    tg_lcd_display.reset_clip = false;          
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
    tg_lcd_display.vmalloc = NULL;
    tg_lcd_display.vrmalloc = NULL;
    tg_lcd_display.vfree = NULL;
    tg_lcd_display.disp_ctrl = __lcd_disp_ctrl;
    tg_lcd_display.format_pattern = __lcd_format_pattern;

    if(gk_install_display(&tg_lcd_display,(char*)para))
        return (ptu32_t)&tg_lcd_display;
    else
        return NULL;      
}







