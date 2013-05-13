/*
 * mmu.h
 *
 *  Created on: 2011-12-5
 *      Author: djyos
 */

#ifndef __MMU_H_
#define __MMU_H_

#ifdef __cplusplus
extern "C" {
#endif

extern u32 mmu_page_table[];

// cp15协处理器中的寄存器定义
// c0 - ID编码（只读）

// c1 - 控制寄存器
#define bm_cp15_c1_mmu_en 0x00000001 //使能MMU或者PU
#define bo_cp15_c1_mmu_en 0
#define bm_cp15_c1_mem_align_check_en 0x00000002 //使能内存访问时的地址对齐检查
#define bo_cp15_c1_mem_align_check_en 1
#define bm_cp15_c1_cache_data_en 0x00000004 //使能数据Cache（如果指令和数据Cache统一时，设置这个位会同时禁止指令和数据Cache）
#define bo_cp15_c1_cache_data_en 2
#define bm_cp15_c1_write_buffer_en 0x00000008 //使能写缓冲
#define bo_cp15_c1_write_buffer_en 3
#define bm_cp15_c1_mem_big_endian 0x00000080 //选择使用big-endian内存模式（仅对于两种端模式都支持的ARM才有效）
#define bo_cp15_c1_mem_big_endian 7
#define bm_cp15_c1_sys_protection 0x00000100 //在基于MMU的存储系统中，本控制位用作系统保护
#define bo_cp15_c1_sys_protection 8
#define bm_cp15_c1_rom_protection 0x00000200 //在基于MMU的存储系统中，本控制位用作ROM保护
#define bo_cp15_c1_rom_protection 9
#define bm_cp15_c1_br_prediction_en 0x00000800 //使能跳转预测功能（如果支持）
#define bo_cp15_c1_br_prediction_e
#define bm_cp15_c1_cache_instruction_en 0x00001000 //使能指令Cache
#define bo_cp15_c1_cache_instruction_en 12
#define bm_cp15_c1_exceptions_location 0x00002000 //异常中断向量表的位置选择（0:0x0-0x1c;1:0xFFFF0000-0xFFFF001C）
#define bo_cp15_c1_exceptions_location 13
#define bm_cp15_c1_rr_strategy 0x00004000 //系统中Cache的淘汰算法（0:随机淘汰；1:Round-robin淘汰算法）
#define bo_cp15_c1_rr_strategy 14

// 一级描述符中各个位的定义（段定义）
#define bm_first_level_des_ap 0x00000C00 //AP两个位
#define bo_first_level_des_ap 10
#define bm_first_level_des_domain 0x000001E0 //域
#define bo_first_level_des_domain 5
#define bm_first_level_des_cache 0x00000008 //C位
#define bo_first_level_des_cache 3 //C位
#define bm_first_level_des_buffer 0x00000004 //B位
#define bo_first_level_des_buffer 2 //B位
#ifdef __cplusplus
}
#endif

#endif /* __MMU_H_ */
