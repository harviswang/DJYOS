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

#ifndef __GK_WIN_H__
#define __GK_WIN_H__

#ifdef __cplusplus
extern "C" {
#endif

struct gksc_para_create_desktop;
struct gksc_para_create_gkwin;
struct gksc_para_set_rop_code;
struct gksc_para_set_pat_buf;
struct gksc_para_set_prio;
struct gksc_para_set_transparentcolor;
struct gksc_para_set_bitmsk;
struct gksc_para_destroy_win;
struct gksc_para_set_bound_mode;
struct gksc_para_move_win;
struct gksc_para_change_win_area;
struct gksc_para_set_direct_screen;
struct gksc_para_unset_direct_screen;
//λͼ����ɫ�ʸ�ʽ���壬
//�ͻ���ʽ���Կ�driver�ڲ���ʹ���������������˽�и�ʽ��
//Ӧ�ó������ͨ��gk_api_get_pixel_format��ѯ�Կ�ʵ��ʹ�õĸ�ʽ��
//����Կ�ʹ��˽�и�ʽ�����ϲ��ѯ�Կ���ʽʱ������0x8000(cn_custom_pf).
//Ӧ�ó�����Ƶ㡢�ߡ����ȹ���ʱ������ʹ��cn_sys_pf_e8r8g8b8��ʽ��
//Ӧ�ó������λͼʱ�����ʹ���Կ�ʵ��ʹ�õ����ظ�ʽ����������Ż�����ʾ�ٶȡ�
//��alpha�ĸ�ʽ���ں����alpha���Ե�bitmap����Ч
#define cn_custom_pf            0x8000              //���λΪ1���Կ��Զ����ʽ

#define cn_sys_pf_r5g6b5        (0x00 | (16<<8))    //16ɫ��r�ڸ�λ
#define cn_sys_pf_a1r5g5b5      (0x01 | (16<<8))    //15λɫ+1λalpha��a�ڸ�λ
#define cn_sys_pf_gray1         (0x02 | (1<<8))    //��ɫ
#define cn_sys_pf_gray2         (0x03 | (2<<8))    //2λ4���Ҷ�
#define cn_sys_pf_gray4         (0x04 | (4<<8))    //4λ16���Ҷ�
#define cn_sys_pf_gray8         (0x05 | (8<<8))    //8λ�Ҷȣ����ںڰ���Ƭ��
#define cn_sys_pf_r3g3b2        (0x06 | (8<<8))    //256ɫ��r�ڸ�λ
#define cn_sys_pf_r4g4b4e4      (0x07 | (16<<8))   //12λɫ,16λ�洢��ռ��12bit��r�ڸ�λ
#define cn_sys_pf_e4r4g4b4      (0x08 | (16<<8))   //12λɫ,16λ�洢��ռռ��12bit��r�ڸ�λ
#define cn_sys_pf_r5g5b5c1      (0x09 | (16<<8))   //16λɫ,r�ڸ�λ��cΪrgb���õ�λ
#define cn_sys_pf_e6r6g6b6      (0x0a | (24<<8))   //18λɫ,r�ڸ�λ��24λ�洢����6bit��
#define cn_sys_pf_e12r6g6b6     (0x0b | (32<<8))   //18λɫ,r�ڸ�λ��32λ�洢����14bit��
#define cn_sys_pf_r10g10b10     (0x0c | (32<<8))   //30λɫ,����λ��
#define cn_sys_pf_r8g8b8        (0x0d | (24<<8))    //24λɫ��r�ڸ��ֽڣ�
#define cn_sys_pf_e8r8g8b8      (0x0e | (32<<8)) //24λɫ��r�ڸ��ֽڣ�����ֽڿ�
#define cn_sys_pf_a8r8g8b8      (0x0f | (32<<8))    //24λɫ+8λalpha��a�ڸ��ֽ�
#define cn_sys_pf_palette1      (0x10 | (1<<8))     //˫ɫ����ɫ��
#define cn_sys_pf_palette4      (0x11 | (4<<8))     //4λɫ����ɫ��
#define cn_sys_pf_palette8      (0x12 | (8<<8))     //8λɫ����ɫ��
#define cn_sys_pf_last           0x12


//���Լ��������¸�ʽ��ʹ֮����Ŀ��ϵͳʵ��ɫ�ʸ�ʽ����������޶ȵ��Ż���ʾ�ٶȣ�
//ͬʱ�ֲ�Ӱ�����Ŀ���ֲ�ԡ�

//������ɫ����
#define cn_color_red                (0xff<<16)
#define cn_color_green              (0xff<<8)
#define cn_color_blue               (0xff)
#define cn_color_black              (0)
#define cn_color_white              (0xffffff)

#define cn_winbuf_none      0       //û�л�����
#define cn_winbuf_buf       1       //�л�����

struct point_cdn;  //���ص���������
struct st_rect;   //���ζ���
struct display_rsc;

//������Ϣ����
enum gk_error_code
{
    enum_gk_no_error = cn_gk_no_error,      //û�д���
    enum_gk_gkwin_limit,                    //��������̫��
    enum_gk_no_memory,                      //�Դ治��
    enum_gk_blt_no_suport,                  //��֧�ֵ�blt����
    enum_gk_charset_install_error,          //��װ�ַ�������
    enum_gk_font_install_error,             //��װ�������
};

struct point_cdn
{
    s32 x,y;       //��������,��������Ǹ���(������)
};

struct st_rect
{
    s32 left,top,right,bottom;      //��������Ǹ���,������right��bottom
};

struct st_rect_bitmap   //����λͼ
{
    u16 pf_type;            //����ɫ�ʸ�ʽ
    u32 width;              //λͼ�Ŀ��(������Ϊ��λ)
    u32 height;             //λͼ�ĸ߶�(������Ϊ��λ)
    u32 linebytes;          //һ���ö����ֽ�
    u8  *bm_bits;           //ָ��洢�������е�����
};

struct st_any_bitmap    //������״λͼ
{
    u16 pf_type;             //����ɫ�ʸ�ʽ
    u8  *bm_bits;            //ָ��洢�������е�����
    //........����߽����ߣ���˵��
};

// ������Դ����ϵͳ��Դ���ϣ���װ������ϵͳ��֧�ֵ����壬ÿ�����ڿɴ���Դ����
// ѡ�񱾴���ʹ�õ����壬��ѡ������ñ���������������ʾʱ��gui kernel������ʾ��
// �ַ�����鿴�����е������Ƿ�֧����ʾ���ַ���
struct font_rsc_link
{
    struct font_rsc_link *previous,*next;
    struct font_rsc *my_font;
};

//��¼�����޸Ĳ���
#define cn_gkwin_change_none    0   //û���޸�
#define cn_gkwin_change_part    1   //�����޸�
#define cn_gkwin_change_all     2   //ȫ���޸�

struct gk_chunnel       //�û�����(ͨ����gui windows) �� gui kernel��ͨ�Źܵ�
{
    struct ring_buf ring_syscall;           //gui ϵͳ���û��λ�����
    struct ring_buf ring_usercall;          //gui �û����û��ѻ�������
    u8 syscall_buf[cfg_gk_down_chunnel_deep];     //Ϊϵͳ���û��λ����������ڴ�
    u8 usercall_buf[cfg_gk_up_chunnel_deep];    //Ϊ�û����û��ѻ����������ڴ�
    struct mutex_LCB *syscall_mutex;        //������,���ڷ�ֹ����д
    struct semaphore_LCB *syscall_semp;     //�ź�����дʱ�������������Ⱥ�
//    struct mutex_LCB *usercall_mutex;       //������,���ڷ�ֹ����д
                                            //usercall ringֻ��gkwin���߳�д
    struct semaphore_LCB *usercall_semp;    //�ź�����дʱ�������������Ⱥ�
};
//��������У�ÿ�����ڵĿ�����ͨ���˶�������
//���ⴰ���޿�����
//��С���Ĵ����޿�����
//����ȫ�ڵ��Ĵ����޿�����
//���صĴ����޿�����
struct clip_rect
{
    struct clip_rect *next,*previous;       //�ѿ���������˫��ѭ������
    struct st_rect rect;                    //���������λ�úͳߴ硣
};
#define cn_gkwin_name_limit     127

//��չrop_code���붨��:
//djygui����windows��rop�룬������windows��rop�к�Ϊ0��bit15����Ϊ������չ��־
#define cn_rop_alpha_en     0x0100  //����alpha��ϣ�bit0~7=alpha, 
#define cn_rop_double_alpha 0x0200  //����˫alpha��ϣ�bit0~7=ǰ��alpha��
                                        //bit16~23=����alpha
#define cn_rop_pbp_alpha    0x0400  //���alpha���,�������ظ�ʽֻ����
                                        //cn_sys_pf_a8r8g8b8,cn_rop_alpha_en
                                        //��cn_rop_pbp_alphaֻ��1����λ  
#define cn_rop_transparentcolor     0x0800  //����transparentcolor��ɫ͸��
#define cn_rop_rop4_en      0x1000  //����rop4���ܣ���ʱbit24~31��ʾ����������
#define cn_rop_ext_en       0x8000  //����rop_code����չ����

#define cn_bound_limit      true
#define cn_bound_unlimit    false
//΢��:ԭ�������˹������뽹��,�����ǵ����Ӧ����Ӧ�ó������,ɾ����.
struct gkwin_rsc                  //������Դ����
{
    struct rsc_node node;       //��Դ���
    void *user_private;         //�û����õı�ʶ��ͨ��ָ��һ�����ݽṹ,
                                //���=NULL����ָ��gkwin����
    struct gkwin_rsc *z_back;   //����Z����������ǰ�����ؿ�
    struct gkwin_rsc *z_top;
    char win_name[cn_gkwin_name_limit+1];         //��������(����)
    struct clip_rect *visible_bak;  //ԭ����������,˫��ѭ������
    u32 bak_num;
    u32 vis_num;
    u32 changed_num;
    u32 redraw_num;
    u32 copy_num;
    struct clip_rect *visible_clip; //�¿���������,˫��ѭ������
    struct clip_rect *redraw_clip;  //��Ҫˢ����ʾ�Ŀ���������
    struct clip_rect *changed_clip; //���޸ĵ���������
    struct clip_rect *copy_clip;    //����visible_clip����ʱ����
    struct display_rsc *disp;       //������������ʾ��
//    struct char_encoding_rsc *char_encoding;
//    struct font_rsc_link *font;
//    bool_t  locked;             //�����󣬴������ݿ��Ըı䣬��ֹͣ����ʾ�����
//    bool_t  hided;              //true=���أ��ߴ�λ�ÿɱ䣬������ʾ.
                                //��ʹlocked�������ر�ɼ�ʱ����������һ�����
    sfast_t z_prio;             //ǰ�����ȼ���-128~127����ʾ���ֵܴ����е����λ
                                //�ã�0��ʾ�븸winͬ��������0��ʾ����win����
    bool_t direct_screen;       //ֱ��д�����ԣ�������Ϊtrue�������жԱ�win�Ļ�
                                //�Ʋ�����ֱ�ӻ���screen�ϣ������Ƿ���frame
                                //buffer��win buffer��
    u32 transparentcolor;       //͸����ʾ͸��ɫ
    u32 rop_code;               //��դ�������룬��windows����ı�׼��դ�������⣬
                                //������alpha��ϡ�����transparentcolor͸��
                                //ֻ�л��崰����Ч
    bool_t dest_blend;          //true������ʾʱ��Ҫ�ͱ���ɫ��ϣ�false=����Ҫ
    struct st_rect_bitmap changed_msk;//������Ǵ����еı���д����
                                      //ÿbit����8*8����
    struct st_rect_bitmap wm_bitmap; //����ʵ��(��Ϊ���Σ���������Ϊ������״)
    //����2��ָ�룬��driver�����Կ�(��ģ���Կ�)�Ĺ��ܾ����Ƿ�����ڴ����η���
    struct st_rect_bitmap bitmsk;//λ���룬ÿλһ�����أ�ֻ��֧��rop4����Ҫ
    u8 *pat_buf;                //ͼ��λͼ��������֧��rop���У���driver����
    ufast_t change_flag;        //��ȡֵ:cn_gkwin_change_none��
    bool_t bound_limit;         //true=��ʾ��Χ�����ڸ����ڱ߽��ڣ��κ���ʾ����
                                //ֱ�ӷ�������Ĵ��ڣ��ض�����
                                //false=���ܸ����ڱ߽�����,����������������޵ģ�
                                //���������游����
    s32 absx0,absy0;            //���ھ���λ�ã����������screen������ԭ�㣬
    s32 left,top,right,bottom;  //���ھ��Σ�����ڸ����ڣ���������
    
    s32 limit_left,limit_top; //�����ȴ����и���x�߽磬����ڸ�����
    s32 limit_right,limit_bottom; //�����ȴ����и���y�߽磬����ڸ�����
    
    u32 pen_color;              //������ɫ���ı���ͼ�ξ�ʹ�������ɫ��ʹ������
                                //��ʾ���Ĳ�ɫ���
//    ufast_t draw_mode;          //�ı���������ʾģʽ
//    s32 nib_x,nib_y;            //�ʼ�λ�ã�����ǰ���Ƶ����꣬����
//    u32 pen_width;              //���ʿ��,������
//    u32 canvas_color;           //������ɫ��ͬʱҲ���ı�����ɫ��ʹ������
                                //��ʾ���Ĳ�ɫ���
};
struct bmbuf_info;

ptu32_t module_init_gk(ptu32_t para);
struct gkwin_rsc *__gk_get_zsection_start(struct gkwin_rsc *gkwin);
struct gkwin_rsc *__gk_get_zsection_end(struct gkwin_rsc *gkwin);
struct gkwin_rsc *__gk_get_zsection_all_start(
                                                	struct gkwin_rsc *gkwin);
struct gkwin_rsc *__gk_get_zsection_all_end(
                                                	struct gkwin_rsc *gkwin);
struct gkwin_rsc *gk_create_desktop(struct gksc_para_create_desktop *para);
struct gkwin_rsc *gk_create_win(struct gksc_para_create_gkwin *para);
void gk_set_name(struct gkwin_rsc *gkwin,char *name);
void gk_set_unlock(struct gkwin_rsc *gkwin);
void gk_set_lock(struct gkwin_rsc *gkwin);
void gk_set_hide(struct gkwin_rsc *gkwin);
void gk_set_show(struct gkwin_rsc *gkwin);
void gk_move_win(struct gksc_para_move_win *para);
void __gk_set_bound(struct gkwin_rsc *gkwin);
void gk_set_bound_mode(struct gksc_para_set_bound_mode *para);
void gk_set_prio(struct gksc_para_set_prio *para);
bool_t gk_set_rop_code(struct gksc_para_set_rop_code *para);
bool_t gk_set_pat_buf(struct gksc_para_set_pat_buf *para);
bool_t gk_set_transparentcolor(
                            	struct gksc_para_set_transparentcolor *para);
bool_t gk_set_bitmsk(struct gksc_para_set_bitmsk *para);
void gk_set_direct_screen(struct gksc_para_set_direct_screen *para);
void gk_unset_direct_screen(struct gksc_para_unset_direct_screen *para);
void __gk_destroy_win_child(struct gkwin_rsc *gkwin);
void gk_destroy_win(struct gksc_para_destroy_win *para);
bool_t gk_change_win_area(struct gksc_para_change_win_area *para);
void gk_set_canvas_color(struct gkwin_rsc *gkwin,u32 color);
void gk_get_canvas_color(struct gkwin_rsc *gkwin,u32 *color);
void __gk_output_redraw(struct display_rsc *display);
void __gk_output_frame_buffer(struct display_rsc *display);
u16 gk_syscall_chunnel(u16 command,u32 sync_time,void *param1,u16 size1,
                                                void *param2,u16 size2);
u16 gk_usercall_chunnel(u16 usercall_id,struct semaphore_LCB *semp,
                        void **result,void *param,u16 size);
void gk_server(void);

#ifdef __cplusplus
}
#endif

#endif //__GK_WIN_H__

#endif //   for #if (cfg_gui_kernel == 1)

