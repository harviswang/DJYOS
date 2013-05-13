//========================================================  
// �ļ�ģ��˵��:                                            
// C6000�жϹ���                         
// �ļ��汾: v1.00                                          
// ������Ա: lst                                            
// ����ʱ��: 2011.06.01                                     
// Copyright(c) 2011-2011  ��������������޹�˾             
//========================================================  
// �����޸ļ�¼(���µķ�����ǰ��):                            
//  <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>            
//========================================================  
//���磺
// V1.02 2004-07-14, ����: ���պ������ӶԱ��ĳ�����Ч��У��

#ifndef __OMAPL138_DCORE_H__
#define __OMAPL138_DCORE_H__

#ifdef __cplusplus
extern "C" {
#endif

void (*Dcore_LoadTiBinCode(u8 *code_buf))(void);
void (*Dcore_LoadAisCode(u8 *code_buf))(void);
void Dcore_WakeupDsp(void (*start_address)(void));
void Dcore_AssertDspIntNmi(void);
void Dcore_AssertDspIntCi2(void);
void Dcore_AssertDspIntCi3(void);
void Dcore_AssertArmIntCi0(void);
void Dcore_AssertArmIntCi1(void);
void Dcore_AssertArmIntCi2(void);
void Dcore_AssertArmIntCi3(void);
void Dcore_ClearDspIntNmi(void);
void Dcore_ClearDspIntCi2(void);
void Dcore_ClearDspIntCi3(void);
void Dcore_ClearArmIntCi0(void);
void Dcore_ClearArmIntCi1(void);
void Dcore_ClearArmIntCi2(void);
void Dcore_ClearArmIntCi3(void);

#ifdef __cplusplus
}
#endif

#endif //__OMAPL138_DCORE_H__

