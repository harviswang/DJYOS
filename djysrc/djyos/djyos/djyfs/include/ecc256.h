//----------------------------------------------------
//Copyright (C), 2004-2009,  网络.
//版权所有 (C), 2004-2009,   网络.
//所属模块:flash文件系统
//作者：网络
//版本：V1.0.0
//文件描述:flash文件系统中ECC校验部分
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef _ECC256_H_
#define _ECC256_H_

#ifdef __cplusplus
extern "C" {
#endif

#if ((cfg_djyfs == 1) && (cn_run_mode != cn_mode_tiny) && (cfg_djyfs_flash == 1))

u32 ecc_count_bits(u8 x);
void ecc_make_256(const u8 *data,u8 *ecc);
s32 ecc_corect_256(u8 *data, const u8 *old_ecc);

#endif //#if ((cfg_djyfs == 1) && (cn_run_mode != cn_mode_tiny) && (cfg_djyfs_flash == 1))

#ifdef __cplusplus
}
#endif

#endif // _ECC256_H_

