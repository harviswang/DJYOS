//----------------------------------------------------
//Copyright (C), 2005-2009,  luoshitian.
//��Ȩ���� (C), 2005-2009,   ������.
//����ģ��:�����ļ�
//����:  ������.
//�汾��V1.0.0
//�ļ�����:���������в���ϵͳģ�����õ���ͷ�ļ�
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __os_inc_h__
#define __os_inc_h__

#ifdef __cplusplus
extern "C" {
#endif

#include "config-prj.h"
#include "stdint.h"
#include "arch.h"
#include "endian.h"
#include "kernel.h"
#include "critical.h"
#include "sys_init.h"
#include "int.h"
#include "int_hard.h"
#include "djyos.h"
#include "cpu.h"
#include "rsc.h"
#include "ring.h"
#include "line.h"
#include "lock.h"
#include "memory.h"
#include "memb.h"
#include "mems.h"
#include "driver.h"
#include "clock.h"
#include "pipe.h"
#include "wdt.h"
#include "file.h"
#include "flashfile.h"
#include "stdin_dev.h"
#include "key.h"
#include "touch.h"
#include "char_term.h"
#if(cfg_gui_kernel == 1)
    #include "gk_inc.h"
#endif
#if(cfg_nude_terminal == 1)
    #include "nude_io.h"
#endif
#if(cfg_mmu_used == 1)
    #include "mmu.h"
#endif
#include "version.h"

#ifdef __cplusplus
}
#endif

#endif //__os_inc_h__

