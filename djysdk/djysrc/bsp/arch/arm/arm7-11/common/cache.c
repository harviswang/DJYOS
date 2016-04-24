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
//����ģ��:������
//����:  ������.
//�汾��V1.0.0
//�ļ�����:cache������صĴ��롣
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2011-10-21
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "cache.h"


//----����cache----------------------------------------------------------------
//����: ��ָ��cache������cacheȫ�����ϣ�����cache�е�"��"���ݲ���д�����档
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void Cache_InvalidAll(void)
{
    u32 reg = 0;
#if         defined( arm720t) \
        ||  defined( arm920t) \
        ||  defined( arm922t)\
        ||  defined( arm926ej_s)\
        ||  defined( arm1022e)\
        ||  defined( arm1026ej_s)\
        ||  defined( strong_arm)\
        ||  defined( xscale)
    __asm__(
        "mov    %0,#0 \n\t"
        "mcr    p15,0,%0,c7,c7,0 \n\t"
        :
        :"r"(reg)
        );
#elif       defined( arm940t) ||  defined( arm946e_s)
    __asm__(
        "mov    %0,#0 \n\t"
        "mcr    p15,0,%0,c7,c5,0 \n\t"
        "mcr    p15,0,%0,c7,c6,0 \n\t"
        :
        :"r"(reg)
        );
#endif
}

//----����ָ��cache------------------------------------------------------------
//����: ��ָ��cache����
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void Cache_InvalidInst(void)
{
    u32 reg = 0;
#if     defined( arm920t) \
    ||  defined( arm922t)\
    ||  defined( arm926ej_s)\
    ||  defined( arm940t)\
    ||  defined( arm946e_s)\
    ||  defined( arm1022e)\
    ||  defined( arm1026ej_s)\
    ||  defined( strong_arm)\
    ||  defined( xscale)
    __asm__(
        "mov    %0,#0 \n\t"
        "mcr    p15,0,%0,c7,c5,0 \n\t"
        :
        :"r"(reg)
        );
#elif defined( arm720t) || defined( arm740t)
    __asm__(
        "mov    %0,#0 \n\t"
        "mcr    p15,0,%0,c7,c7,0 \n\t"
        :
        :"r"(reg)
        );
#endif
}

//----��������cache------------------------------------------------------------
//����: ������cacheȫ�����ϣ�����cache�е�"��"���ݲ���д�����档
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void Cache_InvalidData(void)
{
    u32 reg = 0;
#if     defined( arm920t) \
        ||  defined( arm922t)\
        ||  defined( arm926ej_s)\
        ||  defined( arm940t)\
        ||  defined( arm946e_s)\
        ||  defined( arm1022e)\
        ||  defined( arm1026ej_s)\
        ||  defined( strong_arm)\
        ||  defined( xscale)
        __asm__(
            "mov    %0,#0 \n\t"
            "mcr    p15,0,%0,c7,c6,0 \n\t"
            :
            :"r"(reg)
            );
#elif defined( arm720t) || defined( arm740t)
        __asm__(
            "mov    %0,#0 \n\t"
            "mcr    p15,0,%0,c7,c7,0 \n\t"
            :
            :"r"(reg)
            );
#endif
}

//----ʹ��cache----------------------------------------------------------------
//����: ʹ��ָ�������cache
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void Cache_EnableAll(void)
{
    u32 reg = 0;
    __asm__(
        "mrc    p15, 0, %0, c1, c0, 0 \n\t"
        "orr    %0, %0, #0x1000 \n\t"
        "orr    %0, %0, #0x4 \n\t"
        "mcr    p15,0,%0,c1,c0,0 \n\t"
        :
        :"r"(reg)
        );
}

//----ʹ��ָ��cache------------------------------------------------------------
//����: ʹ��ָ��cache
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void Cache_EnableInst(void)
{
    u32 reg = 0;
    __asm__(
        "mrc    p15, 0, %0, c1, c0, 0 \n\t"
        "orr    %0, %0, #0x1000 \n\t"
        "mcr    p15,0,%0,c1,c0,0 \n\t"
        :
        :"r"(reg)
        );
}

//----ʹ������cache------------------------------------------------------------
//����: ʹ������cache
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void Cache_EnableData(void)
{
    u32 reg = 0;
    __asm__(
        "mrc    p15, 0, %0, c1, c0, 0 \n\t"
        "orr    %0, %0, #0x4 \n\t"
        "mcr    p15,0,%0,c1,c0,0 \n\t"
        :
        :"r"(reg)
        );
}

//----��ֹcache----------------------------------------------------------------
//����: ��ָֹ�������cache
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void Cache_DisableAll(void)
{
    u32 reg = 0;
    __asm__(
        "mrc    p15, 0, %0, c1, c0, 0 \n\t"
        "orr    %0, %0, #0x1000 \n\t"
        "orr    %0, %0, #0x4 \n\t"
        "mcr    p15,0,%0,c1,c0,0 \n\t"
        :
        :"r"(reg)
        );
}

//----��ָֹ��cache------------------------------------------------------------
//����: ��ָֹ��cache
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void Cache_DisableInst(void)
{
    u32 reg = 0;
    __asm__(
        "mrc    p15, 0, %0, c1, c0, 0 \n\t"
        "orr    %0, %0, #0x1000 \n\t"
        "mcr    p15, 0, %0, c1, c0, 0 \n\t"
        :
        :"r"(reg)
        );
}

//----��ֹ����cache------------------------------------------------------------
//����: ��ֹ����cache
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void Cache_DisableData(void)
{
    u32 reg = 0;
    __asm__(
        "mrc    p15, 0, %0, c1, c0, 0 \n\t"
        "orr    %0, %0, #0x4 \n\t"
        "mcr    p15,0,%0,c1,c0,0 \n\t"
        :
        :"r"(reg)
        );
}


//----ʹ��д����---------------------------------------------------------------
//����: ʹ��д����
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void Cache_EnableWriteBuf(void)
{
    u32 reg = 0;
    __asm__(
        "mrc    p15, 0, %0, c1, c0, 0 \n\t"
        "orr    %0, %0, #0x8 \n\t"
        "mcr    p15, 0, %0, c1, c0, 0 \n\t"
        :
        :"r"(reg)
        );
}

//----��ֹд����---------------------------------------------------------------
//����: ��ֹд����
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void Cache_DisableWriteBuf(void)
{
    u32 reg = 0;
    __asm__(
        "mrc    p15, 0, %0, c1, c0, 0 \n\t"
        "orr    %0, %0, #0x8 \n\t"
        "mcr    p15,0,%0,c1,c0,0 \n\t"
        :
        :"r"(reg)
        );
}

//----��������cache------------------------------------------------------------
//����: ������������cache������cache�е�"��"����д�����棬�Ա���ͬ����
//����: ��
//����: ��
//-----------------------------------------------------------------------------
void Cache_CleanData(void)
{
#if defined( arm926ej_s)      ||  defined( arm1026ej_s)
    __asm__("clean: \n\t"
        "mrc p15,0,pc,c7,c10,3 \n\t"
        "bne clean \n\t");
#elif       defined( arm920t) \
        ||  defined( arm922t)\
        ||  defined( arm940t)\
        ||  defined( arm946e_s)\
        ||  defined( arm1022e)
    u32 rd,way,line;
    for(way = 0; way < CN_CACHE_WAY; way++)
    {
        //��ʽ����õ�ÿ·������
        for(line = 0;line<CN_CACHE_SIZE/CN_CACHE_WAY/CN_CACHE_LINE_SIZE; line++)
        {
            rd = (way<<CN_C7_OFFSET_WAY) + (line<<CN_C7_OFFSET_SET);
            __asm__("mcr    p15, 0, %0, c7, c10, 2 \n\t"
                    :
                    :"r"(rd)
            );
        }
    }
#elif defined( arm720t) || defined( arm740t)
//����cpuֻ��дͨ��ʽ����������
#else
//�����֧�ֶ����cpu�ˣ��˴������벻��
    ����ʶ��cpu����
#endif
}



