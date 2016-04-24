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
//����ģ�飺SPI FLASHģ��
//���ߣ�liq
//�汾��V0.1.0
//�ļ�������DJYOS��SPI FLASH AT45DB321D�ײ�����
//����˵����
//�޶���ʷ��
//1. ����: 2014-4-15
//   ���ߣ�
//   �°汾�ţ�V0.1.0
//   �޸�˵����ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "cpu_peri.h"
#include "at45db321.h"
#include "board_config.h"

/*------------------------Time Symbol--------------------------------
Symbol	Parameter								Typ 	Max 	Units
fSCK 	SCK Frequency 									66 		MHz
fCAR1 	SCK Frequency for Continuous Array Read			66 		MHz
fCAR2	SCK Frequency for Continuous Array Read
		(Low Frequency) 								33 		MHz
tXFR 	Page to Buffer Transfer Time 					200 	��s
tcomp 	Page to Buffer Compare Time 					200 	��s
tEP 	Page Erase and Programming Time			17 		40 		ms
tP 		Page Programming Time					3 		6 		ms
tPE 	Page Erase Time							15 		35 		ms
tBE 	Block Erase Time						45 		100 	ms
-------------------------------------------------------------------*/

//Read Commands
#define AT45_MainMem_Page_Read 			0xD2
#define AT45_Continous_Array_Read_LC 	0xE8		//(Legacy Command) up to 66M
#define AT45_Continous_Array_Read_LF 	0x03		//(Low Frequency) up to 33M
#define AT45_Continous_Array_Read_HF 	0x0B		//(High Frequency) up to 66M
#define AT45_Buff1_Read_LF				0xD1		//(Low Frequency)
#define AT45_Buff2_Read_LF				0xD3		//(Low Frequency)
#define AT45_Buff1_Read 				0xD4
#define AT45_Buff2_Read 				0xD6
//Program and Erase Commands
#define AT45_Buff1_Write 				0x84
#define AT45_Buff2_Write 				0x87
#define AT45_Buff1_to_MainMem_Page_Prog_vsErase 	0x83	//Buffer 1 to Main Memory Page Program with Built-in Erase
#define AT45_Buff2_to_MainMem_Page_Prog_vsErase 	0x86	//Buffer 2 to Main Memory Page Program with Built-in Erase
#define AT45_Buff1_to_MainMem_Page_Prog_noErase 	0x88	//Buffer 1 to Main Memory Page Program without Built-in Erase
#define AT45_Buff2_to_MainMem_Page_Prog_noErase 	0x89	//Buffer 2 to Main Memory Page Program without Built-in Erase
#define AT45_Page_Erase 				0x81
#define AT45_Block_Erase 				0x50
#define AT45_Sector_Erase 				0x7C
#define AT45_Chip_Erase1 				0xC7
#define AT45_Chip_Erase2 				0x94
#define AT45_Chip_Erase3 				0x80
#define AT45_Chip_Erase4 				0x9A
#define AT45_MainMem_Page_Prog_Through_Buff1 		0x82
#define AT45_MainMem_Page_Prog_Through_Buff2 		0x85
//Additional Commands
#define AT45_MainMem_Page_to_Buff1_Transfer 		0x53
#define AT45_MainMem_Page_to_Buff2_Transfer 		0x55
#define AT45_MainMem_Page_to_Buff1_Compare 			0x60
#define AT45_MainMem_Page_to_Buff2_Compare 			0x61
#define AT45_Auto_Page_Rewrite_through_Buff1 		0x58
#define AT45_Auto_Page_Rewrite_through_Buff2 		0x59
#define AT45_Deep_Power_down 						0xB9
#define AT45_Resume_Power_down 						0xAB
#define AT45_Status_Register_Read 					0xD7
#define AT45_Command_ID         					0x9F	//Manufacturer and Device ID Read
//AT45_Status_Register bit define
#define AT45_Status_Reg_Bit_BUSY 		0x80	//1=Ready   0=Busy
#define AT45_Status_Reg_Bit_COMP 		0x40	//1=not match	0=match
#define AT45_Status_Reg_Bit_PRTC 		0x02	//1=En protect	0=Dis protect
#define AT45_Status_Reg_Bit_PGSZ 		0x01	//1=page size 512	0=page size 528

#ifndef	AT45_Page_Size
//AT45_Page_SizeĬ��ֵΪ528���粻ʹ��Ĭ��ֵ������config-prj.h�ж���
#define AT45_Page_Size		528
#endif

#define AT45_Block_Size		AT45_Page_Size*8

//buff num define
#define AT45_Buff1	1
#define AT45_Buff2	2
//page erase define
#define vs_Erase	1
#define no_Erase	2

tagSpiConfig *flash_spi_Config;

//Commandָ�����
u8 _at45db321_Command[10]={0};
//SPI FLASH���ݻ�����
u8 _at45db321_buff[AT45_Page_Size] = {0};
//��ǰ������д���ݵ�Buff,������һBuff�п�����������FLASHд���ݵĽ׶�
u8 _at45db321_Ready_Buff = AT45_Buff1;

/*---------------------test use only----------------------
#define test_buff_size	10240
section("seg_int_data") u32 Data_write[test_buff_size]={0};
section("seg_int_data") u32 Data_read[test_buff_size]={0};
u32 buff1_cnt=0,buff2_cnt=0;
---------------------test use only----------------------*/

//note:	Ӧ�ó����е�Addressʵ��Ϊ�����ַ����Ҫ����ת�����ܳ�ΪAT45�е�ʵ�ʵ�ַ
//		AT45�õ�ַ��A0-A9����Page�ڵ�ƫ�Ƶ�ַ��A10-A23����Page��ַ������ΪAT45��Page��С��528bytes
//		�����ô˱�ʾ��ʵ�������ַ�ǲ������ģ�����Ӧ�ó�����ʹ�õ������������ַ��Ҫ�����任���ɳ�Ϊʵ�������ַ

//----ͨ�������ַ����Page��ƫ�Ƶ�ַ-------------------------------------------
//���ܣ�ͨ�������ַ����Page��ƫ�Ƶ�ַ
//		Address(23bit) = page_addr(13bit) + byte_offset_addr(10bit for 528)
//		1 page = 528 bytes
//������Address	�����ַ
//���أ�Page��ƫ�Ƶ�ַ��ַbyte_offset_addr
//-----------------------------------------------------------------------------
u32 _at45db321_Offset_Caculate(u32 Address)
{	
#if AT45_Page_Size==512
	return ( (Address%AT45_Page_Size)&0x1FF );
#elif AT45_Page_Size==528
	return ( (Address%AT45_Page_Size)&0x3FF );
#endif
}

//----ͨ�������ַ����Page��ַ------------------------------------------------
//���ܣ�ͨ�������ַ����Page��ַ
//		Address(23bit) = page_addr(13bit) + byte_offset_addr(10bit for 528)
//		1 page = 528 bytes
//������Address	�����ַ
//���أ�Page��ַ
//-----------------------------------------------------------------------------
u32 _at45db321_Page_Caculate(u32 Address)
{	
	return (u32)( Address/AT45_Page_Size );
}

//----ͨ��Page��ַ����Block��ַ------------------------------------------------
//���ܣ�ͨ��Page��ַ����Block��ַ
//		page_addr = block_addr(10bit) + page_offset_addr(3bit)
//		1 block = 8 pages
//������page_addr	Page��ַ
//���أ�Block��ַ
//-----------------------------------------------------------------------------
u32 _at45db321_Block_Caculate(u32 page_addr)
{	
	return ( page_addr>>3 );
}

//----д�����ݳ��ȼ���--------------------------------------------------------
//���ܣ�ͨ���Ƚ�Page�Ĵ�Сȷ�����ο���д������ݳ���
//������byte_offset_addr	Page�ڵ�ƫ�Ƶ�ַ
//		data_len			����д������ݳ���
//���أ�����ʵ���ܹ�д������ݳ���
//-----------------------------------------------------------------------------
u32 _at45db321_Written_Caculate(u32 byte_offset_addr,u32 data_len)
{
	u32 written_data_len;
	written_data_len = AT45_Page_Size-byte_offset_addr;
	if( written_data_len>data_len )
		written_data_len = data_len;
	return written_data_len;
}

//----SPI FLASH�������ݶ�ȡ----------------------------------------------------
//���ܣ���AT45�ĵ�ַAddress����ʼ��������ȡdata_len���ȵ����ݣ�����data
//		����ַ�ۼƵ�FLASHĩβ�򷵻ؿ�ͷ��������ȡ
//������page_addr	Page��ַ
//		byte_offset_addr	Page�ڵ�ƫ�Ƶ�ַ
//		*data		���ݴ洢�׵�ַ
//		data_len	���ݳ���
//���أ�ʵ�ʶ�ȡ�����ݳ���
//-----------------------------------------------------------------------------
u32 _at45db321_Continuous_Array_Read(u32 page_addr,u32 byte_offset_addr,u8 *data,u32 data_len)
{
	u32 i;
			
	Spi_ActiveCS(CFG_FLASH_SPI_BUS,CFG_FLASH_SPI_CS);
	
	//array_addr
	_at45db321_Command[0] = AT45_Continous_Array_Read_HF;
#if AT45_Page_Size==512
	_at45db321_Command[1] = (page_addr>>7)&0xFF;
	_at45db321_Command[2] = ((page_addr<<1)|(byte_offset_addr>>8))&0xFF;
#elif AT45_Page_Size==528
	_at45db321_Command[1] = (page_addr>>6)&0xFF;
	_at45db321_Command[2] = ((page_addr<<2)|(byte_offset_addr>>8))&0xFF;
#endif
	_at45db321_Command[3] = byte_offset_addr & 0xFF;
	_at45db321_Command[4] = 0xFF;
	
	Spi_TxRx(CFG_FLASH_SPI_BUS,_at45db321_Command,5,data,data_len,5);
	
	Spi_InActiveCS(CFG_FLASH_SPI_BUS,CFG_FLASH_SPI_CS);
	
	return data_len;
}

//----SPI FLASHд���ݵ�������--------------------------------------------------
//���ܣ�SPI FLASHд���ݵ�������
//������buff_num	Ŀ�껺����ѡ��
//		buff_addr	��������ƫ�Ƶ�ַ
//		*data		���ݻ�ȡ�׵�ַ
//		data_len	���ݳ���
//���أ�ʵ��д������ݳ���
//-----------------------------------------------------------------------------
u32 _at45db321_Buff_Write(u32 buff_num,u32 buff_addr,u8 *data,u32 data_len)
{
	Spi_ActiveCS(CFG_FLASH_SPI_BUS,CFG_FLASH_SPI_CS);
	
	if(buff_num == AT45_Buff1)
		_at45db321_Command[0] = AT45_Buff1_Write;
	else if(buff_num == AT45_Buff2)
		_at45db321_Command[0] = AT45_Buff2_Write;
	else
	{
		Spi_InActiveCS(CFG_FLASH_SPI_BUS,CFG_FLASH_SPI_CS);
		return 0;
	}
	//buff_addr
	_at45db321_Command[1] = 0xFF;
	_at45db321_Command[2] = (buff_addr>>8)&0xFF;
	_at45db321_Command[3] = (buff_addr)&0xFF;
	
	Spi_TxRx(CFG_FLASH_SPI_BUS,_at45db321_Command,4,NULL,0,0);
	//data
	Spi_TxRx(CFG_FLASH_SPI_BUS,data,data_len,NULL,0,0);
	
	Spi_InActiveCS(CFG_FLASH_SPI_BUS,CFG_FLASH_SPI_CS);
	
	return data_len;
}

//----SPI FLASH������������----------------------------------------------------
//���ܣ�SPI FLASH������������
//������buff_num	Ŀ�껺����ѡ��
//		buff_addr	��������ƫ�Ƶ�ַ
//		*data		���ݽ����׵�ַ
//		data_len	���ݳ���
//���أ�ʵ�ʶ�ȡ�����ݳ���
//-----------------------------------------------------------------------------
u32 _at45db321_Buff_Read(u32 buff_num,u32 buff_addr,u8 *data,u32 data_len)
{	
	Spi_ActiveCS(CFG_FLASH_SPI_BUS,CFG_FLASH_SPI_CS);
	
	if(buff_num == AT45_Buff1)
		_at45db321_Command[0] = AT45_Buff1_Read_LF;
	else if(buff_num == AT45_Buff2)
		_at45db321_Command[0] = AT45_Buff2_Read_LF;
	else
	{
		Spi_InActiveCS(CFG_FLASH_SPI_BUS,CFG_FLASH_SPI_CS);
		return 0;
	}
	//buff_addr
	_at45db321_Command[1] = 0xFF;
	_at45db321_Command[2] = (buff_addr>>8)&0xFF;
	_at45db321_Command[3] = (buff_addr)&0xFF;
	_at45db321_Command[4] = 0xFF;
	
	Spi_TxRx(CFG_FLASH_SPI_BUS,_at45db321_Command,5,data,data_len,5);
	
	Spi_InActiveCS(CFG_FLASH_SPI_BUS,CFG_FLASH_SPI_CS);
	
	return data_len;
}

//----SPI FLASH��Page���ݶ���������--------------------------------------------
//���ܣ�SPI FLASH��Page���ݶ���������
//������buff_num	Ŀ�껺����ѡ��
//		page_addr	Ŀ��Page��ַ
//���أ�false:	ʧ��		
//		true:	�ɹ�
//-----------------------------------------------------------------------------
bool_t _at45db321_Page_to_Buff(u32 buff_num,u32 page_addr)
{
	Spi_ActiveCS(CFG_FLASH_SPI_BUS,CFG_FLASH_SPI_CS);
	
	if(buff_num == AT45_Buff1)
		_at45db321_Command[0] = AT45_MainMem_Page_to_Buff1_Transfer;
	else if(buff_num == AT45_Buff2)
		_at45db321_Command[0] = AT45_MainMem_Page_to_Buff2_Transfer;
	else
	{
		Spi_InActiveCS(CFG_FLASH_SPI_BUS,CFG_FLASH_SPI_CS);
		return 0;
	}
#if AT45_Page_Size==512
	_at45db321_Command[1] = (page_addr>>7)&0xFF;
	_at45db321_Command[2] = (page_addr<<1)&0xFF;
#elif AT45_Page_Size==528
	_at45db321_Command[1] = (page_addr>>6)&0xFF;
	_at45db321_Command[2] = (page_addr<<2)&0xFF;
#endif
	_at45db321_Command[3] = 0xFF;
	
	Spi_TxRx(CFG_FLASH_SPI_BUS,_at45db321_Command,4,NULL,0,0);
	
	Spi_InActiveCS(CFG_FLASH_SPI_BUS,CFG_FLASH_SPI_CS);
	
	return true;
}

//----SPI FLASH������������д��Page--------------------------------------------
//���ܣ�SPI FLASH������������д��Page
//������buff_num	Ŀ�껺����ѡ��
//		page_addr	Ŀ��Page��ַ
//		With_Erase	дPage֮ǰ�Ƿ���Ҫ����
//���أ�false:	ʧ��		
//		true:	�ɹ�
//-----------------------------------------------------------------------------
bool_t _at45db321_Buff_to_Page(u32 buff_num,u32 page_addr,u32 With_Erase)
{
	Spi_ActiveCS(CFG_FLASH_SPI_BUS,CFG_FLASH_SPI_CS);
	
	if(buff_num == AT45_Buff1)
	{
		if(With_Erase == no_Erase)
			_at45db321_Command[0] = AT45_Buff1_to_MainMem_Page_Prog_noErase;
		else if(With_Erase == vs_Erase)
			_at45db321_Command[0] = AT45_Buff1_to_MainMem_Page_Prog_vsErase;
		else
		{
			Spi_InActiveCS(CFG_FLASH_SPI_BUS,CFG_FLASH_SPI_CS);
			return 0;
		}
	}
	else if(buff_num == AT45_Buff2)
	{
		if(With_Erase == no_Erase)
			_at45db321_Command[0] = AT45_Buff2_to_MainMem_Page_Prog_noErase;
		else if(With_Erase == vs_Erase)
			_at45db321_Command[0] = AT45_Buff2_to_MainMem_Page_Prog_vsErase;
		else
		{
			Spi_InActiveCS(CFG_FLASH_SPI_BUS,CFG_FLASH_SPI_CS);
			return 0;
		}
	}
	else
	{
		Spi_InActiveCS(CFG_FLASH_SPI_BUS,CFG_FLASH_SPI_CS);
		return 0;
	}
	
#if AT45_Page_Size==512
	_at45db321_Command[1] = (page_addr>>7)&0xFF;
	_at45db321_Command[2] = (page_addr<<1)&0xFF;
#elif AT45_Page_Size==528
	_at45db321_Command[1] = (page_addr>>6)&0xFF;
	_at45db321_Command[2] = (page_addr<<2)&0xFF;
#endif
	_at45db321_Command[3] = 0xFF;
	
	Spi_TxRx(CFG_FLASH_SPI_BUS,_at45db321_Command,4,NULL,0,0);
	
	Spi_InActiveCS(CFG_FLASH_SPI_BUS,CFG_FLASH_SPI_CS);
	
	return true;
}

//----SPI FLASH Page���������ж�-----------------------------------------------
//���ܣ��ж�FLASH�����е���������Ҫд������ݶ�֮���ƥ��ȣ��ж��Ƿ���Ҫ����
//		FLASH��ӦPAGE����д����Ҫд��������Ѿ�д��Buff��Flash�ж�����������_at45db321_buff��
//������*data		��Ҫд������ݵ���ʼ��ַ
//		data_len	��Ҫд������ݳ���
//���أ�false:	��Ҫ����		
//		true:	����Ҫ����
//-----------------------------------------------------------------------------
bool_t _at45db321_Need_Erase_orNot(u8 *data,u32 data_len)
{
	u32 i;
		
	for(i=0;i<data_len;i++)
	{
		if((_at45db321_buff[i] & data[i]) != data[i]
			|| ((data == NULL)&&(_at45db321_buff[i] != 0xFF)) )
			return false;
	}
	
	return true;
}

//----SPI FLASH Page����-------------------------------------------------------
//���ܣ�SPI FLASH Page����
//��������
//���أ���
//-----------------------------------------------------------------------------
void _at45db321_Page_Erase(u32 page_addr)
{	
	Spi_ActiveCS(CFG_FLASH_SPI_BUS,CFG_FLASH_SPI_CS);
	
	_at45db321_Command[0] = AT45_Page_Erase;
#if AT45_Page_Size==512
	_at45db321_Command[1] = (page_addr>>7)&0xFF;
	_at45db321_Command[2] = (page_addr<<1)&0xFF;
#elif AT45_Page_Size==528
	_at45db321_Command[1] = (page_addr>>6)&0xFF;
	_at45db321_Command[2] = (page_addr<<2)&0xFF;
#endif
	_at45db321_Command[3] = 0xFF;
	
	Spi_TxRx(CFG_FLASH_SPI_BUS,_at45db321_Command,4,NULL,0,0);
	
	Spi_InActiveCS(CFG_FLASH_SPI_BUS,CFG_FLASH_SPI_CS);
}

//----SPI FLASH Block����------------------------------------------------------
//���ܣ�SPI FLASH Block����
//��������
//���أ���
//-----------------------------------------------------------------------------
void _at45db321_Block_Erase(u32 block_addr)
{
/*
	Spi_ActiveCS(CFG_FLASH_SPI_BUS,CFG_FLASH_SPI_CS);
	
	_at45db321_Command[0] = AT45_Block_Erase;
	_at45db321_Command[1] = (block_addr>>4)&0xFF;
	_at45db321_Command[2] = (block_addr<<4)&0xFF;
	_at45db321_Command[3] = 0xFF;
	
	Spi_TxRx(CFG_FLASH_SPI_BUS,_at45db321_Command,4,NULL,0,0);
	
	Spi_InActiveCS(CFG_FLASH_SPI_BUS,CFG_FLASH_SPI_CS);
*/
}

//----SPI FLASHȫƬ����--------------------------------------------------------
//���ܣ�SPI FLASHȫƬ����
//��������
//���أ���
//-----------------------------------------------------------------------------
void _at45db321_Chip_Erase(void)
{
/*
	Spi_ActiveCS(CFG_FLASH_SPI_BUS,CFG_FLASH_SPI_CS);
	
	_at45db321_Command[0] = AT45_Chip_Erase1;
	_at45db321_Command[1] = AT45_Chip_Erase2;
	_at45db321_Command[2] = AT45_Chip_Erase3;
	_at45db321_Command[3] = AT45_Chip_Erase4;
	
	Spi_TxRx(CFG_FLASH_SPI_BUS,_at45db321_Command,4,NULL,0,0);
	
	Spi_InActiveCS(CFG_FLASH_SPI_BUS,CFG_FLASH_SPI_CS);
*/
}

//----SPI FLASH Page Size����--------------------------------------------------
//���ܣ�SPI FLASH Page Size����Ϊ512�����ñ���������Ҫ�����ϵ磬�������FLASH��õ���������
//		ע�⣺�ù��̲����棡��AT45Ĭ��PageSizeΪ528bytes�����ñ����������޸�Ϊ512
//��������
//���أ���
//-----------------------------------------------------------------------------
void _at45db321_Binary_Page_Size_512(void)
{
/*
	Spi_ActiveCS(CFG_FLASH_SPI_BUS,CFG_FLASH_SPI_CS);
	
	_at45db321_Command[0] = 0x3D;
	_at45db321_Command[1] = 0x2A;
	_at45db321_Command[2] = 0x80;
	_at45db321_Command[3] = 0xA6;
	
	Spi_TxRx(CFG_FLASH_SPI_BUS,_at45db321_Command,4,NULL,0,0);
	
	Spi_InActiveCS(CFG_FLASH_SPI_BUS,CFG_FLASH_SPI_CS);
	
	while(1);
*/
}

//----SPI FLASH��ȡStatus Register---------------------------------------------
//���ܣ�SPI FLASH��ȡStatus Registerֵ
//��������
//���أ�Status Registerֵ
//-----------------------------------------------------------------------------
u32 _at45db321_Read_Status(void)
{
	u8 Data;
	
	Spi_ActiveCS(CFG_FLASH_SPI_BUS,CFG_FLASH_SPI_CS);
	
	_at45db321_Command[0] = AT45_Status_Register_Read;	
	Spi_TxRx(CFG_FLASH_SPI_BUS,_at45db321_Command,1,&Data,1,1);
	
	Spi_InActiveCS(CFG_FLASH_SPI_BUS,CFG_FLASH_SPI_CS);
	
	return Data;
}
//----SPI FLASHУ��оƬID------------------------------------------------------
//���ܣ�SPI FLASHУ��оƬID
//��������
//���أ�true = У��������false = У�鲻�ɹ�
//-----------------------------------------------------------------------------
bool_t _at45db321_Check_ID(void)
{
	u8 Data;
	
	_at45db321_Command[0] = AT45_Command_ID;
	
	Spi_ActiveCS(CFG_FLASH_SPI_BUS,CFG_FLASH_SPI_CS);
	
	Spi_TxRx(CFG_FLASH_SPI_BUS,_at45db321_Command,1,&Data,1,1);

	if(Data != 0x1f)//Manufacturer_ID
	{
		Spi_InActiveCS(CFG_FLASH_SPI_BUS,CFG_FLASH_SPI_CS);
		return false;
	}
	Spi_TxRx(CFG_FLASH_SPI_BUS,NULL,0,&Data,1,0);
	if(Data != 0x27)//Device_ID1
	{
		Spi_InActiveCS(CFG_FLASH_SPI_BUS,CFG_FLASH_SPI_CS);
		return false;
	}
	Spi_TxRx(CFG_FLASH_SPI_BUS,NULL,0,&Data,1,0);
	if(Data != 0x01)//Device_ID2
	{
		Spi_InActiveCS(CFG_FLASH_SPI_BUS,CFG_FLASH_SPI_CS);
		return false;
	}
	Spi_TxRx(CFG_FLASH_SPI_BUS,NULL,0,&Data,1,0);
	if(Data != 0x00)//Extended_Info
	{
		Spi_InActiveCS(CFG_FLASH_SPI_BUS,CFG_FLASH_SPI_CS);
		return false;
	}

	Spi_InActiveCS(CFG_FLASH_SPI_BUS,CFG_FLASH_SPI_CS);
	
	return true;	//Match SPI Flash ID successful
}

/*--------------------Ӧ�ó�����ýӿں��������ɴ˿�ʼ------------------------*/

//----SPI FLASHģ���æ--------------------------------------------------------
//���ܣ�SPI FLASHģ���æ����ȡAT45_Status������BUSY��־λ�ĵ�ǰֵ
//��������
//���أ�true = FLASH���У�false = FLASHæ
//-----------------------------------------------------------------------------
bool_t at45db321_Check_Busy(void)
{
	u8 Data;
	
	Data = _at45db321_Read_Status();
	
	if( AT45_Status_Reg_Bit_BUSY != (AT45_Status_Reg_Bit_BUSY & Data) )
		return false;
	else
		return true;
}
//----�ȴ�FLASHģ�����--------------------------------------------------------
//���ܣ��ȴ�FLASHģ����У������ڲ��ж��Ƿ�ʱ
//��������
//���أ�true = FLASH���У�false = FLASH��ʱ�쳣
//-----------------------------------------------------------------------------
bool_t at45db321_Wait_Ready(u32 Time_Out)
{
	u8 Data;
	//u32 Time_Out=500000;//��æ��ʱ�ȴ�ʱ��,��һ��SPI��ȡ����Ϊ��λ,��Chip Eraseʱ��Ϊ��׼:500000@1MHz
	
	Spi_ActiveCS(CFG_FLASH_SPI_BUS,CFG_FLASH_SPI_CS);
	
	_at45db321_Command[0] = AT45_Status_Register_Read;	
	Spi_TxRx(CFG_FLASH_SPI_BUS,_at45db321_Command,1,&Data,1,1);
	
	while( AT45_Status_Reg_Bit_BUSY != (AT45_Status_Reg_Bit_BUSY & Data) )
	{
		Time_Out--;
		if(Time_Out==0)
		{
			Spi_InActiveCS(CFG_FLASH_SPI_BUS,CFG_FLASH_SPI_CS);
			return false;
		}
		Spi_TxRx(CFG_FLASH_SPI_BUS,NULL,0,&Data,1,0);
	}
	
	Spi_InActiveCS(CFG_FLASH_SPI_BUS,CFG_FLASH_SPI_CS);
	
	return true;
}

//----SPI FLASHģ�������------------------------------------------------------
//���ܣ���SPI FLASH��Address��ַ����ʼ������data_len���ȵ����ݣ����ݴ洢�׵�ַΪdata
//		����ַ�ۼӵ�FLASH��ĩβ����᷵��FLASH��ͷ��������ȡ��ֱ������data_len������Ϊֹ
//������Address		��Ҫд��ĵ�ַ��ע�⣬�˵�ַΪӦ�ó�����ʹ�õ������ַ����Ҫ�����任��Ϊ�����ַ��page_addr��byte_offset_addr��
//		*data		��ȡ���ݴ洢λ�õ���ʼ��ַ
//		data_len	��ȡ���ݳ���
//���أ�data_len = ʵ�ʶ�ȡ�����ݳ��ȣ�false = ��ȡʧ��
//-----------------------------------------------------------------------------
u32 SPI_FLASH_Read(u32 Address,u8 *data,u32 data_len)
{	
	u32 page_addr,byte_offset_addr;
	
	////�ж��Ƿ���Ҫ��ʼ��SPI���Է�module_init_at45db321δ������	
	if(!flash_spi_Config)
	{
		flash_spi_Config = &pg_spi_Config;
	
		flash_spi_Config->freq=CFG_FLASH_SPI_SPEED;
	
		Spi_Init(CFG_FLASH_SPI_BUS,flash_spi_Config);
	}
	
	page_addr=_at45db321_Page_Caculate(Address);
	byte_offset_addr=_at45db321_Offset_Caculate(Address);
	
	if(false == at45db321_Wait_Ready(500000))		//��æ������ʱ�򷵻�false
		return false;//��ʱ���˳�
		
	_at45db321_Continuous_Array_Read(page_addr,byte_offset_addr,data,data_len);
	
	return data_len;
}

//----SPI FLASHģ��д����------------------------------------------------------
//���ܣ���SPI FLASH��Address��ַ����ʼ��д��data_len���ȵ����ݣ����ݻ�ȡ�׵�ַΪdata
//		��������д�������ġ����ݳ���Ϊ���Pageʱ����ʹFLASH��Ч�ʴﵽ���š�
//		Ӧ�ó����������FLASH�Ƿ���Ҫ��������д��
//		ע�⣺Ӧ�ó���ϣ��д������ݳ���Ϊdata_len���������򲻱�֤ʵ��д��data_len���ȵ����ݣ�
//		ʵ��д������ݳ��Ȼ�ͨ��written_data_len���ظ�Ӧ�ó��򣨸���Page��С�����ģ�
//������Address		��Ҫд��ĵ�ַ��ע�⣬�˵�ַΪӦ�ó�����ʹ�õ������ַ��
//		*data		д������Դ����ʼ��ַ
//		data_len	д�����ݳ���
//���أ�written_data_len = ʵ��д������ݳ��ȣ�false = д��ʧ��
//-----------------------------------------------------------------------------
u32 SPI_FLASH_Write(u32 Address,u8 *data,u32 data_len)
{
	u32 written_data_len=0,Erase_orNot=vs_Erase;
	u32 page_addr,byte_offset_addr;
	
	////�ж��Ƿ���Ҫ��ʼ��SPI���Է�module_init_at45db321δ������	
	if(!flash_spi_Config)
	{
		flash_spi_Config = &pg_spi_Config;
	
		flash_spi_Config->freq=CFG_FLASH_SPI_SPEED;
	
		Spi_Init(CFG_FLASH_SPI_BUS,flash_spi_Config);
	}
	
	//����Page��ַ��������Page�е�ƫ�Ƶ�ַ
	page_addr = _at45db321_Page_Caculate(Address);
	byte_offset_addr = _at45db321_Offset_Caculate(Address);
	
	//ȷ�ϱ��ο���д������ݳ���
	written_data_len = _at45db321_Written_Caculate(byte_offset_addr,data_len);
		
	//1.���ȣ�����Ҫд�������д�봦�ڿ���״̬��Buff��
	if( false == at45db321_Check_Busy() )
	{//��ǰBuff����ִ��д��FLASH�Ĳ�������һ��Buffд������
		if( _at45db321_Ready_Buff==AT45_Buff1 )
			_at45db321_Ready_Buff = AT45_Buff2;
		else
			_at45db321_Ready_Buff = AT45_Buff1;
	}
	_at45db321_Buff_Write(_at45db321_Ready_Buff,byte_offset_addr,data,written_data_len);
		
	if(false == at45db321_Wait_Ready(500000))		//��æ������ʱ�򷵻�false
		return false;//��ʱ���˳�
	
	//2.��Σ���FLASH���ж����Ѿ����ڵ�����,�ж��Ƿ���Ҫ����
	_at45db321_Continuous_Array_Read(page_addr,byte_offset_addr,_at45db321_buff,written_data_len);//����������_at45db321_buff
	if( false == _at45db321_Need_Erase_orNot(data,written_data_len))
	{//��Ҫ����
		Erase_orNot = vs_Erase;
	}
	else
	{//����Ҫ����
		Erase_orNot = no_Erase;
	}
	
	//3.������д�����
	if((( byte_offset_addr != 0 )||( written_data_len != AT45_Page_Size ))&&(Erase_orNot == vs_Erase))
	{//������Page��д�룬����ǰ��Ҫ����
		_at45db321_Page_to_Buff(_at45db321_Ready_Buff,page_addr);
		if(false == at45db321_Wait_Ready(500000))		//��æ������ʱ�򷵻�false
			return false;//��ʱ���˳�
		_at45db321_Buff_Write(_at45db321_Ready_Buff,byte_offset_addr,data,written_data_len);
	}
	_at45db321_Buff_to_Page(_at45db321_Ready_Buff,page_addr,Erase_orNot);
	//�˴�������Ҫ�ȴ���ɡ��ڽ�����һ��FLASH����ʱ���ж�
	
	return written_data_len;
}


//----��ʼ��SPI FLASHģ��------------------------------------------------------
//���ܣ���ʼ��SPI FLASHģ�飬У��оƬID�Ƿ���ȷ
//������ģ���ʼ������û�в���
//���أ�true = �ɹ���ʼ����false = ��ʼ��ʧ��
//-----------------------------------------------------------------------------
ptu32_t module_init_at45db321(void)
{
    u32 i,j,k,result,temp;

	/* setup baud rate */
	flash_spi_Config = &pg_spi_Config;
	
	flash_spi_Config->freq=CFG_FLASH_SPI_SPEED;
	
	Spi_Init(CFG_FLASH_SPI_BUS,flash_spi_Config);
	
	if(false == _at45db321_Check_ID())	//У��оƬID
		return false;
	
/*---------------------test use only----------------------
	for(i=0;i<test_buff_size;i++) Data_write[i]=i&0xff;
	
	SPI_FLASH_Read(0,Data_read,test_buff_size);
	
	j=test_buff_size;
	k=0;
	while(j!=0)
	{
		result = SPI_FLASH_Write(k,Data_write+k,j);
		j = j - result;
		k = k + result;
	}
	
	SPI_FLASH_Read(0,Data_read,test_buff_size);
	
	for(i=0;i<test_buff_size;i++) 
	{
		if(Data_write[i]!=Data_read[i])
		{
			asm("nop;");
		}
	}
---------------------test use only----------------------*/
    return true;
}
