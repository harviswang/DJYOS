//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
//����ģ��:��������
//����:  ������.
//�汾��V1.0.0
//�ļ�����:��������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef _ENDIAN_H_
#define _ENDIAN_H_

#ifdef __cplusplus
extern "C" {
#endif

u32 __pick_little_16bit(u8 *buf,u32 index);
u32 __pick_little_32bit(u8 *buf,u32 index);
void __fill_little_16bit(u8 *buf,u32 index,u32 data);
void __fill_little_32bit(u8 *buf,u32 index,u32 data);
u32 __pick_big_16bit(u8 *buf,u32 index);
u32 __pick_big_32bit(u8 *buf,u32 index);
void __fill_big_16bit(u8 *buf,u32 index,u32 data);
void __fill_big_32bit(u8 *buf,u32 index,u32 data);

#ifdef __cplusplus
}
#endif

#endif // _ENDIAN_H_

