//----------------------------------------------------
//Copyright (C), 2011,  .
//版权所有 (C), 2011,   djy.
//所属模块: 多字节字符串处理
//作者：mjh
//版本：V1.0.0
//文件描述: 多字节字符串处理
//其他说明:
//修订历史:
//    2. ...
//    1. 日期:2011-05-17
//       作者:mjh
//       新版本号：
//       修改说明:初始版本
//------------------------------------------------------
#if (cfg_gui_kernel == 1)

#ifndef __LOC_STRING_H_
#define __LOC_STRING_H_

//#include <locale.h>

#ifdef __cplusplus
extern "C" {
#endif

s32 mblen(const char* s, u32 n);
s32 mbtowc(u32* pwc, const char* s, u32 n);
s32 mbstowcs(u32* pwc, const char* s, u32 n);
s32 wctomb(char* s, u32 wc);
s32 wcstombs(char* s, const u32* pwc, u32 n);

s32 mblen_l(const char* s, u32 n, struct char_encoding_rsc* locale);
s32 mbtowc_l(u32* pwc, const char* s, u32 n, struct char_encoding_rsc* locale);
s32 mbstowcs_l(u32* pwc, const char* s,u32 n,struct char_encoding_rsc* locale);
s32 wctomb_l(char* s, u32 wc, struct char_encoding_rsc* locale);
s32 wcstombs_l(char* s, const u32* pwc,u32 n,struct char_encoding_rsc* locale);

#ifdef __cplusplus
}
#endif

#endif

#endif      //for #if (cfg_gui_kernel == 1)

