//----------------------------------------------------
//Copyright (C), 2011,  .
//版权所有 (C), 2011,   djy.
//所属模块: GB2312-80标准字体驱动
//作者：mjh
//版本：V1.0.0
//文件描述: GB2312-80字体解析
//其他说明:
//修订历史:
//    2. ...
//    1. 日期:2011-05-17
//       作者:mjh
//       新版本号：
//       修改说明:初始版本
//------------------------------------------------------
#if (cfg_gui_kernel == 1)
#if (cfg_gb2312_8x16_1616 == 1)

#ifndef __GB2312_16_H_
#define __GB2312_16_H_

#ifdef __cplusplus
extern "C" {
#endif


//ascii:8*16,汉字16*16
#define cn_font_gb2312_8x16_1616    "gb2312_8_16_1616"

ptu32_t module_init_font_gb2312_1616(ptu32_t para);


#ifdef __cplusplus
}
#endif

#endif //__GB2312_16_H_

#endif      //#if (cfg_gb2312_8x16_1616 == 1)
#endif //   for #if (cfg_gui_kernel == 1)

