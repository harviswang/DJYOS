//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//��Ȩ���� (C), 2004-2009,   lst.
//����ģ��:flash�ļ�ϵͳ
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����:flash�ļ�ϵͳ��MDR����
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
#include "ddr.h"
#include "mdr.h"
#include <string.h>
//----��ʼ��flashоƬ---------------------------------------------------------
//���ܣ���оƬ���ΪdjyosfsоƬ�������������ļ���
//����: chip����������оƬ
//      reserves������������
//����: true=��ʼ���ɹ���false=ʧ��
//---------------------------------------------------------------------------
void DFFSD_init_MDR(struct flash_chip *chip,uint32_t reserves)
{
    uint8_t MDR_CDR_buffer[cn_MDR_CDR_size];
    struct MDR_CDR_mirror *MDR_CDR = (struct MDR_CDR_mirror *)MDR_CDR_buffer;
    uint32_t valid_blocks = 1;  //�׿�϶��ǿ��õģ��ʳ�ֵ=1
    uint32_t i,MDR_bak_block = 0;
    //ע�⣬nandflashֻ���׿��Ǳ�֤��Ч�ģ���MDR��ߴ粻�ܳ����׿飬�ⲻ�����⣬
    //ĿǰΪֹnandflash��block_sizeû��С��4K�ġ�
    //��norflash��û��������ƣ�norflash��֤��Ƭ���Ǻõġ�
    for(i = 1; i < chip->block_sum; i++)
    {    //��һ���ÿ���ΪMDR���ÿ�
        if(chip->check_block(i))
        {
            valid_blocks++;
        }
    }
    MDR_bak_block = __DFFSD_probe_MDR_bak(chip);
    chip->valid_sum = valid_blocks;
    chip->rev_blocks = reserves;    //����������
    //��������ʼ���
    chip->rev_start = MDR_bak_block
                      +(cn_MDR_size + chip->block_size -1) / chip->block_size;
    chip->DBX_start = chip->rev_start + reserves;   //�ļ�����ʼ���
    if(valid_blocks <= chip->DBX_start)
        return;     //��Ч����̫��
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

//----��MDR_bak��-------------------------------------------------------------
//����: ��MDR�������������
//����: chip��Ŀ��оƬ��chip->bak_start�����Ѿ���ʼ��
//      buf�������������ݵĻ�����
//����: true=��ȷ��ȡ��false=����
//-----------------------------------------------------------------------------
bool_t __DFFSD_read_MDR_bak(struct flash_chip *chip,uint8_t *buf)
{
    uint32_t complete_len=0,read_len;
    uint32_t block_no = chip->MDR.MDR_bak_block;
    do  //��ѭ�����ڶ�ȡ����MDR_main����������MDR_size���ܴ���1�飬��ѭ����
    {
        if((cn_MDR_size-complete_len) > chip->block_size)
            read_len = chip->block_size;
        else
            read_len = cn_MDR_size-complete_len;
        //MDR�������¼����У�飬������У�飬��ÿ�����Ӽ�¼��Ҫ��У���룬������
        chip->read_data_no_ecc(block_no,0,buf+complete_len,read_len);
        block_no++;
        complete_len += read_len;
    }while(complete_len < cn_MDR_size);
    return true;
}

//----��MDR_main��-------------------------------------------------------------
//����: ��MDR�������������
//����: chip��Ŀ��оƬ
//      buf�������������ݵĻ�����
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_read_MDR_main(struct flash_chip *chip,uint8_t *buf)
{
    uint32_t complete_len=0,read_len;
    uint32_t block_no = 0;      //MDR_main��0��ʼ�洢
    do  //��ѭ�����ڶ�ȡ����MDR_main����������MDR_size���ܴ���1�飬��ѭ����
    {
        if((cn_MDR_size-complete_len) > chip->block_size)
            read_len = chip->block_size;
        else
            read_len = cn_MDR_size-complete_len;
        //MDR�������¼����У�飬������У�飬��ÿ�����Ӽ�¼��Ҫ��У���룬������
        chip->read_data_no_ecc(block_no,0,buf+complete_len,read_len);
        block_no++;
        complete_len += read_len;
    }while(complete_len < cn_MDR_size);
    return;
}

//----����һ���洢����---------------------------------------------------------
//����: �������ļ���ʱ�����ô˺����Ӵ洢�����з���洢����
//����: chip��Ŀ��оƬ
//      size��������Ŀ�����
//      result�����ط�����
//����: true=����ɹ������������result�У�false = �����ʧ��
////---------------------------------------------------------------------------
bool_t __DFFSD_allocate_DBX(struct flash_chip *chip,
                            uint32_t size,uint32_t *result)
{
    //uint8_t MDR_buf[cn_MDR_size];
    //�ļ�����ʼ�����飬���ڲ�ѯ���пռ䣬+2��Ϊ�������ܴ��ڵ���β������
    uint32_t block_start[cn_DBX_sum_max+2];
    //�ļ�����������飬���ڲ�ѯ���пռ䣬+2��Ϊ�������ܴ��ڵ���β������
    uint32_t block_end[cn_DBX_sum_max+2];
    struct MDR_DBX_mirror *MDR_DBX;
    uint32_t loop,loop1=0,temp;
    uint8_t *MDR_buf;

    MDR_buf = (uint8_t*)m_malloc(cn_MDR_size, 0);
    if(MDR_buf == NULL)
        return false;
    __DFFSD_read_MDR_main(chip,MDR_buf); //����MDR��������(ԭ��������������)
    block_start[0] = chip->DBX_start;
    block_end[0] = chip->DBX_start;
    //MDR_DBXָ��ָ��MDR�������е�һ��DBX�������ڼ���DBX���Ƿ����
    MDR_DBX = (struct MDR_DBX_mirror *)(MDR_buf+cn_MDR_CDR_size);

    for(loop = 1; loop < cn_DBX_sum_max+1; loop++)
    {//�������Ѿ��������ļ������ʼ��źͽ�����ţ����浽block_start��block_end
    //�������������Ԫ�ر��ļ������������������ڱ����ļ�����β���������
        if(chip->MDR.DBX_created[loop-1])
        {//���Ѿ��������ļ��񣬴�����MDR_DBX�����ж�����ʼ�ͽ������
            
            //block_start[loop] = __pick_little_32bit(MDR_DBX->DBX_start_block,0);
        	block_start[loop] = block_end[loop-1]
                            + __pick_little_32bit(MDR_DBX->DBX_blocks_sum,0);
            //������ţ��ÿ��Ѿ������ڱ��ļ����ˡ�
            block_end[loop] = block_end[loop-1]
                            + __pick_little_32bit(MDR_DBX->DBX_blocks_sum,0);
        }else
        {//Ϊ�������ļ�����ʼ�ͽ�����ž���ʼ��Ϊ������ֵ��ѽ������ļ����
        //��ʼ�ͽ�����ţ���Ч�ڱ��ļ����ǿյġ�
            block_start[loop] = block_end[loop-1];
            block_end[loop] = block_end[loop-1];
        }
        MDR_DBX++;  //������ָ����һ���ļ����MDR_DBX
    }
    
    block_start[cn_DBX_sum_max+1] = chip->block_sum;//оƬβ�������������ʼ���
    block_end[cn_DBX_sum_max+1] = chip->block_sum;  //оƬβ����������Ľ������
    for(loop = 0; loop <cn_DBX_sum_max+1; loop++)
    {//��ð�ݷ��Ը��ļ������ʼ����Ŷ�(���ǿ��п�ߴ�)��оƬ����ʹ�ú���ܷ���
    //�������ļ�����ռ�õĴ洢��λ�ò�������DBX��MDR�е�˳����ͬ��ֻ�а����ַ
    //����󣬲�����Чȷ�������ÿռ�ĳߴ硣
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
    //��һ������Ҫ����������пռ�
    for(loop = 0; loop < cn_DBX_sum_max+1; loop++)
    {

    	if(!chip->MDR.DBX_created[loop])
        {
        	if(block_start[loop+1] - block_end[loop] >= size)
            {
                *result = block_start[loop];  //�ҵ�����Ҫ��Ŀռ�
                    break;
               }
        }
    }
    m_free(MDR_buf);
    if(loop == cn_DBX_sum_max +1)
        return false;   //û���ҵ�����Ҫ���������������
    else
        return true;
}

//----д��һ��MDR_DBX��--------------------------------------------------------
//����: ��һ��MDR_DBX��д��flash�У���ͬʱд��MDR_main��MDR_bak��
//����: chip��Ŀ��оƬ
//      DBX_no���ļ����
//      MDR_DBX_buf���Ѿ����õĻ�����
//����: true=�ɹ���false=ʧ��
//ע������д���Ƿ���ȷ��У�鹦��----db
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
    //��MDR_DBX��MDR���е�ƫ����(�ֽ�)
    block_offset = cn_MDR_CDR_size + DBX_no * cn_MDR_DBX_size;
    block_no = block_offset / chip->block_size;//��MDR_DBX��MDR���еĿ��
    block_offset = block_offset % chip->block_size; //����ƫ����
    //���MDR_main�Ƿ���Ҫ����
    if(chip->query_block_ready_no_ecc(
                            block_no,block_offset,MDR_DBX_buf,cn_MDR_DBX_size))
    {//MDR_main�и�DBX���������
        __DFFSD_write_MDR_DBX_main(chip,MDR_DBX_buf,DBX_no);  //ֱ��д��MDR_main
        //�������MDR_DBXд�뵽MDR_bak��
        block_no += chip->MDR.MDR_bak_block;    //ȡMDR_bak���
        //���MDR_bak�Ƿ���Ҫ����
        if(chip->query_block_ready_no_ecc(
                            block_no,block_offset,MDR_DBX_buf,cn_MDR_DBX_size))
        {//MDR_bak�и�DBX���������
            __DFFSD_write_MDR_DBX_bak(chip,MDR_DBX_buf,DBX_no);
        }else
        {//MDR_bak�и�DBX����Ҫ����
            __DFFSD_read_MDR_main(chip,MDR_buf); //������MDR��
            __DFFSD_uasm_DBX(chip,&MDR_record,MDR_buf);
            //�ӻ�������ȡ������������Ч���ݣ����浽MDR_record�У������ļ���
            //��δ��ɳ�ʼ������������û�и��ļ�������ݡ�
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
    {//MDR_main�и�DBX����Ҫ����
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
//----����һ���ļ���----------------------------------------------------------
//����: ��ָ��оƬ������һ���ļ���,���ļ���ֻ�ǵǼ���оƬ�ϣ���δ����������ϵͳ��
//      ��Ҫ���أ�������������ϵͳ��
//����: chip����������оƬ
//      size�������ӵ��ļ���ߴ�(�ÿ�����ʾ)
//      name�������ӵ��ļ�������
//      nand_ecc�����ļ����Ƿ���ҪeccУ��
//����: true = �ɹ���false = ʧ�ܣ�ʧ��ԭ��һ���Ǵ洢�ռ䲻����
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
    name_len = strnlen(name,cn_flash_DBX_name_limit);  //����ַ������ȣ�
    if(name_len == 0)               //�ļ���������Ϊ0���Ƿ�
        return false;
    for(loop = 0; loop < cn_DBX_sum_max; loop++)//����Ƿ񻹿��Խ����µ��ļ���
    {
        if(!chip->MDR.DBX_created[loop])
        {
            DBX_no = loop;      //��loop���ļ���δ��������MDR_DBX�����ǿյ�
            break;              //Ϊ���ļ����ҵ�MDR_DBX����˳�ѭ��
        }
    }
    if(loop == cn_DBX_sum_max)
        return false;   //MDR���Ѿ�û�п��е�DBX��

    //���³�ʼ�����ļ����MDR_DBX����
    MDR_DBX = (struct MDR_DBX_mirror*)MDR_DBX_buf;

    if(name_len != cn_limit_uint32)     //copy���ļ�����
    {
        strcpy(MDR_DBX->DBX_name,name);
    }else
    {
        memcpy(MDR_DBX->DBX_name,name,cn_flash_DBX_name_limit);
        MDR_DBX->DBX_name[cn_flash_DBX_name_limit] = '\0';
    }
    fs_rsc_node = rsc_search(rsc_get_root(),"dev\\fs"); //ȡ�ļ�ϵͳ�豸����Դ���ָ��
    //���ļ����豸�в���ΪMDR_DBX->DBX_name�Ľ��
    if(rsc_search_son(fs_rsc_node,MDR_DBX->DBX_name))
    {
        djy_error_login(enum_fs_DBX_name_repeat,"�ļ�������");
        printf_ng("�ļ�������");
        return false;
    }
    if( ! __DFFSD_allocate_DBX(chip,size,&DBX_start))   //����洢�ռ�
        return false;
    strcpy(MDR_DBX->DBX_format_flag,"unready"); //��ʽ����־
    __fill_little_32bit(MDR_DBX->DBX_start_block,0,DBX_start);  //��ʼ���
    __fill_little_32bit(MDR_DBX->DBX_blocks_sum,0,size);    //�ļ���ߴ�(����)
    __DFFSD_fill_ECC_MDR_DBX((uint8_t *)MDR_DBX);                 //���ECC��
    if( ! __DFFSD_write_MDR_DBX(chip,DBX_no,MDR_DBX_buf))
        return false;

    chip->MDR.DBX_created[DBX_no] = true;
    //���ļ�ϵͳ�豸�����ֽӿ�
    if((fs_handle_right = dev_open_right("fs",0)) == NULL)
        return false;

    //�����ļ����豸ר�����ݽṹ�ڴ棬�ýṹ��Ƕ�뵽���豸�ṹ��
    DBX_device_tag = (struct st_DBX_device_tag*)
                m_malloc_gbl(sizeof(struct st_DBX_device_tag),0);
    if(DBX_device_tag == NULL)
        goto goto_exit_install_chip;
    //����flash�ļ�ϵͳר�����ݽṹ�ڴ棬�ýṹ��Ƕ�뵽�ļ����豸��
    DBX_flash_tag = (struct st_DBX_flash_tag*)
                m_malloc_gbl(sizeof(struct st_DBX_flash_tag),0);
    if(DBX_flash_tag == NULL)
        goto goto_exit_install_chip;
    DBX_flash_tag->ART_position = NULL;
    DBX_flash_tag->ART_block_no = NULL;
    DBX_flash_tag->ART_times = NULL;
    DBX_flash_tag->writed_DDR_main = NULL;
    DBX_flash_tag->writed_DDR_bak = NULL;
    DBX_flash_tag->chip = chip;         //flash�ļ���洢������оƬ
    DBX_flash_tag->DBX_no = DBX_no;     //���ļ�����оƬ�е����
    DBX_flash_tag->PCRB_no = cn_limit_uint32;   //δ��ʽ��
    DBX_flash_tag->DDR_main = cn_limit_uint32;  //δ��ʽ��
    DBX_flash_tag->DDR_bak = cn_limit_uint32;   //δ��ʽ��
    DBX_flash_tag->start_block = DBX_start;
    DBX_flash_tag->block_sum = size;

    table_size = DBX_flash_tag->block_sum * (4+4+2);
    //Ϊĥ��λ�ñ�����ڴ�
    DBX_flash_tag->ART_position = (uint32_t*)m_malloc_gbl(table_size,0);
    if(DBX_flash_tag->ART_position == NULL)
        goto goto_exit_install_chip;
    //Ϊĥ���ű�����ڴ�
    DBX_flash_tag->ART_block_no = DBX_flash_tag->ART_position
                                  + DBX_flash_tag->block_sum;
    //Ϊĥ�����������ڴ�
    DBX_flash_tag->ART_times = (uint16_t*)(DBX_flash_tag->ART_block_no
                                  + DBX_flash_tag->block_sum);

    //����DDR��ߴ�
    DBX_flash_tag->DDR_size = __DFFSD_calculate_DDR_size(DBX_flash_tag);
    //1��DDR��ռ�õĿ���
    DDR_blocks = (DBX_flash_tag->DDR_size
                        +chip->block_size-1)/chip->block_size;
    //��������DBL�������ڴ�
    DBX_flash_tag->DDR_DBL =(uint32_t*)m_malloc_gbl(DDR_blocks*8,0);
    if(DBX_flash_tag->DDR_DBL == NULL)
        goto goto_exit_install_chip;
    //����MAT�������ڴ�
    DBX_flash_tag->DDR_MAT = (struct MAT_table *)m_malloc_gbl(
                        DBX_flash_tag->block_sum*sizeof(struct MAT_table),0);
    if(DBX_flash_tag->DDR_MAT == NULL)
        goto goto_exit_install_chip;
    //����д���־
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
    //��亯��ָ����ļ�����
    __DFFSD_fill_DBX_device_tag(DBX_device_tag,name);
    //�ļ����豸�Ĵ洢ý��ָ��flash�ļ���洢��
    DBX_device_tag->DBX_medium_tag = (ptu32_t)DBX_flash_tag;
    DBX_device_tag->formatted =false;
    semp_dbx.left = chip->left_semaphore;   //оƬ�������ź���
    semp_dbx.right = chip->right_semaphore; //оƬ�������ź���
    if(dev_ctrl_right(fs_handle_right,enum_fs_add_DBX,
                        (ptu32_t)DBX_device_tag,(ptu32_t)&semp_dbx)
                        != enum_fs_no_error)
    {
        goto goto_exit_install_chip;
    }
    //������������
    dev_close_right(fs_handle_right);     //�ر��ļ�ϵͳ�豸
    return true;
goto_exit_install_chip:
    dev_close_right(fs_handle_right);     //�ر��ļ�ϵͳ�豸
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
    return false;      //�ļ�����뵽�豸��ʧ��
}

//----����MDR_bak�Ŀ��--------------------------------------------------------
//����: �ڶ�MDR_mainʧ�ܣ��޷����л�ȡMDR_bak�Ŀ��ʱ������оƬ�Ŀ�ߴ����
//      MDR_bak���ܵĴ��λ�á����ñ�����ȷ�����ʱ��Ӧ�ôӸÿ�ſ�ʼ��������
//      5�飬ֱ���ҵ��Ϸ���MDR_bak��Ϊֹ�����5�鶼δ���ҵ��Ϸ���MDR_bak������Ϊ
//      ��flashû�а���djyosfs��ʼ������
//����: chip��Ŀ��оƬ
//����: ��ţ�0��ʾû���ҵ�
//-----------------------------------------------------------------------------
uint32_t __DFFSD_probe_MDR_bak(struct flash_chip *chip)
{
    return (cn_MDR_size+chip->block_size-1) / chip->block_size;
}
//----У��MDR_CDR--------------------------------------------------------------
//����: У��MDR_CDR�������������ݣ�������ܸ��������ش���
//����: buf��������δУ���MDR_CDR���ݵĻ�����
//����: true=�޴���߱�������false=�д��Ҳ��ܸ�����
//-----------------------------------------------------------------------------
bool_t __DFFSD_verify_MDR_CDR(uint8_t *buf)
{
    return true;
}
//----У��MDR_DBX--------------------------------------------------------------
//����: У��MDR_DBX�������������ݣ�������ܸ��������ش���
//����: buf��������δУ���MDR_DBX���ݵĻ�����
//����: true=�޴���߱�������false=�д��Ҳ��ܸ�����
//-----------------------------------------------------------------------------
bool_t __DFFSD_verify_MDR_DBX(uint8_t *buf)
{
    return true;
}

//----У��MDR������־----------------------------------------------------------
//����: 1.У��MDR������4���ֽ��Ƿ�AA5555AA,���ֻ����һλ������
//      2.У��CDR��DBX���ECC
//����: buf���ֽڻ�����
//����: true=У����ȷ��false=����
//-----------------------------------------------------------------------------
//�ر�ע�⣬��������/��С������/��СѰַ��Ԫ����8λ���⣬��Щ����ʹ�ò��ܰ�
//AA5555AA���32λ�������Ƚ�
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

//----MDR������¼ECCУ��-------------------------------------------------------
//����: MDR�й����洢�ļ�¼����ECCУ�飬���������ݡ�
//����: buf����¼ָ��
//����: 0=�޴�1=�д���������2=�޷�����
//-----------------------------------------------------------------------------
uint32_t __DFFSD_verify_MDR_record(uint8_t *buf)
{
    return 0;
}

//----ɨ������洢��-----------------------------------------------------------
//����: ɨ������洢����������ȡ�Ѿ���ʽ�����ļ����DDR_main��š�DDR_bak��š�
//      PCRB��š�
//����: chip����������оƬ
//      MDR_buf��������
//      MDR_record������ɨ�赽�����ݵ�ָ��
//      end_offset�����һ����Ч��¼λ��
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_uasm_roll(struct flash_chip *chip,uint8_t *MDR_buf,
                       struct MDR_temp_record *MDR_record,uint32_t end_offset)
{
    uint8_t *record;
    uint32_t loop,loop3,item_sum=0;
    bool_t got_DDR_main[cn_DBX_sum_max];//���ļ���DDR_main����Ƿ��ҵ���־
    bool_t got_DDR_bak[cn_DBX_sum_max]; //���ļ���DDR_bak����Ƿ��ҵ���־
    bool_t got_PCRB[cn_DBX_sum_max];    //���ļ���PCRB����Ƿ��ҵ���־

    for(loop3 = 0; loop3 < cn_DBX_sum_max; loop3++)
    {
        got_DDR_main[loop3] = false;
        got_DDR_bak[loop3] = false;
        got_PCRB[loop3] = false;
        if(MDR_record->formatted[loop3])
        {
            item_sum +=3;   //ÿ���Ѿ���ʽ�����ļ�����3��������¼
        }
    }
    //ѭ��ɨ�����й����洢������Ч��Ŀ��
    for(loop = end_offset-8;loop >= cn_roll_offset; loop-=8)
    {
        record = MDR_buf + loop;
        for(loop3 = 0; loop3 < cn_DBX_sum_max; loop3++)
        {
            if(! MDR_record->formatted)     //�ļ���δ��ʽ��
                continue;
            __DFFSD_verify_MDR_record(record); //У��ü�¼
            if(!got_DDR_main[loop3])        //DDR_main������¼��δ�ҵ�
            {
                //��鱾��Ŀ�ǲ��ǵ�loop���ļ����DDR_main���
                if((record[0] == cn_MDR_DDR_main) && (record[1] == loop3))
                {
                    item_sum--;                 //�ҵ���ʣ����Ŀ������
                    got_DDR_main[loop3] = true;  //��Ǹ�DDR_main��������ҵ�
                    //�����д�뷵�صĽṹָ����
                    MDR_record->DDR_main_block_no[loop3]
                            = __pick_little_32bit(&record[2],0);
                    break;
                }
            }
            if(!got_DDR_bak[loop3])         //DDR_bak������¼��δ�ҵ�
            {
                //��鱾��Ŀ�ǲ��ǵ�loop���ļ����DDR_bak���
                if((record[0] == cn_MDR_DDR_bak) && (record[1] == loop3))
                {
                    item_sum--;                 //�ҵ���ʣ����Ŀ������
                    got_DDR_bak[loop3] = true;  //��Ǹ�DDR_bak��������ҵ�
                    //�����д�뷵�صĽṹָ����
                    MDR_record->DDR_bak_block_no[loop3]
                                    = __pick_little_32bit(&record[2],0);
                    break;
                }
            }
            if(!got_PCRB[loop3])                //PCRB������¼��δ�ҵ�
            {
                //��鱾��Ŀ�ǲ��ǵ�loop���ļ����PCRB���
                if((record[0] == cn_MDR_PCRB) && (record[1] == loop3))
                {
                    item_sum--;                 //�ҵ���ʣ����Ŀ������
                    got_PCRB[loop3] = true;  //��Ǹ�PCRB��������ҵ�
                    //�����д�뷵�صĽṹָ����
                    MDR_record->PCRB_block_no[loop3] =
                                __pick_little_32bit(&record[2],0);
                    break;
                }
            }
        }
        if(item_sum == 0)   //ʣ����ĿΪ0��������Ҫ�ҵ���Ŀ�Ѿ�ȫ���ҵ�
            break;
    }
}

//----���MDR_bak��������------------------------------------------------------
//����: ���MDR_bak�Ƿ��������Ƿ����ϴε���ʱ����д������²������������Ǽ��
//      β����AA5555AA���Լ����һ��������¼��У����
//����: chip��Ŀ��оƬ
//����: true=������false=������
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

//----������������λ��-------------------------------------------------------
//����: �������洢����һ�����м�¼��ƫ���������۰���ҷ���
//����: chip��Ŀ��оƬ
//      MDR_buf��MDR������
//����: ƫ����
//-----------------------------------------------------------------------------
uint32_t __DFFSD_find_roll_next(struct flash_chip *chip,uint8_t *MDR_buf)
{
    uint32_t busy_endian,free_endian,middle,result;
    uint32_t block_offset;
    busy_endian = cn_roll_offset/8;
    free_endian = cn_MDR_size /8 -1;   //���һ�����������Ա��
    while(1)
    {
        middle = (busy_endian+free_endian)/2; //�����м���
        block_offset = 8*middle;
        //���Ե�block_no��ƫ��block_offset��ʼ��8�ֽ��Ƿ������δʹ�ù���
        //��Ȼ������֪��flash���������Ƿ����0xff���ж��Ƿ��������datasheet
        //�ϲ�û������˵���ѱ����첻�����һ�����������ģ��ʰ��жϽ���оƬ
        //driver�ſ�ȷ������ֲ�ԡ�
        if(chip->query_ready_with_data(NULL,MDR_buf+block_offset,8))
        {//��middle��Ԫ�ǿ��е�
            if(middle == busy_endian)
            {
                result = middle*8;
                break;
            }else
                free_endian = middle;
        }else
        {//��middle��Ԫ���Ѿ�ʹ���˵�
            if(free_endian == middle+1)
            {
                result = free_endian*8;
                break;
            }else
                busy_endian = middle;
        }
    }//���ҹ���λ�ý���
    return result;
}

//----����DBX��----------------------------------------------------------------
//����: ɨ��MDR��������3��DBX���ҳ��Ѿ��������ļ��񲢶����ļ������
//����: chip��Ŀ��оƬ��Ҳ���ڱ���ɨ����
//      MDR_record�����ڱ�������ָ��
//      MDR_buf��MDR������
//����: �Ѿ��������ļ�������
//-----------------------------------------------------------------------------
ufast_t __DFFSD_uasm_DBX(struct flash_chip *chip,
                         struct MDR_temp_record *MDR_record,uint8_t *MDR_buf)
{
    struct MDR_DBX_mirror *MDR_DBX;
    uint32_t loop,name_len;
    ufast_t DBX_sum=0;    //�Ѿ��������ļ�������
    bool_t formatted,unformatted;
    //ȡMDR_DBX��¼����ʼ��ַ
    MDR_DBX = (struct MDR_DBX_mirror *)(MDR_buf + cn_MDR_CDR_size);
    for(loop = 0; loop < cn_DBX_sum_max; loop++)
    {//��ѭ���� 1���ж��ļ��񴴽���
     //         2�����ļ�������
     //         3�������ļ������ʼ��źͿ��������ܳ�ʼ����
        formatted = (strcmp(MDR_DBX->DBX_format_flag,"ready")==0);
        unformatted = (strcmp(MDR_DBX->DBX_format_flag,"unready")==0);
        if(formatted || unformatted)    //���ܸ�ʽ���񣬶���ʾ�ļ����Ѿ�������
        {
            DBX_sum++;                              //�ļ�����������
            chip->MDR.DBX_created[loop] = true;     //�ļ��񴴽���־��Ϊtrue
            MDR_record->formatted[loop] = formatted;    //���ʽ����־
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
            //�ļ�����ʼ���
            MDR_record->start_blocks[loop] =
                            __pick_little_32bit(MDR_DBX->DBX_start_block,0);
            //�ļ���ߴ�(�Կ�����ʾ)
            MDR_record->blocks_sum[loop] =
                            __pick_little_32bit(MDR_DBX->DBX_blocks_sum,0);

        }else   //MDR_DBX�Ȳ���"ready"Ҳ����"unready"��ͷ�������ļ���δ����
        {
            chip->MDR.DBX_created[loop] = false;    //�ļ��񴴽���־��Ϊfalse
            MDR_record->formatted[loop] = false;    //���ʽ����־
        }
        //ȡ��һ���ļ���MDR_DBX��ƫ�Ƶ�ַ
        MDR_DBX = (struct MDR_DBX_mirror *)((uint32_t)MDR_DBX +cn_MDR_DBX_size);
    }
    return DBX_sum;
}

//----��ʼ����MDR��------------------------------------------------------------
//����: оƬ��ʼ��ʱ������MDR����ȡ��Ч���ݡ��������MDR_main��MDR_bak������
//      һ���д���ˢ��֮
//����: chip����������оƬ��ͬʱ���ڷ��ؽ��
//      MDR_record��������Ч���ݵ�ָ��
//����: ɨ��MDR�Ľ��������chip��MDR_record�����ṹָ����
//      true=оƬ�Ѿ���djyosfs��ʼ����false=оƬδ��djyosfs��ʼ��
//-----------------------------------------------------------------------------
//��ע: ���Ա�����ʱ�������rvd���������ʷǷ���ַ����windows�رգ���ARM��������
//      ���������´�rvd���������Դ��ϴ��˳��ĵط��������С�
bool_t __DFFSD_read_MDR(struct flash_chip *chip,struct MDR_temp_record *MDR_record)
{
    uint8_t *MDR_buf;
    uint32_t roll_size;
    uint8_t roll_buf[cn_DBX_sum_max*3*8];
    bool_t MDR_main_valid,MDR_bak_valid;
    struct MDR_CDR_mirror *MDR_CDR;
    ufast_t DBX_sum = 0;    //�Ѿ��������ļ�������
    

    MDR_buf = (uint8_t*)m_malloc(cn_MDR_size, 0);
    if(MDR_buf == NULL)
        return false;    
    __DFFSD_read_MDR_main(chip,MDR_buf);
    if(!__DFFSD_verify_end_flag(&MDR_buf[cn_MDR_size-4]))  //���MDR_main�Ƿ�����
    {//MDR_main�д��󣬶�ȡMDR_bak
        MDR_main_valid = false;
        chip->MDR.MDR_bak_block = __DFFSD_probe_MDR_bak(chip);   //̽��MDR_bak�Ĵ洢λ��
    }else
    {
        MDR_main_valid = true;
        MDR_CDR = (struct MDR_CDR_mirror *)MDR_buf;
        if(strcmp(MDR_CDR->chip_init_flag,"djyosfs") != 0)
        {
            m_free(MDR_buf);
            return false;      //�洢����δ��ʼ��
        }
        //�����CDR������ݶ�ȡ��chip��Դ��
        chip->MDR.MDR_bak_block = __pick_little_32bit(MDR_CDR->chip_MDR_bak,0);
        //��ȡ���������������ʼ��ţ����������������������п����л���
        chip->rev_start = chip->MDR.MDR_bak_block
                          + (cn_MDR_size+chip->block_size -1)/chip->block_size;
        chip->rev_blocks = __pick_little_32bit(MDR_CDR->chip_reserve,0);
        //���������ļ������ʼ��ţ�
        chip->DBX_start = chip->rev_start + chip->rev_blocks;
        DBX_sum = __DFFSD_uasm_DBX(chip,MDR_record,MDR_buf);
        if(DBX_sum != 0)
        {
            //�ҵ������洢������һ���洢λ��
            chip->MDR.roll_next_main = __DFFSD_find_roll_next(chip,MDR_buf);
            //ɨ�貢��ȡ������¼�е���Ч��¼(��ÿ���¼�����һ����¼)
            __DFFSD_uasm_roll(chip,MDR_buf,MDR_record,
                              chip->MDR.roll_next_main);
        }else
            chip->MDR.roll_next_main = cn_roll_offset;
    }

    __DFFSD_read_MDR_bak(chip,MDR_buf);     //��MDR_bak����������
    if(!__DFFSD_verify_end_flag(&MDR_buf[cn_MDR_size-4]))  //���MDR_bak�Ƿ�����
    {
        MDR_bak_valid = false;  //MDR_bak�д���
    }else
    {
        MDR_bak_valid = true;
        if(MDR_main_valid)      //���MDR_main��Ч����ֻ��ȡ��һ�������洢λ��
        {
            if(DBX_sum != 0)
                chip->MDR.roll_next_bak = __DFFSD_find_roll_next(chip,MDR_buf);
            else
                chip->MDR.roll_next_bak = cn_roll_offset;
        }else   //���MDR_main��Ч���������ļ�����Ϣ��MDR_bak��ȡ
        {
            MDR_CDR = (struct MDR_CDR_mirror *)MDR_buf;
            if(strcmp(MDR_CDR->chip_init_flag,"djyosfs")!=0)
            {
                m_free(MDR_buf);
                return false;      //�洢����δ��ʼ��
            }
            //�����CDR������ݶ�ȡ��chip��Դ��
            //��ȡ���������������ʼ��ţ����������������������п����л���
            chip->rev_start = chip->MDR.MDR_bak_block
                          +(cn_MDR_size+chip->block_size -1)/chip->block_size;
            chip->rev_blocks = __pick_little_32bit(MDR_CDR->chip_reserve,0);
            //���������ļ������ʼ��ţ�
            chip->DBX_start = chip->rev_start + chip->rev_blocks;
            DBX_sum = __DFFSD_uasm_DBX(chip,MDR_record,MDR_buf);
            if(DBX_sum != 0)
            {
                chip->MDR.roll_next_bak = __DFFSD_find_roll_next(chip,MDR_buf);
                //ɨ�貢��ȡ������¼�е���Ч��¼(��ÿ���¼�����һ����¼)
                __DFFSD_uasm_roll(chip,MDR_buf,MDR_record,
                                  chip->MDR.roll_next_bak);
            }
        }
    }
    if((!MDR_main_valid) && (!MDR_bak_valid))
    {
        m_free(MDR_buf);    //���MDR_main��MDR_bak������˵��δ��ʼ���洢��
        return false;
    }
    else
    {
        roll_size = __DFFSD_asm_roll_buf(chip,MDR_record,roll_buf);
        if(!MDR_main_valid) //MDR_main������MDR_bak�ָ�֮
        {
            __DFFSD_erase_MDR_main(chip);
            memcpy(MDR_buf+cn_roll_offset,roll_buf,roll_size);
            __DFFSD_write_MDR_main(chip,MDR_buf,cn_roll_offset + roll_size);
            __DFFSD_write_end_flag_main(chip);
        }else if(!MDR_bak_valid) //MDR_bak������MDR_main�ָ�֮
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

//----��������ĿУ����-------------------------------------------------------
//����: ��һ��MDR�Ĺ����洢��¼����У���롣
//����: buf����¼ָ��
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_fill_ECC_MDR_record(uint8_t *buf)
{
}

//----���MDR_CDRУ����--------------------------------------------------------
//����: ��һ��MDR_CDR����У���롣
//����: buf��CDR��¼ָ��
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_fill_ECC_MDR_CDR(uint8_t *buf)
{
}

//----���MDR_DBXУ����--------------------------------------------------------
//����: ��һ��MDR_DBX����У���롣
//����: buf��DBX��¼ָ��
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_fill_ECC_MDR_DBX(uint8_t *buf)
{
}

//----дMDR_main�Ľ������-----------------------------------------------------
//����: д��MDR_main�Ľ�����־����ʾMDR_main��������
//����: chip����������оƬ
//����: ��
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

//----дMDR_bak�Ľ������-----------------------------------------------------
//����: д��MDR_bak�Ľ�����־����ʾMDR_bak��������
//����: chip����������оƬ
//����: ��
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

//----дMDR_main--------------------------------------------------------------
//����: ��DDR_main��ͷ���ֿ�ʼ��д��ָ�����ȵ����ݡ�
//����: chip����������оƬ
//      MDR_buf��������ָ��
//      len,д�볤��
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_write_MDR_main(struct flash_chip *chip,uint8_t *MDR_buf,uint32_t len)
{
    uint32_t complete_len=0,write_len;
    uint32_t block_no;
    block_no = 0;   //MDR_mainĬ�ϴ�0�鿪ʼ

    do  //�ѻ������е�����д�뵽flash�У����ݿ��ܿ��߽磬��ѭ��
    {
        if((len - complete_len) > chip->block_size)
            write_len = chip->block_size;
        else
            write_len = len-complete_len;
        //д�����ݵ�flash�У���У��
        chip->write_data_no_ecc(block_no,0,MDR_buf+complete_len,write_len);
        block_no++;
        complete_len += write_len;
    }while(complete_len < len);
}

//----дMDR_bak--------------------------------------------------------------
//����: ��DDR_bak��ͷ���ֿ�ʼ��д��ָ�����ȵ����ݡ�
//����: chip����������оƬ
//      MDR_buf��������ָ��
//      len,д�볤��
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_write_MDR_bak(struct flash_chip *chip,uint8_t *MDR_buf,uint32_t len)
{
    uint32_t complete_len=0,write_len;
    uint32_t block_no;
    block_no = chip->MDR.MDR_bak_block;   //MDR_mainĬ�ϴ�0�鿪ʼ
    do  //�ѻ������е�����д�뵽flash�У����ݿ��ܿ��߽磬��ѭ��
    {
        if((len - complete_len) > chip->block_size)
            write_len = chip->block_size;
        else
            write_len = len-complete_len;
        //д�����ݵ�flash�У���У��
        chip->write_data_no_ecc(block_no,0,MDR_buf+complete_len,write_len);
        block_no++;
        complete_len += write_len;
    }while(complete_len < len);
}

//----дMDR_main��DBX����------------------------------------------------
//����: ��DBXд�뵽MDR_main�У����ô˺���ǰ��ȷ������DBX��¼�Ĳ����Ѿ�������.
//����: chip����������оƬ
//      MDR_buf��������ָ��
//      DBX_no���ļ�����
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_write_MDR_DBX_main(struct flash_chip *chip,uint8_t *MDR_DBX_buf,
                                                      uint32_t DBX_no)
{
    uint32_t complete_len=0,write_len,write_size;
    uint32_t block_no,block_offset;
    write_size = cn_MDR_DBX_size;
    block_no = (cn_MDR_CDR_size + DBX_no*cn_MDR_DBX_size) / chip->block_size ;
    block_offset = (cn_MDR_CDR_size + DBX_no*cn_MDR_DBX_size)% chip->block_size;
    do  //�ѻ������е�����д�뵽flash�У����ݿ��ܿ��߽磬��ѭ��
    {
        if((write_size-complete_len) > chip->block_size-block_offset)
            write_len = chip->block_size-block_offset;
        else
            write_len = write_size-complete_len;
        //д�����ݵ�flash�У���У��
        chip->write_data_no_ecc(block_no,block_offset,
                                MDR_DBX_buf+complete_len,write_len);
        block_no++;
        block_offset = 0;       //��һ��϶��ӿ��׿�ʼд
        complete_len += write_len;
    }while(complete_len < write_size);
}

//----дMDR_bak��DBX����------------------------------------------------
//����: ��DBXд�뵽MDR_bak�У����ô˺���ǰ��ȷ������DBX��¼�Ĳ����Ѿ�������.
//����: chip����������оƬ
//      MDR_buf��������ָ��
//      DBX_no���ļ�����
//����: ��
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
    do  //�ѻ������е�����д�뵽flash�У����ݿ��ܿ��߽磬��ѭ��
    {
        if((write_size-complete_len) > chip->block_size-block_offset)
            write_len = chip->block_size-block_offset;
        else
            write_len = write_size-complete_len;
        //д�����ݵ�flash�У���У��
        chip->write_data_no_ecc(block_no,block_offset,
                                MDR_DBX_buf+complete_len,write_len);
        block_no++;
        block_offset = 0;       //��һ��϶��ӿ��׿�ʼд
        complete_len += write_len;
    }while(complete_len < write_size);
}

//----дһ��MDR������¼--------------------------------------------------------
//����: MDR����д��һ��������¼��ֻ��MDR_main�вŻ��д˲�����MDR_bak�������й���
//      ��¼ͬʱд�룬�����е���д��Ĳ�����
//����: chip����������оƬ
//      DBX_no���ļ�����
//      type��д�����ͣ�����ֵcn_MDR_DDR_main��cn_MDR_DDR_bak��cn_MDR_PCRB
//      value����д�����ֵ
//����: ��
//ע��: ���ܶ�δ��ʽ�����ļ�����ñ�����������һ���ڲ������������Ŀ���ļ���
//      �Ƿ��Ѿ���ʽ��
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
    item_buf[0] = type;             //����¼����
    item_buf[1] = DBX_no;           //����ļ����
    //�����ļ����DDR_main���
    __fill_little_32bit(&item_buf[2],0,value);
    __DFFSD_fill_ECC_MDR_record(item_buf);//���У����
    if((chip->MDR.roll_next_main + 8) > cn_MDR_size-8)
    {//�ر�ע������cn_MDR_size<784,�˴�������ѭ��
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

//----��������洢��-----------------------------------------------------------
//����: �ӹ����洢��¼�����ܹ�ֱ�ӱ�����flash�е����ݣ��Ѹ�ʽ�����ļ���Ż���
//      ������¼��
//����: chip����������оƬ
//      MDR_record�������洢��¼
//      roll_buf�������洢��������
//����: roll_buf����Ч���ݳ���
//-----------------------------------------------------------------------------
uint32_t __DFFSD_asm_roll_buf(struct flash_chip *chip,
                          struct MDR_temp_record *MDR_record,uint8_t *roll_buf)
{
    uint32_t loop,DBX_sum=0;
    uint8_t *buf = roll_buf;
    for(loop = 0; loop < cn_DBX_sum_max; loop++)
    {
        if(MDR_record->formatted[loop])   //�ļ����Ѿ���ʽ��
        {
            buf[0] = (uint8_t)cn_MDR_DDR_main; //����¼����
            buf[1] = (uint8_t)loop;            //����ļ����
            //�����ļ����DDR_main���
            __fill_little_32bit(buf+2,0,MDR_record->DDR_main_block_no[loop]);
            __DFFSD_fill_ECC_MDR_record(buf);//���У����
            buf += 8;
            buf[0] = cn_MDR_DDR_bak; //����¼����
            buf[1] = loop;            //����ļ����

            //�����ļ����DDR_bak���
            __fill_little_32bit(buf+2,0,MDR_record->DDR_bak_block_no[loop]);
            __DFFSD_fill_ECC_MDR_record(buf);//���У����
            buf += 8;
            buf[0] = cn_MDR_PCRB; //����¼����
            buf[1] = loop;            //����ļ����

            //�����ļ����PCRB���
            __fill_little_32bit(buf+2,0,MDR_record->PCRB_block_no[loop]);
            __DFFSD_fill_ECC_MDR_record(buf);//���У����
            buf += 8;
            DBX_sum +=8*3;
        }
    }
    return DBX_sum;
}
//----дMDR_main��ȫ��������¼-------------------------------------------------
//����: д��MDR_main���е�ȫ�������洢��
//����: chip����������оƬ
//      MDR_record������ȫ�������洢���ָ��
//����: ��
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
        //���д�볤��+��ǰƫ�ƴ��ڹ���������
        if((MDR_flag->roll_next_main + write_size) > cn_MDR_size-8)
        {//�ر�ע������cn_MDR_size<784,�˴�������ѭ��
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
        do  //�ѻ������е�����д�뵽flash�У����ݿ��ܿ��߽磬��ѭ��
        {
            if((write_size-complete_len) > chip->block_size-block_offset)
                write_len = chip->block_size-block_offset;
            else
                write_len = write_size-complete_len;
            //д�����ݵ�flash�У���У��
            chip->write_data_no_ecc(block_no,block_offset,
                                    roll_buf+complete_len,write_len);
            block_no++;
            block_offset = 0;       //��һ��϶��ӿ��׿�ʼд
            complete_len += write_len;
        }while(complete_len < write_size);
        if(write_end_flag)
            __DFFSD_write_end_flag_main(chip);
    }
    m_free(MDR_buf);
}

//----дMDR_bak��ȫ��������¼-------------------------------------------------
//����: д��MDR_bak���е�ȫ�������洢��
//����: chip����������оƬ
//      MDR_record������ȫ�������洢���ָ��
//����: ��
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
        {//�ر�ע������cn_MDR_size<784,�˴�������ѭ��
            __DFFSD_read_MDR_bak(chip,MDR_buf);
            __DFFSD_erase_MDR_bak(chip);
            __DFFSD_write_MDR_bak(chip,MDR_buf,cn_CDR_and_DBX_size);
            MDR_flag->roll_next_bak = cn_roll_offset;
            write_end_flag = true;
        }
        block_no = MDR_flag->roll_next_main / chip->block_size ;
        block_offset = MDR_flag->roll_next_main % chip->block_size ;
        MDR_flag->roll_next_bak += write_size;
        do  //�ѻ������е�����д�뵽flash�У����ݿ��ܿ��߽磬��ѭ��
        {
            if((write_size-complete_len) > chip->block_size-block_offset)
                write_len = chip->block_size-block_offset;
            else
                write_len = write_size-complete_len;
            //д�����ݵ�flash�У���У��
            chip->write_data_no_ecc(block_no,block_offset,
                                    roll_buf+complete_len,write_len);
            block_no++;
            block_offset = 0;       //��һ��϶��ӿ��׿�ʼд
            complete_len += write_len;
        }while(complete_len < write_size);
        if(write_end_flag)
            __DFFSD_write_end_flag_bak(chip);
    }
    m_free(MDR_buf);
}

//----д��һ��MDR��¼----------------------------------------------------------
//����: ����һ��MDR��¼����������flash driverʹ�ã�flash driver����֪��ʹ�ù���
//      ��ʽ��¼��������ֻ��__DFFSD_write_MDR_roll_record�����İ�װ���ѡ�
//����: DBX_flash_tag�����������ļ���
//      type��д�����ͣ�����ֵcn_MDR_DDR_main��cn_MDR_DDR_bak��cn_MDR_PCRB
//      value����д�����ֵ
//����: ��
//ע��: ���ܶ�δ��ʽ�����ļ�����ñ�����������һ���ڲ������������Ŀ���ļ���
//      �Ƿ��Ѿ���ʽ��
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

//----����ļ���Ϊ�Ѹ�ʽ��-----------------------------------------------------
//����: ��MDR_DBX���б��һ���ļ���Ϊ�Ѿ���ʽ������MDR_main��MDR_bak�ж�Ҫ�޸ġ�
//������chip����������оƬ
//      DBX_no���ļ����
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_set_MDR_DBX_formatted(struct flash_chip *chip,uint32_t DBX_no)
{
    uint8_t *MDR_buf;
    struct MDR_DBX_mirror *MDR_DBX;

    MDR_buf = (uint8_t*)m_malloc(cn_MDR_size, 0);
    if(MDR_buf == NULL)
        return ;
    //���㱻д���ļ����MDR_DBX��¼�ĵ�ַ
    MDR_DBX = (struct MDR_DBX_mirror *)(MDR_buf +
                        cn_MDR_CDR_size + DBX_no * cn_MDR_DBX_size);
    __DFFSD_read_MDR_bak(chip,MDR_buf);
    strcpy(MDR_DBX->DBX_format_flag,"ready");
    __DFFSD_fill_ECC_MDR_DBX((uint8_t *)MDR_DBX);
    __DFFSD_erase_MDR_bak(chip);
    //���4�ֽ��ǽ�����־AA5555AA���ʼ�4
    __DFFSD_write_MDR_bak(chip,MDR_buf,cn_MDR_size-4);
    __DFFSD_write_end_flag_bak(chip);

    __DFFSD_read_MDR_main(chip,MDR_buf);
    strcpy(MDR_DBX->DBX_format_flag,"ready");
    __DFFSD_fill_ECC_MDR_DBX((uint8_t *)MDR_DBX);
    __DFFSD_erase_MDR_main(chip);
    //���4�ֽ��ǽ�����־AA5555AA���ʼ�4
    __DFFSD_write_MDR_main(chip,MDR_buf,cn_MDR_size-4);
    __DFFSD_write_end_flag_main(chip);
    m_free(MDR_buf);
    return;
}
//----����MDR_main-------------------------------------------------------------
//����: ����MDR_main��MDR_main����ʼ��Ĭ��Ϊ0
//������chip����������оƬ
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_erase_MDR_main(struct flash_chip *chip)
{
    uint32_t blocks,loop;
    blocks = (cn_MDR_size + chip->block_size -1) / chip->block_size;
    for(loop = 0; loop < blocks; loop++)
        chip->erase_block(loop);
}

//----����MDR_bak-------------------------------------------------------------
//����: ����MDR_bak��MDR_bak����ʼ��Ĭ��Ϊ0
//������chip����������оƬ
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_erase_MDR_bak(struct flash_chip *chip)
{
    uint32_t blocks,loop;
    blocks = (cn_MDR_size + chip->block_size -1) / chip->block_size;
    for(loop = 0; loop < blocks; loop++)
        chip->erase_block(loop + chip->MDR.MDR_bak_block);
}

#endif //#if ((cfg_djyfs == 1) && (cn_run_mode != cn_mode_tiny) && (cfg_djyfs_flash == 1))
