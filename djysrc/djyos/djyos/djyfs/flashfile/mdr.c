//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//版权所有 (C), 2004-2009,   lst.
//所属模块:flash文件系统
//作者：lst
//版本：V1.0.0
//文件描述:flash文件系统中MDR表部分
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
#include "ddr.h"
#include "mdr.h"
#include <string.h>
//----初始化flash芯片---------------------------------------------------------
//功能：把芯片标记为djyosfs芯片，但并不建立文件柜。
//参数: chip，被操作的芯片
//      reserves，保留块数量
//返回: true=初始化成功，false=失败
//---------------------------------------------------------------------------
void DFFSD_init_MDR(struct flash_chip *chip,uint32_t reserves)
{
    uint8_t MDR_CDR_buffer[cn_MDR_CDR_size];
    struct MDR_CDR_mirror *MDR_CDR = (struct MDR_CDR_mirror *)MDR_CDR_buffer;
    uint32_t valid_blocks = 1;  //首块肯定是可用的，故初值=1
    uint32_t i,MDR_bak_block = 0;
    //注意，nandflash只有首块是保证有效的，故MDR表尺寸不能超过首块，这不成问题，
    //目前为止nandflash的block_size没有小于4K的。
    //而norflash则没有这个限制，norflash保证整片都是好的。
    for(i = 1; i < chip->block_sum; i++)
    {    //找一个好块作为MDR备用块
        if(chip->check_block(i))
        {
            valid_blocks++;
        }
    }
    MDR_bak_block = __DFFSD_probe_MDR_bak(chip);
    chip->valid_sum = valid_blocks;
    chip->rev_blocks = reserves;    //保留块数量
    //保留块起始块号
    chip->rev_start = MDR_bak_block
                      +(cn_MDR_size + chip->block_size -1) / chip->block_size;
    chip->DBX_start = chip->rev_start + reserves;   //文件柜起始块号
    if(valid_blocks <= chip->DBX_start)
        return;     //有效块数太少
    chip->MDR.MDR_bak_block = MDR_bak_block;
    chip->MDR.roll_next_main = cn_roll_offset;
    chip->MDR.roll_next_bak = cn_roll_offset;
    for(i = 0; i < cn_DBX_sum_max; i++)
        chip->MDR.DBX_created[i] = false;
    strcpy((char*)MDR_CDR_buffer,"djyosfs");
    __fill_little_32bit(MDR_CDR->chip_CDR_size,0,cn_MDR_CDR_size);
    __fill_little_32bit(MDR_CDR->chip_block_sum,0,chip->block_sum);
    __fill_little_32bit(MDR_CDR->chip_block_size,0,chip->block_size);
    __fill_little_32bit(MDR_CDR->chip_MDR_bak,0,MDR_bak_block);
    __fill_little_32bit(MDR_CDR->chip_reserve,0,reserves);
    __DFFSD_fill_ECC_MDR_CDR(MDR_CDR_buffer);
    __DFFSD_erase_MDR_main(chip);
    __DFFSD_write_MDR_main(chip,MDR_CDR_buffer,
                (uint32_t)MDR_CDR->chip_MDR_CDR_ECC-(uint32_t)MDR_CDR +4);

    __DFFSD_write_end_flag_main(chip);

    __DFFSD_erase_MDR_bak(chip);
    __DFFSD_write_MDR_bak(chip,MDR_CDR_buffer,
                (uint32_t)MDR_CDR->chip_MDR_CDR_ECC-(uint32_t)MDR_CDR +4);
    __DFFSD_write_end_flag_bak(chip);

}

//----读MDR_bak表-------------------------------------------------------------
//功能: 把MDR表读到缓冲区。
//参数: chip，目标芯片，chip->bak_start必须已经初始化
//      buf，保存所读数据的缓冲区
//返回: true=正确读取，false=错误。
//-----------------------------------------------------------------------------
bool_t __DFFSD_read_MDR_bak(struct flash_chip *chip,uint8_t *buf)
{
    uint32_t complete_len=0,read_len;
    uint32_t block_no = chip->MDR.MDR_bak_block;
    do  //本循环用于读取整个MDR_main到缓冲区，MDR_size可能大于1块，故循环。
    {
        if((cn_MDR_size-complete_len) > chip->block_size)
            read_len = chip->block_size;
        else
            read_len = cn_MDR_size-complete_len;
        //MDR表逐个记录单独校验，若整块校验，则每次增加记录都要改校验码，不可行
        chip->read_data_no_ecc(block_no,0,buf+complete_len,read_len);
        block_no++;
        complete_len += read_len;
    }while(complete_len < cn_MDR_size);
    return true;
}

//----读MDR_main表-------------------------------------------------------------
//功能: 把MDR表读到缓冲区。
//参数: chip，目标芯片
//      buf，保存所读数据的缓冲区
//返回: 无
//-----------------------------------------------------------------------------
void __DFFSD_read_MDR_main(struct flash_chip *chip,uint8_t *buf)
{
    uint32_t complete_len=0,read_len;
    uint32_t block_no = 0;      //MDR_main从0开始存储
    do  //本循环用于读取整个MDR_main到缓冲区，MDR_size可能大于1块，故循环。
    {
        if((cn_MDR_size-complete_len) > chip->block_size)
            read_len = chip->block_size;
        else
            read_len = cn_MDR_size-complete_len;
        //MDR表逐个记录单独校验，若整块校验，则每次增加记录都要改校验码，不可行
        chip->read_data_no_ecc(block_no,0,buf+complete_len,read_len);
        block_no++;
        complete_len += read_len;
    }while(complete_len < cn_MDR_size);
    return;
}

//----分配一个存储介质---------------------------------------------------------
//功能: 建立新文件柜时，调用此函数从存储介质中分配存储器。
//参数: chip，目标芯片
//      size，待分配的块数。
//      result，返回分配结果
//返回: true=分配成功，结果保存在result中，false = 分配额失败
////---------------------------------------------------------------------------
bool_t __DFFSD_allocate_DBX(struct flash_chip *chip,
                            uint32_t size,uint32_t *result)
{
    //uint8_t MDR_buf[cn_MDR_size];
    //文件柜起始块数组，用于查询空闲空间，+2是为包含可能存在的首尾空闲区
    uint32_t block_start[cn_DBX_sum_max+2];
    //文件柜结束块数组，用于查询空闲空间，+2是为包含可能存在的首尾空闲区
    uint32_t block_end[cn_DBX_sum_max+2];
    struct MDR_DBX_mirror *MDR_DBX;
    uint32_t loop,loop1=0,temp;
    uint8_t *MDR_buf;

    MDR_buf = (uint8_t*)m_malloc(cn_MDR_size, 0);
    if(MDR_buf == NULL)
        return false;
    __DFFSD_read_MDR_main(chip,MDR_buf); //读出MDR到缓冲区(原样读出，不解析)
    block_start[0] = chip->DBX_start;
    block_end[0] = chip->DBX_start;
    //MDR_DBX指针指向MDR缓冲区中第一个DBX区，用于检查各DBX区是否空闲
    MDR_DBX = (struct MDR_DBX_mirror *)(MDR_buf+cn_MDR_CDR_size);

    for(loop = 1; loop < cn_DBX_sum_max+1; loop++)
    {//读出各已经建立的文件柜的起始块号和结束块号，保存到block_start和block_end
    //两个数组里，数组元素比文件柜数多两个，是用于保存文件柜首尾空闲区间的
        if(chip->MDR.DBX_created[loop-1])
        {//对已经建立的文件柜，从它的MDR_DBX缓存中读出起始和结束块号
            
            //block_start[loop] = __pick_little_32bit(MDR_DBX->DBX_start_block,0);
        	block_start[loop] = block_end[loop-1]
                            + __pick_little_32bit(MDR_DBX->DBX_blocks_sum,0);
            //结束块号，该块已经不属于本文件柜了。
            block_end[loop] = block_end[loop-1]
                            + __pick_little_32bit(MDR_DBX->DBX_blocks_sum,0);
        }else
        {//为建立的文件柜，起始和结束块号均初始化为最近发现的已建立的文件柜的
        //起始和结束块号，等效于本文件柜是空的。
            block_start[loop] = block_end[loop-1];
            block_end[loop] = block_end[loop-1];
        }
        MDR_DBX++;  //缓冲区指向下一个文件柜的MDR_DBX
    }
    
    block_start[cn_DBX_sum_max+1] = chip->block_sum;//芯片尾部空闲区间的起始块号
    block_end[cn_DBX_sum_max+1] = chip->block_sum;  //芯片尾部空闲区间的结束块号
    for(loop = 0; loop <cn_DBX_sum_max+1; loop++)
    {//用冒泡法对各文件柜的起始块号排队(不是空闲块尺寸)，芯片反复使用后可能发生
    //交错，各文件柜所占用的存储器位置并不与其DBX在MDR中的顺序相同，只有按块地址
    //排序后，才能有效确定各可用空间的尺寸。
        for(loop1 = loop+1; loop1 < cn_DBX_sum_max+2; loop1++)
        {
            if(block_start[loop] > block_start[loop1])
            {
                temp = block_start[loop];
                block_start[loop] = block_start[loop1];
                block_start[loop1] = temp;
                temp = block_end[loop];
                block_end[loop] = block_end[loop1];
                block_end[loop1] = temp;
            }
        }
    }
    //找一个满足要求的连续空闲空间
    for(loop = 0; loop < cn_DBX_sum_max+1; loop++)
    {

    	if(!chip->MDR.DBX_created[loop])
        {
        	if(block_start[loop+1] - block_end[loop] >= size)
            {
                *result = block_start[loop];  //找到满足要求的空间
                    break;
               }
        }
    }
    m_free(MDR_buf);
    if(loop == cn_DBX_sum_max +1)
        return false;   //没有找到满足要求的连续空闲区域
    else
        return true;
}

//----写入一个MDR_DBX块--------------------------------------------------------
//功能: 把一个MDR_DBX块写入flash中，需同时写入MDR_main和MDR_bak中
//参数: chip，目标芯片
//      DBX_no，文件柜号
//      MDR_DBX_buf，已经填充好的缓冲区
//返回: true=成功，false=失败
//注：加入写入是否正确的校验功能----db
//-----------------------------------------------------------------------------
bool_t __DFFSD_write_MDR_DBX(struct flash_chip *chip,
                           uint32_t DBX_no,uint8_t *MDR_DBX_buf)
{
    struct MDR_temp_record MDR_record;
    uint32_t block_offset,block_no;
    uint8_t *MDR_buf;

    MDR_buf = (uint8_t*)m_malloc(cn_MDR_size, 0);
    if(MDR_buf == NULL)
        return false;
    //新MDR_DBX在MDR表中的偏移量(字节)
    block_offset = cn_MDR_CDR_size + DBX_no * cn_MDR_DBX_size;
    block_no = block_offset / chip->block_size;//新MDR_DBX在MDR表中的块号
    block_offset = block_offset % chip->block_size; //块内偏移量
    //检查MDR_main是否需要擦除
    if(chip->query_block_ready_no_ecc(
                            block_no,block_offset,MDR_DBX_buf,cn_MDR_DBX_size))
    {//MDR_main中该DBX块无需擦除
        __DFFSD_write_MDR_DBX_main(chip,MDR_DBX_buf,DBX_no);  //直接写入MDR_main
        //下面把新MDR_DBX写入到MDR_bak中
        block_no += chip->MDR.MDR_bak_block;    //取MDR_bak块号
        //检查MDR_bak是否需要擦除
        if(chip->query_block_ready_no_ecc(
                            block_no,block_offset,MDR_DBX_buf,cn_MDR_DBX_size))
        {//MDR_bak中该DBX块无需擦除
            __DFFSD_write_MDR_DBX_bak(chip,MDR_DBX_buf,DBX_no);
        }else
        {//MDR_bak中该DBX块需要擦除
            __DFFSD_read_MDR_main(chip,MDR_buf); //读完整MDR表
            __DFFSD_uasm_DBX(chip,&MDR_record,MDR_buf);
            //从缓冲区中取出滚动区的有效数据，保存到MDR_record中，因新文件柜
            //尚未完成初始化，滚动区中没有该文件柜的数据。
            __DFFSD_uasm_roll(chip,MDR_buf,
                              &MDR_record,chip->MDR.roll_next_main);
            memcpy(MDR_buf+cn_MDR_CDR_size + DBX_no * cn_MDR_DBX_size,
                        MDR_DBX_buf,cn_MDR_DBX_size);
            __DFFSD_erase_MDR_bak(chip);
            __DFFSD_write_MDR_bak(chip,MDR_buf,cn_CDR_and_DBX_size);
            __DFFSD_write_roll_bak(chip,&MDR_record);
            __DFFSD_write_end_flag_bak(chip);
        }
    }
    else
    {//MDR_main中该DBX块需要擦除
        __DFFSD_read_MDR_main(chip,MDR_buf);
        __DFFSD_uasm_DBX(chip,&MDR_record,MDR_buf);
        __DFFSD_uasm_roll(chip,MDR_buf,&MDR_record,chip->MDR.roll_next_main);
        memcpy(MDR_buf+cn_MDR_CDR_size + DBX_no * cn_MDR_DBX_size,
                            MDR_DBX_buf,cn_MDR_DBX_size);
        __DFFSD_erase_MDR_bak(chip);
        __DFFSD_write_MDR_bak(chip,MDR_buf,cn_CDR_and_DBX_size);
        __DFFSD_write_roll_bak(chip,&MDR_record);
        __DFFSD_write_end_flag_bak(chip);

        __DFFSD_erase_MDR_main(chip);
        __DFFSD_write_MDR_main(chip,MDR_buf,cn_CDR_and_DBX_size);
        __DFFSD_write_roll_main(chip,&MDR_record);
        __DFFSD_write_end_flag_main(chip);
    }
    m_free(MDR_buf);
    return true;
}
//----建立一个文件柜----------------------------------------------------------
//功能: 在指定芯片上增加一个文件柜,新文件柜只是登记在芯片上，并未加载至操作系统，
//      若要加载，必须重新启动系统。
//参数: chip，被操作的芯片
//      size，新增加的文件柜尺寸(用块数表示)
//      name，新增加的文件柜名字
//      nand_ecc，本文件柜是否需要ecc校验
//返回: true = 成功，false = 失败，失败原因一般是存储空间不够。
//---------------------------------------------------------------------------
bool_t DFFSD_create_DBX(struct flash_chip *chip, uint32_t size,char *name)
{
    struct st_DBX_flash_tag *DBX_flash_tag = NULL;
    struct st_DBX_device_tag *DBX_device_tag = NULL;
    djy_handle_t fs_handle_right = NULL;
    struct MDR_DBX_mirror *MDR_DBX;
    struct rsc_node *fs_rsc_node;
    struct DBX_semp_para semp_dbx;
    uint32_t DBX_start,DBX_no,name_len,loop,table_size,DDR_blocks;
    uint8_t MDR_DBX_buf[cn_MDR_DBX_size];

    if((chip == NULL) || (size == 0) || (name == NULL))
        return false;
    name_len = strnlen(name,cn_flash_DBX_name_limit);  //检测字符串长度，
    if(name_len == 0)               //文件柜名长度为0，非法
        return false;
    for(loop = 0; loop < cn_DBX_sum_max; loop++)//检查是否还可以建立新的文件柜
    {
        if(!chip->MDR.DBX_created[loop])
        {
            DBX_no = loop;      //第loop个文件柜未建立，其MDR_DBX区块是空的
            break;              //为新文件柜找到MDR_DBX块后退出循环
        }
    }
    if(loop == cn_DBX_sum_max)
        return false;   //MDR中已经没有空闲的DBX块

    //以下初始化新文件柜的MDR_DBX缓存
    MDR_DBX = (struct MDR_DBX_mirror*)MDR_DBX_buf;

    if(name_len != cn_limit_uint32)     //copy新文件柜名
    {
        strcpy(MDR_DBX->DBX_name,name);
    }else
    {
        memcpy(MDR_DBX->DBX_name,name,cn_flash_DBX_name_limit);
        MDR_DBX->DBX_name[cn_flash_DBX_name_limit] = '\0';
    }
    fs_rsc_node = rsc_search(rsc_get_root(),"dev\\fs"); //取文件系统设备的资源结点指针
    //在文件柜设备中查名为MDR_DBX->DBX_name的结点
    if(rsc_search_son(fs_rsc_node,MDR_DBX->DBX_name))
    {
        djy_error_login(enum_fs_DBX_name_repeat,"文件柜重名");
        printf_ng("文件柜重名");
        return false;
    }
    if( ! __DFFSD_allocate_DBX(chip,size,&DBX_start))   //分配存储空间
        return false;
    strcpy(MDR_DBX->DBX_format_flag,"unready"); //格式化标志
    __fill_little_32bit(MDR_DBX->DBX_start_block,0,DBX_start);  //起始块号
    __fill_little_32bit(MDR_DBX->DBX_blocks_sum,0,size);    //文件柜尺寸(块数)
    __DFFSD_fill_ECC_MDR_DBX((uint8_t *)MDR_DBX);                 //填充ECC码
    if( ! __DFFSD_write_MDR_DBX(chip,DBX_no,MDR_DBX_buf))
        return false;

    chip->MDR.DBX_created[DBX_no] = true;
    //打开文件系统设备的右手接口
    if((fs_handle_right = dev_open_right("fs",0)) == NULL)
        return false;

    //分配文件柜设备专用数据结构内存，该结构将嵌入到反设备结构中
    DBX_device_tag = (struct st_DBX_device_tag*)
                m_malloc_gbl(sizeof(struct st_DBX_device_tag),0);
    if(DBX_device_tag == NULL)
        goto goto_exit_install_chip;
    //分配flash文件系统专用数据结构内存，该结构将嵌入到文件柜设备中
    DBX_flash_tag = (struct st_DBX_flash_tag*)
                m_malloc_gbl(sizeof(struct st_DBX_flash_tag),0);
    if(DBX_flash_tag == NULL)
        goto goto_exit_install_chip;
    DBX_flash_tag->ART_position = NULL;
    DBX_flash_tag->ART_block_no = NULL;
    DBX_flash_tag->ART_times = NULL;
    DBX_flash_tag->writed_DDR_main = NULL;
    DBX_flash_tag->writed_DDR_bak = NULL;
    DBX_flash_tag->chip = chip;         //flash文件柜存储器所属芯片
    DBX_flash_tag->DBX_no = DBX_no;     //本文件柜在芯片中的序号
    DBX_flash_tag->PCRB_no = cn_limit_uint32;   //未格式化
    DBX_flash_tag->DDR_main = cn_limit_uint32;  //未格式化
    DBX_flash_tag->DDR_bak = cn_limit_uint32;   //未格式化
    DBX_flash_tag->start_block = DBX_start;
    DBX_flash_tag->block_sum = size;

    table_size = DBX_flash_tag->block_sum * (4+4+2);
    //为磨损位置表分配内存
    DBX_flash_tag->ART_position = (uint32_t*)m_malloc_gbl(table_size,0);
    if(DBX_flash_tag->ART_position == NULL)
        goto goto_exit_install_chip;
    //为磨损块号表分配内存
    DBX_flash_tag->ART_block_no = DBX_flash_tag->ART_position
                                  + DBX_flash_tag->block_sum;
    //为磨损次数表分配内存
    DBX_flash_tag->ART_times = (uint16_t*)(DBX_flash_tag->ART_block_no
                                  + DBX_flash_tag->block_sum);

    //计算DDR表尺寸
    DBX_flash_tag->DDR_size = __DFFSD_calculate_DDR_size(DBX_flash_tag);
    //1份DDR表占用的块数
    DDR_blocks = (DBX_flash_tag->DDR_size
                        +chip->block_size-1)/chip->block_size;
    //申请两份DBL表所需内存
    DBX_flash_tag->DDR_DBL =(uint32_t*)m_malloc_gbl(DDR_blocks*8,0);
    if(DBX_flash_tag->DDR_DBL == NULL)
        goto goto_exit_install_chip;
    //申请MAT表所需内存
    DBX_flash_tag->DDR_MAT = (struct MAT_table *)m_malloc_gbl(
                        DBX_flash_tag->block_sum*sizeof(struct MAT_table),0);
    if(DBX_flash_tag->DDR_MAT == NULL)
        goto goto_exit_install_chip;
    //两份写入标志
    DBX_flash_tag->writed_DDR_main = (bool_t*)m_malloc_gbl(
                                        DDR_blocks*sizeof(bool_t)*2,0);
    if(DBX_flash_tag->writed_DDR_main == NULL)
        goto goto_exit_install_chip;
    DBX_flash_tag->writed_DDR_bak=DBX_flash_tag->writed_DDR_main+DDR_blocks;
//    for(loop = 0; loop < DDR_blocks; loop++)
//    {
//        DBX_flash_tag->writed_DDR_main[loop] = false;
//        DBX_flash_tag->writed_DDR_bak[loop] = false;
//    }
    
//    __DFFSD_write_DDR(DBX_flash_tag);  
    //填充函数指针和文件柜名
    __DFFSD_fill_DBX_device_tag(DBX_device_tag,name);
    //文件柜设备的存储媒体指向flash文件柜存储器
    DBX_device_tag->DBX_medium_tag = (ptu32_t)DBX_flash_tag;
    DBX_device_tag->formatted =false;
    semp_dbx.left = chip->left_semaphore;   //芯片的左手信号量
    semp_dbx.right = chip->right_semaphore; //芯片的右手信号量
    if(dev_ctrl_right(fs_handle_right,enum_fs_add_DBX,
                        (ptu32_t)DBX_device_tag,(ptu32_t)&semp_dbx)
                        != enum_fs_no_error)
    {
        goto goto_exit_install_chip;
    }
    //这是正常出口
    dev_close_right(fs_handle_right);     //关闭文件系统设备
    return true;
goto_exit_install_chip:
    dev_close_right(fs_handle_right);     //关闭文件系统设备
    if(DBX_flash_tag !=NULL)
    {
        if(DBX_flash_tag->writed_DDR_main != NULL)
            m_free(DBX_flash_tag->writed_DDR_main);
        if(DBX_flash_tag->DDR_MAT != NULL)
            m_free(DBX_flash_tag->DDR_MAT);
        if(DBX_flash_tag->DDR_DBL != NULL)
            m_free(DBX_flash_tag->DDR_DBL);
        if(DBX_flash_tag->DDR_DBL != NULL)
            m_free(DBX_flash_tag->DDR_DBL);
        if(DBX_flash_tag->ART_position != NULL)
            m_free(DBX_flash_tag->ART_position);
        m_free(DBX_flash_tag);
    }
    if(DBX_device_tag !=NULL)   m_free(DBX_device_tag);
    return false;      //文件柜加入到设备树失败
}

//----查找MDR_bak的块号--------------------------------------------------------
//功能: 在读MDR_main失败，无法从中获取MDR_bak的块号时，根据芯片的块尺寸计算
//      MDR_bak可能的存放位置。当用本函数确定块号时，应该从该块号开始连续查找
//      5块，直到找到合法的MDR_bak块为止，如果5块都未能找到合法的MDR_bak，则认为
//      本flash没有按照djyosfs初始化过。
//参数: chip，目标芯片
//返回: 块号，0表示没有找到
//-----------------------------------------------------------------------------
uint32_t __DFFSD_probe_MDR_bak(struct flash_chip *chip)
{
    return (cn_MDR_size+chip->block_size-1) / chip->block_size;
}
//----校验MDR_CDR--------------------------------------------------------------
//功能: 校验MDR_CDR，并改正的数据，如果不能改正，返回错误
//参数: buf，保存有未校验的MDR_CDR数据的缓冲区
//返回: true=无错或者被修正，false=有错并且不能改正。
//-----------------------------------------------------------------------------
bool_t __DFFSD_verify_MDR_CDR(uint8_t *buf)
{
    return true;
}
//----校验MDR_DBX--------------------------------------------------------------
//功能: 校验MDR_DBX，并改正的数据，如果不能改正，返回错误
//参数: buf，保存有未校验的MDR_DBX数据的缓冲区
//返回: true=无错或者被修正，false=有错并且不能改正。
//-----------------------------------------------------------------------------
bool_t __DFFSD_verify_MDR_DBX(uint8_t *buf)
{
    return true;
}

//----校验MDR结束标志----------------------------------------------------------
//功能: 1.校验MDR表的最后4个字节是否AA5555AA,最多只允许一位不符。
//      2.校验CDR和DBX表的ECC
//参数: buf，字节缓冲区
//返回: true=校验正确，false=错误
//-----------------------------------------------------------------------------
//特别注意，对齐问题/大小端问题/最小寻址单元大于8位问题，这些问题使得不能把
//AA5555AA组成32位整数来比较
bool_t __DFFSD_verify_end_flag(uint8_t *buf)
{
    uint8_t temp[4] = {0xAA,0x55,0x55,0xAA};
    uint8_t xor;
    ufast_t loop4,loop8,error_bit = 0;
    for(loop4 = 0; loop4 < 4; loop4++)
    {
        if(buf[loop4] != temp[loop4])
        {
            xor = buf[loop4] ^ temp[loop4];
            for(loop8 = 0; loop8 < 8; loop8++)
            {
                if(xor & (1<<loop8))
                    error_bit ++;
                if(error_bit > 1)
                    return false;
            }
        }else
            continue;
    }
    if(! __DFFSD_verify_MDR_CDR(buf))
        return false;
    if(! __DFFSD_verify_MDR_DBX(buf))
        return false;
    return true;
}

//----MDR滚动记录ECC校验-------------------------------------------------------
//功能: MDR中滚动存储的记录进行ECC校验，并改正数据。
//参数: buf，记录指针
//返回: 0=无错，1=有错但被修正，2=无法修正
//-----------------------------------------------------------------------------
uint32_t __DFFSD_verify_MDR_record(uint8_t *buf)
{
    return 0;
}

//----扫描滚动存储区-----------------------------------------------------------
//功能: 扫描滚动存储缓冲区，读取已经格式化的文件柜的DDR_main块号、DDR_bak块号、
//      PCRB块号。
//参数: chip，被操作的芯片
//      MDR_buf，缓冲区
//      MDR_record，返回扫描到的数据的指针
//      end_offset，最后一条有效记录位置
//返回: 无
//-----------------------------------------------------------------------------
void __DFFSD_uasm_roll(struct flash_chip *chip,uint8_t *MDR_buf,
                       struct MDR_temp_record *MDR_record,uint32_t end_offset)
{
    uint8_t *record;
    uint32_t loop,loop3,item_sum=0;
    bool_t got_DDR_main[cn_DBX_sum_max];//各文件柜DDR_main块号是否找到标志
    bool_t got_DDR_bak[cn_DBX_sum_max]; //各文件柜DDR_bak块号是否找到标志
    bool_t got_PCRB[cn_DBX_sum_max];    //各文件柜PCRB块号是否找到标志

    for(loop3 = 0; loop3 < cn_DBX_sum_max; loop3++)
    {
        got_DDR_main[loop3] = false;
        got_DDR_bak[loop3] = false;
        got_PCRB[loop3] = false;
        if(MDR_record->formatted[loop3])
        {
            item_sum +=3;   //每个已经格式化的文件柜都有3条滚动记录
        }
    }
    //循环扫描所有滚动存储区的有效条目。
    for(loop = end_offset-8;loop >= cn_roll_offset; loop-=8)
    {
        record = MDR_buf + loop;
        for(loop3 = 0; loop3 < cn_DBX_sum_max; loop3++)
        {
            if(! MDR_record->formatted)     //文件柜未格式化
                continue;
            __DFFSD_verify_MDR_record(record); //校验该记录
            if(!got_DDR_main[loop3])        //DDR_main滚动记录还未找到
            {
                //检查本项目是不是第loop个文件柜的DDR_main块号
                if((record[0] == cn_MDR_DDR_main) && (record[1] == loop3))
                {
                    item_sum--;                 //找到，剩余项目数减量
                    got_DDR_main[loop3] = true;  //标记该DDR_main块号项已找到
                    //将结果写入返回的结构指针中
                    MDR_record->DDR_main_block_no[loop3]
                            = __pick_little_32bit(&record[2],0);
                    break;
                }
            }
            if(!got_DDR_bak[loop3])         //DDR_bak滚动记录还未找到
            {
                //检查本项目是不是第loop个文件柜的DDR_bak块号
                if((record[0] == cn_MDR_DDR_bak) && (record[1] == loop3))
                {
                    item_sum--;                 //找到，剩余项目数减量
                    got_DDR_bak[loop3] = true;  //标记该DDR_bak块号项已找到
                    //将结果写入返回的结构指针中
                    MDR_record->DDR_bak_block_no[loop3]
                                    = __pick_little_32bit(&record[2],0);
                    break;
                }
            }
            if(!got_PCRB[loop3])                //PCRB滚动记录还未找到
            {
                //检查本项目是不是第loop个文件柜的PCRB块号
                if((record[0] == cn_MDR_PCRB) && (record[1] == loop3))
                {
                    item_sum--;                 //找到，剩余项目数减量
                    got_PCRB[loop3] = true;  //标记该PCRB块号项已找到
                    //将结果写入返回的结构指针中
                    MDR_record->PCRB_block_no[loop3] =
                                __pick_little_32bit(&record[2],0);
                    break;
                }
            }
        }
        if(item_sum == 0)   //剩余项目为0，表明需要找的项目已经全部找到
            break;
    }
}

//----检查MDR_bak的完整性------------------------------------------------------
//功能: 检查MDR_bak是否完整，是否因上次掉电时正在写入而导致不完整，方法是检查
//      尾部的AA5555AA，以及最后一条滚动记录的校验码
//参数: chip，目标芯片
//返回: true=完整，false=不完整
//-----------------------------------------------------------------------------
bool_t __DFFSD_check_MDR_bak(struct flash_chip *chip)
{
    uint8_t *MDR_buf;

    MDR_buf = (uint8_t*)m_malloc(cn_MDR_size, 0);
    if(MDR_buf == NULL)
        return false;
    __DFFSD_read_MDR_bak(chip,MDR_buf);
    if(!__DFFSD_verify_end_flag(MDR_buf))
    {
        m_free(MDR_buf);
        return false;
    }
    m_free(MDR_buf);
    return true ;
}

//----检测滚动区结束位置-------------------------------------------------------
//功能: 检测滚动存储区第一个空闲记录的偏移量，用折半查找方法
//参数: chip，目标芯片
//      MDR_buf，MDR表缓冲区
//返回: 偏移量
//-----------------------------------------------------------------------------
uint32_t __DFFSD_find_roll_next(struct flash_chip *chip,uint8_t *MDR_buf)
{
    uint32_t busy_endian,free_endian,middle,result;
    uint32_t block_offset;
    busy_endian = cn_roll_offset/8;
    free_endian = cn_MDR_size /8 -1;   //最后一项用于完整性标记
    while(1)
    {
        middle = (busy_endian+free_endian)/2; //计算中间项
        block_offset = 8*middle;
        //测试第block_no块偏移block_offset开始的8字节是否擦除后未使用过。
        //虽然至今所知的flash都可以用是否等于0xff来判断是否擦除，但datasheet
        //上并没有这样说，难保哪天不会出来一个不是这样的，故把判断交给芯片
        //driver才可确保可移植性。
        if(chip->query_ready_with_data(NULL,MDR_buf+block_offset,8))
        {//第middle单元是空闲的
            if(middle == busy_endian)
            {
                result = middle*8;
                break;
            }else
                free_endian = middle;
        }else
        {//第middle单元是已经使用了的
            if(free_endian == middle+1)
            {
                result = free_endian*8;
                break;
            }else
                busy_endian = middle;
        }
    }//查找滚动位置结束
    return result;
}

//----解析DBX表----------------------------------------------------------------
//功能: 扫描MDR缓冲区的3个DBX表，找出已经建立的文件柜并读出文件柜参数
//参数: chip，目标芯片，也用于保存扫描结果
//      MDR_record，用于保存结果的指针
//      MDR_buf，MDR表缓冲区
//返回: 已经建立的文件柜数量
//-----------------------------------------------------------------------------
ufast_t __DFFSD_uasm_DBX(struct flash_chip *chip,
                         struct MDR_temp_record *MDR_record,uint8_t *MDR_buf)
{
    struct MDR_DBX_mirror *MDR_DBX;
    uint32_t loop,name_len;
    ufast_t DBX_sum=0;    //已经建立的文件柜数量
    bool_t formatted,unformatted;
    //取MDR_DBX记录的起始地址
    MDR_DBX = (struct MDR_DBX_mirror *)(MDR_buf + cn_MDR_CDR_size);
    for(loop = 0; loop < cn_DBX_sum_max; loop++)
    {//本循环： 1、判断文件柜创建否
     //         2、读文件柜名，
     //         3、读各文件柜的起始块号和块数，不管初始化否
        formatted = (strcmp(MDR_DBX->DBX_format_flag,"ready")==0);
        unformatted = (strcmp(MDR_DBX->DBX_format_flag,"unready")==0);
        if(formatted || unformatted)    //不管格式化否，都表示文件柜已经建立。
        {
            DBX_sum++;                              //文件柜总数增量
            chip->MDR.DBX_created[loop] = true;     //文件柜创建标志设为true
            MDR_record->formatted[loop] = formatted;    //设格式化标志
            name_len = strnlen(MDR_DBX->DBX_name,cn_flash_DBX_name_limit);
            if(name_len == 0)
                strcpy(MDR_record->name[loop],"no name deedbox");
            else if(name_len != cn_limit_uint32)
                strcpy(MDR_record->name[loop],MDR_DBX->DBX_name);
            else
            {
                memcpy(MDR_record->name[loop],MDR_DBX->DBX_name,
                                        cn_flash_DBX_name_limit);
                MDR_record->name[loop][cn_flash_DBX_name_limit] = '\0';
            }
            //文件柜起始块号
            MDR_record->start_blocks[loop] =
                            __pick_little_32bit(MDR_DBX->DBX_start_block,0);
            //文件柜尺寸(以块数表示)
            MDR_record->blocks_sum[loop] =
                            __pick_little_32bit(MDR_DBX->DBX_blocks_sum,0);

        }else   //MDR_DBX既不是"ready"也不是"unready"开头，表明文件柜未创建
        {
            chip->MDR.DBX_created[loop] = false;    //文件柜创建标志设为false
            MDR_record->formatted[loop] = false;    //设格式化标志
        }
        //取下一个文件柜MDR_DBX的偏移地址
        MDR_DBX = (struct MDR_DBX_mirror *)((uint32_t)MDR_DBX +cn_MDR_DBX_size);
    }
    return DBX_sum;
}

//----初始化读MDR表------------------------------------------------------------
//功能: 芯片初始化时读整个MDR表，提取有效数据。如果发现MDR_main和MDR_bak中任意
//      一个有错误，刷新之
//参数: chip，被操作的芯片，同时用于返回结果
//      MDR_record，保存有效数据的指针
//返回: 扫描MDR的结果保存在chip和MDR_record两个结构指针中
//      true=芯片已经按djyosfs初始化，false=芯片未按djyosfs初始化
//-----------------------------------------------------------------------------
//特注: 调试本函数时多次引起rvd崩溃，访问非法地址而被windows关闭，而ARM本身运行
//      正常，重新打开rvd后，甚至可以从上次退出的地方接着运行。
bool_t __DFFSD_read_MDR(struct flash_chip *chip,struct MDR_temp_record *MDR_record)
{
    uint8_t *MDR_buf;
    uint32_t roll_size;
    uint8_t roll_buf[cn_DBX_sum_max*3*8];
    bool_t MDR_main_valid,MDR_bak_valid;
    struct MDR_CDR_mirror *MDR_CDR;
    ufast_t DBX_sum = 0;    //已经建立的文件柜数量
    

    MDR_buf = (uint8_t*)m_malloc(cn_MDR_size, 0);
    if(MDR_buf == NULL)
        return false;    
    __DFFSD_read_MDR_main(chip,MDR_buf);
    if(!__DFFSD_verify_end_flag(&MDR_buf[cn_MDR_size-4]))  //检查MDR_main是否完整
    {//MDR_main有错误，读取MDR_bak
        MDR_main_valid = false;
        chip->MDR.MDR_bak_block = __DFFSD_probe_MDR_bak(chip);   //探测MDR_bak的存储位置
    }else
    {
        MDR_main_valid = true;
        MDR_CDR = (struct MDR_CDR_mirror *)MDR_buf;
        if(strcmp(MDR_CDR->chip_init_flag,"djyosfs") != 0)
        {
            m_free(MDR_buf);
            return false;      //存储器尚未初始化
        }
        //下面把CDR表的内容读取到chip资源中
        chip->MDR.MDR_bak_block = __pick_little_32bit(MDR_CDR->chip_MDR_bak,0);
        //读取保留块的数量和起始块号，保留块数量并不考虑其中可能有坏块
        chip->rev_start = chip->MDR.MDR_bak_block
                          + (cn_MDR_size+chip->block_size -1)/chip->block_size;
        chip->rev_blocks = __pick_little_32bit(MDR_CDR->chip_reserve,0);
        //计算用于文件柜的起始块号，
        chip->DBX_start = chip->rev_start + chip->rev_blocks;
        DBX_sum = __DFFSD_uasm_DBX(chip,MDR_record,MDR_buf);
        if(DBX_sum != 0)
        {
            //找到滚动存储区的下一个存储位置
            chip->MDR.roll_next_main = __DFFSD_find_roll_next(chip,MDR_buf);
            //扫描并提取滚动记录中的有效记录(即每类记录的最后一条记录)
            __DFFSD_uasm_roll(chip,MDR_buf,MDR_record,
                              chip->MDR.roll_next_main);
        }else
            chip->MDR.roll_next_main = cn_roll_offset;
    }

    __DFFSD_read_MDR_bak(chip,MDR_buf);     //读MDR_bak到缓冲区中
    if(!__DFFSD_verify_end_flag(&MDR_buf[cn_MDR_size-4]))  //检查MDR_bak是否完整
    {
        MDR_bak_valid = false;  //MDR_bak有错误
    }else
    {
        MDR_bak_valid = true;
        if(MDR_main_valid)      //如果MDR_main有效，则只读取下一个滚动存储位置
        {
            if(DBX_sum != 0)
                chip->MDR.roll_next_bak = __DFFSD_find_roll_next(chip,MDR_buf);
            else
                chip->MDR.roll_next_bak = cn_roll_offset;
        }else   //如果MDR_main无效，则所有文件柜信息从MDR_bak读取
        {
            MDR_CDR = (struct MDR_CDR_mirror *)MDR_buf;
            if(strcmp(MDR_CDR->chip_init_flag,"djyosfs")!=0)
            {
                m_free(MDR_buf);
                return false;      //存储器尚未初始化
            }
            //下面把CDR表的内容读取到chip资源中
            //读取保留块的数量和起始块号，保留块数量并不考虑其中可能有坏块
            chip->rev_start = chip->MDR.MDR_bak_block
                          +(cn_MDR_size+chip->block_size -1)/chip->block_size;
            chip->rev_blocks = __pick_little_32bit(MDR_CDR->chip_reserve,0);
            //计算用于文件柜的起始块号，
            chip->DBX_start = chip->rev_start + chip->rev_blocks;
            DBX_sum = __DFFSD_uasm_DBX(chip,MDR_record,MDR_buf);
            if(DBX_sum != 0)
            {
                chip->MDR.roll_next_bak = __DFFSD_find_roll_next(chip,MDR_buf);
                //扫描并提取滚动记录中的有效记录(即每类记录的最后一条记录)
                __DFFSD_uasm_roll(chip,MDR_buf,MDR_record,
                                  chip->MDR.roll_next_bak);
            }
        }
    }
    if((!MDR_main_valid) && (!MDR_bak_valid))
    {
        m_free(MDR_buf);    //如果MDR_main和MDR_bak都错误，说明未初始化存储器
        return false;
    }
    else
    {
        roll_size = __DFFSD_asm_roll_buf(chip,MDR_record,roll_buf);
        if(!MDR_main_valid) //MDR_main有误，用MDR_bak恢复之
        {
            __DFFSD_erase_MDR_main(chip);
            memcpy(MDR_buf+cn_roll_offset,roll_buf,roll_size);
            __DFFSD_write_MDR_main(chip,MDR_buf,cn_roll_offset + roll_size);
            __DFFSD_write_end_flag_main(chip);
        }else if(!MDR_bak_valid) //MDR_bak有误，用MDR_main恢复之
        {
            __DFFSD_erase_MDR_bak(chip);
            memcpy(MDR_buf+cn_roll_offset,roll_buf,roll_size);
            __DFFSD_write_MDR_bak(chip,MDR_buf,cn_roll_offset + roll_size);
            __DFFSD_write_end_flag_bak(chip);
        }
    }
    m_free(MDR_buf);
    return true;
}

//----填充滚动条目校验码-------------------------------------------------------
//功能: 给一个MDR的滚动存储记录增加校验码。
//参数: buf，记录指针
//返回: 无
//-----------------------------------------------------------------------------
void __DFFSD_fill_ECC_MDR_record(uint8_t *buf)
{
}

//----填充MDR_CDR校验码--------------------------------------------------------
//功能: 给一个MDR_CDR增加校验码。
//参数: buf，CDR记录指针
//返回: 无
//-----------------------------------------------------------------------------
void __DFFSD_fill_ECC_MDR_CDR(uint8_t *buf)
{
}

//----填充MDR_DBX校验码--------------------------------------------------------
//功能: 给一个MDR_DBX增加校验码。
//参数: buf，DBX记录指针
//返回: 无
//-----------------------------------------------------------------------------
void __DFFSD_fill_ECC_MDR_DBX(uint8_t *buf)
{
}

//----写MDR_main的结束标记-----------------------------------------------------
//功能: 写入MDR_main的结束标志，表示MDR_main是完整的
//参数: chip，被操作的芯片
//返回: 无
//-----------------------------------------------------------------------------
void __DFFSD_write_end_flag_main(struct flash_chip *chip)
{
    uint32_t block_no,block_offset;
    uint8_t buf[4];
    block_no = (cn_MDR_size -4)/chip->block_size;
    block_offset = (cn_MDR_size -4)%chip->block_size;
    buf[0] = 0xaa;
    buf[1] = 0x55;
    buf[2] = 0x55;
    buf[3] = 0xaa;
    chip->write_data_no_ecc(block_no,block_offset,buf,4);
}

//----写MDR_bak的结束标记-----------------------------------------------------
//功能: 写入MDR_bak的结束标志，表示MDR_bak是完整的
//参数: chip，被操作的芯片
//返回: 无
//-----------------------------------------------------------------------------
void __DFFSD_write_end_flag_bak(struct flash_chip *chip)
{
    uint32_t block_no,block_offset;
    uint8_t buf[4];
    block_no = (cn_MDR_size -4)/chip->block_size + chip->MDR.MDR_bak_block;
    block_offset = (cn_MDR_size -4)%chip->block_size;
    buf[0] = 0xaa;
    buf[1] = 0x55;
    buf[2] = 0x55;
    buf[3] = 0xaa;
    chip->write_data_no_ecc(block_no,block_offset,buf,4);
}

//----写MDR_main--------------------------------------------------------------
//功能: 从DDR_main开头部分开始，写入指定长度的数据。
//参数: chip，被操作的芯片
//      MDR_buf，缓冲区指针
//      len,写入长度
//返回: 无
//-----------------------------------------------------------------------------
void __DFFSD_write_MDR_main(struct flash_chip *chip,uint8_t *MDR_buf,uint32_t len)
{
    uint32_t complete_len=0,write_len;
    uint32_t block_no;
    block_no = 0;   //MDR_main默认从0块开始

    do  //把缓冲区中的数据写入到flash中，数据可能跨块边界，故循环
    {
        if((len - complete_len) > chip->block_size)
            write_len = chip->block_size;
        else
            write_len = len-complete_len;
        //写入数据到flash中，不校验
        chip->write_data_no_ecc(block_no,0,MDR_buf+complete_len,write_len);
        block_no++;
        complete_len += write_len;
    }while(complete_len < len);
}

//----写MDR_bak--------------------------------------------------------------
//功能: 从DDR_bak开头部分开始，写入指定长度的数据。
//参数: chip，被操作的芯片
//      MDR_buf，缓冲区指针
//      len,写入长度
//返回: 无
//-----------------------------------------------------------------------------
void __DFFSD_write_MDR_bak(struct flash_chip *chip,uint8_t *MDR_buf,uint32_t len)
{
    uint32_t complete_len=0,write_len;
    uint32_t block_no;
    block_no = chip->MDR.MDR_bak_block;   //MDR_main默认从0块开始
    do  //把缓冲区中的数据写入到flash中，数据可能跨块边界，故循环
    {
        if((len - complete_len) > chip->block_size)
            write_len = chip->block_size;
        else
            write_len = len-complete_len;
        //写入数据到flash中，不校验
        chip->write_data_no_ecc(block_no,0,MDR_buf+complete_len,write_len);
        block_no++;
        complete_len += write_len;
    }while(complete_len < len);
}

//----写MDR_main的DBX部分------------------------------------------------
//功能: 把DBX写入到MDR_main中，调用此函数前，确保保存DBX记录的部分已经被擦除.
//参数: chip，被操作的芯片
//      MDR_buf，缓冲区指针
//      DBX_no，文件柜编号
//返回: 无
//-----------------------------------------------------------------------------
void __DFFSD_write_MDR_DBX_main(struct flash_chip *chip,uint8_t *MDR_DBX_buf,
                                                      uint32_t DBX_no)
{
    uint32_t complete_len=0,write_len,write_size;
    uint32_t block_no,block_offset;
    write_size = cn_MDR_DBX_size;
    block_no = (cn_MDR_CDR_size + DBX_no*cn_MDR_DBX_size) / chip->block_size ;
    block_offset = (cn_MDR_CDR_size + DBX_no*cn_MDR_DBX_size)% chip->block_size;
    do  //把缓冲区中的数据写入到flash中，数据可能跨块边界，故循环
    {
        if((write_size-complete_len) > chip->block_size-block_offset)
            write_len = chip->block_size-block_offset;
        else
            write_len = write_size-complete_len;
        //写入数据到flash中，不校验
        chip->write_data_no_ecc(block_no,block_offset,
                                MDR_DBX_buf+complete_len,write_len);
        block_no++;
        block_offset = 0;       //下一块肯定从块首开始写
        complete_len += write_len;
    }while(complete_len < write_size);
}

//----写MDR_bak的DBX部分------------------------------------------------
//功能: 把DBX写入到MDR_bak中，调用此函数前，确保保存DBX记录的部分已经被擦除.
//参数: chip，被操作的芯片
//      MDR_buf，缓冲区指针
//      DBX_no，文件柜编号
//返回: 无
//-----------------------------------------------------------------------------
void __DFFSD_write_MDR_DBX_bak(struct flash_chip *chip,uint8_t *MDR_DBX_buf,
                                                 uint32_t DBX_no)
{
    uint32_t complete_len=0,write_len,write_size;
    uint32_t block_no,block_offset;
    write_size = cn_MDR_DBX_size;
    block_no = (cn_MDR_CDR_size + DBX_no*cn_MDR_DBX_size) / chip->block_size
                                    + chip->MDR.MDR_bak_block;
    block_offset = (cn_MDR_CDR_size + DBX_no*cn_MDR_DBX_size) % chip->block_size ;
    do  //把缓冲区中的数据写入到flash中，数据可能跨块边界，故循环
    {
        if((write_size-complete_len) > chip->block_size-block_offset)
            write_len = chip->block_size-block_offset;
        else
            write_len = write_size-complete_len;
        //写入数据到flash中，不校验
        chip->write_data_no_ecc(block_no,block_offset,
                                MDR_DBX_buf+complete_len,write_len);
        block_no++;
        block_offset = 0;       //下一块肯定从块首开始写
        complete_len += write_len;
    }while(complete_len < write_size);
}

//----写一条MDR滚动记录--------------------------------------------------------
//功能: MDR表中写入一个滚动记录，只有MDR_main中才会有此操作，MDR_bak中是所有滚动
//      记录同时写入，不会有单项写入的操作。
//参数: chip，被操作的芯片
//      DBX_no，文件柜编号
//      type，写入类型，可能值cn_MDR_DDR_main、cn_MDR_DDR_bak、cn_MDR_PCRB
//      value，被写入的数值
//返回: 无
//注意: 不能对未格式化的文件柜调用本函数，这是一个内部函数，不检查目标文件柜
//      是否已经格式化
//-----------------------------------------------------------------------------
void __DFFSD_write_MDR_roll_record(struct flash_chip *chip,uint32_t DBX_no,
                                   uint32_t type,uint32_t value)
{
    uint8_t item_buf[8];
    uint32_t block_no,block_offset;
    uint8_t *MDR_buf;
    struct MDR_temp_record MDR_record;

    MDR_buf = (uint8_t*)m_malloc(cn_MDR_size, 0);  
    if(MDR_buf == NULL)
         return ;
    item_buf[0] = type;             //填充记录类型
    item_buf[1] = DBX_no;           //填充文件柜号
    //填充该文件柜的DDR_main块号
    __fill_little_32bit(&item_buf[2],0,value);
    __DFFSD_fill_ECC_MDR_record(item_buf);//填充校验码
    if((chip->MDR.roll_next_main + 8) > cn_MDR_size-8)
    {//特别注明，如cn_MDR_size<784,此处将成死循环
        __DFFSD_read_MDR_main(chip,MDR_buf);
        __DFFSD_uasm_DBX(chip,&MDR_record,MDR_buf);
        __DFFSD_uasm_roll(chip,MDR_buf,&MDR_record,
                          chip->MDR.roll_next_main);
        if(type == cn_MDR_DDR_main)
            MDR_record.DDR_main_block_no[DBX_no] = value;
        else if(type == cn_MDR_DDR_bak)
            MDR_record.DDR_bak_block_no[DBX_no] = value;
        else if(type == cn_MDR_PCRB)
            MDR_record.PCRB_block_no[DBX_no] = value;
        __DFFSD_write_roll_bak(chip,&MDR_record);
        __DFFSD_erase_MDR_main(chip);
        __DFFSD_write_MDR_main(chip,MDR_buf,cn_CDR_and_DBX_size);
        __DFFSD_write_roll_main(chip,&MDR_record);
        __DFFSD_write_end_flag_main(chip);
    }else
    {
        block_no = chip->MDR.roll_next_main / chip->block_size ;
        block_offset = chip->MDR.roll_next_main % chip->block_size ;
        chip->write_data_no_ecc(block_no,block_offset,item_buf,8);
        chip->MDR.roll_next_main += 8;
    }
    m_free(MDR_buf);
}

//----构造滚动存储区-----------------------------------------------------------
//功能: 从滚动存储记录生成能够直接保存在flash中的数据，已格式化的文件柜才会有
//      滚动记录。
//参数: chip，被操作的芯片
//      MDR_record，滚动存储记录
//      roll_buf，滚动存储区缓冲区
//返回: roll_buf中有效数据长度
//-----------------------------------------------------------------------------
uint32_t __DFFSD_asm_roll_buf(struct flash_chip *chip,
                          struct MDR_temp_record *MDR_record,uint8_t *roll_buf)
{
    uint32_t loop,DBX_sum=0;
    uint8_t *buf = roll_buf;
    for(loop = 0; loop < cn_DBX_sum_max; loop++)
    {
        if(MDR_record->formatted[loop])   //文件柜已经格式化
        {
            buf[0] = (uint8_t)cn_MDR_DDR_main; //填充记录类型
            buf[1] = (uint8_t)loop;            //填充文件柜号
            //填充该文件柜的DDR_main块号
            __fill_little_32bit(buf+2,0,MDR_record->DDR_main_block_no[loop]);
            __DFFSD_fill_ECC_MDR_record(buf);//填充校验码
            buf += 8;
            buf[0] = cn_MDR_DDR_bak; //填充记录类型
            buf[1] = loop;            //填充文件柜号

            //填充该文件柜的DDR_bak块号
            __fill_little_32bit(buf+2,0,MDR_record->DDR_bak_block_no[loop]);
            __DFFSD_fill_ECC_MDR_record(buf);//填充校验码
            buf += 8;
            buf[0] = cn_MDR_PCRB; //填充记录类型
            buf[1] = loop;            //填充文件柜号

            //填充该文件柜的PCRB块号
            __fill_little_32bit(buf+2,0,MDR_record->PCRB_block_no[loop]);
            __DFFSD_fill_ECC_MDR_record(buf);//填充校验码
            buf += 8;
            DBX_sum +=8*3;
        }
    }
    return DBX_sum;
}
//----写MDR_main的全部滚动记录-------------------------------------------------
//功能: 写入MDR_main表中的全部滚动存储项
//参数: chip，被操作的芯片
//      MDR_record，保存全部滚动存储项的指针
//返回: 无
//-----------------------------------------------------------------------------
void __DFFSD_write_roll_main(struct flash_chip *chip,
                             struct MDR_temp_record *MDR_record)
{
    struct st_MDR_flag *MDR_flag = &chip->MDR;
    uint8_t roll_buf[cn_DBX_sum_max*3*8];
    uint32_t complete_len=0,write_len,write_size;
    uint32_t block_no,block_offset;
    bool_t write_end_flag=false;
    uint8_t *MDR_buf;

    MDR_buf = (uint8_t*)m_malloc(cn_MDR_size, 0);
    if(MDR_buf == NULL)
        return ;
    write_size = __DFFSD_asm_roll_buf(chip,MDR_record,roll_buf);
    if(write_size != 0)
    {
        //如果写入长度+当前偏移大于滚动区容量
        if((MDR_flag->roll_next_main + write_size) > cn_MDR_size-8)
        {//特别注明，如cn_MDR_size<784,此处将成死循环
            __DFFSD_write_roll_bak(chip,MDR_record);
            __DFFSD_read_MDR_main(chip,MDR_buf);
            __DFFSD_erase_MDR_main(chip);
            __DFFSD_write_MDR_main(chip,MDR_buf,cn_CDR_and_DBX_size);
            MDR_flag->roll_next_main = cn_roll_offset;
            write_end_flag = true;
        }
        block_no = MDR_flag->roll_next_main / chip->block_size ;
        block_offset = MDR_flag->roll_next_main % chip->block_size;
        MDR_flag->roll_next_main += write_size;
        do  //把缓冲区中的数据写入到flash中，数据可能跨块边界，故循环
        {
            if((write_size-complete_len) > chip->block_size-block_offset)
                write_len = chip->block_size-block_offset;
            else
                write_len = write_size-complete_len;
            //写入数据到flash中，不校验
            chip->write_data_no_ecc(block_no,block_offset,
                                    roll_buf+complete_len,write_len);
            block_no++;
            block_offset = 0;       //下一块肯定从块首开始写
            complete_len += write_len;
        }while(complete_len < write_size);
        if(write_end_flag)
            __DFFSD_write_end_flag_main(chip);
    }
    m_free(MDR_buf);
}

//----写MDR_bak的全部滚动记录-------------------------------------------------
//功能: 写入MDR_bak表中的全部滚动存储项
//参数: chip，被操作的芯片
//      MDR_record，保存全部滚动存储项的指针
//返回: 无
//-----------------------------------------------------------------------------
void __DFFSD_write_roll_bak(struct flash_chip *chip,
                            struct MDR_temp_record *MDR_record)
{
    struct st_MDR_flag *MDR_flag = &chip->MDR;
    uint8_t roll_buf[cn_DBX_sum_max*3*8];
    uint32_t complete_len=0,write_len,write_size;
    uint32_t block_no,block_offset;
    bool_t write_end_flag=false;
    uint8_t *MDR_buf;

    MDR_buf = (uint8_t*)m_malloc(cn_MDR_size, 0);
    if(MDR_buf == NULL)
        return ;
    write_size = __DFFSD_asm_roll_buf(chip,MDR_record,roll_buf);
    if(write_size != 0)
    {
        if((MDR_flag->roll_next_bak + write_size) > cn_MDR_size-8)
        {//特别注明，如cn_MDR_size<784,此处将成死循环
            __DFFSD_read_MDR_bak(chip,MDR_buf);
            __DFFSD_erase_MDR_bak(chip);
            __DFFSD_write_MDR_bak(chip,MDR_buf,cn_CDR_and_DBX_size);
            MDR_flag->roll_next_bak = cn_roll_offset;
            write_end_flag = true;
        }
        block_no = MDR_flag->roll_next_main / chip->block_size ;
        block_offset = MDR_flag->roll_next_main % chip->block_size ;
        MDR_flag->roll_next_bak += write_size;
        do  //把缓冲区中的数据写入到flash中，数据可能跨块边界，故循环
        {
            if((write_size-complete_len) > chip->block_size-block_offset)
                write_len = chip->block_size-block_offset;
            else
                write_len = write_size-complete_len;
            //写入数据到flash中，不校验
            chip->write_data_no_ecc(block_no,block_offset,
                                    roll_buf+complete_len,write_len);
            block_no++;
            block_offset = 0;       //下一块肯定从块首开始写
            complete_len += write_len;
        }while(complete_len < write_size);
        if(write_end_flag)
            __DFFSD_write_end_flag_bak(chip);
    }
    m_free(MDR_buf);
}

//----写入一个MDR记录----------------------------------------------------------
//功能: 保存一个MDR记录，本函数供flash driver使用，flash driver并不知道使用滚动
//      方式记录，本函数只是__DFFSD_write_MDR_roll_record函数的包装而已。
//参数: DBX_flash_tag，被操作的文件柜
//      type，写入类型，可能值cn_MDR_DDR_main、cn_MDR_DDR_bak、cn_MDR_PCRB
//      value，被写入的数值
//返回: 无
//注意: 不能对未格式化的文件柜调用本函数，这是一个内部函数，不检查目标文件柜
//      是否已经格式化
//-----------------------------------------------------------------------------
void __DFFSD_write_MDR_item(struct st_DBX_flash_tag *DBX_flash_tag,
                            uint32_t type,uint32_t value)
{
    uint32_t DBX_no;
    struct flash_chip *chip;
    if(DBX_flash_tag == NULL)
        return;
    if((type==cn_MDR_DDR_main)||(type==cn_MDR_DDR_bak)||(type==cn_MDR_PCRB))
    {
        DBX_no = DBX_flash_tag->DBX_no;
        chip = DBX_flash_tag->chip;
        __DFFSD_write_MDR_roll_record(chip,DBX_no,type,value);
    }
}

//----标记文件柜为已格式化-----------------------------------------------------
//功能: 在MDR_DBX表中标记一个文件柜为已经格式化，在MDR_main和MDR_bak中都要修改。
//参数：chip，被操作的芯片
//      DBX_no，文件柜号
//返回: 无
//-----------------------------------------------------------------------------
void __DFFSD_set_MDR_DBX_formatted(struct flash_chip *chip,uint32_t DBX_no)
{
    uint8_t *MDR_buf;
    struct MDR_DBX_mirror *MDR_DBX;

    MDR_buf = (uint8_t*)m_malloc(cn_MDR_size, 0);
    if(MDR_buf == NULL)
        return ;
    //计算被写的文件柜的MDR_DBX记录的地址
    MDR_DBX = (struct MDR_DBX_mirror *)(MDR_buf +
                        cn_MDR_CDR_size + DBX_no * cn_MDR_DBX_size);
    __DFFSD_read_MDR_bak(chip,MDR_buf);
    strcpy(MDR_DBX->DBX_format_flag,"ready");
    __DFFSD_fill_ECC_MDR_DBX((uint8_t *)MDR_DBX);
    __DFFSD_erase_MDR_bak(chip);
    //最后4字节是结束标志AA5555AA，故减4
    __DFFSD_write_MDR_bak(chip,MDR_buf,cn_MDR_size-4);
    __DFFSD_write_end_flag_bak(chip);

    __DFFSD_read_MDR_main(chip,MDR_buf);
    strcpy(MDR_DBX->DBX_format_flag,"ready");
    __DFFSD_fill_ECC_MDR_DBX((uint8_t *)MDR_DBX);
    __DFFSD_erase_MDR_main(chip);
    //最后4字节是结束标志AA5555AA，故减4
    __DFFSD_write_MDR_main(chip,MDR_buf,cn_MDR_size-4);
    __DFFSD_write_end_flag_main(chip);
    m_free(MDR_buf);
    return;
}
//----擦除MDR_main-------------------------------------------------------------
//功能: 擦除MDR_main，MDR_main的起始块默认为0
//参数：chip，被操作的芯片
//返回: 无
//-----------------------------------------------------------------------------
void __DFFSD_erase_MDR_main(struct flash_chip *chip)
{
    uint32_t blocks,loop;
    blocks = (cn_MDR_size + chip->block_size -1) / chip->block_size;
    for(loop = 0; loop < blocks; loop++)
        chip->erase_block(loop);
}

//----擦除MDR_bak-------------------------------------------------------------
//功能: 擦除MDR_bak，MDR_bak的起始块默认为0
//参数：chip，被操作的芯片
//返回: 无
//-----------------------------------------------------------------------------
void __DFFSD_erase_MDR_bak(struct flash_chip *chip)
{
    uint32_t blocks,loop;
    blocks = (cn_MDR_size + chip->block_size -1) / chip->block_size;
    for(loop = 0; loop < blocks; loop++)
        chip->erase_block(loop + chip->MDR.MDR_bak_block);
}

#endif //#if ((cfg_djyfs == 1) && (cn_run_mode != cn_mode_tiny) && (cfg_djyfs_flash == 1))
