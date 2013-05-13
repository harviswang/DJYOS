//----------------------------------------------------
//Copyright (C), 2011,  mjh.
//版权所有 (C), 2011,   mjh.
//所属模块: 字符编码驱动
//作者：mjh
//版本：V1.0.0
//文件描述: 字符编码资源初始化
//其他说明:
//修订历史:
//    2. ...
//    1. 日期:2011-05-17
//       作者:mjh
//       新版本号：
//       修改说明:初始版本
//------------------------------------------------------
#include "os_inc.h"

#if (cfg_gui_kernel == 1)

#include "ascii.h"
#include "utf8.h"
#include "gb2312.h"

static struct char_encoding_rsc *pg_cur_char_encoding;    // 编码资源
//----安装字符编码-----------------------------------------------------------------
//功能: 把新字符编码安装到系统资源队列中
//参数: encoding，新增的字符编码资源指针
//      name，新增字符编码名
//返回: true
//-----------------------------------------------------------------------------
bool_t nls_install_char_encoding(struct char_encoding_rsc *encoding, char* name)
{
    struct rsc_node *rsc;
    rsc = rsc_search_tree(cn_char_encoding_rsc_tree);
    if(rsc == NULL)
        return false;       //字符资源根结点未创建
        
    rsc_add_son(rsc,&encoding->node,sizeof(struct char_encoding_rsc),name);    

    return true;
}

//----字符编码模块初始化-------------------------------------------------------
//功能: 初始化字符编码模块
//-----------------------------------------------------------------------------
ptu32_t module_init_char_encoding(ptu32_t para)
{
    static struct rsc_node encoding_root_rsc;    // 编码资源

    pg_cur_char_encoding = NULL;
    // 添加字符编码资源根节点
    if(rsc_add_tree_root(&encoding_root_rsc, 
                            sizeof(struct rsc_node), 
                            cn_char_encoding_rsc_tree))
    {
        return 1;
    }else
    {
    	printf_ng("install charset module fail\r\n");
        return 0;
    }
}

//----获取当前字符编码---------------------------------------------------------
//功能: 获取当前使用的字符编码
//返回: 当前字符编码
//-----------------------------------------------------------------------------
struct char_encoding_rsc* nls_get_cur_encoding_rsc(void)
{
    return pg_cur_char_encoding;
}

//----设定当前字符编码---------------------------------------------------------
//功能: 把新字符编码设为当前使用的字符编码,新字符编码必须事先安装到系统中，如果
//      encoding资源未安装，本函数什么都不做，返回原来的当前字符编码
//参数: encoding，指定的字符编码
//返回: NULL，设定失败
//      设定之前的字符编码
//-----------------------------------------------------------------------------
struct char_encoding_rsc* nls_set_cur_encoding_rsc(
                                struct char_encoding_rsc* encoding)
{
    struct rsc_node *rsc;
    if(encoding == NULL)
        return NULL;
    rsc = rsc_search_tree(cn_char_encoding_rsc_tree);
    if(rsc == NULL)
        return NULL;       //字符资源树未创建    
    rsc = rsc_search_son(rsc,encoding->node.name);
    if(rsc != NULL)
    {
        pg_cur_char_encoding = (struct char_encoding_rsc*)rsc;
    }
    return pg_cur_char_encoding;     
}

//----搜索字符编码资源---------------------------------------------------------
//功能: 根据名称搜索字符编码资源
//参数: name，指定的字符编码名称
//返回: NULL，无此编码资源
//      要找的字符编码资源
//-----------------------------------------------------------------------------
struct char_encoding_rsc* nls_search_char_encoding_rsc(const char* name)
{
    struct rsc_node *rsc;
    
    rsc = rsc_search_tree(cn_char_encoding_rsc_tree);
    if(rsc == NULL)
        return NULL;       //字符资源树未创建    

    return (struct char_encoding_rsc*)rsc_search_son(rsc, (char *)name);
}

#endif      //for #if (cfg_gui_kernel == 1)

