//----------------------------------------------------
//Copyright (C), 2011,  .
//版权所有 (C), 2011,   djy.
//所属模块: UTF-8字符编码驱动
//作者：mjh
//版本：V1.0.0
//文件描述: UTF-8字符编码解析
//其他说明:
//修订历史:
//    2. ...
//    1. 日期:2011-05-17
//       作者:mjh
//       新版本号：
//       修改说明:初始版本
//------------------------------------------------------
#if (cfg_charset_utf8 == 1)

#ifndef __UTF8_H_
#define __UTF8_H_

#define cn_nls_charset_utf8             "utf8"

#define ENCODING_UTF8_MAX_LEN 6

ptu32_t module_init_encode_utf8(ptu32_t para);

#endif      //__UTF8_H_

#endif      //for #if (cfg_charset_utf8 == 1)

