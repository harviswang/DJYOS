//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//��Ȩ���� (C), 2004-2009,   lst.
//����ģ��: ϵͳ��ʼ��
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: Ƭ��������ƼĴ����ṹ����
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-08-21
//   ����: lst
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __INFLASH_H__
#define __INFLASH_H__

#ifdef __cplusplus
extern "C" {
#endif

struct st_inflash_fpec_reg     //at 0x40022000
{
  vu32 ACR;
  vu32 KEYR;
  vu32 OPTKEYR;
  vu32 SR;
  vu32 CR;
  vu32 AR;
  vu32 RESERVED;
  vu32 OBR;
  vu32 WRPR;
};

struct st_inflash_ob_reg         //at 0x1FFFF800
{
  vu16 RDP;
  vu16 USER;
  vu16 Data0;
  vu16 Data1;
  vu16 WRP0;
  vu16 WRP1;
  vu16 WRP2;
  vu16 WRP3;
} ;

#ifdef __cplusplus
}
#endif

#endif /*__INFLASH_H__*/


