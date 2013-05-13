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

// cp15Э�������еļĴ�������
// c0 - ID���루ֻ����

// c1 - ���ƼĴ���
#define bm_cp15_c1_mmu_en 0x00000001 //ʹ��MMU����PU
#define bo_cp15_c1_mmu_en 0
#define bm_cp15_c1_mem_align_check_en 0x00000002 //ʹ���ڴ����ʱ�ĵ�ַ������
#define bo_cp15_c1_mem_align_check_en 1
#define bm_cp15_c1_cache_data_en 0x00000004 //ʹ������Cache�����ָ�������Cacheͳһʱ���������λ��ͬʱ��ָֹ�������Cache��
#define bo_cp15_c1_cache_data_en 2
#define bm_cp15_c1_write_buffer_en 0x00000008 //ʹ��д����
#define bo_cp15_c1_write_buffer_en 3
#define bm_cp15_c1_mem_big_endian 0x00000080 //ѡ��ʹ��big-endian�ڴ�ģʽ�����������ֶ�ģʽ��֧�ֵ�ARM����Ч��
#define bo_cp15_c1_mem_big_endian 7
#define bm_cp15_c1_sys_protection 0x00000100 //�ڻ���MMU�Ĵ洢ϵͳ�У�������λ����ϵͳ����
#define bo_cp15_c1_sys_protection 8
#define bm_cp15_c1_rom_protection 0x00000200 //�ڻ���MMU�Ĵ洢ϵͳ�У�������λ����ROM����
#define bo_cp15_c1_rom_protection 9
#define bm_cp15_c1_br_prediction_en 0x00000800 //ʹ����תԤ�⹦�ܣ����֧�֣�
#define bo_cp15_c1_br_prediction_e
#define bm_cp15_c1_cache_instruction_en 0x00001000 //ʹ��ָ��Cache
#define bo_cp15_c1_cache_instruction_en 12
#define bm_cp15_c1_exceptions_location 0x00002000 //�쳣�ж��������λ��ѡ��0:0x0-0x1c;1:0xFFFF0000-0xFFFF001C��
#define bo_cp15_c1_exceptions_location 13
#define bm_cp15_c1_rr_strategy 0x00004000 //ϵͳ��Cache����̭�㷨��0:�����̭��1:Round-robin��̭�㷨��
#define bo_cp15_c1_rr_strategy 14

// һ���������и���λ�Ķ��壨�ζ��壩
#define bm_first_level_des_ap 0x00000C00 //AP����λ
#define bo_first_level_des_ap 10
#define bm_first_level_des_domain 0x000001E0 //��
#define bo_first_level_des_domain 5
#define bm_first_level_des_cache 0x00000008 //Cλ
#define bo_first_level_des_cache 3 //Cλ
#define bm_first_level_des_buffer 0x00000004 //Bλ
#define bo_first_level_des_buffer 2 //Bλ
#ifdef __cplusplus
}
#endif

#endif /* __MMU_H_ */
