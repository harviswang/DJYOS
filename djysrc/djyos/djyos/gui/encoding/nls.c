//----------------------------------------------------
//Copyright (C), 2011,  .
//版权所有 (C), 2011,   djy.
//所属模块: National Language Support
//作者：mjh
//版本：V1.0.0
//文件描述: NLS
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

#include "gk_inc.h"
#include "nls.h"
#include <locale.h>
#include <string.h>

//----获取区域字符编码---------------------------------------------------------
//功能: 根据区域名称获取字符编码，"C"是默认编码的代号。
//参数: 区域名称编码
//返回: 匹配的字符编码，若未有匹配编码，使用默认的ASCII编码
//-----------------------------------------------------------------------------
struct char_encoding_rsc* nls_get_loc_encoding_rsc(const char* loc)
{
    struct char_encoding_rsc* encoding;

    if(strcmp(loc, "C") == 0)
    {
        encoding = nls_search_char_encoding_rsc(cfg_charset_default);
    }else
    {
        encoding = nls_search_char_encoding_rsc(loc);
    }

    return encoding;
}

//----初始化NLS模块------------------------------------------------------------
//功能: 初始化NLS模块
//返回: 1=成功，0=失败
//-----------------------------------------------------------------------------
ptu32_t module_init_nls(ptu32_t para)
{
    struct char_encoding_rsc* encoding;
    
    // 获取默认locale
    //loc = setlocale(LC_ALL, NULL);

    // 根据locale找出对应的字符编码资源
    encoding = nls_get_loc_encoding_rsc("C");
    nls_set_cur_encoding_rsc(encoding);
    
    printf_ng("-module_init_nls\r\n");

    return 1;
}

#endif      //for #if (cfg_gui_kernel == 1)

