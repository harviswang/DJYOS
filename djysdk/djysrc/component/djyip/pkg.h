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
// ����ʱ��: 7:11:44 PM/Mar 3, 2015
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================

#ifndef NETPKG_H_
#define NETPKG_H_

#include "stdint.h"

//tagNetPkg��ԭ��
//bufsize������ʱָ����ʹ�ù�����һֱ����;dataһֱָ��buf����ʼλ�ã����ֲ���
//����PKGд������ʱ��offset���䣬��buf��offset+datalen�ĵط���ʼд�����ݣ�д��֮��
//                  datalen +=len(lenΪд�����ݳ���)
//����PKG��ȡ����ʱ����buf��offset��ʼcpy��cpy���֮��
//                  offset += len,datalen -= len(lenΪȡ�����ݳ���)
#define CN_PKGFLAG_NULL      (1<<0)    //�����ԣ������PKG�����Լ��ֶ�����Pkg_PartFree�ͷ�
#define CN_PKGFLAG_FREE      (1<<1)    //��ʹ�������ͷź���ʱ����־��λ���ᱻ�ͷŵ������򲻻��ͷ�      
#define CN_PKGFLAG_BUFNODE   (1<<2)    //�ñ�־��ʾ��PKG������һ��bufnode��buf���ⲿ�ṩ
typedef struct _tagNetPkg
{    	
	struct _tagNetPkg   *partnext;	       //����ָ�븺�����ݰ���Э��ջ�Ĵ���
	struct _tagNetPkg   **pkglsthead;      //��ָ�����ڸ���洢�����ڿ�������
	u16  flag;          // ��pkg��һЩ����
    u16  datalen;       // buf�е���Ч���ݳ���
	u16  bufsize;	    // buf�ĳ���
	u16  offset;        // ��Ч����ƫ��buf��λ�ã�offset֮ǰ��������Ч,���ֲ����ݻ������ݶ����ʱ�������
    u8   *buf;	        // pkg��buf�����ݻ�������
}tagNetPkg;

tagNetPkg *Pkg_Alloc(u16 bufsize, u16 proper);
bool_t Pkg_PartFree(tagNetPkg *pkg);
bool_t Pkg_PartFlagFree(tagNetPkg *pkg);
bool_t Pkg_LstFree(tagNetPkg *plst);
bool_t Pkg_LstFlagFree(tagNetPkg *plst);
bool_t Pkg_SetAlignOffset(u16 alignsize);

#endif /* NETPKG_H_ */
