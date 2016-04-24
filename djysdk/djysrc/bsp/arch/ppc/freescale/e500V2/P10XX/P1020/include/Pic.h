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
// ģ������:P1020���жϿ�������һЩ�Ĵ�����ַ�Ķ���
// ģ��汾: V1.00(��ʼ���汾)
// ������Ա:zhangqf_cyg
// ����ʱ��:2013-7-26--����10:15:29
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
#ifndef __PIC_H
#define __PIC_H

#include "board-config.h"

#define cn_pic_baddr                 (CN_CCSR_NEWADDR_V + 0x40000)
#define cn_int_register_step         0x10
#define cn_pic_ivpr2idr              0x10

//register that store the interrupt  and priority and configure the dest
#define cn_int_ext_baddr             (cn_pic_baddr + 0x10000)
#define cn_int_interal_baddr         (cn_pic_baddr + 0x10200)
#define cn_int_msg_baddr             (cn_pic_baddr + 0x11600)
#define cn_int_ipi_vp_baddr          (cn_pic_baddr + 0x10A0)
#define cn_int_c0_dispath_baddr      (cn_pic_baddr + 0x20040)
#define cn_int_c1_dispath_baddr      (cn_pic_baddr + 0x21040)
#define cn_int_smsg_baddr            (cn_pic_baddr + 0x11C00)
#define cn_int_gbtimer_ga_baddr      (cn_pic_baddr + 0x1120)
#define cn_int_gbtimer_gb_baddr      (cn_pic_baddr + 0x2120)
//IVPR�Ĵ����ĸ�������λ
#define cn_ivpr_prior_msk            (0x000f0000)
#define cn_ivpr_vector_msk           (0x0000ffff)
#define cn_ivpr_en_msk               (0x01<<31)
#define cn_ivpr_sense_msk            (0x01<<22)
#define cn_ivpr_polarity_msk         (0x01<<23)
#define cn_ivpr_trigertype_msk       (0x03<<22)
#define cn_ivpr_activity_msk         (0x01<<30)

#define cn_prior_core_event            (0x00)//normal event
#define cn_prior_core_asyn_enable      (0x00)//normal state
#define cn_prior_core_asyn_disable     (0x01)//used to msk the asyn signal
#define cn_prior_asyn_signal           (0x01)//asyn signal state
#define cn_prior_real_int_default      (0x0A)//real int state
#define cn_prior_core_highest          (0x0F) //no higher prior any more
//IDR�ĸ�������λ
#define cn_pic_idr_p0                (0x01<<0)
#define cn_pic_idr_p1                (0x01<<1)
#define CN_PIC_IDR_C0                (0X01<<29)
#define CN_PIC_IDR_C1                (0x01<<30)
#define cn_pic_idr_ep                (0x01<<31)

//each core has one,but the same physical addr,copied to different registers
#define cn_core_ctpr_addr            (cn_pic_baddr + 0x80)
#define cn_core_idnumber_addr        (cn_pic_baddr + 0x90)
#define cn_core_iack_addr            (cn_pic_baddr + 0xA0)
#define cn_int_eoi_addr              (cn_pic_baddr + 0xB0)
#define cn_pic_gcr                   (cn_pic_baddr + 0x1020)
#define cn_pic_svr                   (cn_pic_baddr + 0x10e0)
#define cn_pic_svr_vector            0x00ff


#define cn_pic_gcr_addr              (cn_pic_baddr + 0x1020)
#define cn_pic_gcr_mode_m            (0x01<<29)//mixed mode
#define cn_pic_gcr_mode_p            (0x00<<29)//pass through mode
#define cn_pic_gcr_rst               (0x01<<31)//reset

//defines the PIC timers
#define cn_pic_timer_tfrra         (cn_pic_baddr + 0x10f0)
#define cn_pic_timer_tcra          (cn_pic_baddr + 0x1300)
#define cn_pic_timer_tfrrb         (cn_pic_baddr + 0x20f0)
#define cn_pic_timer_tcrb          (cn_pic_baddr + 0x2300)

#define cn_pic_timer_gtccra_base  (cn_pic_baddr + 0x1100)
#define cn_pic_timer_gtbcra_base  (cn_pic_baddr + 0x1110)
#define cn_pic_timer_gtccrb_base  (cn_pic_baddr + 0x2100)
#define cn_pic_timer_gtbcrb_base  (cn_pic_baddr + 0x2110)
#define cn_pic_timer_gt_step       0x40
#define cn_pic_timer_gnumber       0x04

//PIC INFORMATION
#define cn_pic_frr_reg                (cn_pic_baddr + 0x1000)
#define cn_pic_frr_nirq_shift         16
#define cn_pic_frr_nirq_mask          0x7ff
#define cn_pic_frr_ncpu_shift         8
#define cn_pic_frr_ncpu_mask          0x1f
#define cn_pic_frr_vid_shift          0
#define cn_pic_frr_vid_mask           0xff

//not rollover, not casecaded, use ccb/8, all the group timer are independent
#define cn_pic_timer_tcr_set       0x00
#define cn_pic_timer_msk           (1<<31)
#define cn_pic_timer_countermsk    (0x7fffffff)
#define cn_pic_timer_togmsk        (0x80000000)

#endif /* __PIC_H */
