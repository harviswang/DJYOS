//----------------------------------------------------
//Copyright (C), 2004-2011,  ������.
//��Ȩ���� (C), 2004-2011,   ������.
//����ģ��:ʱ��ģ��
//����:  ������
//�汾��V1.0.0
//�ļ�����: �������������
//����˵��:
//�޶���ʷ:
// 1. ����: 2012-10-09
//   ����:  ������
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __CPU_PERI_CMD_H__
#define __CPU_PERI_CMD_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "multiplex.h"
#include <windows.h>

#define     cn_cmd_connect_recv_evtt    0
#define     cn_cmd_disconnect_recv_evtt 1

u32 cmd_int(ufast_t uart_int_line);
u32 WINAPI win32_scanf_pro( LPVOID lpParameter );
ptu32_t ModuleInstall_Cmd(ptu32_t para);
bool_t cmd_MultiplexAdd (ptu32_t PrivateTag,
                         struct tagMultiplexSetsCB *MultiplexSets,
                         u32 DevAlias,
                         u32 SensingBit);
ptu32_t cmd_DriverWrite(ptu32_t PrivateTag,u8 *buf,
                         u32 len,u32 offset,bool_t BlockOption,u32 timeout);
ptu32_t cmd_DriverRead(ptu32_t PrivateTag,u8 *buf,
                                     u32 len,u32 offset,u32 timeout);
ptu32_t cmd_DriverCtrl(ptu32_t PrivateTag,u32 cmd,
                   u32 data1,u32 data2);

#ifdef __cplusplus
}
#endif

#endif /*__CPU_PERI_CMD_H__*/

