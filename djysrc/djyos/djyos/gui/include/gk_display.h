//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
//����ģ��: gui
//����:  ������.
//�汾��V1.0.0
//�ļ�����: gui core����ʾ�����֣�������Ӳ����ز���
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-11-21
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#if (cfg_gui_kernel == 1)

#ifndef __DISP_H__
#define __DISP_H__

#ifdef __cplusplus
extern "C" {
#endif

//��ʾ���豸���Ƴ���
#define cn_get_disp_size            3   //ȡ��ʾ���ֱ���
#define cn_get_disp_font            4   //ȡ��ʾ����Ĭ������ָ��
#define cn_get_disp_font_height     5   //ȡ�ָ�,���ض��ַ�(����)Ϊ����
#define cn_get_disp_font_width      6   //ȡ�ֿ�,���ض��ַ�(����)Ϊ����
#define cn_get_disp_line_height     7   //ȡ��������,���и�,
#define cn_get_disp_line_width      8   //ȡ��������,������������ʾʱ�õ���,

//���ṹ�о��˴�������Ҫ���Դ�����driver������ڴ棬����driver������
//��ʶ�޸�������Դ治���������
struct bmbuf_info
{
    struct st_rect_bitmap vbuf;        //λͼ����
    struct st_rect_bitmap rop4_msk;    //bit maskλ�飬֧��rop4��������Ч
    u8 *pat;        //patternָ�룬pattern�ĳߴ�����patx,patyָ��
};

struct display_rsc
{
    struct rsc_node  node;  //ע:������ʾ����Ϊ����Դ����������
    float xmm,ymm;          //�ú��ױ�ʾ����ʾ���ߴ�
    u32   width,height;     //�����ر�ʾ����ʾ���ߴ�
    u32   patx,paty;        //pattern������pattern�ĳߴ磬һ����8*8
    u16   pixel_format;     //���ظ�ʽ����gk_win.h�ж���
    bool_t reset_clip;      //true = ����ʾ����Ҫ�������ɿ�����
    //֡����ֱ����ʾ����Щcpuʹ��ϵͳ�ڴ����Դ棬���driverʱ���ɽ��Դ�ֱ����Ϊ
    //֡�����á�true=�Դ���֡�����غ�
    bool_t framebuf_direct; 
    struct gkwin_rsc  *frame_buffer;   //����ʾ����֡���壬�Դ��ڹ���
    struct gkwin_rsc  *desktop;     //����ʾ��������
    struct gkwin_rsc *z_topmost;    //z����ǰ�˴���ָ��
    //���ڱ���ʾ���ϵĴ��ڵ���ʾ�������Ǽ���ʾ��ʱ�����draw����һ��ԱΪNULL����
    //ϵͳ���Զ�����pixel_format��Ա��ʼ��draw�ĺ���ָ�룬�������κβ��������
    //�����û����ϵͳδԤ�����ʾ�����ظ�ʽ����ʱ��screen driver��д��Ӧ������
    //ʵ��struct draw_in_win�����г�Ա������
    struct disp_draw draw;

    //������ΪӦ�ó����ṩ����bitmap
//    u8 *(*bmmalloc)(u32 xsize,u32 ysize,u16 *pf_type);   //����bitmap������

    //�����Դ桢pat_buf�����buf->pf_typeָ����rop4��ͬʱ����Ļ���bitmsk_buf��
    //��������changed_msk�����ϵͳ�����Ϊ�ڶ��Ϸ���win_buffer��frame_buffer��
    //����û������Ķ���Ҫ�󣬱�ָ�����ΪNULL
    //����:true=�ɹ���
    bool_t (*vmalloc)(struct bmbuf_info *buf,u32 xsize,u32 ysize,u32 rop_code);
    //���·����Դ棬��ԭ�Դ���Ȼ����Ҫ�󣬿��ܲ������·��䡣
    //�������·��䣬�����ȡ�ȷ�����ͷŵİ취����Ϊdjyos�Ƕ��̻߳��������ͷź�
    //����Ļ�����Ȼ���������ӷ���ĳɹ��ʣ����ͷź��������ռ���ڴ汻��ĸ�����
    //���¼���ȥ�Ļ����ͻ���ɷ���ʧ�ܡ��ȷ�����ͷţ��������ʧ�ܣ����ٻ�����
    //ȷ��ԭ�����Դ治��ʧ�����ϵͳ�����Ϊ�ڶ��Ϸ���win_buffer��frame_buffer��
    //����û������Ķ���Ҫ�󣬱�ָ�����ΪNULL
    //true = �������µ��Դ���������,false = ����ʧ�ܣ�����ԭ�Դ�
    bool_t (*vrmalloc)(struct bmbuf_info *buf,u32 xsize,u32 ysize,u32 rop_code);
    //���ϵͳ�����Ϊ�ڶ��Ϸ���win_buffer��frame_buffer������û���������Ҫ��
    //��ָ�����ΪNULL
    void (*vfree)(struct bmbuf_info* buf);       //�ͷ��Դ�
    
    //������ʾ��
    //������driver�ṩ��һ��Ӧ�ó������ڣ���Ӧ�ó��������ṩһ�����ӻ��ķ�ʽ
    //�趨����ʾ�����п������û��趨�Ĳ���������ֱ��ʺͺ�ɫ�ʲ����������Ĺ���
    //����ͳһ�涨������������ĵ�Ӧ���ṩ������ʹ��˵����
    //���ñ������������ṩ����windows��������ʾ�����ԵĹ��ܡ�
    bool_t (*disp_ctrl)(struct display_rsc *disp);
    //��ʽ��pattern�����û��ύ����Ϊpattern��bitmap��ʽ����driver��ʶ��ĸ�ʽ
    bool_t (*format_pattern)(u8 *pattern,struct st_rect_bitmap *bitmap);
};

bool_t gk_install_display(struct display_rsc *display,
                          char *name);
bool_t gk_install_display_mirror(struct display_rsc *base_display,
                                 struct display_rsc *mirror_display,char *name);
bool_t gk_set_default_display(char *name);
struct gkwin_rsc *gk_get_root_win(struct display_rsc *display);
bool_t gk_switch_frame_buffer(struct display_rsc *display,
                              struct st_rect_bitmap *fbuf);
struct st_rect_bitmap *gk_create_frame_buffer(struct display_rsc *display);

#ifdef __cplusplus
}
#endif

#endif //__DISP_H__

#endif //   for #if (cfg_gui_kernel == 1)

