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
//����ģ��:s29glxxx����
//���ߣ�lst
//�汾��V1.0.1
//�ļ�����:s29glxxx flash���������ṩflash��ԭʼ��������
//����˵��:
//�޶���ʷ:
//2. ����: 2014-03-28
//   ����: liq
//   �°汾��: V1.0.1
//   �޸�˵��: ������������ʹ֧֮��norFlash��8λ��16λ���ֹ���ģʽ������ص�һЩ�궨������sys_space_config.h��
//1. ����: 2013-08-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "easynor_s29glxxx.h"
#include "SysSpace.h"
#include "easynorflash.h"

static struct tagEasynorFlashChip norflash_chip;
static u8 *chip_sector_buf;

#define cn_flash_size                   0x2000000    //32M
#define cn_flash_sector_length          0x20000     // 2*65536 bytes/sector һ�������ж��ٸ��ֽ�
#define cn_flash_sector_sum             (cn_flash_size/cn_flash_sector_length)
#define cn_flash_base_addr              CN_NORFLASH_BASE

#define cn_flash_page_length            cn_norflash_buf_size

#define cmd_erase_sector 0x30
#define cmd_reset_chip   0xF0

#define cn_wr_word_delay_us   500
#define cn_w_buffer_delay_us  1200

//----��дflash�ȴ��������-----------------------------------------------------
//���ܣ�����s29glxxx��toggle���ܣ�����ڲ�д������������ڽ��У��������ζ�������
//      ��ַ�����õ�0/1����Ľ����
//������delay_us,�ȴ�������ɵ�ʱ�䣬΢��
//ȫ�֣�chip_addr��оƬ����ַ
//���أ�true=��ɲ�ѯ��false=��ʱ��������
//-----------------------------------------------------------------------------
static bool_t __wait_end_s29glxxx(u32 delay_us)
{
    static u32 i=0;
    volatile u16 a,b;
    u16 c;
    i++;
    for(c=0;c<delay_us;c++)
    {
        a=*(u16 *)cn_flash_base_addr;
        b=*(u16 *)cn_flash_base_addr;
        if(a==b)
            return(true);
        Djy_DelayUs(1);   //��ʱ1uS
    }
    return(false);
}
//----����sector�ȴ��������-----------------------------------------------------
//���ܣ�����s29glxxx��toggle���ܣ�����ڲ�д������������ڽ��У��������ζ�������
//      ��ַ�����õ�0/1����Ľ��,s29gl128p������ʱ����3.5s��s29gl256s������
//      ʱ��Ϊ1.1s��Ϊ�˼��ݰ汾���˴�ȡ��ֵ
//��������
//ȫ�֣�cn_flash_base_addr��оƬ����ַ
//���أ�true=��ɲ�ѯ��false=��ʱ��������
//-----------------------------------------------------------------------------
static bool_t __wait_erase_end_s29glxxx(void)
{
    static u32 i=0;
    volatile u16 a,b;
    u16 c;
    i++;
    for(c=0;c<35;c++)//������ʱ��Ϊ3.5S
    {
        a=*(u16 *)cn_flash_base_addr;
        b=*(u16 *)cn_flash_base_addr;
        if(a==b)
            return(true);
        Djy_DelayUs(100000);
    }
    return(false);
}
 //----дһ����----------------------------------------------------------------
//���ܣ�дFlashһ����,д���ַӦ�����Ѿ�������,
//������addr,�ֶ����д���ַ,
//      Data:д�����
//ȫ�֣�chip_addr��оƬ����ַ
//���أ���
//---------------------------------------------------------------------------
/*
static void __word_write_s29glxxx(u16 * addr,u16 data)
{
    __wait_end_s29glxxx(cn_wr_word_delay_us);
    if(*addr == data)
        return;
#if(cn_norflash_bus_width==16)
    ((u16 *)cn_flash_base_addr)[0x555]=0xaa;
    ((u16 *)cn_flash_base_addr)[0x2aa]=0x55;
    ((u16 *)cn_flash_base_addr)[0x555]=0xa0;
#endif
#if(cn_norflash_bus_width==8)
    ((u16 *)cn_flash_base_addr)[0xaaa]=0xaa;
    ((u16 *)cn_flash_base_addr)[0x555]=0x55;
    ((u16 *)cn_flash_base_addr)[0xaaa]=0xa0;
#endif
    *addr=data;
    return;
}
*/
//----��bufferдflash---------------------------------------------------------
//���ܣ���buffer��д�뷽ʽдflash����ַ�����ǰ�ҳ��С����
//������dst_addr,ҳ�����д���ַ,�õ�ַΪ��Ե�ַ
//      src_addr,д�����ݵĵ�ַ�������ڲ���ǿ��ת��
//      len,д������ݳ��ȣ���λΪbyte
//ȫ�֣�cn_flash_base_addr��оƬ����ַ
//���أ���
//---------------------------------------------------------------------------
static void __buf_write_s29glxxx(u32 dst_addr, u32 src_addr, u32 len)
{
    u32 i,sector_addr;
    u16 *src = (u16*)src_addr;
    u16 *dst = (u16*)(dst_addr + cn_flash_base_addr);

    //����ÿ����ʼ��ַ
    sector_addr = dst_addr - dst_addr%cn_flash_sector_length + cn_flash_base_addr;
    if(!__wait_end_s29glxxx(cn_w_buffer_delay_us))
        return ;

#if(cn_norflash_bus_width==16)
    ((u16 *)cn_flash_base_addr)[0x555]=0xaa;
    ((u16 *)cn_flash_base_addr)[0x2aa]=0x55;
    len /= 2;
#elif(cn_norflash_bus_width==8)
    ((u16 *)cn_flash_base_addr)[0xaaa]=0xaa;
    ((u16 *)cn_flash_base_addr)[0x555]=0x55;
#endif

    *((u16 *)sector_addr) = 0x25;
    *((u16 *)sector_addr) = len-1;

    for(i = 0; i <= len - 1; i++)
    {
        *dst++ = *src++;
    }

    *((u16 *)sector_addr) = 0x29;

    return;
}

//----��λflashоƬ---------------------------------------------------------
//���ܣ���λnorflashоƬ��ֹͣ����оƬ����
//��������
//���أ���
//--------------------------------------------------------------------------
void reset_norflash(void)
{
    *((u16 *)cn_flash_base_addr) = cmd_reset_chip;
}

//----��ѯsector��׼������-------------------------------------------------
//���ܣ�������д��flash֮ǰ���Ƿ���Ҫ���������ֽڶ�д
//      ��ԭ����Ҫ����offset��(offset+size)����ż����������ȡ������
//      bufΪNULL,�����Ƿ���Ҫ���������Ƿ�ΪȫF��
//������sector:������Ƭ
//      offset:��Ƭ�е�ƫ�������ֽ�Ϊ��λ
//      buf:��������
//      size:����С���ֽ�Ϊ��λ
//���أ�true = ��׼���ã�����Ҫ����(��տ�),false = ��Ҫ������
//-----------------------------------------------------------------------------
bool_t query_sector_ready(u32 sector,u32 offset,u8 *buf,u32 size)
{
    u16 *sector_addr;
    u32 i,length;
    u16 wd,wdn;

    if(size == 0)
        return false;

    if(sector >= cn_flash_sector_sum)           //��������
        return false;
    if(offset +size > cn_flash_sector_length)   //������sector
        size = cn_flash_sector_length - offset;

    //�����offset��size������Ƚϵ�flash���ֳ���
    sector_addr = (u16*)(sector*cn_flash_sector_length+cn_flash_base_addr);
    if(!__wait_end_s29glxxx(cn_w_buffer_delay_us))
        return 0;

#if(cn_norflash_bus_width==16)
    length = ((offset + size)/2 + (offset + size)%2 )-offset/2;
    for(i = 0; i < length; i++)
    {
        if((i == 0) &&  (offset%2))  //��������,offset������,ע���С������
        {
            wd = sector_addr[offset/2 + i] | 0xFF00;//�����еĸ��ֽ�ƽ�Ƶ���8λ���߰�λд1
            if(((wd | buf[0]) != wd) || ((buf == NULL)&&(wd != 0xFFFF)))
                return false;
        }
        else if((i == length - 1) && ((offset + size)%2))//�������һ��,offset+size������
        {
            wd = (sector_addr[offset/2 + i]>>8) | 0xFF00;
            if(((wd | buf[size-1]) != wd) || ((buf == NULL)&&(wd != 0xFFFF)))
                return false;
        }
        else
        {
            wdn = (buf[i*2 - offset%2]<<8) +  buf[i*2+1 - offset%2];
            //�����û�а�0�ĳ�1��;���bufΪNULL�����
            if(((sector_addr[offset/2 + i] | wdn) != sector_addr[offset/2 + i])
                        || ((buf == NULL)&&(sector_addr[offset/2 + i] != 0xFFFF)))
                return false;
        }
    }
#elif(cn_norflash_bus_width==8)
    for(i = 0; i < size; i++)
    {
        wd=sector_addr[offset+i];
        wdn = buf[i];
        //�����û�а�0�ĳ�1��;���bufΪNULL�����
        if(((wd | wdn) != wd)
                    || ((buf == NULL)&&(wd != 0xFF)))
            return false;
    }
#endif

    return true;
}

//----��flash---------------------------------------------------------------
//���ܣ���flash�е�����
//������sector:������
//      offset:��Ƭ�е�ƫ�������ֽ�Ϊ��λ
//      buf:�����������ݴ����buf
//      size:�����ݴ�С���ֽ�Ϊ��λ
//���أ�cn_limit_uint32:��������
//      ����:ʵ�ʶ�����������
//--------------------------------------------------------------------------
u32 read_data(u32 sector,u32 offset,u8 *buf,u32 size)
{
    u16 *sector_addr;
    u32 i,length;

    if(size == 0)
        return 0;

    if(sector >= cn_flash_sector_sum)           //��������
        return CN_LIMIT_UINT32;
    if(offset +size > cn_flash_sector_length)   //������sector
        size = cn_flash_sector_length - offset;

    //�����offset��size������Ƚϵ�flash���ֳ���
    sector_addr = (u16*)(sector*cn_flash_sector_length+cn_flash_base_addr);
    if(!__wait_end_s29glxxx(cn_w_buffer_delay_us))
        return 0;

#if(cn_norflash_bus_width==16)
    length = ((offset + size)/2 + (offset + size)%2 )-offset/2;
    for(i = 0; i < length; i++)
    {
        if((i == 0) &&  (offset%2))  //��������,offset������
            buf[0] = (u8)sector_addr[offset/2] ;
        else if((i == length - 1) && ((offset + size)%2))    //�������һ��,offset+size������
            buf[size-1] = (u8)(sector_addr[offset/2 + length - 1]>>8);
        else    //offset����ż�Ի�Ӱ��buf�±�
        {
           buf[i*2 - offset%2]    = (u8)((sector_addr[i+offset/2])>>8);
           buf[i*2 + 1 - offset%2]= (u8)sector_addr[i+offset/2];
        }
    }
#elif(cn_norflash_bus_width==8)
    for(i = 0; i < size; i++)
    {
        buf[i]=(u8)sector_addr[offset+i];
    }
#endif
    return size;
}

//----дflash---------------------------------------------------------------
//���ܣ�дFlash,�ļ�ϵͳ�ڵ��øú���ǰ�����ж��Ƿ���Ҫ����
//������sector:������
//      offset:��Ƭ�е�ƫ�������ֽ�Ϊ��λ
//      buf:��Ҫ���͵����ݵ�ַ
//      size:д���������ֽ�Ϊ��λ
//���أ�cn_limit_uint32: ��������
//      ����ֵ: ʵ��д��������
//--------------------------------------------------------------------------
/*
u32 write_data(u32 sector,u32 offset,u8 *buf,u32 size)
{
    u16 *sector_addr;
    u32 i,length;
    u16 wd;

    if(size == 0)
        return 0;

    if(sector >= cn_flash_sector_sum)       //��������
        return CN_LIMIT_UINT32;
    if(offset +size > cn_flash_sector_length)//������sector
        size = cn_flash_sector_length - offset;

    sector_addr = (u16*)(sector*cn_flash_sector_length+cn_flash_base_addr);

    //��д���������д����ʼ������������
#if(cn_norflash_bus_width==16)
    length = ((offset + size)/2 + (offset + size)%2 )-offset/2;
    for(i = 0;i < length; i++)
    {
        if((i==0) && (offset%2))
            wd = 0xFF00 | buf[i];
        else if((i ==length -1) && ((offset + size)%2))
            wd = 0x00FF | (buf[size-1]<<8);
        else
            wd = (buf[i*2 - offset%2]<<8) +  buf[i*2+1 - offset%2];

        __data_write_s29glxxx(&sector_addr[i+offset/2], wd);
    }
#elif(cn_norflash_bus_width==8)
    for(i = 0;i < size; i++)
    {
        __data_write_s29glxxx(sector_addr+i+offset, buf[i]);
    }
#endif

    return size;
}
*/
//----дflash---------------------------------------------------------------
//���ܣ�дFlash,�ļ�ϵͳ�ڵ��øú���ǰ�����ж��Ƿ���Ҫ����
//������sector:������
//      offset:��Ƭ�е�ƫ�������ֽ�Ϊ��λ
//      buf:��Ҫ���͵����ݵ�ַ
//      size:д���������ֽ�Ϊ��λ
//���أ�cn_limit_uint32: ��������
//      ����ֵ: ʵ��д��������
//--------------------------------------------------------------------------
u32 write_data(u32 sector,u32 offset,u8 *buf,u32 size)
{
    u32 i,completed = 0,w_size;
    u8 page_buf[cn_flash_page_length];
    u32 page_addr,page_offset;
    u32 page_start,page_end;

    if(size == 0)
        return 0;
    if(sector >= cn_flash_sector_sum)       //��������
        return CN_LIMIT_UINT32;
    if(offset +size > cn_flash_sector_length)//������sector
        size = cn_flash_sector_length - offset;

    //���㱾�β�������ʼ�ͽ���ҳ��
    page_start = offset / cn_flash_page_length;
    page_end   = (offset + size - 1) / cn_flash_page_length;

    for(i = page_start; i <= page_end; i++)
    {
        page_addr = i * cn_flash_page_length;
        //����ǰ��û����ҳ��С��������
        if((i == page_start) && (offset%cn_flash_page_length))
        {
            page_offset = offset%cn_flash_page_length;//�ڸ�ҳ�ڵ�ƫ��
            read_data(sector,page_addr,page_buf,cn_flash_page_length);//������ҳ

            //��ҳ�ڿ�д��������
            w_size = cn_flash_page_length-page_offset;
            w_size = (size > w_size)?w_size:size;//����д�볤��
            memcpy(page_buf+page_offset,buf,w_size);

            completed += w_size;
        }
        else if((i == page_end) && ((offset+size)%cn_flash_page_length))//������ֽ�
        {
            page_offset = 0;
            w_size = (offset+size)%cn_flash_page_length;

            read_data(sector,page_addr,page_buf,cn_flash_page_length);//������ҳ
            memcpy(page_buf,buf+completed,w_size);
            completed += w_size;
        }
        else//�����м��512�ֽ�
        {
            memcpy(page_buf,buf+completed,cn_flash_page_length);
            completed += cn_flash_page_length;
        }

        //ʹ��д�������ķ�ʽ������д��flash
        page_addr = sector*cn_flash_sector_length + page_addr;//�������ҳ��������Ե�ַ
        __buf_write_s29glxxx(page_addr,
                (u32)page_buf,cn_flash_page_length);
    }
    return completed;
}
//----��������-----------------------------------------------------------
//���ܣ�����һ������,s29gl128ÿ128k�ֽ�Ϊ1����.����s29gl128p�����ĵ���
//      ����128K��sector��������0.5S�������Ҫ�ڲ���0.5S����ܶ�д
//������sector�� �����ţ�128k�ֽڶ���
//���أ�true=�ɹ�������false=ʧ�ܡ�
//-----------------------------------------------------------------------
bool_t erase_sector(uint32_t sector)
{
    u16 *sector_addr;
    if(sector >= cn_flash_sector_sum)
        return false;
    sector_addr = (u16*)(sector*cn_flash_sector_length+cn_flash_base_addr);
    if(!__wait_end_s29glxxx(cn_w_buffer_delay_us))   //�ȴ���һ�β������
        return false;
#if(cn_norflash_bus_width==16)
    ((u16 *)cn_flash_base_addr)[0x555]=0xaa;
    ((u16 *)cn_flash_base_addr)[0x2aa]=0x55;
    ((u16 *)cn_flash_base_addr)[0x555]=0x80;
    ((u16 *)cn_flash_base_addr)[0x555]=0xaa;
    ((u16 *)cn_flash_base_addr)[0x2aa]=0x55;
#elif(cn_norflash_bus_width==8)
    ((u16 *)cn_flash_base_addr)[0xaaa]=0xaa;
    ((u16 *)cn_flash_base_addr)[0x555]=0x55;
    ((u16 *)cn_flash_base_addr)[0xaaa]=0x80;
    ((u16 *)cn_flash_base_addr)[0xaaa]=0xaa;
    ((u16 *)cn_flash_base_addr)[0x555]=0x55;
#endif
    *sector_addr = cmd_erase_sector;

    __wait_erase_end_s29glxxx();
    return true;
}

//----��ʼ��norflashоƬ----------------------------------------------------
//���ܣ���ʼ��norflashоƬ�����ļ�ϵͳ
//����: para,���̷���
//����: 0= �ɹ���1=ʧ��
//--------------------------------------------------------------------------
ptu32_t ModuleInstall_fs_s29glxxx(ptu32_t para)
{
    reset_norflash();//��λnorflashоƬ
    chip_sector_buf = (u8*)M_Malloc(cn_flash_sector_length,0);
    if(chip_sector_buf == NULL)
        return 0;

    strcpy(norflash_chip.PTT_name,(const char*)para);
    norflash_chip.block_size     = cn_flash_sector_length;//���С
    norflash_chip.start_block     = CN_CFG_EASYNOR_START_SECTOR;//���ļ�ϵͳ��ʼsector
    norflash_chip.block_sum     = CN_CFG_EASYNOR_LENGTH_SECTOR;//sector���ڴ�Ÿ��ļ�ϵͳ
    norflash_chip.block_buf     = chip_sector_buf;
    norflash_chip.files_sum     = 0;
    norflash_chip.erase_block         = erase_sector;
    norflash_chip.query_block_ready   = query_sector_ready;
    norflash_chip.read_data           = read_data;
    norflash_chip.write_data          = write_data;

    if(Easynor_InstallChip(&norflash_chip) == 0)
    {
        extern ptu32_t Cmd_InstallEasynorFs(ptu32_t para);
        Cmd_InstallEasynorFs(0);
        return 0;
    }
    else
    {
        free(chip_sector_buf);
        return 1;
    }
}

