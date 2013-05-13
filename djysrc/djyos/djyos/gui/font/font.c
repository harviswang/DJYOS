//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//版权所有 (C), 2004-2009,   lst.
//所属模块: 字体驱动
//作者：lst
//版本：V1.0.0
//文件描述: 字体资源初始化
//其他说明:
//修订历史:
//    2. ...
//    1. 日期:2009-11-21
//       作者:lst
//       新版本号：
//       修改说明:初始版本
//------------------------------------------------------
#include "os_inc.h"

#if (cfg_gui_kernel == 1)

#include "font.h"
#include "ascii8x8.h"
#include "ascii8x16.h"
#include "gb2312_16.h"
#include "string.h"

static struct font_rsc *pg_cur_font;


//----获取字体编码-------------------------------------------------------------
//功能: 根据字体名称获取字体资源，"C"是默认字体的代号。
//参数: 字体名称
//返回: 匹配的字符编码，若未有匹配编码，使用默认的ASCII编码
//-----------------------------------------------------------------------------
struct font_rsc* font_get_loc_font_rsc(const char* font_name)
{
    struct font_rsc* font;

    if(strcmp(font_name, "C") == 0){
        
        font = font_search_font_rsc(cfg_font_default);
        
    }else
    {
        font = font_search_font_rsc(font_name);
    }

    return font;   
}

//----安装字体-----------------------------------------------------------------
//功能: 把新字体安装到系统资源队列中
//参数: font，新增的字体资源指针
//      name，新增字体名
//返回: true
//-----------------------------------------------------------------------------
bool_t font_install_font(struct font_rsc *font, char* name)
{
    struct rsc_node *rsc;
    rsc = rsc_search_tree(cn_font_rsc_tree);
    if(rsc == NULL)
        return false;       //字符资源根结点未创建
        
    rsc_add_son(rsc,&font->node,sizeof(struct font_rsc),name);
    if(pg_cur_font == NULL)
        pg_cur_font = font;

    return true;
}

//----字体驱动模块初始化-------------------------------------------------------
//功能: 初始化字体驱动模块
//参数: para: 无效
//返回: 1=成功，0=失败
//-----------------------------------------------------------------------------
ptu32_t module_init_font(ptu32_t para)
{
    static struct rsc_node font_root_rsc;    // 编码资源

    pg_cur_font = NULL;
    // 添加字符编码资源根节点
    if(rsc_add_tree_root(&font_root_rsc, 
                            sizeof(struct rsc_node), 
                            cn_font_rsc_tree))
    {
        return 1;
    }else
    {
    	printf_ng("install font module fail\r\n");
        return 0;
    }
}

//----获取当前字符编码---------------------------------------------------------
//功能: 获取当前使用的字符编码
//返回: 当前字符编码
//-----------------------------------------------------------------------------
struct font_rsc* font_get_cur_font_rsc(void)
{
    return pg_cur_font;
}

//----设定当前字符编码---------------------------------------------------------
//功能: 把新字符编码设为当前使用的字符编码,新字符编码必须事先安装到系统中，如果
//      encoding资源未安装，本函数什么都不做，返回原来的当前字符编码
//参数: encoding，指定的字符编码
//返回: NULL，设定失败
//      设定之前的字符编码
//-----------------------------------------------------------------------------
struct font_rsc* font_set_cur_font_rsc(struct font_rsc* font)
{
    struct rsc_node *rsc;
    if(font == NULL)
        return NULL;
    rsc = rsc_search_tree(cn_font_rsc_tree);
    if(rsc == NULL)
        return NULL;       //字符资源树未创建
    rsc = rsc_search_son(rsc,font->node.name);
    if(rsc != NULL)
    {
        pg_cur_font = (struct font_rsc*)rsc;
    }
    return pg_cur_font;     
}

//----搜索字符编码资源---------------------------------------------------------
//功能: 根据名称搜索字符编码资源
//参数: name，指定的字符编码名称
//返回: NULL，无此编码资源
//      要找的字符编码资源
//-----------------------------------------------------------------------------
struct font_rsc* font_search_font_rsc(const char* name)
{
    struct rsc_node *rsc;
    
    rsc = rsc_search_tree(cn_font_rsc_tree);
    if(rsc == NULL)
        return NULL;       //字符资源树未创建    

    return (struct font_rsc*)rsc_search_son(rsc, (char*)name);
}

#endif      //for #if (cfg_gui_kernel == 1)

