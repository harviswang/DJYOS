//----------------------------------------------------
//Copyright (C), 2011,  .
//版权所有 (C), 2011,   djy.
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
#if (cfg_gui_kernel == 1)

#ifndef __ENCODING_H_
#define __ENCODING_H_

#ifdef __cplusplus
extern "C" {
#endif

#define cn_char_encoding_rsc_tree       "charset encoding"

/* 多字节字符编码资源 */
struct char_encoding_rsc
{
    struct rsc_node node;

    /* 单个字符的最大字节数 */
    u32 max_len; 

//----多字节字符转为ucs4字符---------------------------------------------------
//功能: 把一个多字节字符转换为ucs4字符。
//参数: pwc，保存转换结果的指针，调用方确保不为NULL,不判断，gui将确保本函数不
//      会被其他模块调用。
//      mbs, 指向待转换的多字节字符字节序列的指针(由调用函数判断s的合法性)
//      n，最大检测长度，
//返回: 0: pwc,mbs是NULL指针或者mbs指向空串。
//      -1:mbs指向的不是合法多字节字符，或者长度n内未能检测到完整多字节字符
//      其他:mbs缓冲区内第一个完整多字节字符的长度。
//说明：
//      此函数是C Run-time库的mblen及mbtowc服务函数。
//      传入的s指针必须非NULL
    s32 (*mb_to_ucs4)(u32* pwc, const char* mbs, s32 n);

//----ucs4字符转为多字节字符---------------------------------------------------
//功能: 把一个ucs4字符转换为多字节字符。
//参数: mb，保存转换结果的指针(由调用函数判断s的合法性)
//      wc，待转换的字符
//返回：-1,如果wc不能对应一个有效的多字节字符，
//      字节数，返回对应的多字节字符的字节数。
//说明：
//      此函数是C Run-time库的wctomb服务函数。
    s32 (*ucs4_to_mb)(char* mb, u32 wc);

//----多字节字符串转为ucs4串---------------------------------------------------
//功能: 把一个多字节字符串转换为ucs4字符串
//参数: pwcs，保存转换结果的指针，缓冲区由调用方提供，若空，则本函数转变为只计算
//          保存转换结果所需的字节数
//      mbs，保存多字节字符的缓冲区指针
//      n，最大检测长度，
//返回: 0: mbs是NULL指针
//      -1:结束条件到达前，有不能转换的字符
//      其他:得到的字符数，=n表示源串是不包含串结束符'\0'。
//-----------------------------------------------------------------------------
    s32 (*mbs_to_ucs4s)(u32* pwcs, const char* mbs, s32 n);

//----ucs4字符串转为多字节字符串-----------------------------------------------
//功能: 把一个ucs4字符串转换为多字节字符串。
//参数: mbs，保存转换结果的指针，缓冲区由调用方提供，若空，则本函数转变为只计算
//          保存转换结果所需的字节数
//      pwcs，待转换的字符
//      n，最大检测长度，遇串结束符或长度达到n结束转换，注意ucs4的结束符是连续
//          4个0x00.
//返回: 0: pwcs是NULL指针
//      -1:结束条件到达前，有不能转换的字符
//      其他:写入mbs缓冲区的字节数，含串结束符'\0'
//-----------------------------------------------------------------------------
    s32 (*ucs4s_to_mbs)(char* mbs, const u32* pwcs, s32 n);
};


ptu32_t module_init_char_encoding(ptu32_t para);
bool_t nls_install_char_encoding(struct char_encoding_rsc *encoding, char* name);
struct char_encoding_rsc* nls_get_cur_encoding_rsc(void);
struct char_encoding_rsc* nls_set_cur_encoding_rsc(struct char_encoding_rsc* encoding);
struct char_encoding_rsc* nls_search_char_encoding_rsc(const char* name);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif // __ENCODING_H_

#endif      //for #if (cfg_gui_kernel == 1)

