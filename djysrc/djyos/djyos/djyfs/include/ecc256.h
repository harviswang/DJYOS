//----------------------------------------------------
//Copyright (C), 2004-2009,  ����.
//��Ȩ���� (C), 2004-2009,   ����.
//����ģ��:flash�ļ�ϵͳ
//���ߣ�����
//�汾��V1.0.0
//�ļ�����:flash�ļ�ϵͳ��ECCУ�鲿��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
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

