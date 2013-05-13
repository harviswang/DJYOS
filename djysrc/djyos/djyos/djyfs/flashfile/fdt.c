//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//版权所有 (C), 2004-2009,   lst.
//所属模块:flash文件系统
//作者：lst
//版本：V1.0.0
//文件描述:flash文件系统中FDR表部分
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-01-04
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "os_inc.h"

#if ((cfg_djyfs == 1) && (cn_run_mode != cn_mode_tiny) && (cfg_djyfs_flash == 1))

#include "file.h"
#include "flashfile.h"
#include "fdt.h"
#include "ddr.h"

#include <string.h>

//----初始化FDT表--------------------------------------------------------------
//功能：初始化FDT表，把存储块划分为一个个FDT_item，并用双向链表连接起来。FDT
//      表的第一个FDT_item始终是根目录项。本函数只会在文件柜格式化时调用
//参数: DBX_flash_tag，被操作的flash文件柜的存储媒体标签
//返回：true=成功，false=失败
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
        pl_FDT_item->mattr = 0xff;  //表示空闲FDT_item
        //以下把所有FDT_item的修改和创建日期时间均改为2009-01-04-12-00-00，
        //那是djyos的生日。
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

        if(loop == 0)   //这是根目录项
        {
            //根目录项的next和previous指针用于把空闲块串起来。
            //根目录项的previous指针指向FDT末项
            __fill_little_32bit(pl_FDT_item->previous,0,block_capacity-1);
            //根目录项的next指针指向第一个空闲项
            __fill_little_32bit(pl_FDT_item->next,0,1);
            //根目录项的file_size指针用途特殊，前4字节记录着空闲FDT表项数
            __fill_little_32bit(pl_FDT_item->file_size,0,block_capacity-1);
            //根目录项的file_size指针用途特殊，后4字节记录着总FDT表项数
            __fill_little_32bit(pl_FDT_item->file_size,1,block_capacity);
            //根目录项的父目录指针parent空着
            __fill_little_32bit(pl_FDT_item->parent,0,cn_limit_uint32);
            //根目录项的子目录指针fstart_dson暂时空着
            __fill_little_32bit(pl_FDT_item->fstart_dson,0,cn_limit_uint32);
            //根目录是个只读目录
            pl_FDT_item->mattr = cn_FS_ATTR_DIRECTORY + cn_FS_ATTR_READ_ONLY;
            strcpy(pl_FDT_item->name,"root folder");
        }else if(loop == block_capacity-1)  //空闲的最后一项
        {
            //previous指针前一项
            __fill_little_32bit(pl_FDT_item->previous,0,block_capacity-2);
            //最后一项的next指针指向根目录项
            __fill_little_32bit(pl_FDT_item->next,0,0);
            __fill_little_32bit(pl_FDT_item->parent,0,cn_limit_uint32);
            __fill_little_32bit(pl_FDT_item->fstart_dson,0,cn_limit_uint32);
        }else
        {
            //previous指针指向前一项
            __fill_little_32bit(pl_FDT_item->previous,0,loop - 1);
            //next指针指向后一项
            __fill_little_32bit(pl_FDT_item->next,0,loop + 1);
            __fill_little_32bit(pl_FDT_item->parent,0,cn_limit_uint32);
            __fill_little_32bit(pl_FDT_item->fstart_dson,0,cn_limit_uint32);
        }
    }
    //把缓冲区写入flash块内
    true_block = __DFFSD_write_new_block(DBX_flash_tag,
                                            FDT_block_no,0,chip->block_size);
    if(true_block == cn_limit_uint32)   //如果写入错误
        return false;
    if(true_block != FDT_block_no)      //如果发生了块交换
        __DFFSD_update_MAT_item(DBX_flash_tag,FDT_block_no,true_block);
    return true;
}
//----FDT表增加1块-------------------------------------------------------------
//功能：当FDT表项用完后，调用本函数增加1块存储器，并把新增加的存储器划分为一个
//      个FDT_item块，然后用双向链表连接起来，并连接到空闲表中去。
//参数: DBX_flash_tag，被操作的flash文件柜的存储媒体标签
//返回：true=成功，false=失败，一般是因为没有空闲块
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
        return false;   //剩余可用块数小于等于2时，分配一块给FDT表是毫无意义的
    chip = DBX_flash_tag->chip;
    FDT_item_new_start = DBX_flash_tag->FDT_capacity;
    block_capacity = chip->block_size / 256;
    __DFFSD_read_FDT_item(DBX_flash_tag,0,&FDT_item);
    new_block_no = __DFFSD_allocate_block(DBX_flash_tag); //申请一个flash块
    if(new_block_no == cn_limit_uint32)
        return false;
    FDT_buf = chip->block_buf;
    pl_FDT_item = (struct fdt_info*)FDT_buf;
    //新申请块的第一项previous指针指向根目录项
    __fill_little_32bit(pl_FDT_item->previous,0,0);
    //新申请块的第一项next指针指向新申请块的第二项
    __fill_little_32bit(pl_FDT_item->next,0,(FDT_item_new_start + 1));

    pl_FDT_item = (struct fdt_info*)(FDT_buf + (block_capacity-1) * 256);
    //新申请块的最后一项的previous指针指向倒数第二项，-2是因为条目号从0起计。
    __fill_little_32bit(pl_FDT_item->previous,0,
                        (FDT_item_new_start + block_capacity -2));
    if(0 == DBX_flash_tag->FDT_free_num)    //已经没有空闲FDT_item
    {
        //根目录项的previous指针指向新申请块的末项
        __fill_little_32bit(FDT_item.previous,0,
                            FDT_item_new_start + block_capacity-1);
        //根目录项的next指针指向新申请块的首项
        __fill_little_32bit(FDT_item.next,0,FDT_item_new_start);
        //根目录项写入flash中

        //新申请块的最后一项的next指针指向根目录项
        __fill_little_32bit(pl_FDT_item->next,0,0);
    }else       //还剩余少许空闲FDT_item
    {
        //读第一个空闲FDT_item的条目号
        free_item = __pick_little_32bit(FDT_item.next,0);
        //根目录项的next指针指向新申请块的首项
        __fill_little_32bit(FDT_item.next,0,FDT_item_new_start);
        //debug FDT_item 参数必须换掉，因为上面的还没写入。注意啊！
        //读第一个空闲FDT项到缓冲区中
        __DFFSD_read_FDT_item(DBX_flash_tag,free_item,&FDT_item);
        //原第一个空闲项的向前指针指向新申请块的最后一项
        __fill_little_32bit(FDT_item.previous,0,
                            FDT_item_new_start + block_capacity-1);
        //debug 
        //这个写入，也要放在下面写入吧！不然影响0x40、0x7f项的调试
        //原第一个空闲项写入flash 
        if(! __DFFSD_write_FDT_item(DBX_flash_tag,free_item,&FDT_item))
            return false;
        //新申请块的最后一项的next指针指向原第一个空闲项
        __fill_little_32bit(pl_FDT_item->next,0,free_item);
    }
    for(loop=1; loop < block_capacity-1; loop++)
    {//把新申请块除首末FDT条目以外的其他FDT条目连接起来
        pl_FDT_item = (struct fdt_info*)(FDT_buf + loop * 256);
        __fill_little_32bit(pl_FDT_item->previous,0,
                            FDT_item_new_start + loop - 1);
        __fill_little_32bit(pl_FDT_item->next,0,
                            FDT_item_new_start + loop + 1);
    }
    true_block = __DFFSD_write_new_block(DBX_flash_tag,
                                            new_block_no,0,chip->block_size);
    if(true_block == cn_limit_uint32)
        return false;  //此时，FDT根目录项的指针已经改变并写入flash，但新块缓冲
                       //区却没有写入flash，将造成FDT的free项链断裂，错误可以在
                       //启动时查出，那时将会看到根目录项中第一个空闲项的号码大
                       //于MAT中FDT块的容量。
    DBX_flash_tag->FDT_capacity += block_capacity;
    DBX_flash_tag->FDT_free_num += block_capacity;
    //根目录项的file_size指针用途特殊，前4字节记录着空闲FDT表项数
    __fill_little_32bit(FDT_item.file_size,0,DBX_flash_tag->FDT_free_num);
    //根目录项的file_size指针用途特殊，后4字节记录着总FDT表项数
    __fill_little_32bit(FDT_item.file_size,1,DBX_flash_tag->FDT_capacity);
    if(! __DFFSD_write_FDT_item(DBX_flash_tag,0,&FDT_item))
     	return false;
    //loop保存FDT首块的块号，用于定位MAT表中的FDT链
    loop = DBX_flash_tag->DDR_FDSB;
    do  //本循环用于找到MAT表中FDT链的最后一块
    {
        last_block_no = loop;
        loop = DBX_flash_tag->DDR_MAT[last_block_no].next;
    }while(loop != last_block_no);
    //把新申请块的实际写入的块号加入到MAT表中的FDT块号链中
    __DFFSD_add_MAT_item(DBX_flash_tag,last_block_no,true_block);
    if(!__DFFSD_write_DDR(DBX_flash_tag))
        return false;
    chip->erase_block(DBX_flash_tag->PCRB_no + DBX_flash_tag->start_block);
    return true;
}
//----把一个FDT_item添加到空闲-------------------------------------------------
//功能: 把一个已经被孤立的FDT_item加入到空闲项链表中，成为第一个结点。
//参数: DBX_flash_tag，被操作的flash文件柜的存储媒体标签
//      FDT_item_no，被释放的条目号
//返回: 无
//-----------------------------------------------------------------------------
void __DFFSD_add_FDT_free_item(struct st_DBX_flash_tag *DBX_flash_tag,
                         uint32_t FDT_item_no)
{
    uint32_t free_first;
    struct fdt_info FDT_item;
    
    __DFFSD_read_FDT_item(DBX_flash_tag,0,&FDT_item);   //读出根目录项
    DBX_flash_tag->FDT_free_num++;
    //FDT表根目录项的file_size指针特殊用途:前4字节记录空闲FDT表项数
    __fill_little_32bit(FDT_item.file_size,0,DBX_flash_tag->FDT_free_num);
    free_first = __pick_little_32bit(FDT_item.next,0);
    if(DBX_flash_tag->FDT_free_num == 1)      //原空闲链表是空的
    {
        //根目录项的向前向后指针均指向目标项
        __fill_little_32bit(FDT_item.next,0,FDT_item_no);
        __fill_little_32bit(FDT_item.previous,0,FDT_item_no);
        __DFFSD_write_FDT_item(DBX_flash_tag,0,&FDT_item);  //写入根目录项
        //读出被释放项
        __DFFSD_read_FDT_item(DBX_flash_tag,FDT_item_no,&FDT_item);
        //目标块的向前向后指针均指向根目录项
        __fill_little_32bit(FDT_item.next,0,0);
        __fill_little_32bit(FDT_item.previous,0,0);
        FDT_item.mattr |= cn_FS_ATTR_DELETED;

        __fill_little_32bit(FDT_item.parent,0,cn_limit_uint32);
        __fill_little_32bit(FDT_item.fstart_dson,0,cn_limit_uint32);        
        //被修改项写入flash。
        __DFFSD_write_FDT_item(DBX_flash_tag,FDT_item_no,&FDT_item);
        //为最大限度保留数据恢复信息，不修改除链表以外的其他数据
    }else               //空闲链表非空
    {
        //根目录项的向前指针均指向目标项
        __fill_little_32bit(FDT_item.next,0,FDT_item_no);
        __DFFSD_write_FDT_item(DBX_flash_tag,0,&FDT_item);  //写入根目录项
        //读出被释放项
        __DFFSD_read_FDT_item(DBX_flash_tag,FDT_item_no,&FDT_item);

        __fill_little_32bit(FDT_item.parent,0,cn_limit_uint32);
        __fill_little_32bit(FDT_item.fstart_dson,0,cn_limit_uint32);
        //被释放项前指针指向原最后一个空闲项
        __fill_little_32bit(FDT_item.next,0,free_first);

        __fill_little_32bit(FDT_item.previous,0,0);

    	FDT_item.mattr |= cn_FS_ATTR_DELETED;

    	strcpy(FDT_item.name,"free item");
        //被释放项写入flash中
        __DFFSD_write_FDT_item(DBX_flash_tag,FDT_item_no,&FDT_item);
        //读出原第一个空闲项
        __DFFSD_read_FDT_item(DBX_flash_tag,free_first,&FDT_item);
        //原第一个空闲项的后指针指向被释放项
        __fill_little_32bit(FDT_item.previous,0,FDT_item_no);
        //原最后一个空闲项写回flash
        __DFFSD_write_FDT_item(DBX_flash_tag,free_first,&FDT_item);
    }
}

//----孤立一个忙FDT_item-------------------------------------------------------
//功能: 把一个FDT_item从其原来的FDT链中孤立出来，这是删除文件/目录的第一步。
//参数: DBX_flash_tag，被操作的flash文件柜的存储媒体标签
//      FDT_item_no，被孤立的条目号
//返回: true = 成功，false = 失败，一般是应为FDT_item_no有子目录/文件
//-----------------------------------------------------------------------------
bool_t __DFFSD_acnode_FDT_item(struct st_DBX_flash_tag *DBX_flash_tag,
                         uint32_t FDT_item_no)
{
    uint32_t parent,elder,less,eldest;     //被释放项的父、兄、弟、长兄项目号
    struct fdt_info FDT_item;
    __DFFSD_read_FDT_item(DBX_flash_tag,FDT_item_no,&FDT_item); //读出目标项
    if((FDT_item.mattr & cn_FS_ATTR_DELETED) != 0)
        return false;   //目标FDT_item是一个空闲项/被删除项
    if((((FDT_item.mattr & cn_FS_ATTR_DIRECTORY) == 1) &&
          (__pick_little_32bit(FDT_item.fstart_dson,0)) != cn_limit_uint32))
        return false;   //目标FDT_item有子文件/目录的目录
    parent = __pick_little_32bit(FDT_item.parent,0);    //取父FDT项目号
    elder = __pick_little_32bit(FDT_item.previous,0);   //取兄FDT项目号
    less = __pick_little_32bit(FDT_item.next,0);        //取弟FDT项目号
    if(elder == less)       //兄、弟FDT项目号相等
    {
        if(elder == FDT_item_no)    //FDT_item_no是父目录下唯一的文件/目录
        {
            __DFFSD_read_FDT_item(DBX_flash_tag,parent,&FDT_item);//读父项
            //把子项号设置为没有子项
            __fill_little_32bit(FDT_item.fstart_dson,0,cn_limit_uint32);
            __DFFSD_write_FDT_item(DBX_flash_tag,parent,&FDT_item); //写父项
        }else                   //父目录下有两个文件/目录
        {
            //读出兄(弟)FDT项
            __DFFSD_read_FDT_item(DBX_flash_tag,elder,&FDT_item);
            __fill_little_32bit(FDT_item.previous,0,elder); //前指针指向自己
            __fill_little_32bit(FDT_item.next,0,elder);     //后指针指向自己
            __DFFSD_write_FDT_item(DBX_flash_tag,elder,&FDT_item);  //写回flash
            __DFFSD_read_FDT_item(DBX_flash_tag,parent,&FDT_item);  //读父项
            eldest = __pick_little_32bit(FDT_item.fstart_dson,0); //取长兄项目号
            if(eldest != elder)     //被孤立的恰好就是长兄
            {
                //设置父项的子项号
                __fill_little_32bit(FDT_item.fstart_dson,0,elder);
                __DFFSD_write_FDT_item(DBX_flash_tag,parent,&FDT_item);//写父项
            }
        }
    }else       //兄、弟项目号不相等，说明被孤立项的同级目录项至少有3个
    {
        __DFFSD_read_FDT_item(DBX_flash_tag,parent,&FDT_item);  //读父项
        eldest = __pick_little_32bit(FDT_item.fstart_dson,0);   //取长兄项号
        if(eldest == FDT_item_no)   //被孤立的恰好就是长兄
        {
            //设置父项的子项号
            __fill_little_32bit(FDT_item.fstart_dson,0,less);
            __DFFSD_write_FDT_item(DBX_flash_tag,parent,&FDT_item); //写父项
        }                   
        __DFFSD_read_FDT_item(DBX_flash_tag,elder,&FDT_item);   //读兄结点
        __fill_little_32bit(FDT_item.next,0,less); //兄结点的向后指针指向弟结点
        __DFFSD_write_FDT_item(DBX_flash_tag,elder,&FDT_item);  //写兄结点
        __DFFSD_read_FDT_item(DBX_flash_tag,less,&FDT_item);    //读弟结点
        //弟结点的向前指针指向弟结点
        __fill_little_32bit(FDT_item.previous,0,elder);
        __DFFSD_write_FDT_item(DBX_flash_tag,less,&FDT_item);   //写弟结点
    }
    //注意，为最大限度保留数据恢复信息，不对被孤立项做任何修改
    return true;
}

//----释放FDT_item-------------------------------------------------------------
//功能: 释放一个FDT_item到FDT表中，把它加入空闲项链表
//参数: DBX_flash_tag，被操作的flash文件柜的存储媒体标签
//      FDT_item_no，被释放的条目号
//返回: 无
//-----------------------------------------------------------------------------
void __DFFSD_free_FDT_item(struct st_DBX_flash_tag *DBX_flash_tag,
                         uint32_t FDT_item_no)
{
    struct flash_chip *chip = DBX_flash_tag->chip;
    //从目录链表中取出被操作的FDT项
    if(__DFFSD_acnode_FDT_item(DBX_flash_tag,FDT_item_no))
    {
        //把刚才取出的FDT项加入到空闲FDT链表中去
        __DFFSD_add_FDT_free_item(DBX_flash_tag,FDT_item_no);
    }
    chip->erase_block(DBX_flash_tag->PCRB_no + DBX_flash_tag->start_block);
}


//----分配FDT_item-------------------------------------------------------------
//功能：从FDT表中分配一个FDT_item，把它从空闲链表中取出。如果没有空闲FDT_item，
//      则调用DFFSD_add_FDT_block函数分配一块。
//参数: DBX_flash_tag，被操作的flash文件柜的存储媒体标签
//返回：新分配的FDT_item项号，出错则返回cn_limit_uint32
//-----------------------------------------------------------------------------
uint32_t __DFFSD_allocate_FDT_item(struct st_DBX_flash_tag *DBX_flash_tag)
{
    uint32_t next_free,next_next_free;
    struct fdt_info FDT_root,FDT_free_item;
    struct flash_chip *chip = DBX_flash_tag->chip;
    
    __DFFSD_read_FDT_item(DBX_flash_tag,0,&FDT_root);
    //所有空闲FDT_item都链接在以根目录为核心的双向链表上
    next_free = __pick_little_32bit(FDT_root.next,0);

    if(next_free >= DBX_flash_tag->FDT_capacity)
        return cn_limit_uint32;   //条目号大于FDT容量，肯定出错
    if(DBX_flash_tag->FDT_free_num == 0)
    {
        if(!__DFFSD_add_FDT_block(DBX_flash_tag))     //延长FDT表失败
            return cn_limit_uint32;
        __DFFSD_read_FDT_item(DBX_flash_tag,0,&FDT_root);   //重新读根目录项
        next_free = __pick_little_32bit(FDT_root.next,0); //重读根目录项next指针
    }
    //读取下一个空闲项，也就是即将返回给调用者的项
    __DFFSD_read_FDT_item(DBX_flash_tag,next_free,&FDT_free_item);
    //读取下下一个空闲项的编号
    next_next_free = __pick_little_32bit(FDT_free_item.next,0);
    DBX_flash_tag->FDT_free_num--;
    //FDT表根目录项的file_size指针特殊用途:前4字节记录空闲FDT表项数
    __fill_little_32bit(FDT_root.file_size,0,DBX_flash_tag->FDT_free_num);
    if(next_next_free == 0) //next_next_free指针指向第0项，这是最后一个空闲项
    {
        __fill_little_32bit(FDT_root.next,0,0);
        __fill_little_32bit(FDT_root.previous,0,0);
        if(! __DFFSD_write_FDT_item(DBX_flash_tag,0,&FDT_root))
            return cn_limit_uint32;
    }else
    {
        //读取下下一个空闲项的内容
        __DFFSD_read_FDT_item(DBX_flash_tag,next_next_free,&FDT_free_item);
        //根目录项的next指针指向下下一个空闲项的项号
        __fill_little_32bit(FDT_root.next,0,next_next_free);
        //下下一个空闲项的previous指针指向根目录项
        __fill_little_32bit(FDT_free_item.previous,0,0);
        //根目录项写入flash
        if(! __DFFSD_write_FDT_item(DBX_flash_tag,0,&FDT_root))
            return cn_limit_uint32;
        //下下一个空闲项写入flash
        if(!__DFFSD_write_FDT_item(DBX_flash_tag,next_next_free,&FDT_free_item))
            return cn_limit_uint32;
    }
    //如果写入FDT表时发生坏块替换，就需要刷新DDR表
    if(!__DFFSD_write_DDR(DBX_flash_tag))
        return false;
    chip->erase_block(DBX_flash_tag->PCRB_no + DBX_flash_tag->start_block);
    return next_free;
}
//----压缩FDT表----------------------------------------------------------------
//功能：当FDT表中空闲表项所占的存储器容量超过1个block时，可以调用本函数压缩之。
//参数: DBX_flash_tag，被操作的flash文件柜的存储媒体标签
//返回：无
// db 待增加
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

//----写一个FDT表项------------------------------------------------------------
//功能: 把一个FDT表项写入flash的FDT表中
//参数: DBX_flash_tag，被操作的flash文件柜的存储媒体标签
//      FDT_item_no，条目号
//      FDT_item，FDT表项指针
//返回: true=成功写入，false=失败，一般是遇到坏块且没有空闲块可替换。
//-----------------------------------------------------------------------------
bool_t __DFFSD_write_FDT_item(struct st_DBX_flash_tag *DBX_flash_tag,
                           uint32_t FDT_item_no,struct fdt_info *FDT_item)
{
    uint32_t true_block;
    struct flash_chip *chip;
    uint32_t block_offset,FDT_block_no,loop,FDT_block_serial;
    
    chip = DBX_flash_tag->chip;
    //计算FDT_item_no在fdt表中的块序号
    FDT_block_serial = FDT_item_no * sizeof(struct fdt_info) / chip->block_size;
    block_offset = FDT_item_no * sizeof(struct fdt_info) % chip->block_size;
    //读取FDT首块的块号
    FDT_block_no = DBX_flash_tag->DDR_FDSB;
    //从首块开始，沿MAT表搜索，找到第FDT_block_serial个FDT块的块号
    for(loop = 0; loop < FDT_block_serial; loop++)
        FDT_block_no = DBX_flash_tag->DDR_MAT[FDT_block_no].next;
    memcpy(chip->block_buf+block_offset,FDT_item,256);
    //写入目标块
    true_block = __DFFSD_write_update_block(DBX_flash_tag,FDT_block_no,
                                    block_offset,256);
    if(true_block != cn_limit_uint32)
    {//正确写入
        if(true_block != FDT_block_no)   //写入时发生了块替换
        {
        	__DFFSD_update_MAT_item(DBX_flash_tag,FDT_block_no,true_block);
            if(! __DFFSD_write_DDR(DBX_flash_tag))
                return false;
        }
    }else       //写入错误
        return false;
    chip->erase_block(DBX_flash_tag->PCRB_no + DBX_flash_tag->start_block);
    return true;
}

//----读一个FDT表项------------------------------------------------------------
//功能: 从FDT表中读取一个表项
//参数: DBX_flash_tag，被操作的flash文件柜的存储媒体标签
//      FDT_item_no，条目号
//      FDT_item，返回数据的指针
//返回: 无
//-----------------------------------------------------------------------------
void __DFFSD_read_FDT_item(struct st_DBX_flash_tag *DBX_flash_tag,
                           uint32_t FDT_item_no,struct fdt_info *FDT_item)
{
    struct flash_chip *chip;
    uint32_t block_offset,FDT_block_abs,FDT_block_no,loop,FDT_block_serial;
    chip = DBX_flash_tag->chip;
    //计算FDT_item_no在fdt表中的块序号
    FDT_block_serial = FDT_item_no * sizeof(struct fdt_info) / chip->block_size;
    block_offset = FDT_item_no * sizeof(struct fdt_info) % chip->block_size;
    //读取FDT首块的块号
    FDT_block_no = DBX_flash_tag->DDR_FDSB;
    //从首块开始，沿MAT表搜索，找到第FDT_block_serial个FDT块的块号
    for(loop = 0; loop < FDT_block_serial; loop++)
        FDT_block_no = DBX_flash_tag->DDR_MAT[FDT_block_no].next;
    //计算FDT_block在芯片中的绝对块号
    FDT_block_abs = FDT_block_no + DBX_flash_tag->start_block;
    chip->read_data_with_ecc(FDT_block_abs,block_offset,
                            (uint8_t*)FDT_item,sizeof(struct fdt_info)); 
}

//----构造一个FDT_item---------------------------------------------------------
//功能：构造一个FDT_item的内存影像，但是没有处理构成链表的4个指针。
//参数: FDT_item，待构造的FDT条目指针
//      attr，条目属性
//      name，条目名称
//返回: 无
//-----------------------------------------------------------------------------
void __DFFSD_asm_FDT_item(struct fdt_info *FDT_item,union file_attrs attr,
                          char *name)
{
    uint8_t attr_mirror;
    struct tm file_time;
    ufast_t i;
    //因存在可能的移植问题，不宜直接使用位联合结构的attr参数
    attr_mirror = 0;
    if(attr.bits.read_only)
        attr_mirror +=cn_FS_ATTR_READ_ONLY;
    if(attr.bits.hidden)
        attr_mirror +=cn_FS_ATTR_HIDDEN;
    if(attr.bits.folder)
        attr_mirror +=cn_FS_ATTR_DIRECTORY;
    if(attr.bits.archive)
        attr_mirror +=cn_FS_ATTR_ARCHIVE;   //以上只初始化4个属性，还两个暂不用
    FDT_item->mattr = attr_mirror;
    FDT_item->name_buf_len = cn_flash_file_name_limit +1;
    for(i = 0; i < 8; i++)
        FDT_item->file_size[i] = 0;    //文件尺寸=0
    //name是模块内部提供的字符串指针，已经经过字符串长度合法性检查
    if(strlen(name) <= cn_flash_file_name_limit)//读取文件柜名字
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

//----读取文件(目录)参数-------------------------------------------------------
//功能：从FDT表项的内存镜像中的中读取文件(目录)信息，并初始化文件结构指针fp。
//参数：DBX_flash_tag，被操作的文件柜指针，通用结构，非flash专用。
//      fp，保存文件(目录)属性的文件指针
//      FDT_item，从flash中读出的FDT表项
//返回：无
//-----------------------------------------------------------------------------
void __DFFSD_uasm_FDT_item(struct st_DBX_flash_tag *DBX_flash_tag,
                              struct file_rsc *fp,
                              struct fdt_info *FDT_item)
{
    uint8_t attr_mirror;
    uint32_t name_len;

    //对节点进行初始化，否则rsc_search_son搜索新文件时候，节点没有初始化，
    //无法搜索判断到底是不是NULL。
    //详见__djyfs_DBX_lookfor_item函数中调用rsc_search_son。
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
        fp->attr.bits.archive =1;   //以上只初始化3个属性，还两个暂不用
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
    //读文件尺寸，为防止符号为扩展导致不必要的麻烦，按无符号数移位。
    fp->file_size = __pick_little_32bit(FDT_item->file_size,0)
            +((uint64_t)__pick_little_32bit(FDT_item->file_size,1)<<32);
    name_len = strnlen(FDT_item->name,cn_flash_file_name_limit);
    if(name_len != cn_limit_uint32)  //文件柜名字不超过cn_flash_file_name_limit
        strcpy(fp->name, FDT_item->name);
    else
    {
        memcpy(fp->name, FDT_item->name,cn_flash_file_name_limit);
        fp->name[cn_flash_file_name_limit] = '\0';
    }
    //fp->open_counter = 0;
    return ;
}

//----解析根目录条目-------------------------------------------------------
//功能：从根目录的FDT表项的内存镜像中读取根目录信息，写到DBX_flash_tag中
//参数：DBX_flash_tag，被操作的文件柜指针，通用结构，非flash专用。
//      FDT_item，从flash中读出的FDT表项
//返回：无
//-----------------------------------------------------------------------------
void __DFFSD_uasm_root_folder(struct st_DBX_flash_tag *DBX_flash_tag,
                                struct fdt_info *FDT_item)
{
    //根目录的FDT条目的file_size有特殊用途，前4字节为FDT表中空闲FDT条目数，后
    //4个字节是FDT表中总FDT条目数
    DBX_flash_tag->FDT_free_num = __pick_little_32bit(FDT_item->file_size,0);
    DBX_flash_tag->FDT_capacity = __pick_little_32bit(FDT_item->file_size,1);
    return ;
}

//----修改fdt条目中的起始块号--------------------------------------------------
//功能: 当一个文件的首块被修改时，该文件的fdt条目中保存的起始块号也将被修改。只
//      有在发生磨损平衡块交换时，才有可能调用本函数，故在调用本函数时产生的写
//      操作并不修改磨损次数，即使发生坏块交换，也不调用标准的块分配函数
//      __allocate_block分配空闲块，而是直接从free块中取一块，不修改磨损次数。
//      这样可以避免发生连环套。
//参数: DBX_flash_tag,被操作的flash文件柜的存储媒体标签
//      FDT_item_no,fdt条目编号。
//      new_block_no，替换块号
//返回: 无
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

//----在FDT中修改文件尺寸------------------------------------------------------
//功能: 修改FDT表项中的文件尺寸域。
//参数: DBX_flash_tag，被操作的flash文件柜的存储媒体标签
//      fp，被修改的文件指针，由调用方保证不是目录而是文件。
//      new_size，新的文件尺寸
//返回: 无
//-----------------------------------------------------------------------------
void __DFFSD_update_FDT_file_size(struct st_DBX_flash_tag *DBX_flash_tag,
                                  struct file_rsc *fp,sint64_t new_size)
{
    uint32_t FDT_item_no;
    struct fdt_info FDT_item;
    FDT_item_no = (uint32_t)fp->file_medium_tag;
    __DFFSD_read_FDT_item(DBX_flash_tag,FDT_item_no,&FDT_item);
    __fill_little_32bit(FDT_item.file_size,0,new_size);
    //为防止符号为扩展导致不必要的麻烦，按无符号数移位。
    __fill_little_32bit(FDT_item.file_size,1,(uint64_t)new_size>>32);
    __DFFSD_write_FDT_item(DBX_flash_tag,FDT_item_no,&FDT_item);
    return ;
}

//----在FDT中修改文件名------------------------------------------------------
//功能: 修改FDT表项中的文件(目录)名域。
//参数: DBX_flash_tag，被操作的flash文件柜的存储媒体标签
//      fp，被修改的文件指针
//      name，新的文件(目录)名
//返回: 无
//-----------------------------------------------------------------------------
void __DFFSD_update_FDT_item_name(struct st_DBX_flash_tag *DBX_flash_tag,
                                  struct file_rsc *fp,char *name)
{
    uint32_t FDT_item_no;
    struct fdt_info FDT_item;
    FDT_item_no = (uint32_t)fp->file_medium_tag;
    __DFFSD_read_FDT_item(DBX_flash_tag,FDT_item_no,&FDT_item);
    //name是模块内部提供的字符串指针，已经经过字符串长度合法性检查
    if(strlen(name) <= cn_flash_file_name_limit)//读取文件柜名字
        strcpy(FDT_item.name,name);
    else
    {
        memcpy(FDT_item.name,name,cn_flash_file_name_limit);
        FDT_item.name[cn_flash_file_name_limit] = '\0';
    }
    __DFFSD_write_FDT_item(DBX_flash_tag,FDT_item_no,&FDT_item);
    return ;
}

//----在FDT中修改文件属性------------------------------------------------------
//功能: 修改FDT表项中的文件(目录)属性域。
//参数: DBX_flash_tag，被操作的flash文件柜的存储媒体标签
//      fp，被修改的文件指针
//      attr，新的文件(目录)属性
//返回: 无
//-----------------------------------------------------------------------------
void __DFFSD_update_FDT_item_attr(struct st_DBX_flash_tag *DBX_flash_tag,
                                  struct file_rsc *fp,union file_attrs attr)
{
    uint32_t FDT_item_no;
    struct fdt_info FDT_item;
    uint8_t attr_mirror;
    FDT_item_no = (uint32_t)fp->file_medium_tag;
    __DFFSD_read_FDT_item(DBX_flash_tag,FDT_item_no,&FDT_item);
    //因存在可能的移植问题，不宜直接使用位联合结构的attr参数
    attr_mirror = FDT_item.mattr;
    if(attr.bits.read_only)
        attr_mirror +=cn_FS_ATTR_READ_ONLY;
    if(attr.bits.hidden)
        attr_mirror +=cn_FS_ATTR_HIDDEN;
    if(attr.bits.archive)
        attr_mirror +=cn_FS_ATTR_ARCHIVE;   //以上只初始化3个属性，还两个暂不用
                                            //不能修改目录/文件属性
    FDT_item.mattr = attr_mirror;
    __DFFSD_write_FDT_item(DBX_flash_tag,FDT_item_no,&FDT_item);
    return ;
}
//----在FDT中修改创建时间------------------------------------------------------
//功能: 修改FDT表项中的创建时间域。
//参数: DBX_flash_tag，被操作的flash文件柜的存储媒体标签
//      fp，被修改的文件指针
//      new_time，新的时间
//返回: 无
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

//----在FDT中修改修改时间------------------------------------------------------
//功能: 修改FDT表项中的修改时间域。
//参数: DBX_flash_tag，被操作的flash文件柜的存储媒体标签
//      fp，被修改的文件指针
//      new_time，新的时间
//返回: 无
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
