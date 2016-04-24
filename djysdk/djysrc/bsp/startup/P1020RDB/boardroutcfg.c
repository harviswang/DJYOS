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
// ģ������:P1020rdb�����MMUӳ���
// ģ��汾:
// ������Ա: zhangqf
// ����ʱ��: 9:14:46 AM/Sep 17, 2013
// =======================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =======================================================================
#include "stdint.h"
#include "E500v2RegBits.h"
#include "SysSpace.h"
#include "TLB.h"
#include "LAW.h"
#include "ELBC.h"

const tagTlbConfig tgTlbConfig[] = {
	//ddr
    SET_TLB_ENTRY(1, CN_DDR_ADDR_64MB_V0, CN_DDR_ADDR_64MB_V0,
				MAS3_SX|MAS3_SW|MAS3_SR, MAS2_W | MAS2_G |MAS2_M,
				0, 0, BOOKE_PAGESZ_64M, 1),	//DDR WILL BE WRITE THROUGH
	SET_TLB_ENTRY(1, CN_DDR_ADDR_64MB_V1, CN_DDR_ADDR_64MB_V1,
				MAS3_SW|MAS3_SR, MAS2_W | MAS2_G |MAS2_M,
				0, 1, BOOKE_PAGESZ_64M, 1),	//DDR WILL BE WRITE THROUGH	
	SET_TLB_ENTRY(1, CN_DDR_ADDR_64MB_V2, CN_DDR_ADDR_64MB_V2,
				MAS3_SW|MAS3_SR, MAS2_W | MAS2_G |MAS2_M,
				0, 2, BOOKE_PAGESZ_64M, 1),	//DDR WILL BE WRITE THROUGH
	SET_TLB_ENTRY(1, CN_DDR_ADDR_64MB_V3, CN_DDR_ADDR_64MB_V3,
				MAS3_SW|MAS3_SR, MAS2_W | MAS2_G |MAS2_M,
				0, 3, BOOKE_PAGESZ_64M, 1),	//DDR WILL BE WRITE THROUGH	
				
	SET_TLB_ENTRY(1, CN_DDR_ADDR_256MB_V1, CN_DDR_ADDR_256MB_V1,
				MAS3_SW|MAS3_SR, MAS2_W | MAS2_G |MAS2_M,
				0, 4, BOOKE_PAGESZ_256M, 1),	//DDR WILL BE WRITE THROUGH	
	SET_TLB_ENTRY(1, CN_DDR_ADDR_256MB_V2, CN_DDR_ADDR_256MB_V2,
				MAS3_SW|MAS3_SR, MAS2_W | MAS2_G |MAS2_M,
				0, 5, BOOKE_PAGESZ_256M, 1),	//DDR WILL BE WRITE THROUGH
	SET_TLB_ENTRY(1, CN_DDR_ADDR_256MB_V3, CN_DDR_ADDR_256MB_V3,
				MAS3_SW|MAS3_SR, MAS2_W | MAS2_G |MAS2_M,
				0, 6, BOOKE_PAGESZ_256M, 1),	//DDR WILL BE WRITE THROUGH	
	//other device route			
				
	//ccsr
	SET_TLB_ENTRY(1, CN_CCSR_NEWADDR_V, CN_CCSR_NEWADDR_PL,
				MAS3_SW|MAS3_SR, MAS2_I | MAS2_G|MAS2_M,
			    0, 12, BOOKE_PAGESZ_1M, 1),
	//nor flash  16MB
	SET_TLB_ENTRY(1, CN_NORFLASH_ADDR_V, CN_NORFLASH_ADDR_P,
			    MAS3_SX|MAS3_SW|MAS3_SR,MAS2_I|MAS2_G|MAS2_M,
				0, 13, BOOKE_PAGESZ_16M, 1),
    //nand flash
	SET_TLB_ENTRY(1, CN_NANDFLASH_ADDR_V, CN_NANDFLASH_ADDR_P,
			    MAS3_SX|MAS3_SW|MAS3_SR,  MAS2_I |MAS2_G|MAS2_M,
				0, 14, BOOKE_PAGESZ_4K, 1),
};
const int gTlbCfgItem = ARRAY_SIZE(tgTlbConfig);
//LAW config
//0x00000000--0x3fffffff     1GB reserved for the mem
const tagLawConfig tgLawConfig[] = {
	//DDR
	{0x00000000,0x80F0001d},	
    //ELBC
    {0x000fe000,0x80400018},
};
const int gLawCfgItem = ARRAY_SIZE(tgLawConfig);

//elbc config
const tagElbcCfg  tgP1020RDB_ElbcCfg[] =
{
        {0xFF001001, 0xFF000F87},   //NOR FLASH BOOT,GPCM-CS0
        {0,0},
        {0xC0000C21, 0xF80003AE},   //NAND FLASH-CS2
        {0xEFA00801, 0xFFFE09F7},   //CPLD-CS3
};
const int gElbcCfgItem = ARRAY_SIZE(tgLawConfig);



