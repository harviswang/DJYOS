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
//����ģ��:��С�˵�����صĹ�������
//����:  ������.
//�汾��V1.0.0
//�ļ�����:��������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "arch_feature.h"
#include "endian.h"

//----��ȡһ��С��16λ����-----------------------------------------------------
//����: ��С�˸�ʽ�Ĵ洢���ж���һ��16λ���ݡ�
//����: buf��������
//      index��ƫ���������ڼ���16λ��
//����: ��ȡ��16λ����
//----------------------------------------------------------------------------
u16 pick_little_16bit(u8 *buf,u32 index)
{
    //��ʹ�˸���ͬ��Ҳ����ʹ�� return *(uint16_t *)buf�򻯣�
    //��Ϊ���ܱ�֤buf��һ�������˵ĵ�ַ
#if (CN_BYTE_BITS == 8)
    u32 align8=0;
    align8=(u32)buf;
    align8 &= 0x01;
    //������ȡ����������֣�
    //1.�޶���Ҫ����cpuΪС��ģʽ
    //2.�ж���Ҫ��buf�Ƕ���洢�ģ���cpuΪС��ģʽ
    if(((CN_MEM_ALIGN == 0)&&(CN_CFG_BYTE_ORDER==CN_CFG_LITTLE_ENDIAN))
        ||  ((CN_MEM_ALIGN == 1)&&(align8 == 0)&&(CN_CFG_BYTE_ORDER==CN_CFG_LITTLE_ENDIAN)))
    {
        return *(u16 *)buf;
    }
    else
    {
        u16 data;
        //data = buf[2*index];
        //data+= buf[2*index+1]<<8;
#if (CN_CFG_BYTE_ORDER==CN_CFG_LITTLE_ENDIAN)
        *((u8*)(&data)+0) = buf[2*index+0];//���ݵ�λ
        *((u8*)(&data)+1) = buf[2*index+1];
#elif (CN_CFG_BYTE_ORDER==CN_CFG_BIG_ENDIAN)
        *((u8*)(&data)+1) = buf[2*index+0];
        *((u8*)(&data)+0) = buf[2*index+1];
#endif
        return (u16)data;
    }
#else
    u16 data;
    data = buf[2*index]&0xFF;
    data+= (buf[2*index+1]&0xFF)<<8;
    return (u16)data;
#endif
}

//----��ȡһ��С��32λ����-----------------------------------------------------
//����: ��С�˸�ʽ�Ĵ洢���ж���һ��32λ���ݡ�
//����: buf��������
//      index��ƫ���������ڼ���32λ��
//����: ��ȡ��32λ����
//----------------------------------------------------------------------------
u32 pick_little_32bit(u8 *buf,u32 index)
{
    //��ʹ�˸���ͬ��Ҳ����ʹ�� return *(u32 *)buf�򻯣�
    //��Ϊ���ܱ�֤buf��һ�������˵ĵ�ַ
#if (CN_BYTE_BITS == 8)
    u32 align8=0;
    align8=(u32)buf;
    align8 &= 0x03;
    //������ȡ����������֣�
    //1.�޶���Ҫ����cpuΪС��ģʽ
    //2.�ж���Ҫ��buf�Ƕ���洢�ģ���cpuΪС��ģʽ
    if(((CN_MEM_ALIGN == 0)&&(CN_CFG_BYTE_ORDER==CN_CFG_LITTLE_ENDIAN))
        ||  ((CN_MEM_ALIGN == 1)&&(align8 == 0)&&(CN_CFG_BYTE_ORDER==CN_CFG_LITTLE_ENDIAN)))
    {
        return *(u32 *)buf;
    }
    else
    {
        u32 data;
        //data = buf[4*index];
        //data+= buf[4*index+1]<<8;
        //data+= buf[4*index+2]<<16;
        //data+= buf[4*index+3]<<24;
#if (CN_CFG_BYTE_ORDER==CN_CFG_LITTLE_ENDIAN)
        *((u8*)(&data)+0) = buf[4*index+0];//���ݵ�λ
        *((u8*)(&data)+1) = buf[4*index+1];
        *((u8*)(&data)+2) = buf[4*index+2];
        *((u8*)(&data)+3) = buf[4*index+3];
#elif (CN_CFG_BYTE_ORDER==CN_CFG_BIG_ENDIAN)
        *((u8*)(&data)+3) = buf[4*index+0];
        *((u8*)(&data)+2) = buf[4*index+1];
        *((u8*)(&data)+1) = buf[4*index+2];
        *((u8*)(&data)+0) = buf[4*index+3];
#endif
        return (u32)data;
    }
#else
    u32 data;
    data = buf[4*index]&0xFF;
    data+= (buf[4*index+1]&0xFF)<<8;
    data+= (buf[4*index+2]&0xFF)<<16;
    data+= (buf[4*index+3]&0xFF)<<24;
    return (u32)data;
#endif
}

//----��ȡһ��С��64λ����-----------------------------------------------------
//����: ��С�˸�ʽ�Ĵ洢���ж���һ��64λ���ݡ�
//����: buf��������
//      index��ƫ���������ڼ���64λ��
//����: ��ȡ��64λ����
//----------------------------------------------------------------------------
u64 pick_little_64bit(u8 *buf,u32 index)
{
#if (CN_BYTE_BITS == 8)
    u32 align8=0;
    align8=(u32)buf;
    align8 &= 0x07;
    //������ȡ����������֣�
    //1.�޶���Ҫ����cpuΪС��ģʽ
    //2.�ж���Ҫ��buf�Ƕ���洢�ģ���cpuΪС��ģʽ
    if(((CN_MEM_ALIGN == 0)&&(CN_CFG_BYTE_ORDER==CN_CFG_LITTLE_ENDIAN))
        ||  ((CN_MEM_ALIGN == 1)&&(align8 == 0)&&(CN_CFG_BYTE_ORDER==CN_CFG_LITTLE_ENDIAN)))
    {
        return *(u64 *)buf;
    }
    else
    {
        u64 data=0;

        /*data=buf[8*index+7];
        data<<=8;
        data+=buf[8*index+6];
        data<<=8;
        data+=buf[8*index+5];
        data<<=8;
        data+=buf[8*index+4];
        data<<=8;
        data+=buf[8*index+3];
        data<<=8;
        data+=buf[8*index+2];
        data<<=8;
        data+=buf[8*index+1];
        data<<=8;
        data+=buf[8*index+0];*/
#if (CN_CFG_BYTE_ORDER==CN_CFG_LITTLE_ENDIAN)
        *((u8*)(&data)+0) = buf[8*index+0];//���ݵ�λ
        *((u8*)(&data)+1) = buf[8*index+1];
        *((u8*)(&data)+2) = buf[8*index+2];
        *((u8*)(&data)+3) = buf[8*index+3];
        *((u8*)(&data)+4) = buf[8*index+4];
        *((u8*)(&data)+5) = buf[8*index+5];
        *((u8*)(&data)+6) = buf[8*index+6];
        *((u8*)(&data)+7) = buf[8*index+7];
#elif (CN_CFG_BYTE_ORDER==CN_CFG_BIG_ENDIAN)
        *((u8*)(&data)+7) = buf[8*index+0];
        *((u8*)(&data)+6) = buf[8*index+1];
        *((u8*)(&data)+5) = buf[8*index+2];
        *((u8*)(&data)+4) = buf[8*index+3];
        *((u8*)(&data)+3) = buf[8*index+4];
        *((u8*)(&data)+2) = buf[8*index+5];
        *((u8*)(&data)+1) = buf[8*index+6];
        *((u8*)(&data)+0) = buf[8*index+7];
#endif

        return (u64)data;
    }
#else
    u64 data=0;

    data=buf[8*index+7]&0xFF;
    data<<=8;
    data+=buf[8*index+6]&0xFF;
    data<<=8;
    data+=buf[8*index+5]&0xFF;
    data<<=8;
    data+=buf[8*index+4]&0xFF;
    data<<=8;
    data+=buf[8*index+3]&0xFF;
    data<<=8;
    data+=buf[8*index+2]&0xFF;
    data<<=8;
    data+=buf[8*index+1]&0xFF;
    data<<=8;
    data+=buf[8*index+0]&0xFF;

    return (u64)data;
#endif
}

//----���һ��С��16λ����-----------------------------------------------------
//����: ��һ��16λ������䵽С�˸�ʽ�Ĵ洢���С�
//����: buf��������
//      index��ƫ���������ڼ���16λ��
//      data����������
//����: ��
//----------------------------------------------------------------------------
void fill_little_16bit(u8 *buf,u32 index,u16 data)
{
    buf[2*index+0] = (u8)data;
    buf[2*index+1] = (u8)(data>>8);
#if (CN_BYTE_BITS != 8)
    buf[2*index+0] &= 0xFF;
    buf[2*index+1] &= 0xFF;
#endif  //#if (CN_BYTE_BITS != 8)
}

//----���һ��С��32λ����-----------------------------------------------------
//����: ��һ��32λ������䵽С�˸�ʽ�Ĵ洢���С�
//����: buf��������
//      index��ƫ���������ڼ���32λ��
//      data����������
//����: ��
//----------------------------------------------------------------------------
void fill_little_32bit(u8 *buf,u32 index,u32 data)
{
    buf[4*index+0] = (u8)data;
    buf[4*index+1] = (u8)(data>>8);
    buf[4*index+2] = (u8)(data>>16);
    buf[4*index+3] = (u8)(data>>24);
#if (CN_BYTE_BITS != 8)
    buf[4*index+0] &= 0xFF;
    buf[4*index+1] &= 0xFF;
    buf[4*index+2] &= 0xFF;
    buf[4*index+3] &= 0xFF;
#endif  //#if (CN_BYTE_BITS != 8)
}

//----���һ��С��64λ����-----------------------------------------------------
//����: ��һ��64λ������䵽С�˸�ʽ�Ĵ洢���С�
//����: buf��������
//      index��ƫ���������ڼ���64λ��
//      data����������
//����: ��
//----------------------------------------------------------------------------
void fill_little_64bit(u8 *buf,u32 index,u64 data)
{
    buf[8*index+0] = (u8)data;
    buf[8*index+1] = (u8)(data>>8);
    buf[8*index+2] = (u8)(data>>16);
    buf[8*index+3] = (u8)(data>>24);
    buf[8*index+4] = (u8)(data>>32);
    buf[8*index+5] = (u8)(data>>40);
    buf[8*index+6] = (u8)(data>>48);
    buf[8*index+7] = (u8)(data>>56);
#if (CN_BYTE_BITS != 8)
    buf[8*index+0] &= 0xFF;
    buf[8*index+1] &= 0xFF;
    buf[8*index+2] &= 0xFF;
    buf[8*index+3] &= 0xFF;
    buf[8*index+4] &= 0xFF;
    buf[8*index+5] &= 0xFF;
    buf[8*index+6] &= 0xFF;
    buf[8*index+7] &= 0xFF;
#endif  //#if (CN_BYTE_BITS != 8)
}

//----��ȡһ�����16λ����-----------------------------------------------------
//����: �Ӵ�˸�ʽ�Ĵ洢���ж���һ��16λ���ݡ�
//����: buf��������
//      index��ƫ���������ڼ���16λ��
//����: ��ȡ��16λ����
//----------------------------------------------------------------------------
u16 pick_big_16bit(u8 *buf,u32 index)
{
    //��ʹ�˸���ͬ��Ҳ����ʹ�� return *(uint16_t *)buf�򻯣�
    //��Ϊ���ܱ�֤buf��һ�������˵ĵ�ַ
#if (CN_BYTE_BITS == 8)
    u32 align8=0;
    align8=(u32)buf;
    align8 &= 0x01;
    //������ȡ����������֣�
    //1.�޶���Ҫ����cpuΪ���ģʽ
    //2.�ж���Ҫ��buf�Ƕ���洢�ģ���cpuΪ���ģʽ
    if(((CN_MEM_ALIGN == 0)&&(CN_CFG_BYTE_ORDER==CN_CFG_BIG_ENDIAN))
        ||  ((CN_MEM_ALIGN == 1)&&(align8 == 0)&&(CN_CFG_BYTE_ORDER==CN_CFG_BIG_ENDIAN)))
    {
        return *(u16 *)buf;
    }
    else
    {
        u16 data;
        //data = buf[2*index+1];
        //data+= buf[2*index]<<8;
#if (CN_CFG_BYTE_ORDER==CN_CFG_LITTLE_ENDIAN)
        *((u8*)(&data)+1) = buf[2*index+0];//���ݸ�λ
        *((u8*)(&data)+0) = buf[2*index+1];
#elif (CN_CFG_BYTE_ORDER==CN_CFG_BIG_ENDIAN)
        *((u8*)(&data)+0) = buf[2*index+0];
        *((u8*)(&data)+1) = buf[2*index+1];
#endif
        return (u16)data;
    }
#else
    u16 data;
    data = buf[2*index+1]&0xFF;
    data+= (buf[2*index]&0xFF)<<8;
    return (u16)data;
#endif
}

//----��ȡһ�����32λ����-----------------------------------------------------
//����: �Ӵ�˸�ʽ�Ĵ洢���ж���һ��32λ���ݡ�
//����: buf��������
//      index��ƫ���������ڼ���32λ��
//����: ��ȡ��32λ����
//----------------------------------------------------------------------------
u32 pick_big_32bit(u8 *buf,u32 index)
{
    //��ʹ�˸���ͬ��Ҳ����ʹ�� return *(u32 *)buf�򻯣�
    //��Ϊ���ܱ�֤buf��һ�������˵ĵ�ַ
#if (CN_BYTE_BITS == 8)
    u32 align8=0;
    align8=(u32)buf;
    align8 &= 0x03;
    //������ȡ����������֣�
    //1.�޶���Ҫ����cpuΪ���ģʽ
    //2.�ж���Ҫ��buf�Ƕ���洢�ģ���cpuΪ���ģʽ
    if(((CN_MEM_ALIGN == 0)&&(CN_CFG_BYTE_ORDER==CN_CFG_BIG_ENDIAN))
        ||  ((CN_MEM_ALIGN == 1)&&(align8 == 0)&&(CN_CFG_BYTE_ORDER==CN_CFG_BIG_ENDIAN)))
    {
        return *(u32 *)buf;
    }
    else
    {
        u32 data;
        /*data = buf[4*index+3];
        data+= buf[4*index+2]<<8;
        data+= buf[4*index+1]<<16;
        data+= buf[4*index]<<24;*/
#if (CN_CFG_BYTE_ORDER==CN_CFG_LITTLE_ENDIAN)
        *((u8*)(&data)+3) = buf[4*index+0];//���ݸ�λ
        *((u8*)(&data)+2) = buf[4*index+1];
        *((u8*)(&data)+1) = buf[4*index+2];
        *((u8*)(&data)+0) = buf[4*index+3];
#elif (CN_CFG_BYTE_ORDER==CN_CFG_BIG_ENDIAN)
        *((u8*)(&data)+0) = buf[4*index+0];
        *((u8*)(&data)+1) = buf[4*index+1];
        *((u8*)(&data)+2) = buf[4*index+2];
        *((u8*)(&data)+3) = buf[4*index+3];
#endif
        return (u32)data;
    }
#else
    u32 data;
    data = buf[4*index+3]&0xFF;
    data+= (buf[4*index+2]&0xFF)<<8;
    data+= (buf[4*index+1]&0xFF)<<16;
    data+= (buf[4*index]&0xFF)<<24;
    return (u32)data;
#endif
}

//----��ȡһ�����64λ����-----------------------------------------------------
//����: �Ӵ�˸�ʽ�Ĵ洢���ж���һ��64λ���ݡ�
//����: buf��������
//      index��ƫ���������ڼ���64λ��
//����: ��ȡ��64λ����
//----------------------------------------------------------------------------
u64 pick_big_64bit(u8 *buf,u32 index)
{
#if (CN_BYTE_BITS == 8)
    u32 align8=0;
    align8=(u32)buf;
    align8 &= 0x07;
    //������ȡ����������֣�
    //1.�޶���Ҫ����cpuΪ���ģʽ
    //2.�ж���Ҫ��buf�Ƕ���洢�ģ���cpuΪ���ģʽ
    if(((CN_MEM_ALIGN == 0)&&(CN_CFG_BYTE_ORDER==CN_CFG_BIG_ENDIAN))
        ||  ((CN_MEM_ALIGN == 1)&&(align8 == 0)&&(CN_CFG_BYTE_ORDER==CN_CFG_BIG_ENDIAN)))
    {
        return *(u64 *)buf;
    }
    else
    {
        u64 data=0;

        /*data=buf[8*index+0];
        data<<=8;
        data+=buf[8*index+1];
        data<<=8;
        data+=buf[8*index+2];
        data<<=8;
        data+=buf[8*index+3];
        data<<=8;
        data+=buf[8*index+4];
        data<<=8;
        data+=buf[8*index+5];
        data<<=8;
        data+=buf[8*index+6];
        data<<=8;
        data+=buf[8*index+7];*/
#if (CN_CFG_BYTE_ORDER==CN_CFG_LITTLE_ENDIAN)
        *((u8*)(&data)+7) = buf[4*index+0];//���ݸ�λ
        *((u8*)(&data)+6) = buf[4*index+1];
        *((u8*)(&data)+5) = buf[4*index+2];
        *((u8*)(&data)+4) = buf[4*index+3];
        *((u8*)(&data)+3) = buf[4*index+4];
        *((u8*)(&data)+2) = buf[4*index+5];
        *((u8*)(&data)+1) = buf[4*index+6];
        *((u8*)(&data)+0) = buf[4*index+7];
#elif (CN_CFG_BYTE_ORDER==CN_CFG_BIG_ENDIAN)
        *((u8*)(&data)+0) = buf[4*index+0];
        *((u8*)(&data)+1) = buf[4*index+1];
        *((u8*)(&data)+2) = buf[4*index+2];
        *((u8*)(&data)+3) = buf[4*index+3];
        *((u8*)(&data)+4) = buf[4*index+4];
        *((u8*)(&data)+5) = buf[4*index+5];
        *((u8*)(&data)+6) = buf[4*index+6];
        *((u8*)(&data)+7) = buf[4*index+7];
#endif
        return (u64)data;
    }
#else
    u64 data=0;

    data=buf[8*index+0]&0xFF;
    data<<=8;
    data+=buf[8*index+1]&0xFF;
    data<<=8;
    data+=buf[8*index+2]&0xFF;
    data<<=8;
    data+=buf[8*index+3]&0xFF;
    data<<=8;
    data+=buf[8*index+4]&0xFF;
    data<<=8;
    data+=buf[8*index+5]&0xFF;
    data<<=8;
    data+=buf[8*index+6]&0xFF;
    data<<=8;
    data+=buf[8*index+7]&0xFF;

    return (u64)data;
#endif

}


//----���һ�����16λ����-----------------------------------------------------
//����: ��һ��16λ������䵽��˸�ʽ�Ĵ洢���С�
//����: buf��������
//      index��ƫ���������ڼ���16λ��
//      data����������
//����: ��
//----------------------------------------------------------------------------
void fill_big_16bit(u8 *buf,u32 index,u16 data)
{
    buf[2*index+1] = (u8)data;
    buf[2*index+0] = (u8)(data>>8);
#if (CN_BYTE_BITS != 8)
    buf[2*index+1] &= 0xFF;
    buf[2*index+0] &= 0xFF;
#endif  //#if (CN_BYTE_BITS != 8)
}

//----���һ�����32λ����-----------------------------------------------------
//����: ��һ��32λ������䵽��˸�ʽ�Ĵ洢���С�
//����: buf��������
//      index��ƫ���������ڼ���32λ��
//      data����������
//����: ��
//----------------------------------------------------------------------------
void fill_big_32bit(u8 *buf,u32 index,u32 data)
{
    buf[4*index+3] = (u8)data;
    buf[4*index+2] = (u8)(data>>8);
    buf[4*index+1] = (u8)(data>>16);
    buf[4*index+0] = (u8)(data>>24);
#if (CN_BYTE_BITS != 8)
    buf[4*index+3] &= 0xFF;
    buf[4*index+2] &= 0xFF;
    buf[4*index+1] &= 0xFF;
    buf[4*index+0] &= 0xFF;
#endif  //#if (CN_BYTE_BITS != 8)
}

//----���һ�����64λ����-----------------------------------------------------
//����: ��һ��64λ������䵽��˸�ʽ�Ĵ洢���С�
//����: buf��������
//      index��ƫ���������ڼ���64λ��
//      data����������
//����: ��
//----------------------------------------------------------------------------
void fill_big_64bit(u8 *buf,u32 index,u64 data)
{
    buf[8*index+7] = (u8)data;
    buf[8*index+6] = (u8)(data>>8);
    buf[8*index+5] = (u8)(data>>16);
    buf[8*index+4] = (u8)(data>>24);
    buf[8*index+3] = (u8)(data>>32);
    buf[8*index+2] = (u8)(data>>40);
    buf[8*index+1] = (u8)(data>>48);
    buf[8*index+0] = (u8)(data>>56);
#if (CN_BYTE_BITS != 8)
    buf[8*index+7] &= 0xFF;
    buf[8*index+6] &= 0xFF;
    buf[8*index+5] &= 0xFF;
    buf[8*index+4] &= 0xFF;
    buf[8*index+3] &= 0xFF;
    buf[8*index+2] &= 0xFF;
    buf[8*index+1] &= 0xFF;
    buf[8*index+0] &= 0xFF;
#endif  //#if (CN_BYTE_BITS != 8)
}
////----16λ���ݴ�С�˶Ի�---------------------------------------------------------
////����: 16λ��������С�˶һ�
////����: value,�����16λ����
////����: ת�����16λ����
////------------------------------------------------------------------------------
//
//
////20140529�޸ģ�VisualDSP++��֧��inline
//
//u16 swaps(u16 value)
//{
//    return ((value&((u16)0xff00))>>8)|((value&((u16)0x00ff))<<8);
//}
//
////----32λ���ݴ�С�˶Ի�---------------------------------------------------------
////����: 32λ��������С�˶һ�
////����: value,�����32λ����
////����: ת�����32λ����
////------------------------------------------------------------------------------
//
//u32 swapl(u32 value)
//{
//    return ((value&((u32)0xff000000))>>24)|((value&((u32)0xff0000))>>8)|\
//            ((value&((u32)0xff00))<<8)|((value&((u32)0xff))<<24);
//}
//
////----16λ���ݱ��ض�ת��ΪС��---------------------------------------------------
////����: 16λ���ݱ��ض�ת��ΪС��
////����: value,�����16λ����
////����: ת�����16λ����
////------------------------------------------------------------------------------
//
////20140529�޸ģ�VisualDSP++��֧��inline
//u16  htoles(u16 value)
//{
//    if(CN_CFG_BYTE_ORDER == CN_CFG_LITTLE_ENDIAN)
//    {
//        return value;
//    }
//    else
//    {
//        return swaps(value);
//    }
//}
//
//
////----32λ���ݱ��ض�ת��ΪС��---------------------------------------------------
////����: 32λ���ݱ��ض�ת��ΪС��
////����: value,�����32λ����
////����: ת�����32λ����
////------------------------------------------------------------------------------
//
////20140529�޸ģ�VisualDSP++��֧��inline
//
//u32  htolel(u32 value)
//{
//    if(CN_CFG_BYTE_ORDER == CN_CFG_LITTLE_ENDIAN)
//    {
//        return value;
//    }
//    else
//    {
//        return swapl(value);
//    }
//}
////----16λ���ݱ��ض�ת��Ϊ���---------------------------------------------------
////����: 16λ���ݱ��ض�ת��Ϊ���
////����: value,�����16λ����
////����: ת�����16λ����
////------------------------------------------------------------------------------
//
////20140529�޸ģ�VisualDSP++��֧��inline
//u16  htobes(u16 value)
//{
//    if(CN_CFG_BYTE_ORDER == CN_CFG_BIG_ENDIAN)
//    {
//        return value;
//    }
//    else
//    {
//        return swaps(value);
//    }
//}
//
////----32λ���ݱ��ض�ת��Ϊ���---------------------------------------------------
////����: 32λ���ݱ��ض�ת��Ϊ���
////����: value,�����32λ����
////����: ת�����32λ����
////------------------------------------------------------------------------------
//
////20140529�޸ģ�VisualDSP++��֧��inline
//u32  htobel(u32 value)
//{
//    if(CN_CFG_BYTE_ORDER == CN_CFG_BIG_ENDIAN)
//    {
//        return value;
//    }
//    else
//    {
//        return swapl(value);
//    }
//}



