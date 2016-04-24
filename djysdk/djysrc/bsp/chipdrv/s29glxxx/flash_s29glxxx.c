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
#include "string.h"
#include "flash_s29glxxx.h"
#include "board-config.h"

#define CN_FLASH_SIZE                   0x2000000    //32M
#define CN_FLASH_SECTOR_LENGTH          0x20000     // 2*65536 bytes/sector һ�������ж��ٸ��ֽ�
#define CN_FLASH_SECTOR_SUM             (CN_FLASH_SIZE/CN_FLASH_SECTOR_LENGTH)
#define CN_FLASH_BASE_ADDR              CN_NORFLASH_BASE

#define CN_FLASH_PAGE_LENGTH            cn_norflash_buf_size //оƬbuffer��С

#define cmd_erase_sector 0x30

#define CN_WR_WORD_DELAY_US         500
#define CN_W_BUFFER_DELAY_US        1200

static u8 s_block_buf[CN_FLASH_SECTOR_LENGTH];

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
        a=*(u16 *)CN_FLASH_BASE_ADDR;
        b=*(u16 *)CN_FLASH_BASE_ADDR;
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
        a=*(u16 *)CN_FLASH_BASE_ADDR;
        b=*(u16 *)CN_FLASH_BASE_ADDR;
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
static void __data_write_s29glxxx(u32 addr,u16 data)
{
    __wait_end_s29glxxx(CN_WR_WORD_DELAY_US);
    if(*(u16 *)addr == data)
        return;
#if(cn_norflash_bus_width==16)
    ((u16 *)CN_FLASH_BASE_ADDR)[0x555]=0xaa;
    ((u16 *)CN_FLASH_BASE_ADDR)[0x2aa]=0x55;
    ((u16 *)CN_FLASH_BASE_ADDR)[0x555]=0xa0;
#endif
#if(cn_norflash_bus_width==8)
    ((u16 *)CN_FLASH_BASE_ADDR)[0xaaa]=0xaa;
    ((u16 *)CN_FLASH_BASE_ADDR)[0x555]=0x55;
    ((u16 *)CN_FLASH_BASE_ADDR)[0xaaa]=0xa0;
#endif
    *(u16 *)addr=data;
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
    u16 *dst = (u16*)(dst_addr+CN_FLASH_BASE_ADDR);

    sector_addr = dst_addr - dst_addr%CN_FLASH_SECTOR_LENGTH + CN_FLASH_BASE_ADDR;
    __wait_end_s29glxxx(CN_W_BUFFER_DELAY_US);

#if(cn_norflash_bus_width==16)
    ((u16 *)CN_FLASH_BASE_ADDR)[0x555]=0xaa;
    ((u16 *)CN_FLASH_BASE_ADDR)[0x2aa]=0x55;
    len /= 2;
#elif(cn_norflash_bus_width==8)
    ((u16 *)CN_FLASH_BASE_ADDR)[0xaaa]=0xaa;
    ((u16 *)CN_FLASH_BASE_ADDR)[0x555]=0x55;
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

//----���дǰ�Ƿ������------------------------------------------------------
//���ܣ������д������ǰ�Ƿ���Ҫ���������Ĺ����ǣ�����flash���ԣ�flash��ַ�ռ�
//      ��ÿһ��bitλ��ֻ�ܴ�1�ĳ�Ϊ0�������ܴ�0д��1����ˣ���д�����ݲ��ᵼ��
//      flash�е����ݱ���λ��0���1�������������������Ҫ����
//������addr:д�����ݵ���ʼ��ַ,��Ե�ַ
//      buf:���ݻ�����ָ��
//      len:д���������ֽ�Ϊ��λ
//���أ�True: ����Ҫ����
//      false: дǰ��Ҫ����
//--------------------------------------------------------------------------
static bool_t __Flash_WriteISReady(uint32_t addr, uint8_t *buf, uint32_t len)
{
    uint32_t i,length,offset;
    uint16_t wd,*flash_addr;

    flash_addr = (u16*)CN_FLASH_BASE_ADDR;

    //�����offset��size������Ƚϵ�flash���ֳ���
    offset = addr;
    if(!__wait_end_s29glxxx(CN_W_BUFFER_DELAY_US))
        return false;

#if(cn_norflash_bus_width==16)
    length = addr%2 + (len - addr%2)/2 + (addr + len)%2;      //�����������
    for(i = 0; i < length; i++)
    {
        if((i == 0) &&  (offset%2))  //��������,offset������,ע���С������
        {
            wd = flash_addr[offset/2] | 0xFF00;//�����еĸ��ֽ�ƽ�Ƶ���8λ���߰�λд1
            if(((wd | buf[0]) != wd) || ((buf == NULL)&&(wd != 0xFFFF)))
                return false;
        }
        else if((i == length - 1) && ((offset + len)%2))//�������һ��,offset+size������
        {
            wd = (flash_addr[offset/2 + i]>>8) | 0xFF00;
            if(((wd | buf[len-1]) != wd) || ((buf == NULL)&&(wd != 0xFFFF)))
                return false;
        }
        else
        {
            wd = (buf[i*2 - offset%2]<<8) +  buf[i*2+1 - offset%2];
            //�����û�а�0�ĳ�1��;���bufΪNULL�����
            if(((flash_addr[offset/2 + i] | wd) != flash_addr[offset/2 + i])
                        || ((buf == NULL)&&(flash_addr[offset/2 + i] != 0xFFFF)))
                return false;
        }
    }

#elif(cn_norflash_bus_width==8)
    for(i = 0; i < len; i++)
    {
        wd=flash_addr[offset+i];
        //�����û�а�0�ĳ�1��;���bufΪNULL�����
        if(((wd | buf[i]) != wd)
                    || ((buf == NULL)&&(wd != 0xFFFF)))
            return false;
    }
#endif

    return true;
}


//----дflash---------------------------------------------------------------
//���ܣ�������дflash�����øú���Ӧ�ó�����Ҫ�ж��Ƿ���Ҫ����flash�飬����Ҫ
//      �����飬���Ƚ��ȶ������ڵ����ݣ������ÿ���ٽ����ݺ�������ȫ��д��flash,
//      ������Ҫ�����飬���ֱ�ӵ���д���ݺ���
//������addr:д�����ݵ���ʼ��ַ,��Ե�ַ
//      buf:���ݻ�����ָ��
//      len:д���������ֽ�Ϊ��λ
//���أ�CN_FLASH_PARAM_ERROR: ��������
//      ����ֵ: ʵ��д��������
//--------------------------------------------------------------------------
uint32_t Flash_WriteDataCheck(uint32_t addr, uint8_t *buf, uint32_t len)
{
    u32 sector_addr,sector,offset;
    u32 sector_start,sector_end;
    u32 completed = 0,size_t;
    u8 *block_buf;

    if(len == 0)
        return 0;

    if((addr  +  len) > (CN_FLASH_SIZE - 1))
        return CN_FLASH_PARAM_ERROR;

    //������ʼ��
    sector_start = addr/CN_FLASH_SECTOR_LENGTH;
    sector_end   = (addr + len - 1)/CN_FLASH_SECTOR_LENGTH;

    block_buf = s_block_buf;

    for(sector = sector_start; sector <= sector_end; sector++)
    {
        //����ÿ���ʼ��ַ
        sector_addr = sector*CN_FLASH_SECTOR_LENGTH;
        if(sector == sector_start)
        {
            //����ÿ�д��ƫ��
            offset = addr - sector_addr;
            //����ÿ�д�볤��
            size_t = (addr + len) > (sector_addr + CN_FLASH_SECTOR_LENGTH - 1)
                            ? (sector_addr + CN_FLASH_SECTOR_LENGTH - addr)
                            : len;
        }
        else if(sector == sector_end)
        {
            offset = 0;
            size_t = (addr + len)%CN_FLASH_SECTOR_LENGTH;
        }
        else
        {
            offset = 0;
            size_t = CN_FLASH_SECTOR_LENGTH;
        }

        if(__Flash_WriteISReady(addr+completed,&buf[completed],size_t)==false)
        {
            //��Ҫ����������д��ȥ
            Flash_ReadData(sector_addr,block_buf,CN_FLASH_SECTOR_LENGTH);
            if(Flash_EraseSector(sector)==false)
                return 0;
            memcpy(block_buf+offset, &buf[completed],size_t);
            Flash_WriteData(sector_addr,block_buf,CN_FLASH_SECTOR_LENGTH);
            completed += size_t;
        }
        else
        {
            completed += Flash_WriteData(addr+completed,&buf[completed],size_t);
        }
    }
    return completed;
}

//----дflash---------------------------------------------------------------
//���ܣ�дFlash,�������飬�ú���ֻ������ָ����ַд��ָ�������ݣ���Ӧ�ò�������
//      д֮ǰ�Ƿ���Ҫ������������Ϊ�ڵ��øú���ʱ��д��flash��ַ�ĵ�ַ����ȫȫ0xFF
//������addr:д�����ݵ���ʼ��ַ,��Ե�ַ
//      buf:���ݻ�����ָ��
//      len:д���������ֽ�Ϊ��λ
//���أ�CN_FLASH_PARAM_ERROR: ��������
//      ����ֵ: ʵ��д��������
//--------------------------------------------------------------------------
/*
uint32_t Flash_WriteData(uint32_t addr, uint8_t *buf, uint32_t len)
{
    uint32_t loop,length,offset,waddr;
    uint16_t wd;

    if(len == 0)
        return 0;

    addr = addr + CN_FLASH_BASE_ADDR;

    if((addr < CN_FLASH_BASE_ADDR) ||
        (addr +  len > CN_FLASH_BASE_ADDR + (CN_FLASH_SIZE - 1)))
        return CN_FLASH_PARAM_ERROR;

    offset = addr - CN_FLASH_BASE_ADDR;

    //flash�еĴ洢һ�ɰ�С�˴洢
#if(cn_norflash_bus_width==16)
    length = addr%2 + (len - addr%2)/2 + (addr + len)%2;      //�����������
    for(loop = 0;loop < length; loop++)
    {
        //�ɸ�����flash��д0xFF��Ӱ��flash���ݵ����ԣ�wd�ĸ�8λд��0xFF
        if((loop==0) && (offset%2))
            wd = 0xFF00 | buf[loop];
        else if((loop ==length -1) && ((offset + len)%2))
            wd = 0x00FF | (buf[len-1]<<8);
        else
            wd = (buf[loop*2 - offset%2]<<8) +  buf[loop*2+1 - offset%2];

        waddr = addr + loop * 2 - (offset%2);
        __data_write_s29glxxx((u32)waddr,wd);
    }
#elif(cn_norflash_bus_width==8)
    for(loop = 0;loop < len; loop++)
    {
        __data_write_s29glxxx(addr+loop, buf[loop]);
    }
#endif

    return len;
}
*/

//----дflash---------------------------------------------------------------
//���ܣ�дFlash,�������飬�ú���ֻ������ָ����ַд��ָ�������ݣ���Ӧ�ò�������
//      д֮ǰ�Ƿ���Ҫ������������Ϊ�ڵ��øú���ʱ��д��flash��ַ�ĵ�ַ����ȫȫ0xFF
//������addr:д�����ݵ���ʼ��ַ,��Ե�ַ
//      buf:���ݻ�����ָ��
//      len:д���������ֽ�Ϊ��λ
//���أ�CN_FLASH_PARAM_ERROR: ��������
//      ����ֵ: ʵ��д��������
//--------------------------------------------------------------------------
uint32_t Flash_WriteData(uint32_t addr, uint8_t *buf, uint32_t len)
{
    u8 page_buf[CN_FLASH_PAGE_LENGTH];
    u32 i,page_start,page_end;
    u32 page_offset,page_addr;
    u32 w_size,completed = 0;

    if(len == 0)
        return 0;

    //������Ҫ��bufferд���ҳ��Χ
    page_start = addr/CN_FLASH_PAGE_LENGTH;
    page_end   = (addr+len-1)/CN_FLASH_PAGE_LENGTH;

    for(i = page_start; i <= page_end; i++)
    {
        //�����iҳ��ʼ��ַ
        page_addr = i * CN_FLASH_PAGE_LENGTH;
        if((i == page_start) && (addr%CN_FLASH_PAGE_LENGTH))//������ʼҳ
        {
            //����д��ĸ�ҳ���ݵ�ƫ��
            page_offset = addr%CN_FLASH_PAGE_LENGTH;
            Flash_ReadData(page_addr,page_buf,CN_FLASH_PAGE_LENGTH);

            //�����д���ҳ���ݴ�С
            w_size = CN_FLASH_PAGE_LENGTH - page_offset;
            w_size = (len > w_size)?(w_size):len;
            memcpy(page_buf+page_offset,buf,w_size);

            completed += w_size;
        }
        else if((i == page_end) && ((addr+len)%CN_FLASH_PAGE_LENGTH))
        {
            //����ҳƫ�����ʹ�С
            page_offset = 0;
            w_size = (addr+len)%CN_FLASH_PAGE_LENGTH;

            //������ҳ����
            Flash_ReadData(page_addr,page_buf,CN_FLASH_PAGE_LENGTH);
            memcpy(page_buf,buf+completed,w_size);
            completed += w_size;
        }
        else
        {
            memcpy(page_buf,buf+completed,CN_FLASH_PAGE_LENGTH);
            completed += CN_FLASH_PAGE_LENGTH;
        }
        //��������buffer�ķ�ʽд�뵽flash
        __buf_write_s29glxxx(page_addr,
                (u32)page_buf,CN_FLASH_PAGE_LENGTH);
    }
    return completed;
}
//----��flash---------------------------------------------------------------
//���ܣ���flash��ָ����ַ������
//������addr:�����ݵ���ʼ��ַ,��Ե�ַ
//      buf:�����������ݴ����bufָ��
//      len:�����ݴ�С���ֽ�Ϊ��λ
//���أ�CN_FLASH_PARAM_ERROR:��������
//      ����:ʵ�ʶ�����������
//--------------------------------------------------------------------------
uint32_t Flash_ReadData(uint32_t addr, uint8_t *buf, uint32_t len)
{
    u32 length,i;
    u16 *flash_addr;

    if(len == 0)
        return 0;

    if((addr +  len) > (CN_FLASH_SIZE - 1))//��������
        return CN_FLASH_PARAM_ERROR;

    flash_addr = (u16*)CN_FLASH_BASE_ADDR;
    //����ϴβ����Ƿ����
    if(!__wait_end_s29glxxx(CN_W_BUFFER_DELAY_US))
        return 0;

#if(cn_norflash_bus_width==16)
    length = addr%2 + (len - addr%2)/2 + (addr + len)%2;      //�����������
    for(i = 0; i < length; i++)
    {
        if((i == 0) &&  (addr%2))                           //��������,offset������
            buf[0] = (u8)flash_addr[addr/2] ;
        else if((i == length - 1) && ((addr + len)%2))    //�������һ��,offset+size������
            buf[len-1] = (u8)(flash_addr[addr/2 + length - 1]>>8);
        else    //offset����ż�Ի�Ӱ��buf�±�
        {
           buf[i*2 - addr%2]    = (u8)((flash_addr[i+addr/2])>>8);
           buf[i*2 + 1 - addr%2]= (u8)flash_addr[i+addr/2];
        }
    }
#elif(cn_norflash_bus_width==8)
    for(i = 0; i < len; i++)
    {
        buf[i]=(u8)flash_addr[addr+i];
    }
#endif
    return len;
}

//----��������-----------------------------------------------------------
//���ܣ�����һ������,s29gl128ÿ128k�ֽ�Ϊ1����.����s29gl128p�����ĵ���
//      ����128K��sector��������0.5S�������Ҫ�ڲ���0.5S����ܶ�д
//������sector�� �����ţ�128k�ֽڶ��룬
//���أ�true=�ɹ�������false=ʧ�ܡ�
//----------------------------------------------------------------------------
uint8_t Flash_EraseSector(uint32_t sector)
{
    u16 *sector_addr;
    if(sector >= CN_FLASH_SECTOR_SUM)
        return false;
    sector_addr = (u16*)(sector*CN_FLASH_SECTOR_LENGTH+CN_FLASH_BASE_ADDR);
    if(!__wait_end_s29glxxx(CN_W_BUFFER_DELAY_US))   //�ȴ���һ�β������
        return false;
#if(cn_norflash_bus_width==16)
    ((u16 *)CN_FLASH_BASE_ADDR)[0x555]=0xaa;
    ((u16 *)CN_FLASH_BASE_ADDR)[0x2aa]=0x55;
    ((u16 *)CN_FLASH_BASE_ADDR)[0x555]=0x80;
    ((u16 *)CN_FLASH_BASE_ADDR)[0x555]=0xaa;
    ((u16 *)CN_FLASH_BASE_ADDR)[0x2aa]=0x55;
#elif(cn_norflash_bus_width==8)
    ((u16 *)CN_FLASH_BASE_ADDR)[0xaaa]=0xaa;
    ((u16 *)CN_FLASH_BASE_ADDR)[0x555]=0x55;
    ((u16 *)CN_FLASH_BASE_ADDR)[0xaaa]=0x80;
    ((u16 *)CN_FLASH_BASE_ADDR)[0xaaa]=0xaa;
    ((u16 *)CN_FLASH_BASE_ADDR)[0x555]=0x55;
#endif
    *sector_addr = cmd_erase_sector;

    __wait_erase_end_s29glxxx();
    return true;
}

//----������ַ��-----------------------------------------------------------
//���ܣ�������ʼ��ַΪaddr��������ַΪlen���ڵĿռ�
//������addr�� ��ʼ��ַ,��Ե�ַ
//      len,���ȣ��ֽ�Ϊ��λ
//���أ�true=�ɹ�������false=ʧ�ܡ�
//----------------------------------------------------------------------------
/*
uint8_t Flash_EraseAddr(u32 addr, u32 len)
{
    u32 sector_addr,sector,offset;
    u32 sector_start,sector_end;
    u32 size_t,addr_map;
    u8 *block_buf;

    if(len == 0)
        return 0;

    addr_map = addr + CN_FLASH_BASE_ADDR;//ת���ɾ��Ե�ַ

    //�жϲ����Ƿ���ȷ
    if((addr_map < CN_FLASH_BASE_ADDR) ||
        (addr_map +  len > CN_FLASH_BASE_ADDR + (CN_FLASH_SIZE - 1)))
        return false;

    //������Ҫ��������ʼ��
    sector_start = (addr_map - CN_FLASH_BASE_ADDR)/CN_FLASH_SECTOR_LENGTH;
    sector_end   = (addr_map + len - CN_FLASH_BASE_ADDR - 1)/CN_FLASH_SECTOR_LENGTH;

    block_buf = s_block_buf;

    for(sector = sector_start; sector <= sector_end; sector++)
    {
        //����ÿ����ʼ��ַ
        sector_addr = (sector*CN_FLASH_SECTOR_LENGTH+CN_FLASH_BASE_ADDR);
        //�����ʼ�����
        if(sector == sector_start)
        {
            //�ÿ��ڲ�����ʼ��ַ
            offset = addr_map - sector_addr;
            //�ÿ��ڲ�������
            size_t = (addr_map + len) > (sector_addr + CN_FLASH_SECTOR_LENGTH - 1)
                        ? (sector_addr + CN_FLASH_SECTOR_LENGTH - addr_map)
                        : len;

            //����ǰ�ȶ�����
            Flash_ReadData(addr-offset,block_buf,offset);//����ǰ����
            Flash_ReadData(addr+size_t,block_buf + (offset+size_t),
                    CN_FLASH_SECTOR_LENGTH - (size_t + offset));//�����󲿷�

            Flash_EraseSector(sector);
            //д������
            Flash_WriteData(addr-offset,block_buf,offset);//д��ǰ����
            Flash_WriteData(addr+size_t,block_buf + (offset+size_t),
                    CN_FLASH_SECTOR_LENGTH - (size_t + offset));//д��󲿷�
        }
        else if(sector == sector_end)//������������
        {
            //�ÿ��ڲ�����ʼ��ַ
            offset = 0;
            //�ÿ��ڲ�������
            size_t = addr_map + len - sector_addr;

            //ֻ������󲿷�
            Flash_ReadData(addr+len,block_buf,
                    CN_FLASH_SECTOR_LENGTH - size_t);
            Flash_EraseSector(sector);
            //д��󲿷ֱ���������
            Flash_WriteData(addr+len,block_buf,
                    CN_FLASH_SECTOR_LENGTH - size_t);
        }
        else    //�м��ַ��ֱ�Ӳ���
        {
            Flash_EraseSector(sector);
        }
    }
    return true;
}
*/
uint8_t Flash_EraseAddr(u32 addr, u32 len)
{
    u32 sector_addr,sector,offset;
    u32 sector_start,sector_end;
    u32 size_t;
    u8 *block_buf;

    if(len == 0)
        return 0;

    //�жϲ����Ƿ���ȷ
    if((addr + len) > (CN_FLASH_SIZE - 1))
        return false;

    //������Ҫ��������ʼ��
    sector_start = addr/CN_FLASH_SECTOR_LENGTH;
    sector_end   = (addr + len - 1)/CN_FLASH_SECTOR_LENGTH;

    block_buf = s_block_buf;

    for(sector = sector_start; sector <= sector_end; sector++)
    {
        //����ÿ����ʼ��ַ
        sector_addr = sector*CN_FLASH_SECTOR_LENGTH;
        //�����ʼ�����
        if(sector == sector_start)
        {
            //�ÿ��ڲ�����ʼ��ַ
            offset = addr - sector_addr;
            //�ÿ��ڲ�������
            size_t = (addr + len) > (sector_addr + CN_FLASH_SECTOR_LENGTH - 1)
                        ? (sector_addr + CN_FLASH_SECTOR_LENGTH - addr)
                        : len;

            //����ǰ�ȶ�����
            Flash_ReadData(sector_addr,block_buf,offset);//����ǰ����
            Flash_ReadData(addr+size_t,block_buf + (offset+size_t),
                    CN_FLASH_SECTOR_LENGTH - (size_t + offset));//�����󲿷�

            Flash_EraseSector(sector);
            //д������
            Flash_WriteData(sector_addr,block_buf,offset);//д��ǰ����
            Flash_WriteData(addr+size_t,block_buf + (offset+size_t),
                    CN_FLASH_SECTOR_LENGTH - (size_t + offset));//д��󲿷�
        }
        else if(sector == sector_end)//������������
        {
            //�ÿ��ڲ�����ʼ��ַ
            offset = 0;
            //�ÿ��ڲ�������
            size_t = addr + len - sector_addr;

            //ֻ������󲿷�
            Flash_ReadData(addr+len,block_buf,
                    CN_FLASH_SECTOR_LENGTH - size_t);
            Flash_EraseSector(sector);
            //д��󲿷ֱ���������
            Flash_WriteData(addr+len,block_buf,
                    CN_FLASH_SECTOR_LENGTH - size_t);
        }
        else    //�м��ַ��ֱ�Ӳ���
        {
            Flash_EraseSector(sector);
        }
    }
    return true;
}

//----���Flash�����Ƿ����---------------------------------------------------
//���ܣ��鿴Flash�����Ƿ���ɣ���ʱ����
//������us����ʱʱ��
//���أ�true,�����ɹ�����ȷ���أ�false,����ʧ�ܣ����󷵻�
//--------------------------------------------------------------------------
bool_t Flash_OpetionIsOK(void)
{
    return __wait_end_s29glxxx(CN_W_BUFFER_DELAY_US);
}
//----��λflashоƬ---------------------------------------------------------
//���ܣ���λnorflashоƬ��ֹͣ����оƬ����
//��������
//���أ���
//--------------------------------------------------------------------------
void Flash_ResetChip(void)
{
    *((u16 *)CN_FLASH_BASE_ADDR) = 0x00F0;
}

//----��ȡflashоƬDeviceID---------------------------------------------------------
//���ܣ���ȡflashоƬDevice ID
//��������
//���أ�Device ID
//--------------------------------------------------------------------------
u32 Flash_GetDevID(void)
{
    u32 pnDevCode;
    u16 tmp;
#if(cn_norflash_bus_width==16)
    ((u16 *)CN_FLASH_BASE_ADDR)[0x555]=0xaa;
    ((u16 *)CN_FLASH_BASE_ADDR)[0x2aa]=0x55;
    ((u16 *)CN_FLASH_BASE_ADDR)[0x555]=0x90;
    tmp=*(u16 *)(CN_FLASH_BASE_ADDR+0x201);
    pnDevCode = (tmp&0xFFFF) << 16;
    tmp=*(u16 *)(CN_FLASH_BASE_ADDR+0x20E);
    pnDevCode += (tmp&0xFF) << 8;
    tmp=*(u16 *)(CN_FLASH_BASE_ADDR+0x20F);
    pnDevCode += tmp&0xFF;
#elif(cn_norflash_bus_width==8)
    ((u16 *)CN_FLASH_BASE_ADDR)[0xaaa]=0xaa;
    ((u16 *)CN_FLASH_BASE_ADDR)[0x555]=0x55;
    ((u16 *)CN_FLASH_BASE_ADDR)[0xaaa]=0x90;
    tmp=*(u16 *)(CN_FLASH_BASE_ADDR+0x002);
    pnDevCode = (tmp&0xFFFF) << 16;
    tmp=*(u16 *)(CN_FLASH_BASE_ADDR+0x01C);
    pnDevCode += (tmp&0xFF) << 8;
    tmp=*(u16 *)(CN_FLASH_BASE_ADDR+0x01E);
    pnDevCode += tmp&0xFF;
#endif
    return pnDevCode;
}



