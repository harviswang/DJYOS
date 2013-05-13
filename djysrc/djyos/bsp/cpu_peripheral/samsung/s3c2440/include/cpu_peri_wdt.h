//----------------------------------------------------
//Copyright (C), 2004-2011,  djyos team.
//��Ȩ���� (C), 2004-2011,   djyos�Ŷ�.
//���ߣ�djyos�Ŷ�
//�汾��V1.0.0
//�ļ�����: s3c2440���Ź�ģ������
//����˵��:
//�޶���ʷ:
// 1. ����: 2011-11-29
//   ����: djyos�Ŷ�
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __cpu_peri_wdt_h__
#define __cpu_peri_wdt_h__

#ifdef __cplusplus
extern "C" {
#endif

#include "os_inc.h"
struct wdt_reg;

struct wdt_reg
{
    u32 WTCON;     //0x53000000
    u32 WTDAT;     //0x53000004
    u32 WTCNT;     //0x53000008
};

void wdt_star(void);
void wdt_reboot(void);
void wdt_hard_reset(void);
void wdt_clear(void);
void wdt_init_hard(void);
#ifdef __cplusplus
}
#endif

#endif //__cpu_peri_wdt_h__

