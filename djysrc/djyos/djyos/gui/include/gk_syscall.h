//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
//����ģ��: gui
//����:  ������.
//�汾��V1.0.0
//�ļ�����: gui kernel���岿��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-11-21
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#if (cfg_gui_kernel == 1)

#ifndef __GK_SYSCALL_H__
#define __GK_SYSCALL_H__

#ifdef __cplusplus
extern "C" {
#endif

struct st_rect_bitmap;
//gui kernel��syscall����,���ϲ�ͨ���ܵ����ݸ�gui kernel
//���ڿ���������
#define cn_gksc_set_pen_color       0x000   //���ô��ڻ�����ɫ
#define cn_gksc_get_pen_color       0x001   //ȡ������ɫ
#define cn_gksc_set_canvas_color    0x002   //���ô��ڻ�����ɫ
#define cn_gksc_get_canvas_color    0x003   //ȡ������ɫ
#define cn_gksc_set_size            0x004   //���ô��ڷֱ���
#define cn_gksc_get_size            0x005   //ȡ���ڷֱ���
#define cn_gksc_set_cursor          0x006   //���ô��ڹ��λ��
#define cn_gksc_get_cursor          0x007   //���ô��ڹ��λ��
#define cn_gksc_create_gkwin        0x008   //������������
#define cn_gksc_create_desktop      0x009   //������������
#define cn_gksc_set_direct_screen	0x00a   //���ô��ڵ�ֱ��д������
#define cn_gksc_unset_direct_screen	0x00b   //ȡ�����ڵ�ֱ��д������
#define cn_gksc_flush_wbuf          0x00c   //�Ѵ��ڻ������е�ͼ��ˢ�µ�screen
#define cn_gksc_flush_fbuf          0x00d   //��֡�������е�ͼ��ˢ�µ�screen
#define cn_gksc_set_rop_code        0x00e   //���ô��ڵĹ�դ����
#define cn_gksc_set_pat_buf         0x00f   //���ô��ڵ�ͼ��λͼ
#define cn_gksc_set_transparentcolor 0x010   //���ô��ڵ�͸��ɫ
#define cn_gksc_set_bitmsk          0x011   //���ô��ڵ�����λͼ
#define cn_gksc_destroy_win         0x012   //�ͷŴ��ڵ���Դ
#define cn_gksc_set_prio            0x013   //�ı䴰�ڵ����ȼ�
#define cn_gksc_set_bound_mode      0x014   //���ô��ڱ߽�ģʽ
#define cn_gksc_move_win            0x015   //�ƶ�����
#define cn_gksc_change_win_area     0x016   //�ƶ�����

//����������
#define cn_gksc_set_pixel           0x100   //��������
#define cn_gksc_lineto              0x101   //�������ؿ�����������������˵�
#define cn_gksc_lineto_inc_end      0x102   //�������ؿ���������������˵�
#define cn_gksc_draw_bitmap         0x103   //��bitmap,�޹�դ����
#define cn_gksc_draw_bitmap_rop     0x104   //��bitmap,����դ����
#define cn_gksc_fill_win            0x105   //��ָ����ɫ��䴰��
#define cn_gksc_draw_text           0x106
#define cn_gksc_fill_part_win       0x107   //��ָ����ɫ���һ�����ڵ�һ����
#define cn_gksc_draw_circle         0x108   //��Բ
#define cn_gksc_Bezier              0x109   //������������

struct gksc_para_create_desktop
{
//    u16 command;
    struct display_rsc *display;        //������ʾ��
    struct gkwin_rsc *desktop;          //���洰��,���ϲ����ṹ��
    s32 width,height;           //����ߴ磬��С����ʾ���ߴ������Ϊ��ʾ���ߴ�
    u32 color;                          //��������ʱ������ɫ
    bool_t buf_mode;    //true=����������Ĵ��ڣ�ֻ�д�frame buffer��ʾ������Ч
};
struct gksc_para_create_gkwin
{
//    u16 command;
    struct gkwin_rsc *parent_gkwin;     //�´��ڵĸ�����
    struct gkwin_rsc *gkwin;            //�´���,�ɵ����߷���ṹ��
    struct gkwin_rsc **result;          //���ڷ��ؽ��
    s32 left,top,right,bottom;          //�´��ڵ�λ�ã�����ڸ����ڵ�����
    u32 color;                          //��������ʱ������ɫ
    bool_t buf_mode;    //true=����������Ĵ��ڣ�ֻ�д�frame buffer��ʾ������Ч
    char *name;                         //��������(����)
    u32  transparentcolor;     //͸��ɫ��rop_code������͸��ɫʱ����
    u32 rop_code;       //���ڵĹ�դ���ԣ���windows�Ĺ�դ������Ļ���������չ
};
struct gksc_para_fill_win
{
//    u16 command;
    struct gkwin_rsc *gkwin;        //���Ƶ�Ŀ�괰��
    u32 color;                      //���ʹ�õ���ɫ
};
struct gksc_para_fill_part_win
{
//    u16 command;
    struct gkwin_rsc *gkwin;        //���Ƶ�Ŀ�괰��
    struct st_rect rect;            //�����ľ���
    u32 color;                      //���ʹ�õ���ɫ
};
struct gksc_para_draw_text
{
//    u16 command;
    struct gkwin_rsc* gkwin;        //Ŀ�괰��
    s32 x, y;                       //��ʾλ�ã������gkwin
    s32 count;                      //���������ȣ��ֽ�����������������
};
struct gksc_para_set_pixel
{
//    u16 command;
    struct gkwin_rsc *gkwin;        //���Ƶ�Ŀ�괰��
    s32 x,y;                        //���Ӧ������
    u32 color;                      //����ʹ�õ���ɫ
    u32 rop2_code;                  //��Ԫ��դ������
};
struct gksc_para_draw_bitmap
{
//    u16 command;
    struct gkwin_rsc *gkwin;        //���Ƶ�Ŀ�괰��
    struct st_rect_bitmap bitmap;   //�����Ƶ�λͼ
    s32 x,y;                        //����λ�õ����Ͻ�����
};
struct gksc_para_draw_bitmap_rop
{
//    u16 command;
    struct gkwin_rsc *gkwin;        //���Ƶ�Ŀ�괰��
    struct st_rect_bitmap bitmap;   //�����Ƶ�λͼ
    struct st_rect_bitmap pattern;  //Ҫ���õ�ͼ��λͼ   
    struct st_rect_bitmap bitmsk;   //rop4��������Ҫ������λͼ��һ����ɫλͼ
    u32 transparentcolor;           //Ҫ���õ�͸��ɫ   
    s32 x,y;                        //����λ�õ����Ͻ�����
    u32 rop_code;                   //��դ������
};
struct gksc_para_lineto
{
//    u16 command;
    struct gkwin_rsc *gkwin;        //���Ƶ�Ŀ�괰��
    s32 x1,y1,x2,y2;                //Ŀ��ֱ�ߵ���㣬�յ�����
    u32 color;                      //����ֱ��ʹ�õ���ɫ
    u32 rop2_code;                  //��Ԫ��դ������
};
struct gksc_para_draw_circle
{
//    u16 command;
    struct gkwin_rsc *gkwin;        //���Ƶ�Ŀ�괰��
    s32 x0,y0;                      //Բ������
    s32 r;                          //Բ�İ뾶              
    u32 color;                      //��Բʹ�õ���ɫ
    u32 rop2_code;                  //��Ԫ��դ������
};
struct gksc_para_Bezier
{
    struct gkwin_rsc *gkwin;        //���Ƶ�Ŀ�괰��
    float x1,y1,x2,y2,x3,y3,x4,y4;  //����Bezier���ߵ��ĸ����Ƶ�
    u32 color;                      //��Bezier����ʹ�õ���ɫ
    u32 rop2_code;                  //��Ԫ��դ������
};
struct gksc_para_move_win
{
    struct gkwin_rsc *gkwin;        //Ŀ�괰��
    s32 left,top;                   //Ŀ�괰����λ�õ����Ͻ�����
};
struct gksc_para_change_win_area
{
    struct gkwin_rsc *gkwin;        //Ŀ�괰��
    s32 left,top,right,bottom;      //Ŀ�괰���µ�����
};
struct gksc_para_set_bound_mode
{
    struct gkwin_rsc *gkwin;        //Ŀ�괰��
    bool_t mode;                    //���ڱ߽�ģʽ��trueΪ���ޣ�falseΪ������
};
struct gksc_para_set_prio
{
    struct gkwin_rsc *gkwin;        //Ŀ�괰��
    sfast_t prio;                  //�µ����ȼ�
};
struct gksc_para_set_rop_code
{
//    u16 command;
    struct gkwin_rsc *gkwin;        //���Ƶ�Ŀ�괰��
    struct st_rect_bitmap bitmsk;   //Ҫ���õ�����λͼ 
    u32 rop_code;                   //�µĹ�դ������
}; 
struct gksc_para_set_pat_buf
{
//    u16 command;
    struct gkwin_rsc *gkwin;        //���Ƶ�Ŀ�괰��           
    struct st_rect_bitmap pattern;  //Ҫ���õ�ͼ��λͼ
};
struct gksc_para_set_transparentcolor
{
//    u16 command;
    struct gkwin_rsc *gkwin;        //���Ƶ�Ŀ�괰��
    u32 transparentcolor;           //Ҫ���õ�͸��ɫ   
};
struct gksc_para_set_bitmsk
{
//    u16 command;
    struct gkwin_rsc *gkwin;        //���Ƶ�Ŀ�괰��
    struct st_rect_bitmap bitmsk;   //Ҫ���õ�����λͼ   
};
struct gksc_para_set_direct_screen
{
    struct gkwin_rsc *gkwin;        //Ŀ�괰��
};
struct gksc_para_unset_direct_screen
{
    struct gkwin_rsc *gkwin;        //Ŀ�괰��
};
struct gksc_para_destroy_win
{
//    u16 command;
    struct gkwin_rsc *gkwin;        //Ҫ���ٵ�Ŀ�괰��
};
#ifdef __cplusplus
}
#endif

#endif //__GK_SYSCALL_H__

#endif //   for #if (cfg_gui_kernel == 1)

