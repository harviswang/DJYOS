//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//版权所有 (C), 2004-2009,   罗侍田.
//所属模块: gui
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: gui core的显示器部分，不包含硬件相关部分
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-11-21
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#if (cfg_gui_kernel == 1)

#ifndef __DISP_H__
#define __DISP_H__

#ifdef __cplusplus
extern "C" {
#endif

//显示器设备控制常数
#define cn_get_disp_size            3   //取显示器分辨率
#define cn_get_disp_font            4   //取显示器的默认字体指针
#define cn_get_disp_font_height     5   //取字高,以特定字符(汉字)为参数
#define cn_get_disp_font_width      6   //取字宽,以特定字符(汉字)为参数
#define cn_get_disp_line_height     7   //取最大字体高,即行高,
#define cn_get_disp_line_width      8   //取最大字体宽,在竖排文字显示时用得着,

//本结构列举了窗口中需要从显存中由driver分配的内存，并由driver操作。
//标识修改区域的显存不在这里分配
struct bmbuf_info
{
    struct st_rect_bitmap vbuf;        //位图阵列
    struct st_rect_bitmap rop4_msk;    //bit mask位块，支持rop4操作才有效
    u8 *pat;        //pattern指针，pattern的尺寸在由patx,paty指定
};

struct display_rsc
{
    struct rsc_node  node;  //注:镜像显示器作为子资源结点挂在这里
    float xmm,ymm;          //用毫米表示的显示器尺寸
    u32   width,height;     //按像素表示的显示器尺寸
    u32   patx,paty;        //pattern操作中pattern的尺寸，一般是8*8
    u16   pixel_format;     //像素格式，在gk_win.h中定义
    bool_t reset_clip;      //true = 本显示器需要重新生成可视域
    //帧缓冲直接显示。有些cpu使用系统内存做显存，设计driver时，可将显存直接作为
    //帧缓冲用。true=显存与帧缓冲重合
    bool_t framebuf_direct; 
    struct gkwin_rsc  *frame_buffer;   //本显示器的帧缓冲，以窗口管理
    struct gkwin_rsc  *desktop;     //本显示器的桌面
    struct gkwin_rsc *z_topmost;    //z轴最前端窗体指针
    //用于本显示器上的窗口的显示函数，登记显示器时，如果draw的任一成员为NULL，则
    //系统会自动根据pixel_format成员初始化draw的函数指针，否则不做任何操作。这就
    //允许用户添加系统未预设的显示器像素格式，此时，screen driver编写者应该自行
    //实现struct draw_in_win中所有成员函数。
    struct disp_draw draw;

    //可用于为应用程序提供兼容bitmap
//    u8 *(*bmmalloc)(u32 xsize,u32 ysize,u16 *pf_type);   //分配bitmap缓冲区

    //分配显存、pat_buf，如果buf->pf_type指定了rop4，同时分配的还有bitmsk_buf，
    //但不分配changed_msk，如果系统被设计为在堆上分配win_buffer和frame_buffer，
    //并且没有特殊的对齐要求，本指针可置为NULL
    //返回:true=成功，
    bool_t (*vmalloc)(struct bmbuf_info *buf,u32 xsize,u32 ysize,u32 rop_code);
    //重新分配显存，若原显存仍然符合要求，可能不会重新分配。
    //若需重新分配，必须采取先分配后释放的办法，因为djyos是多线程环境，先释放后
    //申请的话，虽然能少许增加分配的成功率，但释放后可能因抢占，内存被别的高优先
    //级事件抢去的话，就会造成分配失败。先分配后释放，如果分配失败，至少还可以
    //确保原来的显存不丢失，如果系统被设计为在堆上分配win_buffer和frame_buffer，
    //并且没有特殊的对齐要求，本指针可置为NULL
    //true = 分配了新的显存或无须分配,false = 分配失败，保持原显存
    bool_t (*vrmalloc)(struct bmbuf_info *buf,u32 xsize,u32 ysize,u32 rop_code);
    //如果系统被设计为在堆上分配win_buffer和frame_buffer，并且没有特殊对齐要求，
    //本指针可置为NULL
    void (*vfree)(struct bmbuf_info* buf);       //释放显存
    
    //控制显示器
    //这是由driver提供的一个应用程序的入口，该应用程序用于提供一个可视化的方式
    //设定该显示器所有可以由用户设定的参数，比如分辨率和和色彩参数。函数的功能
    //不做统一规定，驱动程序的文档应该提供函数的使用说明。
    //利用本函数，可以提供类似windows中设置显示器属性的功能。
    bool_t (*disp_ctrl)(struct display_rsc *disp);
    //格式化pattern，把用户提交的作为pattern的bitmap格式化成driver能识别的格式
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

