//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
//����ģ��: ԭʼ�������
//����:  ������.
//�汾��V1.0.0
//�ļ�����: nude_ioģ���ͷ�ļ�
//����˵��:
//�޶���ʷ:
//1. ����:2009-07-22
//   ����:  ������.
//   �°汾�ţ�1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __nude_io_h__
#define __nude_io_h__

#ifdef __cplusplus
extern "C" {
#endif

uint8_t scan_byte_nude(void);
bool_t read_byte_nude(uint8_t *byte);
void printf_byte_nude(uint8_t byte);
void printf_str_nude(uint8_t *str);
bool_t nude_terminal_init(void);

#ifdef __cplusplus
}
#endif

#endif //__nude_io_h__

