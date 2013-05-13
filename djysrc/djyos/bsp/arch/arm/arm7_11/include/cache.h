//----------------------------------------------------
//Copyright (C), 2004-2011,  luoshitian.
//版权所有 (C), 2004-2011,   罗侍田.
//所属模块:调度器
//作者:  罗侍田.
//版本：V1.0.0
//文件描述:cache操作相关的代码。
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2011-10-21
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __CACHE_H__
#define __CACHE_H__

#ifdef __cplusplus
extern "C" {
#endif

enum cache_operation_type
{
    write_to_no_invalid_no_lock,    //写通，不需要清除，不支持锁定
};
struct cache_type_flag
{
    u32 ints_cache_type:12;     //指令cache属性
    u32 data_cache_type:12;     //数据cache属性
    u32 uni_cache:1;            //1=指令和数据独立cache，0=统一cache
    u32 operation_type:4;       //cache操作类型。
};

#if cn_cache_used == 1

#if defined( arm720t)
    #define cn_cache_size       0x2000
    #define cn_cache_way        4
    #define cn_cache_line_size  16
#elif defined( arm740t)     //740t cache size可变，在arch.h中定义cache尺寸
    #define cn_cache_way        4
    #define cn_cache_line_size  16
#elif defined( arm920t)
    #define cn_cache_size       0x4000
    #define cn_cache_way        64
    #define cn_cache_line_size  32
    #define cn_c7_offset_set    5   //组索引在c7中的偏移位数
    #define cn_c7_offset_way    26  //路索引在c7中的偏移位数
    #define cn_c9_offset_way    26  //路索引在c9中的偏移位数
#elif defined( arm922t)
    #define cn_cache_size       0x2000
    #define cn_cache_way        64
    #define cn_cache_line_size  32
    #define cn_c7_offset_set    5   //组索引在c7中的偏移位数
    #define cn_c7_offset_way    26  //路索引在c7中的偏移位数
    #define cn_c9_offset_way    26  //路索引在c9中的偏移位数
#elif defined( arm940t)
    #define cn_cache_size       0x1000
    #define cn_cache_way        64
    #define cn_cache_line_size  16
    #define cn_c7_offset_set    4   //组索引在c7中的偏移位数
    #define cn_c7_offset_way    26  //路索引在c7中的偏移位数
    #define cn_c9_offset_way    0   //路索引在c9中的偏移位数
#elif defined( arm926ej_s)  //926ej_s cache size可变,在arch.h中定义cache尺寸
    #define cn_cache_way        4
    #define cn_cache_line_size  32
    #define cn_c7_offset_set    4   //组索引在c7中的偏移位数
    #define cn_c7_offset_way    30  //路索引在c7中的偏移位数
#elif defined( arm946e_s)    //946e_s cache size可变,在arch.h中定义cache尺寸
    #define cn_cache_way        4
    #define cn_cache_line_size  16
    #define cn_c7_offset_set    4   //组索引在c7中的偏移位数
    #define cn_c7_offset_way    30  //路索引在c7中的偏移位数
    #define cn_c9_offset_way    0   //路索引在c9中的偏移位数
#elif defined( arm1022e)
    #define cn_cache_size       0x4000
    #define cn_cache_way        64
    #define cn_cache_line_size  32
    #define cn_c7_offset_set    5   //组索引在c7中的偏移位数
    #define cn_c7_offset_way    26  //路索引在c7中的偏移位数
    #define cn_c9_offset_way    26  //路索引在c9中的偏移位数
#elif defined( arm1026ej_s)//1026ej_s cache size可变,在arch.h中定义cache尺寸
    #define cn_cache_way        4
    #define cn_cache_line_size  32
    #define cn_c7_offset_set    5   //组索引在c7中的偏移位数
    #define cn_c7_offset_way    30  //路索引在c7中的偏移位数
#elif defined( strong_arm)
    #define cn_cache_size       0x4000
    #define cn_cache_way        32
    #define cn_cache_line_size  16
#elif defined( xscale)
    #define cn_cache_size       0x8000
    #define cn_cache_way        32
    #define cn_cache_line_size  32
#endif

void cache_invalid_all(void);
void cache_invalid_inst(void);
void cache_invalid_data(void);
void cache_enable_all(void);
void cache_enable_inst(void);
void cache_enable_data(void);
void cache_disable_all(void);
void cache_disable_inst(void);
void cache_disable_data(void);
void cache_enable_write_buf(void);
void cache_disable_write_buf(void);
void cache_clean_data(void);


#else
//以下把cache操作函数全部置空。
#define cache_invalid_all()
#define cache_invalid_inst()
#define cache_invalid_data()
#define cache_enable_all()
#define cache_enable_inst()
#define cache_enable_data()
#define cache_disable_all()
#define cache_disable_inst()
#define cache_disable_data()
#define cache_enable_write_buf()
#define cache_disable_write_buf()
#define cache_clean_data()

#endif

#ifdef __cplusplus
}
#endif

#endif /*__CACHE_H__*/

