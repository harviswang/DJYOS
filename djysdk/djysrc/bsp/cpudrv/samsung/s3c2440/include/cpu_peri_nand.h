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
//���ߣ�djyos�Ŷ�
//�汾��V1.0.0
//�ļ�����: s3c2440 nand�������Ĵ�������
//����˵��:
//�޶���ʷ:
// 1. ����: 2011-11-29
//   ����: djyos�Ŷ�
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __CPU_PERI_NAND__
#define __CPU_PERI_NAND__

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

struct nand_reg         //��ַ��0x4e000000
{
    vu16 NFCONF;    //  0x4e000000 NAND Flash configuration
    vu16 revs0;
    vu16 NFCONT;    //  0x4e000004
    vu16 revs1;
    vu16 NFCMD ;    //  0x4e000008 NADD Flash command
    vu16 revs2;
    vu16 NFADDR;    //  0x4e00000c NAND Flash address
    vu16 revs3;
    vu8 NFDATA;     //  0x4e000010 NAND Flash data
    vu8 revs30;
    vu8 revs31;
    vu8 revs32;
    vu32 NFMECCD0 ; //  0x4e000014 NAND Flash ECCD0
    vu32 NFMECCD1 ; //  0x4e000018 NAND Flash ECCD1
    vu32 NFSECCD ;  //  0x4e00001C NAND Flash SPARE AREA ECC REGISTER
    vu16 NFSTAT;    //  0x4e000020 NAND Flash operation status
    vu16 revs4;
    vu32 NFESTAT0;  //  0x4e000024 NAND flash ECC status for I/O[7:0]
    vu32 NFESTAT1;  //  0x4e000028 NAND flash ECC status for I/O[15:8]
    vu32 NFMECC0;   //  0x4e00002c NAND flash main area ECC0 status
    vu32 NFMECC1;   //  0x4e000030 NAND flash main area ECC1 status
    vu32 NFSECC;    //  0x4e000034 NAND flash spare area ECC status
    vu32 NFSBLK;    //  0x4e000038 NAND flash start block address
    vu32 NFEBLK;    //  0x4e00003c NAND flash end block address
};


#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_NAND__
