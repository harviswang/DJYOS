//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
//����ģ��: gui
//����:  ������.
//�汾��V1.0.0
//�ļ�����: �������ظ�ʽ���弰�ڻ�������ʾ��������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-10-26
//   ����:  ������.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#if (cfg_gui_kernel == 1)

#ifndef __draw_h__
#define __draw_h__

#ifdef __cplusplus
extern "C" {
#endif
struct gksc_para_draw_bitmap_rop;
struct gksc_para_set_pixel;
struct gksc_para_lineto;
struct gksc_para_draw_bitmap;
//��Ԫ��դ������������windows�����һ�£�ֻ��ǰ�����ǰ׺cn_
//windows�Ǵ�0��ʼ�ţ���Ϊ��1��ʼ
#define cn_r2_black         0   //  0   dest = black����ɫ��
#define cn_r2_notmergepen   1   // dpon dest = ~(dest or pen)
#define cn_r2_masknotpen    2   // dpna dest = ~pen and dest
#define cn_r2_notcopypen    3   // pn   dest = ~pen
#define cn_r2_maskpennot    4   // pdna dest = ~dest and pen
#define cn_r2_not           5   // dn   dest = ~dest
#define cn_r2_xorpen        6   // dpx  dest = dest xor pen
#define cn_r2_notmaskpen    7   // dpan dest = ~(dest and pen)
#define cn_r2_maskpen       8   // dpa  dest = dest and pen
#define cn_r2_notxorpen     9   // dpxn dest = ~(dest xor pen)
#define cn_r2_nop           10  // d    dest = dest��ԭɫ��
#define cn_r2_mergenotpen   11  // dpno dest = ~pen or dest
#define cn_r2_copypen       12  // p    dest = pen������ɫ��
#define cn_r2_mergepennot   13  // pdno dest = ~dest or pen
#define cn_r2_mergepen      14  // dpo  dest = dest or pen
#define cn_r2_white         15  //  1   dest = white����ɫ��
#define cn_r2_last          15

//��Ԫ��դ����,������windows�����һ�£�ֻ��ǰ�����ǰ׺cn_r3_
#define cn_r3_srccopy       (u32)0x00cc0020 // dest = source
#define cn_r3_srcpaint      (u32)0x00ee0086 // dest = source or dest
#define cn_r3_srcand        (u32)0x008800c6 // dest = source and dest
#define cn_r3_srcinvert     (u32)0x00660046 // dest = source xor dest
#define cn_r3_srcerase      (u32)0x00440328 // dest = source and (not dest )
#define cn_r3_notsrccopy    (u32)0x00330008 // dest = (not source)
#define cn_r3_notsrcerase   (u32)0x001100a6 // dest = (not src) and (not dest)
#define cn_r3_mergecopy     (u32)0x00c000ca // dest = (source and pattern)
#define cn_r3_mergepaint    (u32)0x00bb0226 // dest = (not source) or dest
#define cn_r3_patcopy       (u32)0x00f00021 // dest = pattern
#define cn_r3_patpaint      (u32)0x00fb0a09 // dest = dpsnoo
#define cn_r3_patinvert     (u32)0x005a0049 // dest = pattern xor dest
#define cn_r3_dstinvert     (u32)0x00550009 // dest = (not dest)
#define cn_r3_blackness     (u32)0x00000042 // dest = black
#define cn_r3_whiteness     (u32)0x00ff0062 // dest = white

#define cn_r3_nop           (u32)0x00AA0029 // dest = dest
#define cn_rop_p            0xF0
#define cn_rop_s            0xCC
#define cn_rop_d            0xAA

//���Ʋ������Ĵ���:
//1����bitmap�ϻ���
//2����screen�ϻ���
//3��ȡbitmap�ϵ����ػ�bitmap
//4��ȡscreen�ϵ����ػ�bitmap��
//��bitmap�ϣ������ء���ʱ���Ǵ�rop2������ģ�Ӳ��:��֧�֡�����֧�֡�ȫ֧��
//bitmap�������ε�ʱ�򣬲���rop2�����룬Ӳ������֧�֣�Ҳ���ܲ�֧��
//bitmap�ϻ�λͼʱ����rop3�����룬Ӳ��:��֧�֡�����֧�֡�ȫ֧��
//���������������������NULL��֮������֧�ֵ�ʵ��֮����֧�ֵķ���false����gui
//kernelʵ��֮�������Ļ���Ҳ������check_raster�����ˡ�

//gui kernelȷ��������Ա�����Ĳ�������Ϊ��ͼĿ���bitmap��������win buffer����
//frame buffer�е�bitmap�������û������ṩ��bitmap��������win buffer ����
//frame buffer�������ظ�ʽ��������ʾ����ͬ
//����Ϊͼ��Դ��bitmap���������cn_sys_pf_xxx�е�����һ�֣������뱾��ʾ����ʹ��
//�����ظ�ʽ��ͬ
struct disp_draw
{
    //�ھ���λͼ�л�һ�����أ�����ʾ��ʹ��cn_custom_pf��ʽ��������Ӳ�����ٹ��ܣ�
    //Ӧ��ʵ���������������ֱ�ӷ���false����ʹ��Ӳ�����٣����ü��ٹ��ܲ�֧��
    //r2_code����Ļ���Ҳ����false��color�ĸ�ʽ��cn_sys_pf_e8r8g8b8
    bool_t (*set_pixel_bm)(struct st_rect_bitmap *bitmap,
                         s32 x,s32 y,u32 color,u32 r2_code);
    //�ھ���λͼ�л�һ������ϸֱ�ߣ������˵㣬����ʾ��ʹ��cn_custom_pf��ʽ������
    //��Ӳ�����ٹ��ܣ�Ӧ��ʵ���������������ֱ�ӷ���false����ʹ��Ӳ�����٣�����
    //���ٹ��ܲ�֧��r2_code����Ļ���Ҳ����false��color��ʽ��cn_sys_pf_e8r8g8b8
    bool_t (*line_bm)(struct st_rect_bitmap *bitmap,struct st_rect *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code);
    //�ھ���λͼ�л�һ������ϸֱ�ߣ����˵㣬����ʾ��ʹ��cn_custom_pf��ʽ������
    //��Ӳ�����ٹ��ܣ�Ӧ��ʵ���������������ֱ�ӷ���false����ʹ��Ӳ�����٣�����
    //���ٹ��ܲ�֧��r2_code����Ļ���Ҳ����false��color��ʽ��cn_sys_pf_e8r8g8b8
    bool_t (*line_bm_ie)(struct st_rect_bitmap *bitmap,struct st_rect *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code);
    //�ھ���λͼ�������Σ�����ʾ��ʹ��cn_custom_pf��ʽ��������Ӳ�����ٹ��ܣ�
    //Ӧ��ʵ���������������ֱ�ӷ���false��color��ʽ��cn_sys_pf_e8r8g8b8
    //����color�ĸ�ʽ������cn_sys_pf_e8r8g8b8
    bool_t (*fill_rect_bm)(struct st_rect_bitmap *dst_bitmap,
                         struct st_rect *dst_rect,u32 color);
    
    //����������λͼ��λ�鴫�ͣ������ʾ��ʹ�õ����ظ�ʽ��cn_custom_pf������Ӳ��
    //2d����֧��λ�鴫�ͣ���ʵ���������������ֱ�ӷ���false����ʹ��Ӳ�����٣���
    //�ü��ٹ��ܲ�֧��rop_code����Ļ���Ҳ����false������������false��
    //����gui kernel�������������ط���������
    //Ŀ��λͼ�����ظ�ʽĬ��Ϊ��ʾ����ͬ�ĸ�ʽ��
    //���rop_codeҪ��ִ�е���rop4������������λͼ�ĳߴ��λ����Դλͼһ��
    bool_t (*blt_bm_to_bm)( struct st_rect_bitmap *dst_bitmap,
                            struct st_rect *dst_rect,
                            struct st_rect_bitmap *src_bitmap,s32 xsrc,s32 ysrc,
                            u8 *pat,struct st_rect_bitmap *bitmsk,u32 rop_code,
                            u32 transparentcolor);
    //�Ӿ���λͼ��ȡһ���أ���ת����cn_sys_pf_e8r8g8b8�������ʾ����ʽ����
    //cn_custom_pf����ΪNULL��
    u32 (*get_pixel_bm)(struct st_rect_bitmap *bitmap,s32 x,s32 y);
    //��srcλͼ��rect������������ݸ��Ƶ�dest�У�����ǰ�������ú�dest��pf_type��
    //�����������src��pf_type��ֱ��ʹ��screen�ĸ�ʽ��
    //���������ڴӴ��ڼ��о��Σ���blt_bm_to_bm������ʾ���Ρ�
    //�����ʾ��ʹ�õ����ظ�ʽ��cn_custom_pf������Ӳ��2d����֧��bm�俽��ͼ�Σ���
    //ʵ���������������ֱ�ӷ���false��
    //����dest->pf_type���ںܶ���ܣ���ʹ��Ӳ�����٣�Ҳ����ֻ֧�ֲ���pf_type����
    //�����Բ�֧�ֵĸ�ʽ������false��
    bool_t (*get_rect_bm)(struct st_rect_bitmap *src,struct st_rect *rect,
                        struct st_rect_bitmap *dest);
    //��ѯָ������Ԫ(��Ԫ)��դ�����Ƿ�֧��,������չ��դ�����룬��alpha�ȡ�
    //OS�ڿ����ṩ���ģ���դ�����Ĵ��룬���Ӳ�����ٲ�֧�ֹ�դ������driver�Ƿ�
    //���ÿ��к�����֧�ֹ�դ��������ȫ��driver���߾�����Ҳ���Բ���������ʵ�ֲ���
    //��դ����������ʵ�ֳ��õ�16����դ������
//    bool_t (*check_raster)(u32 rop_code);

    //��������screen�л�ͼ�ĺ���
    //��screen�л�һ�����أ���frame buffer������£�������ʾgui����������������
    //�������direct_screen==true������ܵ��ñ�������
    //��ˣ������Ƿ���frame buffer��driver�������ṩ���ұ���ʵ�ֱ�����
    //������ʾ����driver���ṩ���������Ҳ����ʵ�ֱ�����
    bool_t (*set_pixel_screen)(s32 x,s32 y,u32 color,u32 r2_code);
    //��screen�л�һ������ֱ�ߣ������˵㣬��Ӳ�����ٲ�֧����screen�ϻ��ߣ�driver
    //���Լ򻯣�ֱ�ӷ���false���ɡ���frame buffer������£�������ʾgui���������
    //���������������direct_screen==true������ܵ��ñ�����������������false��
    //�������һ������set_pixel_screen����
    //������ʾ����driver���ṩ������������ڲ�֪��������ʾ���������������ʾ��
    //driver����ʵ�ֱ����������ܼ򵥷���false��
    //color�ĸ�ʽ��cn_sys_pf_e8r8g8b8
    bool_t (*line_screen)(struct st_rect *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code);

    //��screen�л�һ������ֱ�ߣ������˵㣬��Ӳ�����ٲ�֧����screen�ϻ��ߣ�driver
    //���Լ򻯣�ֱ�ӷ���false���ɡ���frame buffer������£�������ʾgui���������
    //���������������direct_screen==true������ܵ��ñ�����������������false��
    //�������һ������set_pixel_screen����
    //������ʾ����driver���ṩ������������ڲ�֪��������ʾ���������������ʾ��
    //driver����ʵ�ֱ����������ܼ򵥷���false��
    //color�ĸ�ʽ��cn_sys_pf_e8r8g8b8
    bool_t (*line_screen_ie)(struct st_rect *limit,
                           s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code);
    //screen�о�����䣬��Ӳ�����ٲ�֧����screen�Ͼ�����䣬driver���Լ򻯣�ֱ��
    //����false���ɡ���frame buffer������£�������ʾgui���������
    //���������������direct_screen==true������ܵ��ñ�����������������false��
    //�������һ������set_pixel_screen����
    //������ʾ����driver���ṩ������������ڲ�֪��������ʾ���������������ʾ��
    //driver����ʵ�ֱ����������ܼ򵥷���false��
    //color�ĸ�ʽ��cn_sys_pf_e8r8g8b8
    bool_t (*fill_rect_screen)(struct st_rect *dst_rect,u32 color);
    //���ڴ滺������screenλ�鴫�ͣ�ֻ֧�ֿ鿽������֧��rop������
    //��Ӳ�����ٲ�֧����screen�ϻ���λͼ��driver���Լ򻯣�ֱ��
    //����false���ɡ���frame buffer������£�__gk_output_redraw�л�����������
    //���������direct_screen==true��Ҳ���ܵ��ñ�����������������false��
    //����gui kernel���һ������set_pixel_screen����
    //��ʹӲ������֧�֣��������֧�־����src_bitmap->pf_type,Ҳ�ɷ���false
    //����������辵����ʾ̫����������ʾ����driver����ʵ�ֱ����������ܼ򵥷���false��
    bool_t (*bm_to_screen)(struct st_rect *dst_rect,
                struct st_rect_bitmap *src_bitmap,s32 xsrc,s32 ysrc);
    
    //��screen��ȡһ���أ���ת����cn_sys_pf_e8r8g8b8
    u32 (*get_pixel_screen)(s32 x,s32 y);
    //��screen�ھ�����������ݸ��Ƶ�bitmap������ǰ�������ú�dest��pf_type
    bool_t (*get_rect_screen)(struct st_rect *rect,struct st_rect_bitmap *dest);
};
//ֱ��д����֧��rop����
bool_t gk_rop_needsno_destination(u32 rop_code);
bool_t gk_rop_needsno_source(u32 rop_code);
bool_t gk_rop_needsno_pattern(u32 rop_code);
u32 __gk_convert_color_to_24(u16 pf_type,u32 color);
u32 __gk_convert_color_to_pf(u16 pf_type,u32 color);
u32 __gk_convert_part_pf_to_pf(u16 spf_type,u16 dpf_type,u32 color);
u32 __gk_blend_rop2(u32 dest,u32 pen,u32 rop2_code);
u32 __gk_blend_rop3(u32 dest,u32 src,u32 pat,u8 rop3_code);
u32 __gk_point_sect(s32 x,s32 y,struct st_rect *limit);
u32 __gk_oline_sect_inter(struct st_rect *limit,s32 *x1,s32 *y1,s32 *x2,s32 *y2);
void __gk_draw_oline(struct gkwin_rsc *gkwin,s32 x1,s32 y1,
                        s32 x2,s32 y2,u32 color,u32 rop2_code);
u32 __gk_get_pixel_bm(struct st_rect_bitmap *bitmap,s32 x,s32 y);
void __gk_get_rgb_bm(u16 pf_type,u32 color,u32 *a,u32 *r,
                u32 *g,u32 *b);
u32 __gk_rop_alpha(struct st_rect_bitmap *dst_bitmap,
            struct st_rect_bitmap *src_bitmap,s32 x_dst,s32 y_dst,
            s32 x_src,s32 y_src,u32 fore,u32 back);
void __gk_set_pixel_rop_ext_bm(struct st_rect_bitmap *dst_bitmap,
                        struct st_rect_bitmap *src_bitmap,u8 *pat,
                        s32 x_dst,s32 y_dst,s32 x_src,s32 y_src,
                        u32 rop_code,struct st_rect_bitmap *bitmsk,
                        u32 transparentcolor);
void __gk_set_pixel_rop2_bm(struct st_rect_bitmap *bitmap,
                        s32 x,s32 y,u32 color,u32 rop2_code);
void __gk_set_pixel_rop3_bm(struct st_rect_bitmap *dst_bitmap,
                        struct st_rect_bitmap *src_bitmap,
                        u8 *pat,s32 x_dst,s32 y_dst,s32 x_src,
                        s32 y_src,u32 rop3_code);
void gk_set_pixel(struct gksc_para_set_pixel *para);
void __gk_shading_pixel(struct gkwin_rsc *gkwin,s32 x,s32 y);
void __gk_shading_rect(struct gkwin_rsc *gkwin,struct st_rect *rect);
void __gk_shading_line(struct gkwin_rsc *gkwin,struct st_rect *limit,
                     s32 x1,s32 y1,s32 x2,s32 y2);
void __gk_shading_clear(struct gkwin_rsc *gkwin);
void __gk_vlineto_bm(struct gkwin_rsc *gkwin,struct st_rect *limit,
                    s32 x1,s32 y1,s32 y2,u32 color,u32 rop2_code);
void __gk_hlineto_bm(struct gkwin_rsc *gkwin,struct st_rect *limit,
                    s32 x1,s32 y1,s32 x2,u32 color,u32 rop2_code);
void __gk_olineto_bm(struct gkwin_rsc *gkwin,struct st_rect *limit,
                    s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 rop2_code);
void __gk_lineto_bm(struct gkwin_rsc *gkwin,struct st_rect *limit,
                s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 rop2_code);
void __gk_set_pixel_screen(struct display_rsc *display,s32 x,s32 y,
                        u32 color,u32 rop2_code);
void __gk_lineto_screen(struct display_rsc *display,struct st_rect *limit,
                    s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 rop2_code);
void __gk_blt_bm_to_screen(struct display_rsc *display,struct st_rect *rect,
                        struct st_rect_bitmap *bitmap,s32 x,s32 y);
void __gk_fill_rect(struct st_rect_bitmap *dst_bitmap,
                     struct st_rect *dst_rect,u32 color);
void __gk_fill_bm(struct st_rect_bitmap *dst_bitmap,u32 color);
void __gk_set_pixels_circle(struct gkwin_rsc *gkwin,struct st_rect *limit,u32 r,
                                struct st_rect_bitmap *bitmap,s32 x0,s32 y0,
                                s32 x,s32 y,u32 color,u32 rop2_code);
void __gk_draw_circle_bm(struct gkwin_rsc *gkwin,struct st_rect *limit,
                        s32 x0,s32 y0,s32 r,u32 color,u32 rop2_code);
void __gk_draw_circle_screen(struct display_rsc *display,struct st_rect *limit,
                        s32 x0,s32 y0,s32 r,u32 color,u32 rop2_code);
void gk_draw_circle(struct gksc_para_draw_circle *para);
void __gk_Bezier_bm(struct gkwin_rsc *gkwin,struct st_rect *limit,
                    float x1,float y1,float x2,float y2,float x3,float y3,
                    float x4,float y4,u32 color,u32 rop2_code);
void __gk_Bezier_screen(struct display_rsc *display,struct st_rect *limit,
                    float x1,float y1,float x2,float y2,float x3,float y3,
                    float x4,float y4,u32 color,u32 rop2_code);
void gk_Bezier(struct gksc_para_Bezier *para);
void gk_lineto(struct gksc_para_lineto *para);
void gk_lineto_ie(struct gksc_para_lineto *para);
void gk_draw_bitmap(struct gksc_para_draw_bitmap *para);
void gk_draw_text(struct gksc_para_draw_text *para,const char *text);
void gk_draw_bitmap_rop(struct gksc_para_draw_bitmap_rop *para);
void gk_fill_part_win(struct gksc_para_fill_part_win *para);
void gk_fill_win(struct gksc_para_fill_win *para);


#ifdef __cplusplus
}
#endif

#endif /*__draw_h__*/

#endif //   for #if (cfg_gui_kernel == 1)

