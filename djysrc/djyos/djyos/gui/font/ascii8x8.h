//----------------------------------------------------
//Copyright (C), 2011,  .
//版权所有 (C), 2011,   djy.
//所属模块: ASCII 8x8点阵字体驱动
//作者：mjh
//版本：V1.0.0
//文件描述: ASCII 8x8点阵字体解析
//其他说明:
//修订历史:
//    2. ...
//    1. 日期:2011-05-17
//       作者:mjh
//       新版本号：
//       修改说明:初始版本
//------------------------------------------------------
#if (cfg_ascii_8x8 == 1)

#ifndef __ASCII8X8_H_
#define __ASCII8X8_H_

#ifdef __cplusplus
extern "C" {
#endif


//ascii:8*8 点阵字体
#define cn_font_ascii_8x8           "ascii8x8"

ptu32_t module_init_font_ascii_8x8(ptu32_t para);


#ifdef __cplusplus
}
#endif

#endif //__ASCII8X8_H_

#endif      //#if (cfg_ascii_8x8 == 1)


