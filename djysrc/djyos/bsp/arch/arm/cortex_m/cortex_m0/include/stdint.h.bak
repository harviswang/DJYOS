//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//版权所有 (C), 2004-2009,   lst.
//所属模块:堆管理模块
//作者：lst
//版本：V1.0.0
//文件描述: 内核配置头文件
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-10-16
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __STDINT_H__
#define __STDINT_H__

#ifdef __cplusplus
extern "C" {
#endif


typedef unsigned  long long uint64_t;
typedef signed    long long sint64_t;
typedef  unsigned  int uint32_t;
typedef int                 sint32_t;
typedef int                 int32_t;    //
typedef unsigned  short     uint16_t;
typedef short int           sint16_t;
typedef short int           int16_t;    //
typedef unsigned  char      uint8_t;
typedef signed  char        sint8_t;
typedef signed  char        int8_t;        //
typedef uint64_t                u64;
typedef sint64_t                s64;
typedef uint32_t                u32;
typedef sint32_t                s32;
typedef uint16_t                u16;
typedef sint16_t                s16;
typedef uint8_t                 u8;
typedef sint8_t                 s8;
typedef uint64_t const          uc64;
typedef sint64_t const          sc64;
typedef uint32_t const          uc32;
typedef sint32_t const          sc32;
typedef uint16_t const          uc16;
typedef sint16_t const          sc16;
typedef uint8_t  const          uc8;
typedef sint8_t  const          sc8;
typedef unsigned char           ucpu_char;
typedef char                    scpu_char;

typedef volatile uint64_t       vu64;
typedef volatile sint64_t       vs64;
typedef volatile uint32_t       vu32;
typedef volatile sint32_t       vs32;
typedef volatile uint16_t       vu16;
typedef volatile sint16_t       vs16;
typedef volatile uint8_t        vu8;
typedef volatile sint8_t        vs8;
typedef volatile uint64_t const vuc64;
typedef volatile sint64_t const vsc64;
typedef volatile uint32_t const vuc32;
typedef volatile sint32_t const vsc32;
typedef volatile uint16_t const vuc16;
typedef volatile sint16_t const vsc16;
typedef volatile uint8_t  const vuc8;
typedef volatile sint8_t  const vsc8;


//ptu32_t类型与cpu的寻址范围有关，也与CPU的结构有关，如果该系统中指针长度小于或
//等于32位，则ptu32_t被定义为32位，否则与指针等长，它的用途有二:
//1.用于内存分配的指针限定,或者用于描述内存长度。
//2.用于可能需要转换成指针使用的整形数据，比如struct pan_device结构中的
//  ptu32_t  private_tag
//一般来说ptu32_t与ucpu_t相同,但也有意外,尤其是8位机和16位机中,例如51机的ucpu_t
//是8位的,但寻址范围是16位的,ptu32_t=16位.80c296单片机的ucpu_t是16位的，寻址范围
//是24位的，ptu32_t=32位。
typedef uint32_t                ptu32_t;

typedef uint32_t                ucpu_t;

typedef sint32_t                scpu_t;

typedef u8                      ufast_t;

typedef s8                      sfast_t;

typedef u64                     align_type;     //arm中，u64能满足所有对齐要求

#ifndef false
  #define false (ufast_t)0
#endif
#ifndef true
  #define true  (ufast_t)1
#endif

typedef ufast_t         bool_t;

//typedef s64             time_t;

#define cn_limit_uint64     0xffffffffffffffff
#define cn_limit_uint32     0xffffffff
#define cn_limit_uint16     0xffff
#define cn_limit_uint8      0xff
#define cn_limit_sint64     0x7fffffffffffffff
#define cn_limit_sint32     0x7fffffff
#define cn_limit_sint16     0x7fff
#define cn_limit_sint8      0x7f

#define cn_min_uint64       0
#define cn_min_uint32       0
#define cn_min_uint16       0
#define cn_min_uint8        0
#define cn_min_sint64       0x8000000000000000
#define cn_min_sint32       0x80000000
#define cn_min_sint16       0x8000
#define cn_min_sint8        0x80


#ifndef NULL
#define    NULL    0
#endif

#ifdef __cplusplus
}
#endif
#endif // __STDINT_H_


