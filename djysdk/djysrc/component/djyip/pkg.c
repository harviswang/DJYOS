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
// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// ģ������: 
// ģ��汾: V1.00
// ������Ա: Admin
// ����ʱ��: 7:11:21 PM/Mar 3, 2015
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"

#include "os.h"

#include "pkg.h"

//use this to define the net pkg mem size
extern const u32 gNetPkgMemSize;   
//this var used to make the pkg aligned by the specified value
static u16 sgPkgBufAlignSize = 0x10;
#define CN_NETPKG_SIZE  sizeof(tagNetPkg)

//PKG�ĸ���FREE����
#define CN_NETPKG_LEVEL             6
static tagNetPkg *sgPkgFreeLst[CN_NETPKG_LEVEL];  //used to list the free pkg
static u32 sgPkgLevlMap[CN_NETPKG_LEVEL]={64,128,256,512,1024,2048}; 
#define CN_PKG_SIZELEVEL_MAP 32
static u8 sgPkgSizeLevelMap[CN_PKG_SIZELEVEL_MAP]=
{0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5};

// =============================================================================
// �������ܣ�Pkg_SetAlignOffset
//          PKG���ö���
// ���������alignsize,�����С
// �����������
// ����ֵ  ��true����ʼ���ɹ� ��false��ʼ��ʧ��
// ˵��������֮�󣬸ö�������еĺ���������Ч�������ǰ���ù����ߵģ���ô�������ò���ɹ�
// =============================================================================
bool_t Pkg_SetAlignOffset(u16 alignsize)
{
	bool_t result;

	result = false;
	if(alignsize >sgPkgBufAlignSize)
	{
		result = true;
	}
	return result;
}

static u8  *pgPkgMemSrc = NULL;
static u32  sgPkgMemOffset = 0;
// =============================================================================
// �������ܣ�Pkg_Init
//          PKG����Ԫ��ʼ��
// �����������
// �����������
// ����ֵ  ��true����ʼ���ɹ� ��false��ʼ��ʧ��
// ˵������ʼ���б�͸��Ե�������
// =============================================================================
bool_t Pkg_Init(void)
{
    //FIRST MALLOC THE MEM FROM THE HEAP
    pgPkgMemSrc = M_Malloc(gNetPkgMemSize,0);
    if(NULL == pgPkgMemSrc)
    {
    	printk("Pkg_Init:No memory for net pkg!\n\r");
    	return false;
    }
    else
    {
    	printk("Pkg: MemSrc = %08x\n\r",(u32)pgPkgMemSrc);
    	sgPkgMemOffset = 0;
    	sgPkgBufAlignSize = 0x10;//initialize the buf align request(16 bytes)
    	return true;	
    }
}

// =============================================================================
// �������ܣ�Pkg_Alloc
//          ������ʴ�С��PKG
// ���������bufsize,�û�����Ļ������Ĵ�С
// �����������
// ����ֵ  �������PKG��NULL��ʾ����ʧ�ܣ���������Ի����ڴ治�㣩
// ˵��    ��bufsize��ʾ���ǻ������Ĵ�С��һ����ԣ�bufsize����pkg����ռ�õĿռ�һ������
//        ����2K�������ر�������Ҫ����2K�ģ�����ʹ�ö�������ϼ��ɡ�
//        ���۴ӳ��з���Ļ��Ǵ�FREE�������Ļ��Ƕ�̬����ģ�һ������ɹ������
//        ���б�Ҫ�ĳ�ʼ��������ӵ�pgTCPIPPkgScanLst������
// =============================================================================
tagNetPkg *Pkg_Alloc(u16 bufsize, u16 flag)
{
	u8 i ;
	u16  pkgsize;
	atom_low_t atomop;
	tagNetPkg *result = NULL;
	
	pkgsize = bufsize + CN_NETPKG_SIZE +sgPkgBufAlignSize;
	i = pkgsize/64;
	if(i < CN_PKG_SIZELEVEL_MAP)
	{
		//map the size to the defined size
		i = sgPkgSizeLevelMap[i];
		pkgsize = sgPkgLevlMap[i];
		
		atomop = Int_LowAtomStart();
		if(NULL != sgPkgFreeLst[i])
		{
			//fetch the pkg from the free lst
			result = sgPkgFreeLst[i];
			sgPkgFreeLst[i] = result->partnext;
		}
		else if((pkgsize+sgPkgMemOffset)<gNetPkgMemSize)
		{
			//alloc pkg from the specified mem src
			result = (tagNetPkg *)(pgPkgMemSrc + sgPkgMemOffset);
			sgPkgMemOffset += pkgsize;
//			printk("PkgAlloc:New mem 0ffse =%08x\n\r",sgPkgMemOffset);
			
		}
		else
		{
			printk("PkgAlloc:Not Enough Mem size = %08x\n\r",pkgsize);
		}
		if(NULL != result)
		{
//			printk("PkgAlloc:%08x %08x %08x\n\r",(u32)result,pkgsize,i);
			
			result->partnext = NULL;
			result->bufsize = pkgsize - CN_NETPKG_SIZE;
			result->buf = (u8 *)result + CN_NETPKG_SIZE;
			result->datalen = 0;
			result->flag = flag;
			//CN_NETPKG_DEBUG
			result->pkglsthead = &sgPkgFreeLst[i];
			//you'd better do the align here
			i = (sgPkgBufAlignSize-1)&((u32)result->buf);
			if(i != 0)
			{
				result->offset = sgPkgBufAlignSize- i;
			}
			else
			{
				result->offset =0;
			}
		}
		
		Int_LowAtomEnd(atomop);
	}

	return result;
}
//void __Pkg_Free(tagNetPkg *pkg)
//{
//	tagNetPkg **pkglsthead;
//	
//	printk("PkgFree:%08x \n\r",(u32)pkg);
//	pkglsthead =pkg->pkglsthead;
//	pkg->partnext = *pkglsthead;
//	*pkglsthead = pkg;
//}
//// =============================================================================
//// �������ܣ�Pkg_PartFree
//// ���������pkg,���ͷŵ����ݰ�
//// �����������
//// ����ֵ  ��
//// ˵��    ���ú�����Ҫ������Щ�Լ������PKG��ͬʱû��ָ�����ͷŷ�ʽ;�ȷ�TCP�Լ����䣬�յ�
////         ACK�Լ��ͷŵ����ݰ������Բ������ַ�ʽ�����ĳ��PKG���Ǹ�Э�����ģ���ʹ��
////         Pkg_LstFlagFree���ͷ�
//// =============================================================================
//bool_t Pkg_PartFree(tagNetPkg *pkg)
//{
//	atom_low_t atomop;
//
//	if(NULL != pkg)
//	{
//		atomop = Int_LowAtomStart();
//		__Pkg_Free(pkg);
//		
//		Int_LowAtomEnd(atomop);
//	}
//	return true;
//}
//// =============================================================================
//// �������ܣ�Pkg_PartFlagFree
//// ���������pkg,���ͷŵ����ݰ�
//// �����������
//// ����ֵ  ��
//// ˵��    ���ú����ͷ�һ��PKG��������flag��־λ
//// =============================================================================
//bool_t Pkg_PartFlagFree(tagNetPkg *pkg)
//{
//	atom_low_t atomop;
//
//	if((NULL != pkg)&&(pkg->flag&CN_PKGFLAG_FREE))
//	{
//		atomop = Int_LowAtomStart();
//		__Pkg_Free(pkg);
//
//		Int_LowAtomEnd(atomop);
//	}
//	return true;
//}
//// =============================================================================
//// �������ܣ�Pkg_LstFree
//// ���������plst,���ͷŵ�pkg����
//// �����������
//// ����ֵ  ��true �ɹ� falseʧ��
//// ˵�����ú���ֱ���ͷ�PKG���У����ж���FLAG��
//// =============================================================================
//bool_t Pkg_LstFree(tagNetPkg *plst)
//{
//	tagNetPkg *pkg;
//	atom_low_t atomop;
//	tagNetPkg *pkgnxt;
//
//	atomop = Int_LowAtomStart();
//	pkg = plst;
//	while(NULL != pkg)
//	{
//		pkgnxt = pkg->partnext;
//		__Pkg_Free(pkg);
//
//		pkg = pkgnxt;
//	}
//	Int_LowAtomEnd(atomop);
//	return true;
//}
//
//// =============================================================================
//// �������ܣ�Pkg_LstFlagFree
//// ���������plst,���ͷŵ�pkg����
//// �����������
//// ����ֵ  ��true �ɹ� falseʧ��
//// ˵�����ú�������һ����PKG�ı�־�������Ƿ��ͷ�
//// =============================================================================
//bool_t Pkg_LstFlagFree(tagNetPkg *plst)
//{
//	tagNetPkg *pkg;
//	atom_low_t atomop;
//	tagNetPkg **pkglsthead;
//	tagNetPkg *pkgnxt;
//
//	atomop = Int_LowAtomStart();
//	pkg = plst;
//	while(NULL != pkg)
//	{
//		pkgnxt = pkg->partnext;
//		if(pkg->flag & CN_PKGFLAG_FREE)//�����ͷ�
//		{
//			__Pkg_Free(pkg);
//		}
//		pkg = pkgnxt;
//	}
//	Int_LowAtomEnd(atomop);
//	return true;
//}
// =============================================================================
// �������ܣ�Pkg_PartFree
// ���������pkg,���ͷŵ����ݰ�
// �����������
// ����ֵ  ��
// ˵��    ���ú�����Ҫ������Щ�Լ������PKG��ͬʱû��ָ�����ͷŷ�ʽ;�ȷ�TCP�Լ����䣬�յ�
//         ACK�Լ��ͷŵ����ݰ������Բ������ַ�ʽ�����ĳ��PKG���Ǹ�Э�����ģ���ʹ��
//         Pkg_LstFlagFree���ͷ�
// =============================================================================
bool_t Pkg_PartFree(tagNetPkg *pkg)
{
	atom_low_t atomop;
	tagNetPkg **pkglsthead;

	if(NULL != pkg)
	{
		atomop = Int_LowAtomStart();
		pkglsthead =pkg->pkglsthead;
		pkg->partnext = *pkglsthead;
		*pkglsthead = pkg;
		Int_LowAtomEnd(atomop);
	}
	return true;
}
// =============================================================================
// �������ܣ�Pkg_PartFlagFree
// ���������pkg,���ͷŵ����ݰ�
// �����������
// ����ֵ  ��
// ˵��    ���ú����ͷ�һ��PKG��������flag��־λ
// =============================================================================
bool_t Pkg_PartFlagFree(tagNetPkg *pkg)
{
	atom_low_t atomop;
	tagNetPkg **pkglsthead;

	if((NULL != pkg)&&(pkg->flag&CN_PKGFLAG_FREE))
	{
		atomop = Int_LowAtomStart();
		pkglsthead =pkg->pkglsthead;
		pkg->partnext = *pkglsthead;
		*pkglsthead = pkg;
		Int_LowAtomEnd(atomop);
	}
	return true;
}
// =============================================================================
// �������ܣ�Pkg_LstFree
// ���������plst,���ͷŵ�pkg����
// �����������
// ����ֵ  ��true �ɹ� falseʧ��
// ˵�����ú���ֱ���ͷ�PKG���У����ж���FLAG��
// =============================================================================
bool_t Pkg_LstFree(tagNetPkg *plst)
{
	tagNetPkg *pkg;
	atom_low_t atomop;
	tagNetPkg **pkglsthead;
	tagNetPkg *pkgnxt;

	atomop = Int_LowAtomStart();
	pkg = plst;
	while(NULL != pkg)
	{
		pkgnxt = pkg->partnext;
		pkglsthead =pkg->pkglsthead;
		pkg->partnext = *pkglsthead;
		*pkglsthead = pkg;
		pkg = pkgnxt;
	}
	Int_LowAtomEnd(atomop);
	return true;
}

// =============================================================================
// �������ܣ�Pkg_LstFlagFree
// ���������plst,���ͷŵ�pkg����
// �����������
// ����ֵ  ��true �ɹ� falseʧ��
// ˵�����ú�������һ����PKG�ı�־�������Ƿ��ͷ�
// =============================================================================
bool_t Pkg_LstFlagFree(tagNetPkg *plst)
{
	tagNetPkg *pkg;
	atom_low_t atomop;
	tagNetPkg **pkglsthead;
	tagNetPkg *pkgnxt;

	atomop = Int_LowAtomStart();
	pkg = plst;
	while(NULL != pkg)
	{
		pkgnxt = pkg->partnext;
		if(pkg->flag & CN_PKGFLAG_FREE)//�����ͷ�
		{
			pkglsthead =pkg->pkglsthead;
			pkg->partnext = *pkglsthead;
			*pkglsthead = pkg;
		}
		pkg = pkgnxt;
	}
	Int_LowAtomEnd(atomop);
	return true;
}


