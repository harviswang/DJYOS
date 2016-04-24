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


#ifndef  _CPU_PERI_ADDRESS_H
#define  _CPU_PERI_ADDRESS_H
//------some register that relations to the interrupt
//------makes me not so happy as other codes
//
//the pic base address 0xFF700000(CCSBAR) + 0X00040000(OFFSET)

#define cn_ccsbar_base_addr          0x00000000


//#define cn_gpio_baddr                (cn_ccsbar_base_addr + 0xf000)
#define cn_uart_baddr                (cn_ccsbar_base_addr + 0x3c00)
#define CN_IIC0_BADDR                 (cn_ccsbar_base_addr + 0x4400)
#define cn_spi1_baddr                (cn_ccsbar_base_addr + 0x1000)
#define cn_spi2_baddr                (cn_ccsbar_base_addr + 0x2800)

#define cn_linkport0_baddr           (cn_ccsbar_base_addr + 0x4c00)
#define cn_linkport1_baddr           (cn_ccsbar_base_addr + 0x4c20)


#define cn_dma_linkport0_baddr       (cn_ccsbar_base_addr + 0x4c18)
#define cn_dma_linkport1_baddr       (cn_ccsbar_base_addr + 0x4c38)

#define cn_sport_group1_baddr         (cn_ccsbar_base_addr+0xc00)
#define cn_sport_group2_baddr         (cn_ccsbar_base_addr+0x400)
#define cn_sport_group3_baddr         (cn_ccsbar_base_addr+0x800)
#define cn_sport_group4_baddr         (cn_ccsbar_base_addr+0x4800)

#define cn_dma_sport0_baddr           (cn_ccsbar_base_addr+0xc40)
#define cn_dma_sport1_baddr           (cn_ccsbar_base_addr+0xc48)
#define cn_dma_sport2_baddr           (cn_ccsbar_base_addr+0x440)
#define cn_dma_sport3_baddr           (cn_ccsbar_base_addr+0x448)
#define cn_dma_sport4_baddr           (cn_ccsbar_base_addr+0x840)
#define cn_dma_sport5_baddr           (cn_ccsbar_base_addr+0x848)
#define cn_dma_sport6_baddr           (cn_ccsbar_base_addr+0x4840)
#define cn_dma_sport7_baddr           (cn_ccsbar_base_addr+0x4848)

#define cn_sport_txrx_group1_baddr            (cn_ccsbar_base_addr+0xc60)
#define cn_sport_txrx_group2_baddr            (cn_ccsbar_base_addr+0x460)
#define cn_sport_txrx_group3_baddr            (cn_ccsbar_base_addr+0x860)
#define cn_sport_txrx_group4_baddr            (cn_ccsbar_base_addr+0x4860)

//use core private per-cpu registers,--TODO, may be changed as pic base address 
//#define cn_ctpr_core_addr_p   0xffe60080
//#define cn_iack_core_addr_p   0xffe600a0
//#define cn_eoi_core_addr_p    0xffe600b0   
//#define cn_whoami_core_addr_p 0xffe60090
//
//#define cn_ctpr_core_addr_g   0xffe40080
//#define cn_iack_core_addr_g   0xffe400a0
//#define cn_eoi_core_addr_g    0xffe400b0   
//#define cn_whoami_core_addr_g 0xffe40090



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

//each core has one,but the same physical addr,copied to different registers
#define cn_core_ctpr_addr            (cn_pic_baddr + 0x80)
#define cn_core_idnumber_addr        (cn_pic_baddr + 0x90)
#define cn_core_iack_addr            (cn_pic_baddr + 0xA0)
#define cn_int_eoi_addr              (cn_pic_baddr + 0xB0)
//define for some important register in the PIC
#define ctpr_offset  0x80
#define eoi_offset   0xb0
#define iack_offset  0xA0
#define gcr_offset   0x1020


//the fallowing regs used for importent control
#define cn_pic_gcr_addr              (cn_pic_baddr + 0x1020)
#define cn_pic_gcr_mode_m            (0x01<<29)//mixed mode
#define cn_pic_gcr_mode_p            (0x00<<29)//pass through mode
#define cn_pic_gcr_rst               (0x01<<31)//reset

//global modules
#define cn_guts_rstcr                (cn_ccsbar_base_addr + 0xe00b0)


#endif
