//----------------------------------------------------
//Copyright (C), 2011,  .
//版权所有 (C), 2011,   djy.
//所属模块: ASCII字符编码驱动
//作者：mjh
//版本：V1.0.0
//文件描述: ASCII字符编码解析
//其他说明:
//修订历史:
//    2. ...
//    1. 日期:2011-05-17
//       作者:mjh
//       新版本号：
//       修改说明:初始版本
//------------------------------------------------------
#if (cfg_charset_ascii == 1)

#ifndef __GB2312_H_
#define __GB2312_H_

#ifdef __cplusplus
extern "C" {
#endif

#define cn_nls_charset_ascii            "ascii"

ptu32_t module_init_encode_ascii(ptu32_t para);

#ifdef __cplusplus
}
#endif

#endif //__GB2312_H_

#endif          //#if (cfg_charset_ascii == 1)

