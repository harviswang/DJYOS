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
#if (cfg_gui_kernel == 1)

#ifndef __NLS_MODULE_H_
#define __NLS_MODULE_H_

#define cn_nls_charset_default          cn_nls_charset_gb2312

struct char_encoding_rsc* nls_get_loc_encoding_rsc(const char* loc);
ptu32_t module_init_nls(ptu32_t para);

#endif

#endif      //for #if (cfg_gui_kernel == 1)

