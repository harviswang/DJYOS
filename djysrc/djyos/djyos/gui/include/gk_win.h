//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块: gui
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: gui kernel主体部分
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-11-21
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
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
//位图像素色彩格式定义，
//客户格式，显卡driver内部可使用上述定义以外的私有格式。
//应用程序可以通过gk_api_get_pixel_format查询显卡实际使用的格式。
//如果显卡使用私有格式，则上层查询显卡格式时，返回0x8000(cn_custom_pf).
//应用程序绘制点、线、填充等功能时，必须使用cn_sys_pf_e8r8g8b8格式。
//应用程序绘制位图时，如果使用显卡实际使用的像素格式，将获得最优化的显示速度。
//带alpha的格式仅在含逐点alpha属性的bitmap中有效
#define cn_custom_pf            0x8000              //最高位为1，显卡自定义格式

#define cn_sys_pf_r5g6b5        (0x00 | (16<<8))    //16色，r在高位
#define cn_sys_pf_a1r5g5b5      (0x01 | (16<<8))    //15位色+1位alpha，a在高位
#define cn_sys_pf_gray1         (0x02 | (1<<8))    //单色
#define cn_sys_pf_gray2         (0x03 | (2<<8))    //2位4级灰度
#define cn_sys_pf_gray4         (0x04 | (4<<8))    //4位16级灰度
#define cn_sys_pf_gray8         (0x05 | (8<<8))    //8位灰度，用于黑白照片等
#define cn_sys_pf_r3g3b2        (0x06 | (8<<8))    //256色，r在高位
#define cn_sys_pf_r4g4b4e4      (0x07 | (16<<8))   //12位色,16位存储，占高12bit，r在高位
#define cn_sys_pf_e4r4g4b4      (0x08 | (16<<8))   //12位色,16位存储，占占低12bit，r在高位
#define cn_sys_pf_r5g5b5c1      (0x09 | (16<<8))   //16位色,r在高位，c为rgb共用低位
#define cn_sys_pf_e6r6g6b6      (0x0a | (24<<8))   //18位色,r在高位，24位存储，高6bit空
#define cn_sys_pf_e12r6g6b6     (0x0b | (32<<8))   //18位色,r在高位，32位存储，高14bit空
#define cn_sys_pf_r10g10b10     (0x0c | (32<<8))   //30位色,高两位空
#define cn_sys_pf_r8g8b8        (0x0d | (24<<8))    //24位色，r在高字节，
#define cn_sys_pf_e8r8g8b8      (0x0e | (32<<8)) //24位色，r在高字节，最高字节空
#define cn_sys_pf_a8r8g8b8      (0x0f | (32<<8))    //24位色+8位alpha，a在高字节
#define cn_sys_pf_palette1      (0x10 | (1<<8))     //双色，调色板
#define cn_sys_pf_palette4      (0x11 | (4<<8))     //4位色，调色板
#define cn_sys_pf_palette8      (0x12 | (8<<8))     //8位色，调色板
#define cn_sys_pf_last           0x12


//可以继续定义新格式，使之包含目标系统实际色彩格式，可以最大限度地优化显示速度，
//同时又不影响程序的可移植性。

//常用颜色定义
#define cn_color_red                (0xff<<16)
#define cn_color_green              (0xff<<8)
#define cn_color_blue               (0xff)
#define cn_color_black              (0)
#define cn_color_white              (0xffffff)

#define cn_winbuf_none      0       //没有缓冲区
#define cn_winbuf_buf       1       //有缓冲区

struct point_cdn;  //像素点坐标类型
struct st_rect;   //矩形定义
struct display_rsc;

//出错信息定义
enum gk_error_code
{
    enum_gk_no_error = cn_gk_no_error,      //没有错误
    enum_gk_gkwin_limit,                    //窗口数量太多
    enum_gk_no_memory,                      //显存不足
    enum_gk_blt_no_suport,                  //不支持的blt代码
    enum_gk_charset_install_error,          //安装字符集出错
    enum_gk_font_install_error,             //安装字体出错
};

struct point_cdn
{
    s32 x,y;       //像素坐标,坐标可以是负的(窗口外)
};

struct st_rect
{
    s32 left,top,right,bottom;      //坐标可以是负的,不包含right和bottom
};

struct st_rect_bitmap   //矩形位图
{
    u16 pf_type;            //像素色彩格式
    u32 width;              //位图的宽度(以像素为单位)
    u32 height;             //位图的高度(以像素为单位)
    u32 linebytes;          //一行用多少字节
    u8  *bm_bits;           //指向存储像素阵列的数组
};

struct st_any_bitmap    //任意形状位图
{
    u16 pf_type;             //像素色彩格式
    u8  *bm_bits;            //指向存储像素阵列的数组
    //........定义边界曲线，再说吧
};

// 字体资源链表，系统资源树上，安装了所有系统所支持的字体，每个窗口可从资源树上
// 选择本窗口使用的字体，可选多个，用本链表链起来。显示时，gui kernel将待显示的
// 字符逐个查看链表中的字体是否支持显示该字符。
struct font_rsc_link
{
    struct font_rsc_link *previous,*next;
    struct font_rsc *my_font;
};

//记录窗口修改参数
#define cn_gkwin_change_none    0   //没有修改
#define cn_gkwin_change_part    1   //部分修改
#define cn_gkwin_change_all     2   //全部修改

struct gk_chunnel       //用户程序(通常是gui windows) 和 gui kernel的通信管道
{
    struct ring_buf ring_syscall;           //gui 系统调用环形缓冲区
    struct ring_buf ring_usercall;          //gui 用户调用唤醒缓冲区。
    u8 syscall_buf[cfg_gk_down_chunnel_deep];     //为系统调用环形缓冲区分配内存
    u8 usercall_buf[cfg_gk_up_chunnel_deep];    //为用户调用唤醒缓冲区分配内存
    struct mutex_LCB *syscall_mutex;        //互斥量,用于防止并发写
    struct semaphore_LCB *syscall_semp;     //信号量，写时如果缓冲区满则等候。
//    struct mutex_LCB *usercall_mutex;       //互斥量,用于防止并发写
                                            //usercall ring只有gkwin单线程写
    struct semaphore_LCB *usercall_semp;    //信号量，写时如果缓冲区满则等候。
};
//可视域队列，每个窗口的可视域通过此队列连接
//虚拟窗口无可视域
//最小化的窗口无可视域
//被完全遮挡的窗口无可视域
//隐藏的窗口无可视域
struct clip_rect
{
    struct clip_rect *next,*previous;       //把可视域连成双向循环链表
    struct st_rect rect;                    //可是区域的位置和尺寸。
};
#define cn_gkwin_name_limit     127

//扩展rop_code代码定义:
//djygui兼容windows的rop码，并利用windows的rop中恒为0的bit15，作为功能扩展标志
#define cn_rop_alpha_en     0x0100  //允许alpha混合，bit0~7=alpha, 
#define cn_rop_double_alpha 0x0200  //允许双alpha混合，bit0~7=前景alpha，
                                        //bit16~23=背景alpha
#define cn_rop_pbp_alpha    0x0400  //逐点alpha混合,窗体像素格式只能是
                                        //cn_sys_pf_a8r8g8b8,cn_rop_alpha_en
                                        //和cn_rop_pbp_alpha只能1个置位  
#define cn_rop_transparentcolor     0x0800  //允许transparentcolor颜色透明
#define cn_rop_rop4_en      0x1000  //允许rop4功能，此时bit24~31表示背景操作码
#define cn_rop_ext_en       0x8000  //允许rop_code的扩展功能

#define cn_bound_limit      true
#define cn_bound_unlimit    false
//微言:原来定义了光标和输入焦点,但考虑到光标应该是应用程序的事,删除了.
struct gkwin_rsc                  //窗口资源定义
{
    struct rsc_node node;       //资源结点
    void *user_private;         //用户设置的标识，通常指向一个数据结构,
                                //如果=NULL，则指向gkwin自身
    struct gkwin_rsc *z_back;   //窗口Z序链表，从最前端往回看
    struct gkwin_rsc *z_top;
    char win_name[cn_gkwin_name_limit+1];         //窗口名字(标题)
    struct clip_rect *visible_bak;  //原可视域链表,双向循环链表
    u32 bak_num;
    u32 vis_num;
    u32 changed_num;
    u32 redraw_num;
    u32 copy_num;
    struct clip_rect *visible_clip; //新可视域链表,双向循环链表
    struct clip_rect *redraw_clip;  //需要刷新显示的可视域链表
    struct clip_rect *changed_clip; //被修改的区域链表
    struct clip_rect *copy_clip;    //用于visible_clip的临时备份
    struct display_rsc *disp;       //本窗口所属显示器
//    struct char_encoding_rsc *char_encoding;
//    struct font_rsc_link *font;
//    bool_t  locked;             //锁定后，窗口内容可以改变，但停止向显示器输出
//    bool_t  hided;              //true=隐藏，尺寸位置可变，但不显示.
                                //即使locked，由隐藏变可见时，都会引起一次输出
    sfast_t z_prio;             //前端优先级，-128~127，表示在兄弟窗口中的相对位
                                //置，0表示与父win同级，大于0表示被父win覆盖
    bool_t direct_screen;       //直接写屏属性，本属性为true，则所有对本win的绘
                                //制操作均直接画在screen上，无论是否有frame
                                //buffer和win buffer。
    u32 transparentcolor;       //透明显示透明色
    u32 rop_code;               //光栅操作代码，除windows定义的标准光栅操作码外，
                                //还包括alpha混合、正反transparentcolor透明
                                //只有缓冲窗口有效
    bool_t dest_blend;          //true窗口显示时需要和背景色混合，false=不需要
    struct st_rect_bitmap changed_msk;//用来标记窗口中的被改写区域，
                                      //每bit代表8*8像素
    struct st_rect_bitmap wm_bitmap; //窗口实体(暂为矩形，考虑升级为任意形状)
    //以下2个指针，由driver根据显卡(或模拟显卡)的功能决定是否分配内存和如何分配
    struct st_rect_bitmap bitmsk;//位掩码，每位一个像素，只有支持rop4才需要
    u8 *pat_buf;                //图案位图缓冲区，支持rop才有，由driver决定
    ufast_t change_flag;        //可取值:cn_gkwin_change_none等
    bool_t bound_limit;         //true=显示范围限制在父窗口边界内，任何显示器的
                                //直接放在桌面的窗口，必定受限
                                //false=不受父窗口边界限制,但如果父窗口是受限的，
                                //则限制于祖父窗口
    s32 absx0,absy0;            //窗口绝对位置，相对于所属screen的桌面原点，
    s32 left,top,right,bottom;  //窗口矩形，相对于父窗口，像素坐标
    
    s32 limit_left,limit_top; //被祖先窗口切割后的x边界，相对于父窗口
    s32 limit_right,limit_bottom; //被祖先窗口切割后的y边界，相对于父窗口
    
    u32 pen_color;              //画笔颜色，文本和图形均使用这个颜色，使用所属
                                //显示器的彩色表达
//    ufast_t draw_mode;          //文本、线条显示模式
//    s32 nib_x,nib_y;            //笔尖位置，即当前绘制点坐标，像素
//    u32 pen_width;              //画笔宽度,像素数
//    u32 canvas_color;           //画布颜色，同时也是文本背景色，使用所属
                                //显示器的彩色表达
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

