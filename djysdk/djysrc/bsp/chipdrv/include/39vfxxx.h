//----------------------------------------------------
//Copyright (C), 2004-2009,  �����߲���ϵͳ�����Ŷ�
//��Ȩ���� (C), 2004-2009,   �����߲���ϵͳ�����Ŷ�
//����ģ��:sst39vf160����
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����:sst39vf160 flash���������ṩflash��ԭʼ��������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __flash_H__
#define __flash_H__
#ifdef __cplusplus
extern "C" {
#endif

#define flash_size              0x800000
#define flash_sector_length     4096
#define flash_sector_sum        (flash_size/flash_sector_length)
#define flash_block_length      65536
#define flash_block_sum         (flash_size/flash_block_length)
#define flash_base_addr         0x60000000
struct flash_info
{
    u16 mft_id;
    u16 device_id;
    u32 sector_size;
    u32 block_size;
    u32 block_sum;
};
#ifdef __cplusplus
}
#endif
#endif  //   __flash_H__
