//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//版权所有  (C), 2004-2009,   lst.
//所属模块: 字体驱动
//作者：lst
//版本：V1.0.0
//文件描述: 字体资源头文件
//其他说明:
//修订历史:
//    2. ...
//    1. 日期:2009-11-21
//       作者:lst
//       新版本号：
//       修改说明:初始版本
//------------------------------------------------------
#if (cfg_gui_kernel == 1)
#ifndef __FONT_H_
#define __FONT_H_

#ifdef __cplusplus
extern "C" {
#endif

#define cn_font_rsc_tree            "font"

/* 字体类型 */
#define cn_font_type_dot            1       // 点阵字体
#define cn_font_type_truetype       2       //矢量字体
struct font_info
{
    u8 type;
};

struct font_rsc
{
    struct  rsc_node node;

    struct font_info fi;

    bool_t (*load_font)(u8 *zk_addr);      //加载字体

    void (*unload_font)(void);      //卸载字体

// 获取ucs4编码为charcode字符的显示点阵，把点阵填充到font_bitmap中，调用者应该提
// 供font_bitmap所需内存。
// 如果font_bitmap参数中的bm_bits参数为NULL，则本函数退化为查询函数。在
// font_bitmap参数中返回图像参数，利用它可以计算bm_bits所需内存尺寸，这在一个文
// 档中使用多种尺寸的文字时，特别有用。
// resv,保留参数。
// 返回: true = 成功执行，false=不支持字符
    bool_t (*get_char_bitmap)(u32 charcode, u32 size, u32 resv,
                              struct st_rect_bitmap *font_bitmap);
};

bool_t font_install_font(struct font_rsc *font,char *name);

ptu32_t module_init_font(ptu32_t para);

struct font_rsc* font_get_cur_font_rsc(void);
struct font_rsc* font_set_cur_font_rsc(struct font_rsc* font);
struct font_rsc* font_search_font_rsc(const char* name);

#ifdef __cplusplus
}
#endif

#endif //__FONT_H_
#endif //   for #if (cfg_gui_kernel == 1)

