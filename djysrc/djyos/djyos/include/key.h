//----------------------------------------------------
//Copyright (C), 2004-2009,  luoshitian.
//��Ȩ���� (C), 2004-2009,   ������.
//����ģ��:����
//����:  ������.
//�汾��V1.0.0
//�ļ�����: �ṩ�����豸������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef _KEY_H_
#define _KEY_H_

#ifdef __cplusplus
extern "C" {
#endif

#if cfg_keyboard == 1

#define cn_break_code       0xf0    //���̶���

//��������
ptu32_t module_init_keyboard(ptu32_t para);
void key_scan(void);
bool_t key_hard_init(void);
ufast_t key_scan_hard(uint16_t *key);
#endif   //for #if cfg_keyboard == 1

#ifdef __cplusplus
}
#endif

#endif // _KEY_H_

