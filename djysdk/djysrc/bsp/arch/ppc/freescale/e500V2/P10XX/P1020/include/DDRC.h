//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
// ģ������:p1020_ddrcontroller_ctl.h/WB8298
// ģ��汾: V1.0.0
// ������Ա: zhangqf_cyg
// ����ʱ��: 10:37:15 AM/Dec 4, 2013
// ��         ע  ��
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================

#ifndef __DDRC_H
#define __DDRC_H

#include "SysSpace.h"

#define DEBUG_3_ADDR    (CN_CCSR_NEWADDR_V +0x2F08)
#define DEBUG_13_ADDR   (CN_CCSR_NEWADDR_V +0x2F30)
#define DEBUG_22_ADDR   (CN_CCSR_NEWADDR_V +0x2F54)
#define DEBUG_2_ADDR    (CN_CCSR_NEWADDR_V +0x2F04)

#define DDRmc1_CS0_BNDS_ADDR         (CN_CCSR_NEWADDR_V +0x2000)
#define DDRmc1_CS1_BNDS_ADDR         (CN_CCSR_NEWADDR_V +0x2008)
#define DDRmc1_CS0_CONFIG_ADDR       (CN_CCSR_NEWADDR_V +0x2080)
#define DDRmc1_CS1_CONFIG_ADDR       (CN_CCSR_NEWADDR_V +0x2084)
#define DDRmc1_CS0_CONFIG_2_ADDR     (CN_CCSR_NEWADDR_V +0x20C0)
#define DDRmc1_CS1_CONFIG_2_ADDR     (CN_CCSR_NEWADDR_V +0x20C4)
#define DDRmc1_TIMING_CFG_0_ADDR     (CN_CCSR_NEWADDR_V +0x2104)
#define DDRmc1_TIMING_CFG_1_ADDR     (CN_CCSR_NEWADDR_V +0x2108)
#define DDRmc1_TIMING_CFG_2_ADDR     (CN_CCSR_NEWADDR_V +0x210C)
#define DDRmc1_TIMING_CFG_3_ADDR     (CN_CCSR_NEWADDR_V +0x2100)
#define DDRmc1_TIMING_CFG_4_ADDR     (CN_CCSR_NEWADDR_V +0x2160)
#define DDRmc1_TIMING_CFG_5_ADDR     (CN_CCSR_NEWADDR_V +0x2164)
#define DDRmc1_SDRAM_CFG_ADDR        (CN_CCSR_NEWADDR_V +0x2110)
#define DDRmc1_SDRAM_CFG_2_ADDR      (CN_CCSR_NEWADDR_V +0x2114)
#define DDRmc1_SDRAM_INTERVAL_ADDR   (CN_CCSR_NEWADDR_V +0x2124)
#define DDRmc1_SDRAM_CLK_CNTL_ADDR   (CN_CCSR_NEWADDR_V +0x2130)
#define DDRmc1_ZQ_CNTL_ADDR          (CN_CCSR_NEWADDR_V +0x2170)
#define DDRmc1_WRLVL_CNTL_ADDR       (CN_CCSR_NEWADDR_V +0x2174)
#define DDRmc1_WRLVL_CNTL_2_ADDR     (CN_CCSR_NEWADDR_V +0x2190)
#define DDRmc1_WRLVL_CNTL_3_ADDR     (CN_CCSR_NEWADDR_V +0x2194)
#define DDRmc1_SDRAM_MODE_ADDR       (CN_CCSR_NEWADDR_V +0x2118)
#define DDRmc1_SDRAM_MODE_2_ADDR     (CN_CCSR_NEWADDR_V +0x211C)

#define DDRmc1_INIT_ADDR_ADDR        (CN_CCSR_NEWADDR_V +0x2148)

#define DDRmc1_INIT_EXT_ADDR_ADDR    (CN_CCSR_NEWADDR_V +0x214C)
#define DDRmc1_SDRAM_RCW_1_ADDR      (CN_CCSR_NEWADDR_V +0x2180)
#define DDRmc1_SDRAM_RCW_2_ADDR      (CN_CCSR_NEWADDR_V +0x2184)
#define DDRmc1_DATA_INIT_ADDR        (CN_CCSR_NEWADDR_V +0x2128)
#define DDRmc1_SDRAM_MD_CNTL_ADDR    (CN_CCSR_NEWADDR_V +0x2120)
#define DDRmc1_DDRCDR_1_ADDR         (CN_CCSR_NEWADDR_V +0x2B28)
#define DDRmc1_DDRCDR_2_ADDR         (CN_CCSR_NEWADDR_V +0x2B2C)

/* Errata: B4, T4 for v1.0
Before enabling the DDR controller, set the DDR memory mapped register space at offset 0xF70 to a value of 0x30003000."*/

#define DDRmc1_DDR_MEMORY_MAPPED_SPACE        (CN_CCSR_NEWADDR_V +0x2F70)
//#define DDRmc1_DDR_MEMORY_MAPPED_SPACE_VAL      0x30003000
//
//#define SDRAM_CFG_MEM_EN_MASK       0x80000000
//#define SDRAM_CFG2_D_INIT_MASK      0x00000010
//#define TIMING_CFG2_CPO_MASK            0xF07FFFFF
//#define CS0_BNDS_DEBUG3_MASK            0x00000400
//#define CS0_BNDS_DEBUG13_MASK           0x00000015
//#define CS0_BNDS_DEBUG22_MASK           0x24000000
//#define ZQ_CNTL_ZQ_EN_DISABLE_MASK      0x7FFFFFFF
//#define SDRAM_INTERVAL_REFINT_MASK  0x0000FFFF
//#define SDRAM_MD_CNTL_MASK                      0x84080000
//#define SDRAM_MD_CNTL_MD_EN_MASK        0x80000000
//#define SDRAM_MD_CNTL_CKE_CNTL_MASK     0x00200000
//#define WRLVL_CNTL_WRLVL_EN_DISABLE_MASK        0x7FFFFFFF
//#define SDRAM_CFG2_RCW_D_INT_DISABLE_MASK       0xFFFFFFEB
//
///* DDR Controller configured registers' values */
//#define DDRmc1_CS0_BNDS_VAL         0x3F
//#define DDRmc1_CS1_BNDS_VAL         0x00
//#define DDRmc1_CS0_CONFIG_VAL       0x80014302
//#define DDRmc1_CS1_CONFIG_VAL       0x00
//#define DDRmc1_CS0_CONFIG_2_VAL     0x00
//#define DDRmc1_CS1_CONFIG_2_VAL     0x00
//
//#define DDRmc1_TIMING_CFG_0_VAL     0x40210104
//#define DDRmc1_TIMING_CFG_1_VAL     0x5C5BE544
//#define DDRmc1_TIMING_CFG_2_VAL     0x0FA8910B
//#define DDRmc1_TIMING_CFG_3_VAL     0x00020000
//#define DDRmc1_TIMING_CFG_4_VAL     0x01
//#define DDRmc1_TIMING_CFG_5_VAL     0x02401400
//#define DDRmc1_SDRAM_CFG_VAL        0xC70C0008
//#define DDRmc1_SDRAM_CFG_2_VAL      0x24401010
//#define DDRmc1_SDRAM_INTERVAL_VAL   0x0A28028A
//#define DDRmc1_SDRAM_CLK_CNTL_VAL   0x02000000
//#define DDRmc1_ZQ_CNTL_VAL          0x89080600
//#define DDRmc1_WRLVL_CNTL_VAL       0x8655F605
//#define DDRmc1_WRLVL_CNTL_2_VAL     0x00
//#define DDRmc1_WRLVL_CNTL_3_VAL     0x00
//#define DDRmc1_SDRAM_MODE_VAL       0x00041220
//#define DDRmc1_SDRAM_MODE_2_VAL     0x00
//
//#define DDRmc1_DDRCDR_1_VAL         0x80040000
//#define DDRmc1_DDRCDR_2_VAL         0x00
//#define DDRmc1_INIT_ADDR_VAL        0x00000000
//
//#define DDRmc1_INIT_EXT_ADDR_VAL    0x00000000
//#define DDRmc1_SDRAM_RCW_1_VAL      0x00
//#define DDRmc1_SDRAM_RCW_2_VAL      0x00
//#define DDRmc1_DATA_INIT_VAL        0xDEADBEEF
//#define DDRmc1_SDRAM_MD_CNTL_VAL    0x00000000
#define DDRmc1_DDR_MEMORY_MAPPED_SPACE_VAL      0x30003000

#define SDRAM_CFG_MEM_EN_MASK       0x80000000
#define SDRAM_CFG2_D_INIT_MASK      0x00000010
#define TIMING_CFG2_CPO_MASK            0xF07FFFFF
#define CS0_BNDS_DEBUG3_MASK            0x00000400
#define CS0_BNDS_DEBUG13_MASK           0x00000015
#define CS0_BNDS_DEBUG22_MASK           0x24000000
#define ZQ_CNTL_ZQ_EN_DISABLE_MASK      0x7FFFFFFF
#define SDRAM_INTERVAL_REFINT_MASK  0x0000FFFF
#define SDRAM_MD_CNTL_MASK                      0x84080000
#define SDRAM_MD_CNTL_MD_EN_MASK        0x80000000
#define SDRAM_MD_CNTL_CKE_CNTL_MASK     0x00200000
#define WRLVL_CNTL_WRLVL_EN_DISABLE_MASK        0x7FFFFFFF
#define SDRAM_CFG2_RCW_D_INT_DISABLE_MASK       0xFFFFFFEB

/* DDR Controller configured registers' values */
#define DDRmc1_CS0_BNDS_VAL         0x3F
#define DDRmc1_CS1_BNDS_VAL         0x00
#define DDRmc1_CS0_CONFIG_VAL       0x80014302
#define DDRmc1_CS1_CONFIG_VAL       0x00
#define DDRmc1_CS0_CONFIG_2_VAL     0x00
#define DDRmc1_CS1_CONFIG_2_VAL     0x00

#define DDRmc1_TIMING_CFG_0_VAL     0x40210104
#define DDRmc1_TIMING_CFG_1_VAL     0x6D6BE544
#define DDRmc1_TIMING_CFG_2_VAL     0x0FA8918E
#define DDRmc1_TIMING_CFG_3_VAL     0x00020000
#define DDRmc1_TIMING_CFG_4_VAL     0x01
#define DDRmc1_TIMING_CFG_5_VAL     0x02401400
#define DDRmc1_SDRAM_CFG_VAL        0xC70C0008
#define DDRmc1_SDRAM_CFG_2_VAL      0x24401010
#define DDRmc1_SDRAM_INTERVAL_VAL   0x0A28028A
#define DDRmc1_SDRAM_CLK_CNTL_VAL   0x02000000
#define DDRmc1_ZQ_CNTL_VAL          0x89080600
#define DDRmc1_WRLVL_CNTL_VAL       0x8655F605
#define DDRmc1_WRLVL_CNTL_2_VAL     0x00
#define DDRmc1_WRLVL_CNTL_3_VAL     0x00
#define DDRmc1_SDRAM_MODE_VAL       0x00041220
#define DDRmc1_SDRAM_MODE_2_VAL     0x00

#define DDRmc1_DDRCDR_1_VAL         0x80040000
#define DDRmc1_DDRCDR_2_VAL         0x00
#define DDRmc1_INIT_ADDR_VAL        0x00000000

#define DDRmc1_INIT_EXT_ADDR_VAL    0x00000000
#define DDRmc1_SDRAM_RCW_1_VAL      0x00
#define DDRmc1_SDRAM_RCW_2_VAL      0x00
#define DDRmc1_DATA_INIT_VAL        0xDEADBEEF
#define DDRmc1_SDRAM_MD_CNTL_VAL    0x00000000
#endif /* __DDRC_H */
