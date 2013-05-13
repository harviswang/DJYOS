//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//��Ȩ���� (C), 2004-2009,   lst.
//����ģ��:flash�ļ�ϵͳ
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����:flash�ļ�ϵͳ��FDR����
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "os_inc.h"

#if ((cfg_djyfs == 1) && (cn_run_mode != cn_mode_tiny) && (cfg_djyfs_flash == 1))

#include "file.h"
#include "flashfile.h"
#include "fdt.h"
#include "ddr.h"

#include <string.h>

//----��ʼ��FDT��--------------------------------------------------------------
//���ܣ���ʼ��FDT���Ѵ洢�黮��Ϊһ����FDT_item������˫����������������FDT
//      ��ĵ�һ��FDT_itemʼ���Ǹ�Ŀ¼�������ֻ�����ļ����ʽ��ʱ����
//����: DBX_flash_tag����������flash�ļ���Ĵ洢ý���ǩ
//���أ�true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t __DFFSD_init_FDT(struct st_DBX_flash_tag *DBX_flash_tag)
{
    uint8_t *FDT_buf;
    struct fdt_info *pl_FDT_item;
    struct flash_chip *chip;
    uint32_t true_block,FDT_block_no;
    uint32_t block_capacity,loop;
    
    chip = DBX_flash_tag->chip;
    FDT_block_no = DBX_flash_tag->DDR_FDSB;
    block_capacity = chip->block_size / 256;
    DBX_flash_tag->FDT_capacity = block_capacity;
    DBX_flash_tag->FDT_free_num= block_capacity - 1;
    FDT_buf = chip->block_buf;

    for(loop=0; loop < block_capacity; loop++)
    {
        pl_FDT_item = (struct fdt_info*)(FDT_buf + loop * 256);
        pl_FDT_item->name_buf_len = cn_flash_file_name_limit +1;
        pl_FDT_item->mattr = 0xff;  //��ʾ����FDT_item
        //���°�����FDT_item���޸ĺʹ�������ʱ�����Ϊ2009-01-04-12-00-00��
        //����djyos�����ա�
        pl_FDT_item->second_create  = 0x00;
        pl_FDT_item->minute_create  = 0x00;
        pl_FDT_item->hour_create    = 0x0c;
        pl_FDT_item->date_create    = 0x04;
        pl_FDT_item->month_create   = 0x01;
        pl_FDT_item->year_low_create= 0xd9;
        pl_FDT_item->year_high_create=0x07;
        pl_FDT_item->second_modify  = 0x00;
        pl_FDT_item->minute_modify  = 0x00;
        pl_FDT_item->hour_modify    = 0x0c;
        pl_FDT_item->date_modify    = 0x04;
        pl_FDT_item->month_modify   = 0x01;
        pl_FDT_item->year_low_modify =0xd9;
        pl_FDT_item->year_high_modify=0x07;
        strcpy(pl_FDT_item->name,"free item");
        __fill_little_32bit(pl_FDT_item->file_size,0,0);
        __fill_little_32bit(pl_FDT_item->file_size,1,0);

        if(loop == 0)   //���Ǹ�Ŀ¼��
        {
            //��Ŀ¼���next��previousָ�����ڰѿ��п鴮������
            //��Ŀ¼���previousָ��ָ��FDTĩ��
            __fill_little_32bit(pl_FDT_item->previous,0,block_capacity-1);
            //��Ŀ¼���nextָ��ָ���һ��������
            __fill_little_32bit(pl_FDT_item->next,0,1);
            //��Ŀ¼���file_sizeָ����;���⣬ǰ4�ֽڼ�¼�ſ���FDT������
            __fill_little_32bit(pl_FDT_item->file_size,0,block_capacity-1);
            //��Ŀ¼���file_sizeָ����;���⣬��4�ֽڼ�¼����FDT������
            __fill_little_32bit(pl_FDT_item->file_size,1,block_capacity);
            //��Ŀ¼��ĸ�Ŀ¼ָ��parent����
            __fill_little_32bit(pl_FDT_item->parent,0,cn_limit_uint32);
            //��Ŀ¼�����Ŀ¼ָ��fstart_dson��ʱ����
            __fill_little_32bit(pl_FDT_item->fstart_dson,0,cn_limit_uint32);
            //��Ŀ¼�Ǹ�ֻ��Ŀ¼
            pl_FDT_item->mattr = cn_FS_ATTR_DIRECTORY + cn_FS_ATTR_READ_ONLY;
            strcpy(pl_FDT_item->name,"root folder");
        }else if(loop == block_capacity-1)  //���е����һ��
        {
            //previousָ��ǰһ��
            __fill_little_32bit(pl_FDT_item->previous,0,block_capacity-2);
            //���һ���nextָ��ָ���Ŀ¼��
            __fill_little_32bit(pl_FDT_item->next,0,0);
            __fill_little_32bit(pl_FDT_item->parent,0,cn_limit_uint32);
            __fill_little_32bit(pl_FDT_item->fstart_dson,0,cn_limit_uint32);
        }else
        {
            //previousָ��ָ��ǰһ��
            __fill_little_32bit(pl_FDT_item->previous,0,loop - 1);
            //nextָ��ָ���һ��
            __fill_little_32bit(pl_FDT_item->next,0,loop + 1);
            __fill_little_32bit(pl_FDT_item->parent,0,cn_limit_uint32);
            __fill_little_32bit(pl_FDT_item->fstart_dson,0,cn_limit_uint32);
        }
    }
    //�ѻ�����д��flash����
    true_block = __DFFSD_write_new_block(DBX_flash_tag,
                                            FDT_block_no,0,chip->block_size);
    if(true_block == cn_limit_uint32)   //���д�����
        return false;
    if(true_block != FDT_block_no)      //��������˿齻��
        __DFFSD_update_MAT_item(DBX_flash_tag,FDT_block_no,true_block);
    return true;
}
//----FDT������1��-------------------------------------------------------------
//���ܣ���FDT��������󣬵��ñ���������1��洢�������������ӵĴ洢������Ϊһ��
//      ��FDT_item�飬Ȼ����˫���������������������ӵ����б���ȥ��
//����: DBX_flash_tag����������flash�ļ���Ĵ洢ý���ǩ
//���أ�true=�ɹ���false=ʧ�ܣ�һ������Ϊû�п��п�
//-----------------------------------------------------------------------------
bool_t __DFFSD_add_FDT_block(struct st_DBX_flash_tag *DBX_flash_tag)
{
    uint8_t *FDT_buf;
    struct fdt_info *pl_FDT_item,FDT_item;
    struct flash_chip *chip;
    uint32_t FDT_item_new_start,free_item,new_block_no,true_block;
    uint32_t block_capacity,last_block_no;
    uint32_t loop;
    
    if(DBX_flash_tag->valid_sum <= 2)
        return false;   //ʣ����ÿ���С�ڵ���2ʱ������һ���FDT���Ǻ��������
    chip = DBX_flash_tag->chip;
    FDT_item_new_start = DBX_flash_tag->FDT_capacity;
    block_capacity = chip->block_size / 256;
    __DFFSD_read_FDT_item(DBX_flash_tag,0,&FDT_item);
    new_block_no = __DFFSD_allocate_block(DBX_flash_tag); //����һ��flash��
    if(new_block_no == cn_limit_uint32)
        return false;
    FDT_buf = chip->block_buf;
    pl_FDT_item = (struct fdt_info*)FDT_buf;
    //�������ĵ�һ��previousָ��ָ���Ŀ¼��
    __fill_little_32bit(pl_FDT_item->previous,0,0);
    //�������ĵ�һ��nextָ��ָ���������ĵڶ���
    __fill_little_32bit(pl_FDT_item->next,0,(FDT_item_new_start + 1));

    pl_FDT_item = (struct fdt_info*)(FDT_buf + (block_capacity-1) * 256);
    //�����������һ���previousָ��ָ�����ڶ��-2����Ϊ��Ŀ�Ŵ�0��ơ�
    __fill_little_32bit(pl_FDT_item->previous,0,
                        (FDT_item_new_start + block_capacity -2));
    if(0 == DBX_flash_tag->FDT_free_num)    //�Ѿ�û�п���FDT_item
    {
        //��Ŀ¼���previousָ��ָ����������ĩ��
        __fill_little_32bit(FDT_item.previous,0,
                            FDT_item_new_start + block_capacity-1);
        //��Ŀ¼���nextָ��ָ��������������
        __fill_little_32bit(FDT_item.next,0,FDT_item_new_start);
        //��Ŀ¼��д��flash��

        //�����������һ���nextָ��ָ���Ŀ¼��
        __fill_little_32bit(pl_FDT_item->next,0,0);
    }else       //��ʣ���������FDT_item
    {
        //����һ������FDT_item����Ŀ��
        free_item = __pick_little_32bit(FDT_item.next,0);
        //��Ŀ¼���nextָ��ָ��������������
        __fill_little_32bit(FDT_item.next,0,FDT_item_new_start);
        //debug FDT_item �������뻻������Ϊ����Ļ�ûд�롣ע�Ⱑ��
        //����һ������FDT���������
        __DFFSD_read_FDT_item(DBX_flash_tag,free_item,&FDT_item);
        //ԭ��һ�����������ǰָ��ָ�������������һ��
        __fill_little_32bit(FDT_item.previous,0,
                            FDT_item_new_start + block_capacity-1);
        //debug 
        //���д�룬ҲҪ��������д��ɣ���ȻӰ��0x40��0x7f��ĵ���
        //ԭ��һ��������д��flash 
        if(! __DFFSD_write_FDT_item(DBX_flash_tag,free_item,&FDT_item))
            return false;
        //�����������һ���nextָ��ָ��ԭ��һ��������
        __fill_little_32bit(pl_FDT_item->next,0,free_item);
    }
    for(loop=1; loop < block_capacity-1; loop++)
    {//������������ĩFDT��Ŀ���������FDT��Ŀ��������
        pl_FDT_item = (struct fdt_info*)(FDT_buf + loop * 256);
        __fill_little_32bit(pl_FDT_item->previous,0,
                            FDT_item_new_start + loop - 1);
        __fill_little_32bit(pl_FDT_item->next,0,
                            FDT_item_new_start + loop + 1);
    }
    true_block = __DFFSD_write_new_block(DBX_flash_tag,
                                            new_block_no,0,chip->block_size);
    if(true_block == cn_limit_uint32)
        return false;  //��ʱ��FDT��Ŀ¼���ָ���Ѿ��ı䲢д��flash�����¿黺��
                       //��ȴû��д��flash�������FDT��free�������ѣ����������
                       //����ʱ�������ʱ���ῴ����Ŀ¼���е�һ��������ĺ����
                       //��MAT��FDT���������
    DBX_flash_tag->FDT_capacity += block_capacity;
    DBX_flash_tag->FDT_free_num += block_capacity;
    //��Ŀ¼���file_sizeָ����;���⣬ǰ4�ֽڼ�¼�ſ���FDT������
    __fill_little_32bit(FDT_item.file_size,0,DBX_flash_tag->FDT_free_num);
    //��Ŀ¼���file_sizeָ����;���⣬��4�ֽڼ�¼����FDT������
    __fill_little_32bit(FDT_item.file_size,1,DBX_flash_tag->FDT_capacity);
    if(! __DFFSD_write_FDT_item(DBX_flash_tag,0,&FDT_item))
     	return false;
    //loop����FDT�׿�Ŀ�ţ����ڶ�λMAT���е�FDT��
    loop = DBX_flash_tag->DDR_FDSB;
    do  //��ѭ�������ҵ�MAT����FDT�������һ��
    {
        last_block_no = loop;
        loop = DBX_flash_tag->DDR_MAT[last_block_no].next;
    }while(loop != last_block_no);
    //����������ʵ��д��Ŀ�ż��뵽MAT���е�FDT�������
    __DFFSD_add_MAT_item(DBX_flash_tag,last_block_no,true_block);
    if(!__DFFSD_write_DDR(DBX_flash_tag))
        return false;
    chip->erase_block(DBX_flash_tag->PCRB_no + DBX_flash_tag->start_block);
    return true;
}
//----��һ��FDT_item��ӵ�����-------------------------------------------------
//����: ��һ���Ѿ���������FDT_item���뵽�����������У���Ϊ��һ����㡣
//����: DBX_flash_tag����������flash�ļ���Ĵ洢ý���ǩ
//      FDT_item_no�����ͷŵ���Ŀ��
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_add_FDT_free_item(struct st_DBX_flash_tag *DBX_flash_tag,
                         uint32_t FDT_item_no)
{
    uint32_t free_first;
    struct fdt_info FDT_item;
    
    __DFFSD_read_FDT_item(DBX_flash_tag,0,&FDT_item);   //������Ŀ¼��
    DBX_flash_tag->FDT_free_num++;
    //FDT���Ŀ¼���file_sizeָ��������;:ǰ4�ֽڼ�¼����FDT������
    __fill_little_32bit(FDT_item.file_size,0,DBX_flash_tag->FDT_free_num);
    free_first = __pick_little_32bit(FDT_item.next,0);
    if(DBX_flash_tag->FDT_free_num == 1)      //ԭ���������ǿյ�
    {
        //��Ŀ¼�����ǰ���ָ���ָ��Ŀ����
        __fill_little_32bit(FDT_item.next,0,FDT_item_no);
        __fill_little_32bit(FDT_item.previous,0,FDT_item_no);
        __DFFSD_write_FDT_item(DBX_flash_tag,0,&FDT_item);  //д���Ŀ¼��
        //�������ͷ���
        __DFFSD_read_FDT_item(DBX_flash_tag,FDT_item_no,&FDT_item);
        //Ŀ������ǰ���ָ���ָ���Ŀ¼��
        __fill_little_32bit(FDT_item.next,0,0);
        __fill_little_32bit(FDT_item.previous,0,0);
        FDT_item.mattr |= cn_FS_ATTR_DELETED;

        __fill_little_32bit(FDT_item.parent,0,cn_limit_uint32);
        __fill_little_32bit(FDT_item.fstart_dson,0,cn_limit_uint32);        
        //���޸���д��flash��
        __DFFSD_write_FDT_item(DBX_flash_tag,FDT_item_no,&FDT_item);
        //Ϊ����޶ȱ������ݻָ���Ϣ�����޸ĳ������������������
    }else               //��������ǿ�
    {
        //��Ŀ¼�����ǰָ���ָ��Ŀ����
        __fill_little_32bit(FDT_item.next,0,FDT_item_no);
        __DFFSD_write_FDT_item(DBX_flash_tag,0,&FDT_item);  //д���Ŀ¼��
        //�������ͷ���
        __DFFSD_read_FDT_item(DBX_flash_tag,FDT_item_no,&FDT_item);

        __fill_little_32bit(FDT_item.parent,0,cn_limit_uint32);
        __fill_little_32bit(FDT_item.fstart_dson,0,cn_limit_uint32);
        //���ͷ���ǰָ��ָ��ԭ���һ��������
        __fill_little_32bit(FDT_item.next,0,free_first);

        __fill_little_32bit(FDT_item.previous,0,0);

    	FDT_item.mattr |= cn_FS_ATTR_DELETED;

    	strcpy(FDT_item.name,"free item");
        //���ͷ���д��flash��
        __DFFSD_write_FDT_item(DBX_flash_tag,FDT_item_no,&FDT_item);
        //����ԭ��һ��������
        __DFFSD_read_FDT_item(DBX_flash_tag,free_first,&FDT_item);
        //ԭ��һ��������ĺ�ָ��ָ���ͷ���
        __fill_little_32bit(FDT_item.previous,0,FDT_item_no);
        //ԭ���һ��������д��flash
        __DFFSD_write_FDT_item(DBX_flash_tag,free_first,&FDT_item);
    }
}

//----����һ��æFDT_item-------------------------------------------------------
//����: ��һ��FDT_item����ԭ����FDT���й�������������ɾ���ļ�/Ŀ¼�ĵ�һ����
//����: DBX_flash_tag����������flash�ļ���Ĵ洢ý���ǩ
//      FDT_item_no������������Ŀ��
//����: true = �ɹ���false = ʧ�ܣ�һ����ӦΪFDT_item_no����Ŀ¼/�ļ�
//-----------------------------------------------------------------------------
bool_t __DFFSD_acnode_FDT_item(struct st_DBX_flash_tag *DBX_flash_tag,
                         uint32_t FDT_item_no)
{
    uint32_t parent,elder,less,eldest;     //���ͷ���ĸ����֡��ܡ�������Ŀ��
    struct fdt_info FDT_item;
    __DFFSD_read_FDT_item(DBX_flash_tag,FDT_item_no,&FDT_item); //����Ŀ����
    if((FDT_item.mattr & cn_FS_ATTR_DELETED) != 0)
        return false;   //Ŀ��FDT_item��һ��������/��ɾ����
    if((((FDT_item.mattr & cn_FS_ATTR_DIRECTORY) == 1) &&
          (__pick_little_32bit(FDT_item.fstart_dson,0)) != cn_limit_uint32))
        return false;   //Ŀ��FDT_item�����ļ�/Ŀ¼��Ŀ¼
    parent = __pick_little_32bit(FDT_item.parent,0);    //ȡ��FDT��Ŀ��
    elder = __pick_little_32bit(FDT_item.previous,0);   //ȡ��FDT��Ŀ��
    less = __pick_little_32bit(FDT_item.next,0);        //ȡ��FDT��Ŀ��
    if(elder == less)       //�֡���FDT��Ŀ�����
    {
        if(elder == FDT_item_no)    //FDT_item_no�Ǹ�Ŀ¼��Ψһ���ļ�/Ŀ¼
        {
            __DFFSD_read_FDT_item(DBX_flash_tag,parent,&FDT_item);//������
            //�����������Ϊû������
            __fill_little_32bit(FDT_item.fstart_dson,0,cn_limit_uint32);
            __DFFSD_write_FDT_item(DBX_flash_tag,parent,&FDT_item); //д����
        }else                   //��Ŀ¼���������ļ�/Ŀ¼
        {
            //������(��)FDT��
            __DFFSD_read_FDT_item(DBX_flash_tag,elder,&FDT_item);
            __fill_little_32bit(FDT_item.previous,0,elder); //ǰָ��ָ���Լ�
            __fill_little_32bit(FDT_item.next,0,elder);     //��ָ��ָ���Լ�
            __DFFSD_write_FDT_item(DBX_flash_tag,elder,&FDT_item);  //д��flash
            __DFFSD_read_FDT_item(DBX_flash_tag,parent,&FDT_item);  //������
            eldest = __pick_little_32bit(FDT_item.fstart_dson,0); //ȡ������Ŀ��
            if(eldest != elder)     //��������ǡ�þ��ǳ���
            {
                //���ø���������
                __fill_little_32bit(FDT_item.fstart_dson,0,elder);
                __DFFSD_write_FDT_item(DBX_flash_tag,parent,&FDT_item);//д����
            }
        }
    }else       //�֡�����Ŀ�Ų���ȣ�˵�����������ͬ��Ŀ¼��������3��
    {
        __DFFSD_read_FDT_item(DBX_flash_tag,parent,&FDT_item);  //������
        eldest = __pick_little_32bit(FDT_item.fstart_dson,0);   //ȡ�������
        if(eldest == FDT_item_no)   //��������ǡ�þ��ǳ���
        {
            //���ø���������
            __fill_little_32bit(FDT_item.fstart_dson,0,less);
            __DFFSD_write_FDT_item(DBX_flash_tag,parent,&FDT_item); //д����
        }                   
        __DFFSD_read_FDT_item(DBX_flash_tag,elder,&FDT_item);   //���ֽ��
        __fill_little_32bit(FDT_item.next,0,less); //�ֽ������ָ��ָ��ܽ��
        __DFFSD_write_FDT_item(DBX_flash_tag,elder,&FDT_item);  //д�ֽ��
        __DFFSD_read_FDT_item(DBX_flash_tag,less,&FDT_item);    //���ܽ��
        //�ܽ�����ǰָ��ָ��ܽ��
        __fill_little_32bit(FDT_item.previous,0,elder);
        __DFFSD_write_FDT_item(DBX_flash_tag,less,&FDT_item);   //д�ܽ��
    }
    //ע�⣬Ϊ����޶ȱ������ݻָ���Ϣ�����Ա����������κ��޸�
    return true;
}

//----�ͷ�FDT_item-------------------------------------------------------------
//����: �ͷ�һ��FDT_item��FDT���У������������������
//����: DBX_flash_tag����������flash�ļ���Ĵ洢ý���ǩ
//      FDT_item_no�����ͷŵ���Ŀ��
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_free_FDT_item(struct st_DBX_flash_tag *DBX_flash_tag,
                         uint32_t FDT_item_no)
{
    struct flash_chip *chip = DBX_flash_tag->chip;
    //��Ŀ¼������ȡ����������FDT��
    if(__DFFSD_acnode_FDT_item(DBX_flash_tag,FDT_item_no))
    {
        //�Ѹղ�ȡ����FDT����뵽����FDT������ȥ
        __DFFSD_add_FDT_free_item(DBX_flash_tag,FDT_item_no);
    }
    chip->erase_block(DBX_flash_tag->PCRB_no + DBX_flash_tag->start_block);
}


//----����FDT_item-------------------------------------------------------------
//���ܣ���FDT���з���һ��FDT_item�������ӿ���������ȡ�������û�п���FDT_item��
//      �����DFFSD_add_FDT_block��������һ�顣
//����: DBX_flash_tag����������flash�ļ���Ĵ洢ý���ǩ
//���أ��·����FDT_item��ţ������򷵻�cn_limit_uint32
//-----------------------------------------------------------------------------
uint32_t __DFFSD_allocate_FDT_item(struct st_DBX_flash_tag *DBX_flash_tag)
{
    uint32_t next_free,next_next_free;
    struct fdt_info FDT_root,FDT_free_item;
    struct flash_chip *chip = DBX_flash_tag->chip;
    
    __DFFSD_read_FDT_item(DBX_flash_tag,0,&FDT_root);
    //���п���FDT_item���������Ը�Ŀ¼Ϊ���ĵ�˫��������
    next_free = __pick_little_32bit(FDT_root.next,0);

    if(next_free >= DBX_flash_tag->FDT_capacity)
        return cn_limit_uint32;   //��Ŀ�Ŵ���FDT�������϶�����
    if(DBX_flash_tag->FDT_free_num == 0)
    {
        if(!__DFFSD_add_FDT_block(DBX_flash_tag))     //�ӳ�FDT��ʧ��
            return cn_limit_uint32;
        __DFFSD_read_FDT_item(DBX_flash_tag,0,&FDT_root);   //���¶���Ŀ¼��
        next_free = __pick_little_32bit(FDT_root.next,0); //�ض���Ŀ¼��nextָ��
    }
    //��ȡ��һ�������Ҳ���Ǽ������ظ������ߵ���
    __DFFSD_read_FDT_item(DBX_flash_tag,next_free,&FDT_free_item);
    //��ȡ����һ��������ı��
    next_next_free = __pick_little_32bit(FDT_free_item.next,0);
    DBX_flash_tag->FDT_free_num--;
    //FDT���Ŀ¼���file_sizeָ��������;:ǰ4�ֽڼ�¼����FDT������
    __fill_little_32bit(FDT_root.file_size,0,DBX_flash_tag->FDT_free_num);
    if(next_next_free == 0) //next_next_freeָ��ָ���0��������һ��������
    {
        __fill_little_32bit(FDT_root.next,0,0);
        __fill_little_32bit(FDT_root.previous,0,0);
        if(! __DFFSD_write_FDT_item(DBX_flash_tag,0,&FDT_root))
            return cn_limit_uint32;
    }else
    {
        //��ȡ����һ�������������
        __DFFSD_read_FDT_item(DBX_flash_tag,next_next_free,&FDT_free_item);
        //��Ŀ¼���nextָ��ָ������һ������������
        __fill_little_32bit(FDT_root.next,0,next_next_free);
        //����һ���������previousָ��ָ���Ŀ¼��
        __fill_little_32bit(FDT_free_item.previous,0,0);
        //��Ŀ¼��д��flash
        if(! __DFFSD_write_FDT_item(DBX_flash_tag,0,&FDT_root))
            return cn_limit_uint32;
        //����һ��������д��flash
        if(!__DFFSD_write_FDT_item(DBX_flash_tag,next_next_free,&FDT_free_item))
            return cn_limit_uint32;
    }
    //���д��FDT��ʱ���������滻������Ҫˢ��DDR��
    if(!__DFFSD_write_DDR(DBX_flash_tag))
        return false;
    chip->erase_block(DBX_flash_tag->PCRB_no + DBX_flash_tag->start_block);
    return next_free;
}
//----ѹ��FDT��----------------------------------------------------------------
//���ܣ���FDT���п��б�����ռ�Ĵ洢����������1��blockʱ�����Ե��ñ�����ѹ��֮��
//����: DBX_flash_tag����������flash�ļ���Ĵ洢ý���ǩ
//���أ���
// db ������
//-----------------------------------------------------------------------------
void __DFFSD_compress_FDT(struct st_DBX_flash_tag *DBX_flash_tag)
{
}

struct FDT_test
{
    u8 res[16];
    u32 free,sum;
    u32 parent,son,pre,next;
    u8 nane[216];
};

//----дһ��FDT����------------------------------------------------------------
//����: ��һ��FDT����д��flash��FDT����
//����: DBX_flash_tag����������flash�ļ���Ĵ洢ý���ǩ
//      FDT_item_no����Ŀ��
//      FDT_item��FDT����ָ��
//����: true=�ɹ�д�룬false=ʧ�ܣ�һ��������������û�п��п���滻��
//-----------------------------------------------------------------------------
bool_t __DFFSD_write_FDT_item(struct st_DBX_flash_tag *DBX_flash_tag,
                           uint32_t FDT_item_no,struct fdt_info *FDT_item)
{
    uint32_t true_block;
    struct flash_chip *chip;
    uint32_t block_offset,FDT_block_no,loop,FDT_block_serial;
    
    chip = DBX_flash_tag->chip;
    //����FDT_item_no��fdt���еĿ����
    FDT_block_serial = FDT_item_no * sizeof(struct fdt_info) / chip->block_size;
    block_offset = FDT_item_no * sizeof(struct fdt_info) % chip->block_size;
    //��ȡFDT�׿�Ŀ��
    FDT_block_no = DBX_flash_tag->DDR_FDSB;
    //���׿鿪ʼ����MAT���������ҵ���FDT_block_serial��FDT��Ŀ��
    for(loop = 0; loop < FDT_block_serial; loop++)
        FDT_block_no = DBX_flash_tag->DDR_MAT[FDT_block_no].next;
    memcpy(chip->block_buf+block_offset,FDT_item,256);
    //д��Ŀ���
    true_block = __DFFSD_write_update_block(DBX_flash_tag,FDT_block_no,
                                    block_offset,256);
    if(true_block != cn_limit_uint32)
    {//��ȷд��
        if(true_block != FDT_block_no)   //д��ʱ�����˿��滻
        {
        	__DFFSD_update_MAT_item(DBX_flash_tag,FDT_block_no,true_block);
            if(! __DFFSD_write_DDR(DBX_flash_tag))
                return false;
        }
    }else       //д�����
        return false;
    chip->erase_block(DBX_flash_tag->PCRB_no + DBX_flash_tag->start_block);
    return true;
}

//----��һ��FDT����------------------------------------------------------------
//����: ��FDT���ж�ȡһ������
//����: DBX_flash_tag����������flash�ļ���Ĵ洢ý���ǩ
//      FDT_item_no����Ŀ��
//      FDT_item���������ݵ�ָ��
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_read_FDT_item(struct st_DBX_flash_tag *DBX_flash_tag,
                           uint32_t FDT_item_no,struct fdt_info *FDT_item)
{
    struct flash_chip *chip;
    uint32_t block_offset,FDT_block_abs,FDT_block_no,loop,FDT_block_serial;
    chip = DBX_flash_tag->chip;
    //����FDT_item_no��fdt���еĿ����
    FDT_block_serial = FDT_item_no * sizeof(struct fdt_info) / chip->block_size;
    block_offset = FDT_item_no * sizeof(struct fdt_info) % chip->block_size;
    //��ȡFDT�׿�Ŀ��
    FDT_block_no = DBX_flash_tag->DDR_FDSB;
    //���׿鿪ʼ����MAT���������ҵ���FDT_block_serial��FDT��Ŀ��
    for(loop = 0; loop < FDT_block_serial; loop++)
        FDT_block_no = DBX_flash_tag->DDR_MAT[FDT_block_no].next;
    //����FDT_block��оƬ�еľ��Կ��
    FDT_block_abs = FDT_block_no + DBX_flash_tag->start_block;
    chip->read_data_with_ecc(FDT_block_abs,block_offset,
                            (uint8_t*)FDT_item,sizeof(struct fdt_info)); 
}

//----����һ��FDT_item---------------------------------------------------------
//���ܣ�����һ��FDT_item���ڴ�Ӱ�񣬵���û�д����������4��ָ�롣
//����: FDT_item���������FDT��Ŀָ��
//      attr����Ŀ����
//      name����Ŀ����
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_asm_FDT_item(struct fdt_info *FDT_item,union file_attrs attr,
                          char *name)
{
    uint8_t attr_mirror;
    struct tm file_time;
    ufast_t i;
    //����ڿ��ܵ���ֲ���⣬����ֱ��ʹ��λ���Ͻṹ��attr����
    attr_mirror = 0;
    if(attr.bits.read_only)
        attr_mirror +=cn_FS_ATTR_READ_ONLY;
    if(attr.bits.hidden)
        attr_mirror +=cn_FS_ATTR_HIDDEN;
    if(attr.bits.folder)
        attr_mirror +=cn_FS_ATTR_DIRECTORY;
    if(attr.bits.archive)
        attr_mirror +=cn_FS_ATTR_ARCHIVE;   //����ֻ��ʼ��4�����ԣ��������ݲ���
    FDT_item->mattr = attr_mirror;
    FDT_item->name_buf_len = cn_flash_file_name_limit +1;
    for(i = 0; i < 8; i++)
        FDT_item->file_size[i] = 0;    //�ļ��ߴ�=0
    //name��ģ���ڲ��ṩ���ַ���ָ�룬�Ѿ������ַ������ȺϷ��Լ��
    if(strlen(name) <= cn_flash_file_name_limit)//��ȡ�ļ�������
        strcpy(FDT_item->name,name);
    else
    {
        memcpy(FDT_item->name,name,cn_flash_file_name_limit);
        FDT_item->name[cn_flash_file_name_limit] = '\0';
    }
    file_time = localtime(djy_time(NULL));
    FDT_item->second_create = (u8)file_time.tm_sec;
    FDT_item->minute_create = (u8)file_time.tm_min;
    FDT_item->hour_create = (u8)file_time.tm_hour;
    FDT_item->date_create = (u8)file_time.tm_mday;
    FDT_item->month_create = (u8)file_time.tm_mon;
    FDT_item->year_low_create = (u8)file_time.tm_year;
    FDT_item->year_high_create = (u8)(file_time.tm_year >> 8);
    FDT_item->second_modify = (u8)file_time.tm_sec;
    FDT_item->minute_modify = (u8)file_time.tm_min;
    FDT_item->hour_modify = (u8)file_time.tm_hour;
    FDT_item->date_modify = (u8)file_time.tm_mday;
    FDT_item->month_modify = (u8)file_time.tm_mon;
    FDT_item->year_low_modify = (u8)file_time.tm_year;
    FDT_item->year_high_modify = (u8)(file_time.tm_year >> 8);
    return;
}

//----��ȡ�ļ�(Ŀ¼)����-------------------------------------------------------
//���ܣ���FDT������ڴ澵���е��ж�ȡ�ļ�(Ŀ¼)��Ϣ������ʼ���ļ��ṹָ��fp��
//������DBX_flash_tag�����������ļ���ָ�룬ͨ�ýṹ����flashר�á�
//      fp�������ļ�(Ŀ¼)���Ե��ļ�ָ��
//      FDT_item����flash�ж�����FDT����
//���أ���
//-----------------------------------------------------------------------------
void __DFFSD_uasm_FDT_item(struct st_DBX_flash_tag *DBX_flash_tag,
                              struct file_rsc *fp,
                              struct fdt_info *FDT_item)
{
    uint8_t attr_mirror;
    uint32_t name_len;

    //�Խڵ���г�ʼ��������rsc_search_son�������ļ�ʱ�򣬽ڵ�û�г�ʼ����
    //�޷������жϵ����ǲ���NULL��
    //���__djyfs_DBX_lookfor_item�����е���rsc_search_son��
	fp->file_node.child = NULL;
	fp->file_node.next = NULL;
	fp->file_node.previous = NULL;
	fp->file_node.parent = NULL;

    fp->attr.all = 0;
    attr_mirror = FDT_item->mattr;
    if(attr_mirror & cn_FS_ATTR_READ_ONLY)
        fp->attr.bits.read_only =1;
    if(attr_mirror & cn_FS_ATTR_DIRECTORY)
        fp->attr.bits.folder =1;
    if(attr_mirror & cn_FS_ATTR_HIDDEN)
        fp->attr.bits.hidden =1;
    if(attr_mirror & cn_FS_ATTR_ARCHIVE)
        fp->attr.bits.archive =1;   //����ֻ��ʼ��3�����ԣ��������ݲ���
    fp->second_create = FDT_item->second_create;
    fp->minute_create = FDT_item->minute_create;
    fp->hour_create = FDT_item->hour_create;
    fp->date_create = FDT_item->date_create;
    fp->month_create = FDT_item->month_create ;
    fp->year_low_create = FDT_item->year_low_create;
    fp->year_high_create = FDT_item->year_high_create;
    fp->second_modify = FDT_item->second_modify;
    fp->minute_modify = FDT_item->minute_modify;
    fp->hour_modify = FDT_item->hour_modify;
    fp->date_modify = FDT_item->date_modify;
    fp->month_modify = FDT_item->month_modify ;
    fp->year_low_modify = FDT_item->year_low_modify;
    fp->year_high_modify = FDT_item->year_high_modify;
    //���ļ��ߴ磬Ϊ��ֹ����Ϊ��չ���²���Ҫ���鷳�����޷�������λ��
    fp->file_size = __pick_little_32bit(FDT_item->file_size,0)
            +((uint64_t)__pick_little_32bit(FDT_item->file_size,1)<<32);
    name_len = strnlen(FDT_item->name,cn_flash_file_name_limit);
    if(name_len != cn_limit_uint32)  //�ļ������ֲ�����cn_flash_file_name_limit
        strcpy(fp->name, FDT_item->name);
    else
    {
        memcpy(fp->name, FDT_item->name,cn_flash_file_name_limit);
        fp->name[cn_flash_file_name_limit] = '\0';
    }
    //fp->open_counter = 0;
    return ;
}

//----������Ŀ¼��Ŀ-------------------------------------------------------
//���ܣ��Ӹ�Ŀ¼��FDT������ڴ澵���ж�ȡ��Ŀ¼��Ϣ��д��DBX_flash_tag��
//������DBX_flash_tag�����������ļ���ָ�룬ͨ�ýṹ����flashר�á�
//      FDT_item����flash�ж�����FDT����
//���أ���
//-----------------------------------------------------------------------------
void __DFFSD_uasm_root_folder(struct st_DBX_flash_tag *DBX_flash_tag,
                                struct fdt_info *FDT_item)
{
    //��Ŀ¼��FDT��Ŀ��file_size��������;��ǰ4�ֽ�ΪFDT���п���FDT��Ŀ������
    //4���ֽ���FDT������FDT��Ŀ��
    DBX_flash_tag->FDT_free_num = __pick_little_32bit(FDT_item->file_size,0);
    DBX_flash_tag->FDT_capacity = __pick_little_32bit(FDT_item->file_size,1);
    return ;
}

//----�޸�fdt��Ŀ�е���ʼ���--------------------------------------------------
//����: ��һ���ļ����׿鱻�޸�ʱ�����ļ���fdt��Ŀ�б������ʼ���Ҳ�����޸ġ�ֻ
//      ���ڷ���ĥ��ƽ��齻��ʱ�����п��ܵ��ñ����������ڵ��ñ�����ʱ������д
//      ���������޸�ĥ���������ʹ�������齻����Ҳ�����ñ�׼�Ŀ���亯��
//      __allocate_block������п飬����ֱ�Ӵ�free����ȡһ�飬���޸�ĥ�������
//      �������Ա��ⷢ�������ס�
//����: DBX_flash_tag,��������flash�ļ���Ĵ洢ý���ǩ
//      FDT_item_no,fdt��Ŀ��š�
//      new_block_no���滻���
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_update_FDT_fstart_dson(struct st_DBX_flash_tag *DBX_flash_tag,
                       uint32_t FDT_item_no,uint32_t new_block_no)
{
    struct fdt_info FDT_item;
    __DFFSD_read_FDT_item(DBX_flash_tag,FDT_item_no,&FDT_item);
    __fill_little_32bit(FDT_item.fstart_dson,0,new_block_no);
    __DFFSD_write_FDT_item(DBX_flash_tag,FDT_item_no,&FDT_item);
    return ;
}

//----��FDT���޸��ļ��ߴ�------------------------------------------------------
//����: �޸�FDT�����е��ļ��ߴ���
//����: DBX_flash_tag����������flash�ļ���Ĵ洢ý���ǩ
//      fp�����޸ĵ��ļ�ָ�룬�ɵ��÷���֤����Ŀ¼�����ļ���
//      new_size���µ��ļ��ߴ�
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_update_FDT_file_size(struct st_DBX_flash_tag *DBX_flash_tag,
                                  struct file_rsc *fp,sint64_t new_size)
{
    uint32_t FDT_item_no;
    struct fdt_info FDT_item;
    FDT_item_no = (uint32_t)fp->file_medium_tag;
    __DFFSD_read_FDT_item(DBX_flash_tag,FDT_item_no,&FDT_item);
    __fill_little_32bit(FDT_item.file_size,0,new_size);
    //Ϊ��ֹ����Ϊ��չ���²���Ҫ���鷳�����޷�������λ��
    __fill_little_32bit(FDT_item.file_size,1,(uint64_t)new_size>>32);
    __DFFSD_write_FDT_item(DBX_flash_tag,FDT_item_no,&FDT_item);
    return ;
}

//----��FDT���޸��ļ���------------------------------------------------------
//����: �޸�FDT�����е��ļ�(Ŀ¼)����
//����: DBX_flash_tag����������flash�ļ���Ĵ洢ý���ǩ
//      fp�����޸ĵ��ļ�ָ��
//      name���µ��ļ�(Ŀ¼)��
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_update_FDT_item_name(struct st_DBX_flash_tag *DBX_flash_tag,
                                  struct file_rsc *fp,char *name)
{
    uint32_t FDT_item_no;
    struct fdt_info FDT_item;
    FDT_item_no = (uint32_t)fp->file_medium_tag;
    __DFFSD_read_FDT_item(DBX_flash_tag,FDT_item_no,&FDT_item);
    //name��ģ���ڲ��ṩ���ַ���ָ�룬�Ѿ������ַ������ȺϷ��Լ��
    if(strlen(name) <= cn_flash_file_name_limit)//��ȡ�ļ�������
        strcpy(FDT_item.name,name);
    else
    {
        memcpy(FDT_item.name,name,cn_flash_file_name_limit);
        FDT_item.name[cn_flash_file_name_limit] = '\0';
    }
    __DFFSD_write_FDT_item(DBX_flash_tag,FDT_item_no,&FDT_item);
    return ;
}

//----��FDT���޸��ļ�����------------------------------------------------------
//����: �޸�FDT�����е��ļ�(Ŀ¼)������
//����: DBX_flash_tag����������flash�ļ���Ĵ洢ý���ǩ
//      fp�����޸ĵ��ļ�ָ��
//      attr���µ��ļ�(Ŀ¼)����
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_update_FDT_item_attr(struct st_DBX_flash_tag *DBX_flash_tag,
                                  struct file_rsc *fp,union file_attrs attr)
{
    uint32_t FDT_item_no;
    struct fdt_info FDT_item;
    uint8_t attr_mirror;
    FDT_item_no = (uint32_t)fp->file_medium_tag;
    __DFFSD_read_FDT_item(DBX_flash_tag,FDT_item_no,&FDT_item);
    //����ڿ��ܵ���ֲ���⣬����ֱ��ʹ��λ���Ͻṹ��attr����
    attr_mirror = FDT_item.mattr;
    if(attr.bits.read_only)
        attr_mirror +=cn_FS_ATTR_READ_ONLY;
    if(attr.bits.hidden)
        attr_mirror +=cn_FS_ATTR_HIDDEN;
    if(attr.bits.archive)
        attr_mirror +=cn_FS_ATTR_ARCHIVE;   //����ֻ��ʼ��3�����ԣ��������ݲ���
                                            //�����޸�Ŀ¼/�ļ�����
    FDT_item.mattr = attr_mirror;
    __DFFSD_write_FDT_item(DBX_flash_tag,FDT_item_no,&FDT_item);
    return ;
}
//----��FDT���޸Ĵ���ʱ��------------------------------------------------------
//����: �޸�FDT�����еĴ���ʱ����
//����: DBX_flash_tag����������flash�ļ���Ĵ洢ý���ǩ
//      fp�����޸ĵ��ļ�ָ��
//      new_time���µ�ʱ��
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_update_FDT_item_ctime(struct st_DBX_flash_tag *DBX_flash_tag,
                                  struct file_rsc *fp,struct tm new_time)
{
    uint32_t FDT_item_no;
    struct fdt_info FDT_item;
    FDT_item_no = (uint32_t)fp->file_medium_tag;
    __DFFSD_read_FDT_item(DBX_flash_tag,FDT_item_no,&FDT_item);

    FDT_item.second_create = (u8)new_time.tm_sec;
    FDT_item.minute_create = (u8)new_time.tm_min;
    FDT_item.hour_create = (u8)new_time.tm_hour;
    FDT_item.date_create = (u8)new_time.tm_mday;
    FDT_item.month_create = (u8)new_time.tm_mon;
    FDT_item.year_low_create = (u8)new_time.tm_year;
    FDT_item.year_high_create = (u8)(new_time.tm_year >> 8);
    
    __DFFSD_write_FDT_item(DBX_flash_tag,FDT_item_no,&FDT_item);
    return ;
}

//----��FDT���޸��޸�ʱ��------------------------------------------------------
//����: �޸�FDT�����е��޸�ʱ����
//����: DBX_flash_tag����������flash�ļ���Ĵ洢ý���ǩ
//      fp�����޸ĵ��ļ�ָ��
//      new_time���µ�ʱ��
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_update_FDT_item_utime(struct st_DBX_flash_tag *DBX_flash_tag,
                                  struct file_rsc *fp,struct tm new_time)
{
    uint32_t FDT_item_no;
    struct fdt_info FDT_item;
    FDT_item_no = (uint32_t)fp->file_medium_tag;
    __DFFSD_read_FDT_item(DBX_flash_tag,FDT_item_no,&FDT_item);

    FDT_item.second_modify = (u8)new_time.tm_sec;
    FDT_item.minute_modify = (u8)new_time.tm_min;
    FDT_item.hour_modify = (u8)new_time.tm_hour;
    FDT_item.date_modify = (u8)new_time.tm_mday;
    FDT_item.month_modify = (u8)new_time.tm_mon;
    FDT_item.year_low_modify = (u8)new_time.tm_year;
    FDT_item.year_high_modify = (u8)(new_time.tm_year >> 8);
    
    __DFFSD_write_FDT_item(DBX_flash_tag,FDT_item_no,&FDT_item);
    return ;
}

#endif //#if ((cfg_djyfs == 1) && (cn_run_mode != cn_mode_tiny) && (cfg_djyfs_flash == 1))
