//----------------------------------------------------
//Copyright (C), 2004-2009,  lst.
//��Ȩ���� (C), 2004-2009,   lst.
//����ģ��:flash�ļ�ϵͳ
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����:flash�ļ�ϵͳ��DDR��Ĳ���
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
#include "fdt.h"
#include <string.h>

//----�ع�DDR_bak--------------------------------------------------------------
//���ܣ���ȷ��ȡDDR_main���ڴ������У�鷢��flash�е�DDR_bak���ڴ��е�DDR��
//      ��һ�£������ñ���������DBL�����⣬����DDR������ಿ������ͬ�ģ���ֻ��
//      ����DDR���е�MAT���ع���һ��DDR_DBL_bak���ɣ�Ȼ�������DDR_bak����Ϊ
//      ��Ҫд�롣
//����: DBX_flash_tag�����������ļ���flash�ļ����ר�����ݽṹָ��
//���أ���
//-----------------------------------------------------------------------------
void __DFFSD_rebuild_DDR_bak(struct st_DBX_flash_tag *DBX_flash_tag)
{
    uint32_t i,loop,DBL_offset,DDR_blocks;
    struct flash_chip *chip = DBX_flash_tag->chip;
    bool_t *writed = DBX_flash_tag->writed_DDR_bak; //ָ���Ƿ����д���ָ��
    uint32_t *DBL_buf;
    DDR_blocks = (DBX_flash_tag->DDR_size + chip->block_size -1)
                                / chip->block_size;    //����DDR��ռ�õĿ���
    DBL_buf = DBX_flash_tag->DDR_DBL+DDR_blocks;
    for(loop = 0; loop < DDR_blocks; loop++)    //��������DDR_bak��д��
        writed[loop] = false;
    //���¼���ɨ�������ļ��񣬳�ʼ��DBL_bak��
    DBL_offset = 0;
    for(loop = 0; loop < DBX_flash_tag->block_sum; loop++)
    {
        if(DBX_flash_tag->DDR_MAT[loop].attr == cn_DDR_bak_block)
        {
            DBL_buf[DBL_offset] = loop;     //��loop����DDR_bak��
            DBL_offset ++;                  //DBLƫ������
        }
    }
    //�쳣�ϵ����������£��������MAT�е�DDR_bak�������������������ĵ�
    //��DDR���쳣�ϵ��㷨��
    //Ϊ��ȱ�ٵ�DBL������¿�
    //��ʱ��ʼ����δ��ɣ�����ʹ�������Ŀ�����㷨��ֱ����free�飬
    if(DBL_offset < DDR_blocks)
    {
        for(loop = 0; loop < DBX_flash_tag->block_sum; loop++)
        {
            if(DBX_flash_tag->DDR_MAT[loop].attr == cn_free_block)
            {
                //��loop���ǿ��п�,��ΪDDR_bak��
                DBL_buf[DBL_offset] = loop;
                //��MAT���е�loop������Ը�ΪDDR_bak��
                DBX_flash_tag->DDR_MAT[loop].attr = cn_DDR_bak_block;
                //����MAT���е�loop��ĵ�ַ��DDR���е�ƫ����
                i = DBX_flash_tag->DDR_size-(DBX_flash_tag->block_sum-loop)*4*2;
                //����MAT���е�loop��ĵ�ַ��DDR���е�ƫ�ƿ��
                i = i / chip->block_size;
                //��loop�����;���޸ģ�MAT��koop��λ��DDR_mainҲ���޸ģ���д֮
                DBX_flash_tag->writed_DDR_main[i] = false;
                DBL_offset ++;      //DBLƫ������
                if(DBL_offset == DDR_blocks)
                    break;
            }
        }
    }
}

//----�ع�DDR_main-------------------------------------------------------------
//���ܣ���ȷ��ȡDDR_main���ڴ��󣬿϶�DDR_main��������ģ������ñ������ع�
//      DDR_main����DBL�����⣬����DDR������ಿ������ͬ�ģ���ֻ�����DDR����
//      ��MAT���ع���һ��DDR_DBL_main���ɣ�Ȼ�������DDR_main����Ϊ��Ҫд�롣
//����: DBX_flash_tag�����������ļ���flash�ļ����ר�����ݽṹָ��
//���أ���
//-----------------------------------------------------------------------------
void __DFFSD_rebuild_DDR_main(struct st_DBX_flash_tag *DBX_flash_tag)
{
    uint32_t i,loop,DBL_offset,DDR_blocks;
    struct flash_chip *chip = DBX_flash_tag->chip;

    bool_t *writed = DBX_flash_tag->writed_DDR_main; //ָ���Ƿ����д���ָ��
    uint32_t *DBL_buf;
    DDR_blocks = (DBX_flash_tag->DDR_size + chip->block_size -1)
                                / chip->block_size;    //����DDR��ռ�õĿ���
    DBL_buf = DBX_flash_tag->DDR_DBL;
    for(loop = 0; loop < DDR_blocks; loop++)    //������DDR_main��д��
        writed[loop] = false;
    //���¼���ɨ�������ļ��񣬳�ʼ��DBL_main��
    DBL_offset = 0;
    for(loop = 0; loop < DBX_flash_tag->block_sum; loop++)
    {
        if(DBX_flash_tag->DDR_MAT[loop].attr == cn_DDR_main_block)
        {
            DBL_buf[DBL_offset] = loop;     //��loop����DDR_main��
            DBL_offset ++;                  //DBLƫ������
        }
    }
    //�쳣�ϵ����������£��������MAT�е�DDR_main�������������������ĵ�
    //��DDR���쳣�ϵ��㷨��
    //Ϊ��ȱ�ٵ�DBL������¿�
    //��ʱ��ʼ����δ��ɣ�����ʹ�������Ŀ�����㷨��ֱ����free�飬
    if(DBL_offset < DDR_blocks)
    {
        for(loop = 0; loop < DBX_flash_tag->block_sum; loop++)
        {
            if(DBX_flash_tag->DDR_MAT[loop].attr == cn_free_block)
            {
                //��loop���ǿ��п�,��ΪDDR_bak��
                DBL_buf[DBL_offset] = loop;
                //��MAT���е�loop������Ը�ΪDDR_main��
                DBX_flash_tag->DDR_MAT[loop].attr = cn_DDR_main_block;
                //����MAT���е�loop��ĵ�ַ��DDR���е�ƫ����
                i = DBX_flash_tag->DDR_size -(DBX_flash_tag->block_sum-loop)*4*2;
                //����MAT���е�loop��ĵ�ַ��DDR���е�ƫ�ƿ��
                i = i / chip->block_size;
                //��loop�����;���޸ģ�MAT��koop��λ��DDR_mainҲ���޸ģ���д֮
                DBX_flash_tag->writed_DDR_main[i] = false;
                DBL_offset ++;      //DBLƫ������
                if(DBL_offset == DDR_blocks)
                    break;
            }
        }
    }
}

//----����MAT����ʼƫ��--------------------------------------------------------
//����: ����DDR����MAT�����ʼƫ�Ƶ�ַ�����DDR_size������1�飬��ART���ֱ�Ӵ�
//      MAT������MAT��Ҫ��8�ֽڶ������ⵥ��MAT�����顣
//����: DBX_flash_tag�����������ļ���flash�ļ����ר�����ݽṹָ��
//����: MAT����ʼ��ַƫ��
//-----------------------------------------------------------------------------
uint32_t __DFFSD_MAT_start(struct st_DBX_flash_tag *DBX_flash_tag)
{
    uint32_t MAT_start;
    struct flash_chip *chip = DBX_flash_tag->chip;

    MAT_start=(DBX_flash_tag->DDR_size+chip->block_size -1)/chip->block_size*4
                +4 +4 + DBX_flash_tag->block_sum*2;
    //��������룬һ��MAT�����һ���ڵ�һ�飬һ���ں�һ�顣�������
    if(DBX_flash_tag->DDR_size > chip->block_size)
    {
        MAT_start = align_up_sys(MAT_start);
    }
    return MAT_start;
}

//----��DDR��------------------------------------------------------------------
//���ܣ���������__DFFSD_scan_DDR�������ã�ɨ��һ��DDR����ȡDDR������ݣ�Ȼ��
//      ��ʼ��DBX_flash_tag��DDR���йصĳ�Ա��
//����: DBX_flash_tag�����������ļ���flash�ļ����ר�����ݽṹָ��
//      which����ȡ��һ��DDR��0=DDR_main��1=DDR_bak
//���أ�true=��ȫ�޴�flash��DDR�����뵽DBX_flash_tag�С�
//      false=�д���flash��DDR�����κδ���__DFFSD_scan_DDR���������
//      __rebuild_DDR_bak��__rebuild_DDR_main�����ع�DDR��
//-----------------------------------------------------------------------------
bool_t __DFFSD_read_DDR(struct st_DBX_flash_tag *DBX_flash_tag,
                            ufast_t which)
{
    uint32_t block_no;      //DDR��Ŀ��
    uint32_t DDR_blocks;    //DDR��ռ�õĿ���
    uint32_t loop,read_size;
    uint32_t offset = 0,completed = 0;
    uint32_t ECCF_start,FDSB_start,ARB_start,ART_start,ART_end,MAT_start;
    uint32_t temp;
    struct MAT_table *MAT_item;
    struct flash_chip *chip = DBX_flash_tag->chip;  //��ȡ�ļ����Ӧ��оƬ
    uint32_t *DBL_buf;

    DDR_blocks = (DBX_flash_tag->DDR_size+chip->block_size-1)/chip->block_size;
    if(which == 0)
    {
        DBL_buf = DBX_flash_tag->DDR_DBL;  //ȡDBL_main������ָ��
        DBL_buf[0] = DBX_flash_tag->DDR_main;
    }else
    {
        DBL_buf = DBX_flash_tag->DDR_DBL+DDR_blocks;   //ȡDBL_bak������ָ��
        DBL_buf[0] = DBX_flash_tag->DDR_bak;
    }
    //����DDR��������Ա��ռ�õĿռ䣬���м���
    ECCF_start = DDR_blocks*4;
    FDSB_start = ECCF_start + 4;
    ARB_start = FDSB_start + 4;
    ART_start = ARB_start +4;
    ART_end = ART_start + DBX_flash_tag->block_sum * 2;
    MAT_start = __DFFSD_MAT_start(DBX_flash_tag);
    MAT_item = DBX_flash_tag->DDR_MAT;
    for(loop = 0; loop < DDR_blocks; loop++)  //���׿鵽ĩ��ѭ��
    {
        block_no = DBL_buf[loop] + DBX_flash_tag->start_block;
        if(loop == DDR_blocks-1)
            read_size = DBX_flash_tag->DDR_size - completed;
        else
            read_size = chip->block_size;
        if(chip->read_data_with_ecc(block_no,0,chip->block_buf,read_size)
                                    != read_size)
        {
            return false;   //���������ش���
        }else
        {
            while(offset - completed < read_size)
            {
                if(offset < ECCF_start)         //DBL
                {
                    DBL_buf[offset/4] = __pick_little_32bit(
                                        chip->block_buf+(offset-completed),0);
                    offset +=4;
                }
                else if(offset < FDSB_start)         //DBL
                {
                    DBX_flash_tag->nand_ecc = __pick_little_32bit(
                                        chip->block_buf+(offset-completed),0);
                    offset +=4;
                }else if(offset < ARB_start)  //FDSB
                {
                    DBX_flash_tag->DDR_FDSB = __pick_little_32bit(
                                        chip->block_buf+(offset-completed),0);
                    offset +=4;
                }else if(offset < ART_start) //ARB
                {
                    DBX_flash_tag->DDR_ARB = __pick_little_32bit(
                                        chip->block_buf+(offset-completed),0);
                    offset +=4;
                }else if(offset < ART_end)   //ART
                {
                    temp = (offset-ART_start)/2;
                    DBX_flash_tag->ART_times[temp] = __pick_little_16bit(
                                        chip->block_buf+(offset-completed),0);

                    DBX_flash_tag->ART_position[temp] = temp;
                    DBX_flash_tag->ART_block_no[temp] = temp;
                    offset +=2;
                }else if(offset < MAT_start)    //�����������
                {
                    offset = MAT_start;
                }else if(offset <= DBX_flash_tag->DDR_size)
                {
                    temp = __pick_little_32bit(
                                        chip->block_buf+(offset-completed),0);
                    MAT_item->attr = temp >>30;
                    //��ǰָ��
                    MAT_item->previous = temp & 0x3fffffff;
                    temp = __pick_little_32bit(
                                        chip->block_buf+(offset-completed),1);
                    MAT_item->attr += (temp >>30)<<2;
                    //���ָ��
                    MAT_item->next = temp & 0x3fffffff;
                    offset +=8;
                    MAT_item++;
                }
            }
            completed += chip->block_size;
        }
    }
    for(loop = 0; loop < DBX_flash_tag->block_sum; loop++)  //ĥ�������
    {
        DBX_flash_tag->ART_block_no[loop] = loop;
        DBX_flash_tag->ART_position[loop] = loop;
    }
    return true;        //������������κδ��󣬺����߲�������
}

//----У��DDR_bak-------------------------------------------------------------
//���ܣ������Ѿ���ȷ�����ڴ��DDR_main��У��flash�е�DDR_bak�Ƿ���ȷ��У����ֻҪ
//      ����һ���ֽڲ�һ�£��ͷ��ش��󣬽��������ع�DDR_bak������
//      ������ǰ��DBX_flash_tag����DDR����صĳ�ԱӦ�ó�ʼ����(��DBL_bak��)��
//      ������û�ж�Ӧ��__DFFSD_verify_DDR_main��������Ϊ����ʱ�����Ҫ��
//      DDR_bak����DDR_main����У�飬�϶����д���ġ�
//����: DBX_flash_tag�����������ļ���flash�ļ����ר�����ݽṹָ��
//���أ�true=��ȫ�޴�flash��DDR�����뵽DBX_flash_tag�С�
//      false=�д���flash��DDR�����κδ���__DFFSD_scan_DDR���������
//      __DFFSD_rebuild_DDR_bak
//-----------------------------------------------------------------------------
bool_t __DFFSD_verify_DDR_bak(struct st_DBX_flash_tag *DBX_flash_tag)
{
    uint32_t block_no;      //DDR��Ŀ��
    uint32_t DDR_blocks;    //DDR��ռ�õĿ���
    uint32_t loop,read_size;
    uint32_t offset = 0,completed = 0;
    uint32_t FDSB_start,ARB_start,ART_start,ART_end,MAT_start;
    uint32_t temp;
    struct MAT_table *MAT_item;
    struct flash_chip *chip = DBX_flash_tag->chip;  //��ȡ�ļ����Ӧ��оƬ
    uint32_t *DBL_buf;
    uint8_t attr;

    DDR_blocks = (DBX_flash_tag->DDR_size+chip->block_size-1)/chip->block_size;
    DBL_buf = DBX_flash_tag->DDR_DBL+DDR_blocks;   //ȡDBL_bak������ָ��
    DBL_buf[0] = DBX_flash_tag->DDR_bak;
    FDSB_start = DDR_blocks*4;
    ARB_start = FDSB_start + 4;
    ART_start = ARB_start +4;
    ART_end = ART_start + DBX_flash_tag->block_sum *2;
    MAT_start = __DFFSD_MAT_start(DBX_flash_tag);
    MAT_item = DBX_flash_tag->DDR_MAT;
    for(loop = 0; loop < DDR_blocks; loop++)  //���׿鵽ĩ��ѭ��
    {
        block_no = DBL_buf[loop] + DBX_flash_tag->start_block;
        if(loop == DDR_blocks-1)
            read_size = DBX_flash_tag->DDR_size - completed;
        else
            read_size = chip->block_size;
        if(chip->read_data_with_ecc(block_no,0,chip->block_buf,read_size)
                                    != read_size)
        {
            return false;   //���������ش���
        }else
        {
            while(offset - completed < read_size)
            {
                if(offset < FDSB_start)         //DBL
                {
                    DBL_buf[offset/4] = __pick_little_32bit(
                                        chip->block_buf+(offset-completed),0);
                    offset +=4;
                }else if(offset < ARB_start)  //FDSB
                {
                    temp = __pick_little_32bit(
                                        chip->block_buf+(offset-completed),0);
                    if(DBX_flash_tag->DDR_FDSB != temp)
                        return false;
                    offset +=4;
                }else if(offset < ART_start) //ARB
                {
                    temp = __pick_little_32bit(
                                        chip->block_buf+(offset-completed),0);
                    if(DBX_flash_tag->DDR_ARB != temp)
                        return false;
                    offset +=4;
                }else if(offset < ART_end)   //ART
                {
                    temp = __pick_little_16bit(
                                        chip->block_buf+(offset-completed),0);
                    if(DBX_flash_tag->ART_times[(offset-ART_start)/2]
                                != (uint16_t)temp)
                        return false;
                    offset +=2;
                }else if(offset < MAT_start)    //�����������
                {
                    offset = MAT_start;
                }else if(offset < DBX_flash_tag->DDR_size)
                {

                    temp = __pick_little_32bit(
                                        chip->block_buf+(offset-completed),0);
                    attr = temp >>30;
                    //��ǰָ��
                    if((temp & 0x3fffffff) != MAT_item->previous)
                        return false;
                    temp = __pick_little_32bit(
                                        chip->block_buf+(offset-completed),1);
                    attr += (temp >>30)<<2;
                    //���ָ��
                    if((temp & 0x3fffffff) != MAT_item->next)
                        return false;
                    if(attr != MAT_item->attr)
                        return false;
                    offset +=8;
                    MAT_item++;
                }
            }
            completed += chip->block_size;
        }
    }
    return true;        //������������κδ��󣬺����߲�������
}

//----��ȡһ���ļ����DDR��----------------------------------------------------
//����: ��ȡһ���ļ����DDR�����������Ч�ԡ��ȶ�DDR_main������д��󣬾Ͷ�
//      DDR_bak������Ȼ���󣬷��ش��󡣶�ȡһ����ȷ��DDR����������ȷ��DDR��
//      У����һ��DDR��������ִ�����DBX_flash_tag��writed�����б�ǡ�
//      У����㷨̫���ӣ������ڴ˽����������ר���ĵ�˵������ȡ�����ݱ�����
//      DBX_flash_tag��DDR_mirrorָ���У�����flash��DDR��Ľ��ƾ���
//����: DBX_flash_tag�����������ļ���flash�ļ����ר�����ݽṹָ��
//����: true=��ȷ��ȡ��false=����
//��ע: �ڴ��л��棬ƽʱ�����flash�ж�ȡDDR,�����ļ�����δ��ɳ�ʼ��ʱ��DDR����
//-----------------------------------------------------------------------------
bool_t __DFFSD_scan_DDR(struct st_DBX_flash_tag *DBX_flash_tag)
{
    if(__DFFSD_read_DDR(DBX_flash_tag,0) == true)  //�ȶ�DDR_main
    {
        //DDR_main��ȷ��ȡ����У��DDR_bak�Ƿ�һ�£�У������п��ܲ����޸�
        //DDR_main������
        if(__DFFSD_verify_DDR_bak(DBX_flash_tag) == false);
            __DFFSD_rebuild_DDR_bak(DBX_flash_tag);   //�ع�DDR_bak
    }else if(__DFFSD_read_DDR(DBX_flash_tag,1) == true)
    {
        //ִ�е���DDR_main�϶��д�����У�飬ֱ���ع��Ϳ�����
        __DFFSD_rebuild_DDR_main(DBX_flash_tag);
    }else
        return false;
    return true;
}

//----��Ծ����-----------------------------------------------------------------
//����: ĳ���һ��ĥ�����ת�Ƶ���һ��ĥ�����
//����: DBX_flash_tag�����������ļ���flash�ļ����ר�����ݽṹָ��
//      from��ԭ����
//      to��Ŀ������
//      position�����ƶ��Ŀ��ԭʼλ��
//����: ��
//��ע: ���÷�ȷ����from�������ǿյģ�Ҳ����˵��position����Ч����
//-----------------------------------------------------------------------------
void __DFFSD_ART_flip(struct st_DBX_flash_tag *DBX_flash_tag,
                         ufast_t from,ufast_t to,uint32_t position)
{
    uint32_t loop,block_position,block_no_temp;
    uint32_t *pos_tab,*block_no_tab;
    uint16_t *times_tab;
    uint32_t block_no,*balance_region;
    uint16_t times_bak;

    block_position = position;
    balance_region = DBX_flash_tag->balance_region;
    pos_tab = DBX_flash_tag->ART_position;          //ĥ��λ�ñ�
    block_no_tab = DBX_flash_tag->ART_block_no;     //ĥ���ű��������ͬ��
    times_tab = DBX_flash_tag->ART_times;           //ĥ�������
    block_no = block_no_tab[position];
    times_bak = times_tab[block_position];          //���ƶ����ĥ�����

    if(to > from)       //������Ծ
    {
        for(loop = from; loop < to; loop++)
        {
            if(balance_region[loop] == balance_region[loop+1])//��loop�����ǿյ�
            {
                continue;
            }
            //ȡ��loop���������һ����Ԫ�Ŀ��(����pos���е�ƫ����)
            block_no_temp = block_no_tab[balance_region[loop+1]-1];
            //��loop�������һ����Ԫ��ĥ�����copy���ϴ�ѭ���ƿյĵ�Ԫ
            times_tab[block_position] = times_tab[balance_region[loop+1]-1];
            //��loop�������һ����Ԫ�Ŀ��copy���ϴ�ѭ���ƿյĵ�Ԫ
            block_no_tab[block_position] = block_no_temp;
            //��loop������һ����Ԫ��λ�ñ�ָ���ϴ��ƿյĵ�Ԫ��
            pos_tab[block_no_temp] = block_position;
            block_position = balance_region[loop+1]-1;    //������յĵ�Ԫ������
        }
        for(loop = to; loop > from; loop--)
            balance_region[loop]--;        //����ָ������һ��
    }else               //������Ծ
    {
        for(loop = from; loop > to; loop--)
        {
            if(loop != from)//����������Ҫ����Ϊ���loop=18�Ļ���loop+1�±�Խ��
            {
                if(balance_region[loop] == balance_region[loop+1])//��loop������
                {
                    continue;
                }
            }
            //ȡ��loop������һ����Ԫ�Ŀ��(����pos���е�ƫ����)
            block_no_temp = block_no_tab[balance_region[loop]];
            //��loop������һ����Ԫ��ĥ�����copy���ϴ�ѭ���ƿյĵ�Ԫ
            times_tab[block_position] = times_tab[balance_region[loop]];
            //��loop������һ����Ԫ�Ŀ��copy���ϴ�ѭ���ƿյĵ�Ԫ
            block_no_tab[block_position] = block_no_temp;
            //��loop������һ����Ԫ��λ�ñ�ָ��Ŀ�굥Ԫ��
            pos_tab[block_no_temp] = block_position;
            block_position = balance_region[loop];    //������յĵ�Ԫ������
        }
        for(loop = from; loop > to; loop--)
            balance_region[loop]++;        //����ָ������һ��
    }
    //���ƶ���Ĳ�������д��Ŀ�������(����)����(����)��Ԫ
    times_tab[block_position] = times_bak;
    //���ƶ���Ŀ��д��Ŀ����ART_block_no������(����)����(����)��Ԫ
    block_no_tab[block_position] = block_no;
    //pos��ı��ƶ���ָ��ָ��Ŀ���������(����)����(����)��Ԫ
    pos_tab[block_no] = block_position;
}

//----ĥ��ƽ��齻��----------------------------------------------------------
//���ܣ���ĥ��ƽ����г�������ʧ��ʱ�����ñ���������busy[0]�е�һ����free[8]�е�
//      һ�齻������busy[0]������free[8]�г�Ϊbusy[8]����������ʹ��������busy��
//      ��һ�����¼������free[0]�д��ã���free[8]�е�һ�鵽busy[8]�С�˯��һ��
//      ��������ʧ��ı�׼��:free[0~7]���գ���free[8]��busy[0]���ա�
//����: DBX_flash_tag�����������ļ���flash�ļ����ר�����ݽṹָ��
//���أ�true=�ɹ�����һ�鵽free[0]��false=ʧ��
//-----------------------------------------------------------------------------
bool_t __DFFSD_exchange_busy0_free8(struct st_DBX_flash_tag *DBX_flash_tag)
{
    uint32_t *busy_region,*free_region;
    uint32_t *pos_tab,*block_no_tab;
    uint16_t *times_tab,times_bak;
    uint32_t block_no_bak,block_no;
    uint32_t loop,invalid_num;
    struct flash_chip *chip = DBX_flash_tag->chip;  //ȡ�ļ�������оƬ
    pos_tab = DBX_flash_tag->ART_position;          //ȡĥ��λ�ñ�
    block_no_tab = DBX_flash_tag->ART_block_no;     //ȡĥ���ű��������ͬ��
    times_tab = DBX_flash_tag->ART_times;           //ȡĥ�������
    busy_region = &DBX_flash_tag->balance_region[9];    //��һ��æ����
    free_region = DBX_flash_tag->balance_region;        //��һ�����з���

    //�������䱣��busy0.end��Ŀ��ĥ������Ϳ��,
    times_bak = times_tab[busy_region[1]-1];
    block_no_bak = block_no_tab[busy_region[1]-1];
    block_no = DBX_flash_tag->start_block + block_no_bak;
    //����busy0.end������
    chip->read_data_with_ecc(block_no,0,chip->block_buf,chip->block_size);
    //��busy0.end������д�뵽free8.0�У���д�벻�ɹ�����д�뵽free8.1����������
    //ֱ���ɹ���
    for(invalid_num=0;invalid_num < DBX_flash_tag->valid_sum;invalid_num++)
    {
        block_no = block_no_tab[free_region[8]+invalid_num];
        block_no += DBX_flash_tag->start_block;
        if(chip->write_data_with_ecc(block_no,0,chip->block_buf,
                                     chip->block_size) == chip->block_size)
        {
            break;  //��ȷд�룬˵���ҵ���һ���ÿ飬����ѭ��
        }
    }
    //�²������������ȿ��ÿ������٣�˵��busy0.end�Ѿ���ȷд�뵽��
    //free_region[8]+invalid_num����
    if(invalid_num < DBX_flash_tag->valid_sum)
    {
        for(loop = 1; loop < 8; loop++)
        {       //��ѭ����busy1~7��������һ����Ŀ���ճ�busy7.end
            busy_region[loop]--;        //busy_region[loop]ָ������һ��
            if(busy_region[loop] == busy_region[loop-1])
                continue;       //��loop��busy�����ǿյ�
            times_tab[busy_region[loop]] = times_tab[busy_region[loop+1]-1];
            block_no = block_no_tab[busy_region[loop+1]-1];
            block_no_tab[busy_region[loop]] = block_no;
            pos_tab[block_no] = busy_region[loop];
        }
        busy_region[8]--;  //busy_region8ָ��ָ��ԭbusy7.end��λ��
        //�����free8.invalid_num��copy��busy_region8.0��λ��
        times_tab[busy_region[8]] = times_tab[free_region[8] + invalid_num]+1;
        block_no = block_no_tab[free_region[8] + invalid_num];
        block_no_tab[busy_region[8]] = block_no;
        pos_tab[block_no] = busy_region[8];
        //����ѱ����ԭbusy0.end����Ϣcopy��free8.invalid_num��
        times_tab[free_region[8] + invalid_num] = times_bak;
        block_no_tab[free_region[8]+invalid_num] = block_no_bak;
        pos_tab[block_no_bak] = free_region[8]+invalid_num;
        //������ݿ齻������޸�MAT��,block_no_bak�������ԭbusy0.end�Ŀ�ţ�
        //block_no������Ǹÿ鱻��������Ŀ���š�
        __DFFSD_update_MAT_item(DBX_flash_tag,block_no_bak,block_no);
    }
    for(loop = 0; loop < invalid_num; loop++)//��ѭ�������л���ת�Ƶ�invalid_region
    {
        block_no = block_no_tab[free_region[8]+loop];
        __DFFSD_register_invalid(DBX_flash_tag,block_no);
    }
    return true;
}

//----�򵥷���һ��------------------------------------------------------------
//����: ��__allocate_block�������ƣ����ǲ����κο齻��������ֱ�Ӵ�free[0]��ȡ��
//      һ�顣���free[0]�գ���ʵ��ȡ���Ŀ�����free[1]��������Ŀ�
//����: DBX_flash_tag�����������ļ���flash�ļ����ר�����ݽṹָ��
//����: ��ţ����ļ������Կ�ţ���û�п��п飬����-1
//----------------------------------------------------------------------------
uint32_t __DFFSD_allocate_block_simple(struct st_DBX_flash_tag *DBX_flash_tag)
{
    uint32_t block_no;
    uint32_t *block_no_tab;
    uint16_t *times_tab,times_bak;
    uint32_t target_region;
    if(DBX_flash_tag->free_sum== 0)
        return cn_limit_uint32;
    else
        DBX_flash_tag->free_sum--;
    block_no_tab = DBX_flash_tag->ART_block_no;   //ȡĥ���ű��������ͬ��
    times_tab = DBX_flash_tag->ART_times;         //ȡĥ�������
    times_bak =times_tab[0];        //��ȡfree��0����һ��ĥ�����
    //�ÿ���æ��������Ӧæ��������free[0]��ʼ���ܻ������ɸ����������ʲ�
    //�ܼ򵥵���Ϊfree[0]��ȡ���Ŀ鲻�ı�ĥ������������busy[0]��
    target_region = times_bak/128 + 9;      //ȡæ����Ŀ������
    //��ȡfree��0����һ����
    block_no = block_no_tab[0];
    //�ӵ�0����λ��0������target_region��
    __DFFSD_ART_flip(DBX_flash_tag,0,target_region,0);
    return block_no;
}

//----����һ��----------------------------------------------------------------
//����: ��free����ȡ��һ�飬���Ѹÿ�ŵ�busy���С�Ŀ����ĥ����������������:
//      1.���п�ȫ��������free_region[8]������busy_region[0]�ǿգ���Ҫ���л���
//        ĥ��ƽ���滻����busy_region[0]��ȡһ��A���ٴ�free_region[8]��ȡһ��B��
//        A�������copy��B�飬��B��ŵ�busy_region[8]�С�ע���������Ҫ�漰��
//        �޸�MAT��
//      2.���ͬʱ����busy_region[0]��free_region[0]���յ�����ĥ���������ת
//        һȦ��ʹbusy_region[8]��free_region[8]���ա�
//����: DBX_flash_tag�����������ļ���flash�ļ����ר�����ݽṹָ��
//����: ��ţ����ļ������Կ�ţ���û�п��п飬����-1
//----------------------------------------------------------------------------
uint32_t __DFFSD_allocate_block(struct st_DBX_flash_tag *DBX_flash_tag)
{
    uint32_t block_no;
    uint32_t *block_no_tab;
    uint16_t *times_tab,times_bak;
    uint32_t *free_region;
    uint32_t target_region;
    if(DBX_flash_tag->free_sum== 0)
        return cn_limit_uint32;
    else
        DBX_flash_tag->free_sum--;
    block_no_tab = DBX_flash_tag->ART_block_no;   //ȡĥ���ű��������ͬ��
    times_tab = DBX_flash_tag->ART_times;   //ȡĥ�������
    free_region = DBX_flash_tag->balance_region;        //��һ�����з���
    if(free_region[0] == free_region[8])    //free[0~7]�Ѿ���
        __DFFSD_exchange_busy0_free8(DBX_flash_tag);  //����дĥ��ƽ��齻������
    times_bak =times_tab[0];//��ȡfree��0����һ��ĥ�����
//    times_bak++;    //ĥ�������1��ע�͵�����Ϊʵ�ʷ�����дʱ�����ӡ�
    //�ÿ���æ������end_region��������free[0]��ʼ���ܻ������ɸ����������ʲ�
    //�ܼ򵥵���Ϊfree[0]��ȡ���Ŀ鲻�ı�ĥ������������busy[0]��
    target_region = times_bak/128 + 9;
    //��ȡfree��0����һ����
    block_no = block_no_tab[0];
    //�ӵ�0����λ��0������target_region��
    __DFFSD_ART_flip(DBX_flash_tag,0,target_region,0);
    return block_no;
}

//----дDDR��flash-----------------------------------------------------------
//����: ����DBX_flash_tag->writed_main���е����ݣ���DDR������Ҫд�������д
//      �뵽flash��DDR_main���С�
//      1.��DDR��д�뵽flash�У��洢λ����DBL_main�ṩ
//      2.����������飬�������¿飬����DBL_main��MAT_main��MAT_bak�б�ǡ�
//      3.����������飬MAT_bak��MAT_main�����ݶ����޸ģ���ֻ��which����ָ����
//        �Ǹ�MAT���޸ĺ�����ݻ��ڱ���д��flash������һ����__DFFSD_write_DDR��
//        �����ٴε��ñ��������д��
//      4.Ϊ���㷨,��ʹ���������滻,Ҳ���޸�ĥ�����,ֻ��mat���аѱ�ʾ���飬��
//        �¿��ʾΪDDR��.����,���������滻ʱ,ֻ��Ҫ�޸�MAT���DBL��Ϳ�����.����
//        ģʽ�������辫ȷƽ�⣬������������ġ�
//����: DBX_flash_tag�����������ļ���flash�ļ����ר�����ݽṹָ��
//      which����ȡ��һ��DDR��0=DDR_main��1=DDR_bak
//����: true=�ɹ�д��
//      false=ʧ�ܣ���д���������飬��û�п��п�ɹ�����ʱ����ʧ��
//��ע: ��ѭ��д�룬
//----------------------------------------------------------------------------
bool_t __DFFSD_write_DDR_exe(struct st_DBX_flash_tag *DBX_flash_tag,
                             ufast_t which)
{
    struct flash_chip *chip;    //оƬָ��
    uint32_t loop,DDR_blocks,DDR_no,write_result;
    uint32_t write_len,completed = 0;
    uint32_t block_no,block_new,temp;
    bool_t *writed;         //ָ���Ƿ����д���ָ��
    bool_t finished;
    struct MAT_table *MAT_item;
    uint32_t *DBL_buf;
    uint8_t *buf;
    uint32_t ECCF_start,FDSB_start,ARB_start,ART_start,ART_end,MAT_start,offset;
    uint8_t block_attr;
    
    chip = DBX_flash_tag->chip; //ȡ���ļ�������оƬ
    buf = chip->block_buf;
    // 1��DDR��ռ�õĿ�����
    DDR_blocks = (DBX_flash_tag->DDR_size+chip->block_size-1)/chip->block_size;
    if(which == 0)
    {
        writed = DBX_flash_tag->writed_DDR_main;
        block_attr = cn_DDR_main_block;
        DBL_buf = DBX_flash_tag->DDR_DBL;
    }else
    {
        writed = DBX_flash_tag->writed_DDR_bak;
        block_attr = cn_DDR_bak_block;
        DBL_buf = DBX_flash_tag->DDR_DBL + DDR_blocks;
    }
    MAT_item = DBX_flash_tag->DDR_MAT;
    //��ַ0����DBL��ָ���ַ
    //FDSB,ARB,ART��uint32_t���ʶ���4�ֽ�
    ECCF_start = DDR_blocks*4;
    FDSB_start = ECCF_start + 4;
    ARB_start = FDSB_start + 4;
    ART_start = ARB_start +4;
    ART_end = ART_start + DBX_flash_tag->block_sum *2;
    //����MAT��洢����ʼ��ַ
    MAT_start = __DFFSD_MAT_start(DBX_flash_tag);
    do
    {
        finished = true;    //�ٶ���ѭ���������ȫ��д�����
        for(loop = 0; loop < DDR_blocks; loop++)
        {
            if(writed[loop] == true)
            {
                continue;                       //�����Ѿ�д�룬������
            }
            completed = loop * chip->block_size;
            offset = completed;
            if(loop == DDR_blocks-1)
                write_len = DBX_flash_tag->DDR_size - completed;
            else
                write_len = chip->block_size;

            while(offset - completed < write_len)
            {
                if(offset < ECCF_start)         //д��DBL
                {
                    __fill_little_32bit(buf+(offset-completed),0,
                                        DBL_buf[offset/4]);
                    offset +=4;
                }else if(offset < FDSB_start)  //д��FDSB
                {
                    __fill_little_32bit(buf+(offset-completed),0,
                                        DBX_flash_tag->nand_ecc);
                    offset +=4;
                }else if(offset < ARB_start)  //д��FDSB
                {
                    __fill_little_32bit(buf+(offset-completed),0,
                                        DBX_flash_tag->DDR_FDSB);
                    offset +=4;
                }else if(offset < ART_start) //д��ARB
                {
                    __fill_little_32bit(buf+(offset-completed),0,
                                        DBX_flash_tag->DDR_ARB);
                    offset +=4;
                }else if(offset < ART_end)   //д��ART
                {
                    __fill_little_16bit(buf+(offset-completed),0,
                                DBX_flash_tag->ART_times[(offset-ART_start)/2]);
                    offset +=2;
                }else if(offset < MAT_start)    //�����������
                {
                    offset = MAT_start;
                }else if(offset <= DBX_flash_tag->DDR_size)//д��MAT����
                {//MAT������ݽṹ�ĳ�Ա����һ�����ݽṹ��һ���������attr�Ǵ洢
                //��previousָ��ĸ���λ��nextָ��ĸ���λ�ϲ������Ŀռ䡣��MAT
                //�����ݽṹ��Сֻ��8�ֽڣ�������9�ֽڡ����ڶ��롣
                    //��ȡ��ǰָ��ǰ30λ
                    temp = MAT_item->previous & 0x3fffffff;
                    //��ȡattr�ĵ�1��2λ
                    temp += (uint32_t)MAT_item->attr<<30;
                    //��ǰָ���attr�ĵ�1��2λд��
                    __fill_little_32bit(buf+(offset-completed),0,temp);
                    //��ȡ���ָ��ǰ30λ
                    temp = MAT_item->next & 0x3fffffff;
                    //��ȡattr�ĵ�3.4λ
                    temp += (uint32_t)(MAT_item->attr>>2)<<30;
                    //���ָ���attr�ĵ�3.4λд��
                    __fill_little_32bit(buf+(offset-completed),1,temp);
                    MAT_item++;//׼��д����һ��MAT���������
                    offset +=8;
                }
            }

            block_no = DBL_buf[loop];
            //����Ŀ���
            chip->erase_block(block_no + DBX_flash_tag->start_block);
            write_result = chip->write_data_with_ecc(
                                     block_no + DBX_flash_tag->start_block,
                                     0,chip->block_buf,write_len);    //��д��
            if( write_result == write_len)    //����Ƿ���ȷд��
            {
                __DFFSD_inc_abrasion_times(DBX_flash_tag,block_no);
                writed[loop] = true;    //��ȷд�����Ǳ����Ѿ�д��
            }else
            {//д����󣬲����˻��飬�����д���:
                //block_no���²�������Ŀ�ţ�block_new���滻����ĺÿ���
                block_new = __DFFSD_allocate_block(DBX_flash_tag);
                if(block_new == cn_limit_uint32)
                    return false;       //�ļ�������������������
                writed[loop] = false;   //��ǵ�loop��δд��
                finished = false;       //��ѭ��û�����д��
                //�ѻ���Ǽǵ�����ƽ����У���û��д�뵽flash
                __DFFSD_register_invalid(DBX_flash_tag,block_no);

                //������DBL�е�λ�ã����¿��Ÿ���
                DBL_buf[loop] = block_new;
                //����DBL�д��block_new��λ����DDR���еĿ��
                DDR_no = 4 * loop/chip->block_size;
                writed[DDR_no] = false;

                //����MAT����block_new�������
                MAT_item[block_new].attr = block_attr;
                //����block_new��MAT����λ�ô���DDR�����ǵڼ���
                DDR_no = (MAT_start+block_new*8) / chip->block_size;
                //DDR_main�б�Ǹÿ�Ϊδд��
                DBX_flash_tag->writed_DDR_main[DDR_no] = false;
                //DDR_bak�б�Ǹÿ�Ϊδд��
                DBX_flash_tag->writed_DDR_bak[DDR_no] = false;

                //��MAT����block_no�������Ϊ����
                MAT_item[block_no].attr = cn_invalid_block;
                //����block_no��MAT����λ�ô���DDR�����ǵڼ���
                DDR_no=(MAT_start+block_no*8) / chip->block_size;
                //DDR_main�б�Ǹÿ�Ϊδд��
                DBX_flash_tag->writed_DDR_main[DDR_no] = false;
                //DDR_bak�б�Ǹÿ�Ϊδд��
                DBX_flash_tag->writed_DDR_bak[DDR_no] = false;
            }
        }
    }while(finished == false);
    return true;
}

//----дһ���ļ����DDR��-----------------------------------------------------
//���ܣ���buf�������е�DDR��д��ָ���ļ����DDR��
//����: DBX_flash_tag�����������ļ���flash�ļ����ר�����ݽṹָ��
//���أ�true=�ɹ�ִ�У�false=д�����
//---------------------------------------------------------------------------
bool_t __DFFSD_write_DDR(struct st_DBX_flash_tag *DBX_flash_tag)
{
    struct flash_chip *chip;        //оƬָ��
    bool_t finished;
    uint32_t i,DDR_blocks;
    chip = DBX_flash_tag->chip; //ȡ���ļ�������оƬ

    DDR_blocks = (DBX_flash_tag->DDR_size + chip->block_size -1)
                        / chip->block_size;     //����DDR����ռ�Ŀ���
    do
    {
        finished = true;
        __DFFSD_write_DDR_exe(DBX_flash_tag,0);    //д��DDR_main
        __DFFSD_write_DDR_exe(DBX_flash_tag,1);    //д��DDR_bak
        for(i = 0; i < DDR_blocks; i++)
        {
            if(DBX_flash_tag->writed_DDR_main[i] == false)
            {
                finished = false;
                break;
            }
            if(DBX_flash_tag->writed_DDR_bak[i] == false)
            {
                finished = false;
                break;
            }
        }
    }while(finished == false);  //ֱ��DDR_main��DDR_bakȫ�����
    return true;
}

//----MAT�������--------------------------------------------------------------
//����: ��һ��MAT�����ӳ�һ�飬�����ӵĿ��������β����������û�а�MAT��д��flash��
//      ��д���ļ������ļ��ӳ�����Ҫ���Ӵ洢��ʱ�����ñ������ӳ����ļ���Ӧ��MAT
//      ��ı��
//����: DBX_flash_tag�����������ļ���flash�ļ����ר�����ݽṹָ��
//      last_block_no��ԭ�����е����һ��
//      new_block_no���滻��ţ�����ԭ���ǿ��п�
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_add_MAT_item(struct st_DBX_flash_tag *DBX_flash_tag,
                           uint32_t last_block_no,uint32_t new_block_no)
{
    struct flash_chip *chip = DBX_flash_tag->chip;
    uint32_t i,MAT_start;
    struct MAT_table *MAT_item = DBX_flash_tag->DDR_MAT;

    MAT_start = __DFFSD_MAT_start(DBX_flash_tag);
    //ԭ���һ������ָ��ָ���¿�
    MAT_item[last_block_no].next = new_block_no;
    //�¿����ǰָ��ָ��ԭ���һ��
    MAT_item[new_block_no].previous = last_block_no;
    //�¿�����ָ��ָ���Լ�
    MAT_item[new_block_no].next = new_block_no;
    if(MAT_item[last_block_no].attr == cn_FDT_block)
    {
        //�¿�����Ը�ΪFDT��
        MAT_item[new_block_no].attr = cn_FDT_block;
    }else
    {
        //�¿�����Ը�Ϊ�ļ�������
        MAT_item[new_block_no].attr = cn_file_block_continue;
    }
    //����last_block_no��MAT����λ�ô���DDR�����ǵڼ���
    i=(MAT_start + last_block_no*4*2) / chip->block_size;
    //DDR_bak�б�Ǹÿ�Ϊδд��
    DBX_flash_tag->writed_DDR_bak[i] = false;
    //DDR_main�б�Ǹÿ�Ϊδд��
    DBX_flash_tag->writed_DDR_main[i] = false;

    //����new_block_no��MAT����λ�ô���DDR�����ǵڼ���
    i=(MAT_start +new_block_no*4*2) / chip->block_size;
    //DDR_bak�б�Ǹÿ�Ϊδд��
    DBX_flash_tag->writed_DDR_bak[i] = false;
    //DDR_main�б�Ǹÿ�Ϊδд��
    DBX_flash_tag->writed_DDR_main[i] = false;
}

//----�޸�MAT����--------------------------------------------------------------
//����: ��һ��MAT�����е�һ���滻����һ�飬�����MAT����պ���һ���ļ����׿飬
//      ��Ҫ�޸�FDT��������û�а�MAT��д��flash��
//����: DBX_flash_tag�����������ļ���flash�ļ����ר�����ݽṹָ��
//      old_block_no�����滻�Ŀ�ţ��滻�󲻸ı���ԭ������
//      new_block_no���滻��ţ�����ԭ���ǿ��п�
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_update_MAT_item(struct st_DBX_flash_tag *DBX_flash_tag,
                       uint32_t old_block_no,uint32_t new_block_no)
{
    struct MAT_table *MAT_item = DBX_flash_tag->DDR_MAT;
    struct flash_chip *chip = DBX_flash_tag->chip;  //ȡ�ļ�������оƬ
    uint32_t pre_block_no,next_block_no,temp,DDR_blocks,u32_temp;
    uint32_t MAT_start,FDSB_start;

    if(old_block_no == new_block_no)
        return;
    MAT_start = __DFFSD_MAT_start(DBX_flash_tag);

    //����new_block_no��MAT����λ�ô���DDR�����ǵڼ���
    temp = (MAT_start + new_block_no*4*2) / chip->block_size;
    //DDR_bak�б�Ǹÿ�Ϊδд��
    DBX_flash_tag->writed_DDR_bak[temp] = false;
    //DDR_main�б�Ǹÿ�Ϊδд��
    DBX_flash_tag->writed_DDR_main[temp] = false;

    switch(MAT_item[old_block_no].attr)
    {
        case cn_file_block_start:  //ԭold_block_no�����ļ��׿�
        {
            //new_block_no����Ϊ�ļ��׿�
            MAT_item[new_block_no].attr = cn_file_block_start;
            //new_block_no����ǰָ��ָ��old_block_no����ǰָ��(��fdt��Ŀ��)
            MAT_item[new_block_no].previous=MAT_item[old_block_no].previous;
            //�޸�fdt����Ӧ����Ŀ���������Ҳ���ܻ����__DFFSD_update_MAT_item
            //�����������õ��Ļ������ѭ������Ϊ��ʱ__DFFSD_update_MAT_item��
            //ִ��·����switch--case cn_FDT_block
            __DFFSD_update_FDT_fstart_dson(DBX_flash_tag,
                              MAT_item[new_block_no].previous,new_block_no);

            if(MAT_item[old_block_no].next == old_block_no)
            {//�����׿�����ĩ�飬����һ��ֻ��һ����ļ�
                //new_block_no�����ָ��ָ���Լ�
                MAT_item[new_block_no].next = new_block_no;
            }else
            {//���׿鵫����ĩ��
                next_block_no = MAT_item[old_block_no].next;
                //new_block_no�����ָ��ָ��old_block_no�����ָ��
                MAT_item[new_block_no].next = next_block_no;
                //old_block_no�ĺ�һ�����ǰָ��ָ��new_block_no
                MAT_item[next_block_no].previous = new_block_no;

                //����old_block_no�ĺ�һ����MAT����λ�ô���DDR�����ǵڼ���
                temp = (MAT_start + next_block_no*4*2) / chip->block_size;
                //DDR_bak�б�Ǹÿ�Ϊδд��
                DBX_flash_tag->writed_DDR_bak[temp] = false;
                //DDR_main�б�Ǹÿ�Ϊδд��
                DBX_flash_tag->writed_DDR_main[temp] = false;
            }

        }break;
        case cn_file_block_continue: //ԭold_block_no���ļ�������
        {
            //new_block_no����Ϊ�ļ��׿�
            MAT_item[new_block_no].attr = cn_file_block_continue;
            pre_block_no = MAT_item[old_block_no].previous;
            //new_block_no����ǰָ��ָ��old_block_no����ǰָ��
            MAT_item[new_block_no].previous = pre_block_no;
            MAT_item[pre_block_no].next = new_block_no;

            //����old_block_no��ǰһ����MAT����λ�ô���DDR�����ǵڼ���
            temp = (MAT_start + pre_block_no*4*2) / chip->block_size;
            //DDR_bak�б�Ǹÿ�Ϊδд��
            DBX_flash_tag->writed_DDR_bak[temp] = false;
            //DDR_main�б�Ǹÿ�Ϊδд��
            DBX_flash_tag->writed_DDR_main[temp] = false;
            if(MAT_item[old_block_no].next == old_block_no)
            {//��ĩ��
                //old_block_no��ǰһ������ָ��ָ��new_block_no
                MAT_item[pre_block_no].next = new_block_no;
                //new_block_no�����ָ��ָ���Լ�
                MAT_item[new_block_no].next = new_block_no;

            }else
            {//����ĩ�顣
                next_block_no = MAT_item[old_block_no].next;
                //new_block_no�����ָ��ָ��old_block_no�����ָ��
                MAT_item[new_block_no].next = next_block_no;
                //old_block_no�ĺ�һ�����ǰָ��ָ��new_block_no
                MAT_item[next_block_no].previous = new_block_no;

                //����old_block_no�ĺ�һ����MAT����λ�ô���DDR�����ǵڼ���
                temp = (MAT_start+next_block_no*4*2) / chip->block_size;
                //DDR_bak�б�Ǹÿ�Ϊδд��
                DBX_flash_tag->writed_DDR_bak[temp] = false;
                //DDR_main�б�Ǹÿ�Ϊδд��
                DBX_flash_tag->writed_DDR_main[temp] = false;
            }
        }break;
        case cn_FDT_block : //ԭold_block_no����FDT��
        {
            FDSB_start = (DBX_flash_tag->DDR_size + chip->block_size -1)
                                / chip->block_size * 4;
            //new_block_no����ΪFDT��
            MAT_item[new_block_no].attr = cn_FDT_block;
            if(old_block_no == DBX_flash_tag->DDR_FDSB)        //�׿��־
            {
                DBX_flash_tag->DDR_FDSB = new_block_no;
                //����DDR_FDSB_off��MAT����λ�ô���DDR�����ǵڼ���
                temp = FDSB_start / chip->block_size;
                //DDR_bak�б�Ǹÿ�Ϊδд��
                DBX_flash_tag->writed_DDR_bak[temp] = false;
                //DDR_main�б�Ǹÿ�Ϊδд��
                DBX_flash_tag->writed_DDR_main[temp] = false;
                //new_block_no����ǰָ��ָ���Լ�
                MAT_item[new_block_no].previous = new_block_no;
                if(MAT_item[old_block_no].next == old_block_no)
                {//�����׿�����ĩ�飬fdt��ֻռ��1��
                    MAT_item[new_block_no].next = new_block_no;
                }else       //����ĩ�飬���м��
                {
                    next_block_no = MAT_item[old_block_no].next;  //��һ��Ŀ��
                    //new_block_no�����ָ��ָ��next_block_no
                    MAT_item[new_block_no].next = next_block_no;
                    //��һ�����ǰָ��ָ��new_block_no
                    MAT_item[next_block_no].previous = new_block_no;
                    //����old_block_no�ĺ�һ����MAT����λ�ô���DDR�����ǵڼ���
                    temp = (MAT_start + next_block_no*4*2) / chip->block_size;
                    //DDR_bak�б�Ǹÿ�Ϊδд��
                    DBX_flash_tag->writed_DDR_bak[temp] = false;
                    //DDR_main�б�Ǹÿ�Ϊδд��
                    DBX_flash_tag->writed_DDR_main[temp] = false;
                }
                //new_block_no�����ָ��ָ���Լ�
            }else
            {//�����׿飬��ĩ��
                pre_block_no = MAT_item[old_block_no].previous;
                //new_block_no����ǰָ��ָ��old_block_no����ǰָ��
                MAT_item[new_block_no].previous = pre_block_no;
                //old_block_no��ǰһ������ָ��ָ��new_block_no
                MAT_item[pre_block_no].next = new_block_no;
                //����old_block_no��ǰһ����MAT����λ�ô���DDR�����ǵڼ���
                temp = (MAT_start+pre_block_no*4*2) / chip->block_size;
                //DDR_bak�б�Ǹÿ�Ϊδд��
                DBX_flash_tag->writed_DDR_bak[temp] = false;
                //DDR_main�б�Ǹÿ�Ϊδд��
                DBX_flash_tag->writed_DDR_main[temp] = false;
                if(MAT_item[old_block_no].next == old_block_no)     //��ĩ��
                {
                    //new_block_no�����ָ��ָ���Լ�
                    MAT_item[new_block_no].next = new_block_no;
                }else       //���м��
                {
                    next_block_no = MAT_item[old_block_no].next;
                    //new_block_no�����ָ��ָ���һ��
                    MAT_item[new_block_no].next = next_block_no;
                    //old_block_no�ĺ�һ�����ǰָ��ָ��new_block_no
                    MAT_item[next_block_no].previous = new_block_no;
                    //����old_block_no�ĺ�һ����MAT����λ�ô���DDR�����ǵڼ���
                    temp=(MAT_start+next_block_no*4*2) / chip->block_size;
                    //DDR_bak�б�Ǹÿ�Ϊδд��
                    DBX_flash_tag->writed_DDR_bak[temp] = false;
                    //DDR_main�б�Ǹÿ�Ϊδд��
                    DBX_flash_tag->writed_DDR_main[temp] = false;
                }

            }
        }break;
        case cn_invalid_block : //ԭold_block_no���ǻ��飬���ǲ����ܵ����
        {
        }break;
        case cn_free_block :    //ԭold_block_no���ǿ��п飬Ҳ�ǲ����ܵ�
        {
        }break;
        case cn_DDR_main_block ://ԭold_block_no����DDR_main��
        {
            //new_block_no����ΪDDR_main��
            MAT_item[new_block_no].attr = cn_DDR_main_block;
            DDR_blocks = (DBX_flash_tag->DDR_size + chip->block_size -1)
                                / chip->block_size;     //����DDR����ռ�Ŀ���
            //����DDR_main�Ŀ鱭��������Ҫ��Ӧ��DBL��
            for(temp = 0; temp < DDR_blocks; temp++)
            {
                u32_temp = DBX_flash_tag->DDR_DBL[temp];
                if(u32_temp == old_block_no)
                {
                    DBX_flash_tag->DDR_DBL[temp] = new_block_no;
                    break;
                }
            }
            //����DBL���޸ĵ�λ�ô���DDR�����ǵڼ���
            temp= temp*4 / chip->block_size;
            //DDR_main�б�Ǹÿ�Ϊδд��
            DBX_flash_tag->writed_DDR_main[temp] = false;
        }break;
        case cn_DDR_bak_block : //ԭold_block_no����DDR_bak��
        {
            //new_block_no����ΪDDR_main��
            MAT_item[new_block_no].attr = cn_DDR_bak_block;
            DDR_blocks = (DBX_flash_tag->DDR_size + chip->block_size -1)
                                / chip->block_size;     //����DDR����ռ�Ŀ���
            for(temp = 0; temp < DDR_blocks; temp++)
            {
                u32_temp = DBX_flash_tag->DDR_DBL[DDR_blocks + temp];
                if(u32_temp == old_block_no)
                    DBX_flash_tag->DDR_DBL[DDR_blocks + temp] = new_block_no;
            }
            //����DDR���޸ĵ�λ�ô���DDR�����ǵڼ���
            temp= (DDR_blocks+ temp)*4 / chip->block_size;
            //DDR_main�б�Ǹÿ�Ϊδд��
            DBX_flash_tag->writed_DDR_bak[temp] = false;
        }break;
        case cn_PCRB_block :     //ԭold_block_no���ǵ���ָ��飬��˵��
        {
        }break;
        default : break;
    }
    //���ԭold_block_no��ĳ�ļ����׿飬��Ҫ�޸�fdt(Ŀ¼)���и��ļ���Ӧ����Ŀ
    //���ԭold_block_no��fdt����׿飬��Ҫ�޸�DDR���е�FDSB
}

//----ĥ��ƽ�����ת-----------------------------------------------------------
//���ܣ�busy_region[0]��free_region[0]���յ�ʱ��ĥ��ƽ������תһȦ��ʹ
//      busy_region[8]��free_region[8]��ɿյġ�
//����: DBX_flash_tag�����������ļ���flash�ļ����ר�����ݽṹָ��
//���أ���
//-----------------------------------------------------------------------------
void __DFFSD_whirl_ART_table(struct st_DBX_flash_tag *DBX_flash_tag)
{
    struct flash_chip *chip = DBX_flash_tag->chip;
    uint32_t *busy_region,*free_region,*balance_region;
    uint32_t loop,ARB_block;
    uint16_t whirl_regions;
    uint16_t *times_tab;
    times_tab = DBX_flash_tag->ART_times;               //ȡĥ�������
    busy_region = &DBX_flash_tag->balance_region[9];    //��һ��æ����
    free_region = DBX_flash_tag->balance_region;        //��һ�����з���
    balance_region = DBX_flash_tag->balance_region;
    for(whirl_regions = 0; whirl_regions < 8; whirl_regions++)
    {//�������ת����������������busy��free��0����ʼ�����free��busyͬʱΪ�յ�
     //����������ֱ����һ����ͬʱΪ�յ���Ϊֹ��
        if((busy_region[whirl_regions] != busy_region[whirl_regions+1])
              || (free_region[whirl_regions] != free_region[whirl_regions+1]))
            break;
    }
    if(whirl_regions == 0)
        return;
    DBX_flash_tag->DDR_ARB += whirl_regions * 128;//ĥ���������������ת����
    //����DDR_ARB��MAT����λ�ô���DDR�����ǵڼ���
    ARB_block = (DBX_flash_tag->DDR_size + chip->block_size -1)
                    / chip->block_size * 4 + 4;
    ARB_block = ARB_block / chip->block_size;
    //DDR_bak�б�Ǹÿ�Ϊδд��
    DBX_flash_tag->writed_DDR_bak[ARB_block] = false;
    //DDR_main�б�Ǹÿ�Ϊδд��
    DBX_flash_tag->writed_DDR_main[ARB_block] = false;
    for(loop = 0; loop < balance_region[18]; loop++)
    { //���п��ĥ�����������ת����*128
        times_tab[loop] -= whirl_regions*128;  
    }
    for(loop = 1; loop <= 9 - whirl_regions; loop++)
    {//ÿ��æ����������ת��whirl������ʹæ�����������ʼ���㣬whirl������ɿյ�
        busy_region[loop] = busy_region[loop+whirl_regions];
        free_region[loop] = free_region[loop+whirl_regions];
    }
    for(; loop < 9; loop++)
    {//��ԭ��æ��������8������Ϣת�Ƶ���loop��������æ���յ�8��Ϊ��
        busy_region[loop] = balance_region[18];
        free_region[loop] = busy_region[0];
    }
}

//----ĥ�������1--------------------------------------------------------------
//����: ��ĳ���ĥ���������ʱ�����ñ������������ֱ仯�Ǽǵ��ڴ���ĥ��ƽ����У�
//      ������д�뵽flash��
//����: DBX_flash_tag�����������ļ���flash�ļ����ר�����ݽṹָ��
//      block_no�������Ӵ����Ŀ�
//����: ��
//----------------------------------------------------------------------------
void __DFFSD_inc_abrasion_times(struct st_DBX_flash_tag *DBX_flash_tag,
                                uint32_t block_no)
{
    uint32_t temp,block_position;
    uint32_t *pos_tab;
    uint16_t *times_tab;
    uint32_t *busy_region,*free_region;
    uint16_t block_times;

    pos_tab = DBX_flash_tag->ART_position;      //ȡĥ��λ�ñ�
    times_tab = DBX_flash_tag->ART_times;       //ȡĥ�������
    //ȡĿ�����ĥ���ű�(������)�е�λ��
    block_position = pos_tab[block_no];
    times_tab[block_position]++;   //Ŀ����ĥ���������
    block_times = times_tab[block_position];  //ȡĿ����ĥ�����
    if((block_times % 128 != 0)||(block_times >= 1024))
    {
        return;                         //���Ӵ����󲻿���,ֱ�ӷ��ء�
    }
    //���洦����Ҫ���������,block_no��Ҫ�ŵ�ĥ�������һ��
    busy_region = &DBX_flash_tag->balance_region[9];    //��һ��æ����
    temp = block_times/128 - 1;                    //ȡblock_no�����ڵ�ĥ�����
    //��block_no��æ�黹���п飬ֻ��æ��Ż����Ӵ���������ж�ֻ���ݴ����
    if(block_position >= busy_region[0])
        temp += 9;     //block_no��æ�飬����9������
    __DFFSD_ART_flip(DBX_flash_tag,temp,temp+1,block_position);

    busy_region = &DBX_flash_tag->balance_region[9];    //��һ��æ����
    free_region = DBX_flash_tag->balance_region;        //��һ�����з���
    if((busy_region[0] == busy_region[1]) &&(free_region[0] == free_region[1]))
        //������free[0]��busy[0]ͬʱ�յ������ĥ������תһȦ
        __DFFSD_whirl_ART_table(DBX_flash_tag);
}

//----�Ǽ�һ������-------------------------------------------------------------
//���ܣ���һ������Ǽǵ��ڴ��е�ĥ��ƽ����У�������ֻ�޸��ڴ��еĲ���ƽ���
//      ����д��flash���û���ȷ�����ñ����������ʵ���ʱ��д��flash��
//����: DBX_flash_tag�����������ļ���flash�ļ����ר�����ݽṹָ��
//      block_no��������
//���أ���
//-----------------------------------------------------------------------------
void __DFFSD_register_invalid(struct st_DBX_flash_tag *DBX_flash_tag,
                            uint32_t block_no)
{
    struct flash_chip *chip = DBX_flash_tag->chip;    //ȡ���ļ�������оƬ
    uint32_t loop,block_position,block_no_temp;
    uint32_t *pos_tab,*block_no_tab;
    uint16_t *times_tab;
    uint32_t *balance_region,*busy_region;
    uint16_t block_times;

    pos_tab = DBX_flash_tag->ART_position;        //ȡĥ��λ�ñ�
    block_no_tab = DBX_flash_tag->ART_block_no;   //ȡĥ���ű��������ͬ��
    times_tab = DBX_flash_tag->ART_times;         //ȡĥ�������
    balance_region = DBX_flash_tag->balance_region; //ĥ���������ַ
    busy_region = &DBX_flash_tag->balance_region[9];    //��һ��æ����
    //ȡ������ĥ���ű�(������)�е�λ��
    block_position = pos_tab[block_no];
    block_times = times_tab[block_position];   //�ݴ滵���ĥ�����
    loop = block_times/128 +1;        //ȡ�����ĥ���������һ������
    //������ԭ��æ�黹���п飬���������ȷ�����ʹ�ã�����������ʹ���в�����ʹ��
    //�еĿ�϶���æ�飬��һ�㲻�����п��ɻ�����������һ�´��
    if(block_position >= busy_region[0])
        loop += 9;     //������æ�飬����9����
    for(; loop < 19; loop++)      //��������ĥ��������ѻ����ԭ����ת�Ƶ��������
    {
        balance_region[loop] -= 1;    //����ָ������һ��
        if(balance_region[loop] == balance_region[loop-1])
        {//������������ָ���ص���˵����i-1�����ǿյġ�
            balance_region[loop]--;
            continue;   //����ᶯ�飬������һ�ֱȽϡ�
        }
        //ȡ��i�����ĵ�һ����Ԫ�Ŀ��(����pos���е�ƫ����)
        block_no_temp = block_no_tab[balance_region[loop]];
        //��i������һ����Ԫ��ĥ�����copy���ϴ�ѭ���ƿյĵ�Ԫ
        times_tab[block_position] = times_tab[balance_region[loop]];
        //��i������һ����Ԫ�Ŀ��copy���ϴ�ѭ���ƿյĵ�Ԫ
        block_no_tab[block_position] = block_no_temp;
        //��i������һ����Ԫ��λ�ñ�ָ��Ŀ�굥Ԫ��
        pos_tab[block_no_temp] = block_position;
        block_position = balance_region[loop];    //������յĵ�Ԫ������
    }
    //����Ĳ�������copy������������׵�Ԫ
    times_tab[block_position] = block_times;
    //�����ŷŵ���������׵�Ԫ��ART_block_no����
    block_no_tab[block_position] = block_no;
    //pos��Ļ���ָ��ָ�򻵿�����׵�Ԫ
    pos_tab[block_no] = block_position;

    //����block_no��Ϊ����
    DBX_flash_tag->DDR_MAT[block_no].attr = cn_invalid_block;
    //����block_new��MAT����λ�ô���DDR�����ǵڼ���
    loop =(__DFFSD_MAT_start(DBX_flash_tag)+block_no*4*2) / chip->block_size;
    DBX_flash_tag->writed_DDR_bak[loop] = false;     //DDR_bak�б�Ǹÿ�Ϊδд��
    //DDR_main�б�Ǹÿ�Ϊδд��
    DBX_flash_tag->writed_DDR_main[loop] = false;
}

//----�ͷ�һ�鵽free��--------------------------------------------------------
//����: ��һ���busy��ת�Ƶ�free��
//����: DBX_flash_tag�����������ļ���flash�ļ����ר�����ݽṹָ��
//      block_no���ͷŵĿ��
//���أ���
//----------------------------------------------------------------------------
void __DFFSD_free_block(struct st_DBX_flash_tag *DBX_flash_tag,
                        uint32_t block_no)
{
    uint32_t block_position,i;
    uint32_t *pos_tab;
    uint32_t target_region;
	struct flash_chip *chip = DBX_flash_tag->chip;
    
    pos_tab = DBX_flash_tag->ART_position;
    block_position = pos_tab[block_no];
    //���һ��æ�����׿�Ƚ�
    if(block_position < DBX_flash_tag->balance_region[9])
        return;     //���ͷſ鱾�������п飬�����ͷ�
    else
        DBX_flash_tag->free_sum++;
    //���ͷſ�����š�
    target_region = DBX_flash_tag->ART_times[block_position]/128;
    //��Ŀ����Ƶ�������
    __DFFSD_ART_flip(DBX_flash_tag,target_region+9,target_region,block_position);
    //ֻ�ѿ����Ը�Ϊ���У����޸���������޶ȱ������ݻָ��Ŀ��ܡ�
    DBX_flash_tag->DDR_MAT[block_no].attr = cn_free_block;
    
    //����MAT����block_no�ĵ�ַ��DDR���е�ƫ����
    i = DBX_flash_tag->DDR_size-(DBX_flash_tag->block_sum-block_no)*4*2;
    //����MAT���е�block_no�ĵ�ַ��DDR���е�ƫ�ƿ��
    i = i / chip->block_size;
    DBX_flash_tag->writed_DDR_main[i] = false;
    //ֻҪDDR�������иı䣬��þ�����д��
    __DFFSD_write_DDR(DBX_flash_tag);  
}

//----������Ŀ-----------------------------------------------------------------
//����: ��������ART��Ŀ��
//����: DBX_flash_tag�����������ļ���flash�ļ����ר�����ݽṹָ��
//      pos1,DBX_flash_tag->ART_block_no�����Ŀ��
//      pos2,DBX_flash_tag->ART_block_no�����һ����Ŀ��
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_exchange_ART_item(struct st_DBX_flash_tag *DBX_flash_tag,
                                uint32_t pos1,uint32_t pos2)
{
    uint32_t block_no1,block_no2;
    uint16_t ART_times;
    block_no1 = DBX_flash_tag->ART_block_no[pos1];
    block_no2 = DBX_flash_tag->ART_block_no[pos2];
    DBX_flash_tag->ART_block_no[pos1] = block_no2;
    DBX_flash_tag->ART_block_no[pos2] = block_no1;
    DBX_flash_tag->ART_position[block_no1] = pos2;
    DBX_flash_tag->ART_position[block_no2] = pos1;
    ART_times = DBX_flash_tag->ART_times[pos1];
    DBX_flash_tag->ART_times[pos1] = DBX_flash_tag->ART_times[pos2];
    DBX_flash_tag->ART_times[pos2] = ART_times;
}

//----ĥ���ű�����-----------------------------------------------------------
//����: �Ѵ�flashоƬ�ж�����δ�����ART�����ڴ������򣬰�������д��
//      DBX_flash_tag��������:ART_position,ART_block_no��ART_times�С�
//      ��ִ��ǰ��ǰĥ����ǰ���������
//      �����Ŀ�ű�:
//      ĥ�����   0~127�Ŀ��п���ʼλ��----free_region[0],balance_region[0]
//      ĥ�����   0~127�Ŀ��п����λ��
//      ĥ����� 128~255�Ŀ��п���ʼλ��----free_region[1],balance_region[1]
//      ĥ����� 128~255�Ŀ��п����λ��
//      ĥ����� 256~383�Ŀ��п���ʼλ��----free_region[2],balance_region[2]
//      ĥ����� 256~383�Ŀ��п����λ��
//      ĥ����� 384~511�Ŀ��п���ʼλ��----free_region[3],balance_region[3]
//      ĥ����� 384~511�Ŀ��п����λ��
//      ĥ����� 512~639�Ŀ��п���ʼλ��----free_region[4],balance_region[4]
//      ĥ����� 512~639�Ŀ��п����λ��
//      ĥ����� 640~767�Ŀ��п���ʼλ��----free_region[5],balance_region[5]
//      ĥ����� 640~767�Ŀ��п����λ��
//      ĥ����� 768~895�Ŀ��п���ʼλ��----free_region[6],balance_region[6]
//      ĥ����� 768~895�Ŀ��п����λ��
//      ĥ�����896~1023�Ŀ��п���ʼλ��----free_region[7],balance_region[7]
//      ĥ�����896~1023�Ŀ��п����λ��
//      ĥ�����   >1023�Ŀ��п���ʼλ��----free_region[8],balance_region[8]
//      ĥ�����   >1023�Ŀ��п����λ��
//      ĥ�����   0~127��æ����ʼλ��------busy_region[0],balance_region[9]
//      ĥ�����   0~127��æ�����λ��
//      ĥ����� 128~255��æ����ʼλ��------busy_region[1],balance_region[10]
//      ĥ����� 128~255��æ�����λ��
//      ĥ����� 256~383��æ����ʼλ��------busy_region[2],balance_region[11]
//      ĥ����� 256~383��æ�����λ��
//      ĥ����� 384~511��æ����ʼλ��------busy_region[3],balance_region[12]
//      ĥ����� 384~511��æ�����λ��
//      ĥ����� 512~639��æ����ʼλ��------busy_region[4],balance_region[13]
//      ĥ����� 512~639��æ�����λ��
//      ĥ����� 640~767��æ����ʼλ��------busy_region[5],balance_region[14]
//      ĥ����� 640~767��æ�����λ��
//      ĥ����� 768~895��æ����ʼλ��------busy_region[6],balance_region[15]
//      ĥ����� 768~895��æ�����λ��
//      ĥ�����896~1023��æ����ʼλ��------busy_region[7],balance_region[16]
//      ĥ�����896~1023��æ�����λ��
//      ĥ�����   >1023��æ����ʼλ��------busy_region[8],balance_region[17]
//      ĥ�����   >1023��æ�����λ��
//      ������ʼλ��------------------------balance_region[18]
//      �������λ��
//����: DBX_flash_tag�����������ļ���flash�ļ����ר�����ݽṹָ��
//����: ��
//-----------------------------------------------------------------------------
void __DFFSD_sort_ART(struct st_DBX_flash_tag *DBX_flash_tag)
{
    struct flash_chip *chip = DBX_flash_tag->chip;
    uint16_t *times_tab;
    uint32_t loop1,temp,invalid_offset,loop;
    uint32_t ART_offset,free_offset,block_no;
    uint32_t *pos_tab;
    uint32_t *busy_region,*free_region;
    ART_offset = 4*(DBX_flash_tag->DDR_size + chip->block_size -1)
                            /chip->block_size   //DBL
                  + 8;        //FDSB �� ARB
    pos_tab = DBX_flash_tag->ART_position;      //ĥ��λ�ñ�
    times_tab = DBX_flash_tag->ART_times;       //ĥ�������
    invalid_offset = DBX_flash_tag->block_sum;  //����λ��ָ��ĥ���ײ�
    loop = invalid_offset;
    do  //��һ��ɨ�裬��ȫ�������Ƶ�ĥ����ĩ��
    {
        loop--;
        block_no = pos_tab[loop];
        if(DBX_flash_tag->DDR_MAT[block_no].attr == cn_invalid_block) //����
        {
            invalid_offset--;
            __DFFSD_exchange_ART_item(DBX_flash_tag,loop,invalid_offset);
        }
    }while (loop != 0);
    DBX_flash_tag->balance_region[18] = invalid_offset;
    DBX_flash_tag->valid_sum = invalid_offset;
    free_offset = 0;            //�п��ƫ��
    do  //���п�ŵ�balance_times����ˣ�æ��ŵ�ĩ�ˣ��Ȳ�����
    {
        block_no = pos_tab[loop];
        if(DBX_flash_tag->DDR_MAT[loop].attr == cn_free_block)
        {
            __DFFSD_exchange_ART_item(DBX_flash_tag,loop,free_offset);
            free_offset++;
        }
        loop++;
    }while (loop < invalid_offset);

    //���ˣ�free_offset=�п�����,invalid_offset=�������ף�free_offset=æ������
    //�����free������
    DBX_flash_tag->free_sum = free_offset;
    ART_offset = 0;
    free_region = DBX_flash_tag->balance_region;
    for(loop1 = 0; loop1 < 8; loop1++)  //ɨ��8������
    {
        temp = 128*(loop1+1);
        free_region[loop1] = ART_offset;
        for(loop= ART_offset; loop < free_offset; loop++)
        {
            if(times_tab[loop] < temp)
            {
                //����block_no_tab���е�loop��ART_offset��Ŀ
                __DFFSD_exchange_ART_item(DBX_flash_tag,loop,ART_offset);
                ART_offset++;
            }else
            {
                ;   //ʲô����������ART_offsetά��ԭλ
            }
        }
    }
    free_region[8] = ART_offset; //�г�����ָ�򳬹�1024�ĵ�һ����
    //�����busy������������DDR���ڣ��ض���һ�����ϵ�æ�飬�����账��æ������
    //Ϊ0���������
    ART_offset = free_offset;
    busy_region = &DBX_flash_tag->balance_region[9];
    for(loop1 = 0; loop1 < 8; loop1++)      //ɨ��8��æ��
    {
        temp = 128*(loop1+1);
        busy_region[loop1] = ART_offset;
        for(loop= ART_offset; loop < DBX_flash_tag->valid_sum; loop++)
        {
            if(DBX_flash_tag->ART_times[loop] < temp)
            {
                //����block_no_tab���е�loop��ART_offset��Ŀ
                __DFFSD_exchange_ART_item(DBX_flash_tag,loop,ART_offset);
                ART_offset++;
            }else
            {
                ;   //ʲô����������ART_offsetά��ԭλ
            }
        }
    }
    busy_region[8] = ART_offset; //æ������ָ�򳬹�1024�ĵ�һ����
    return ;
}

//----����DDR��ߴ�------------------------------------------------------------
//���ܣ��������ڴ�����1��DDR������Ҫ���ֽ���.��flash�е�1��DDR���һ��DBL��
//����: DBX_flash_tag�����������ļ���flash�ļ����ר�����ݽṹָ��
//����: �ڴ澵����Ҫ���ֽ���
//----------------------------------------------------------------------------
uint32_t __DFFSD_calculate_DDR_size(struct st_DBX_flash_tag *DBX_flash_tag)
{
    uint32_t size,blocks,fill,flash_MAT;
    uint64_t u64i;
    struct flash_chip *chip = DBX_flash_tag->chip;
    //�������1��DDR��ߴ�,�����μ����ļ���ͷ���ֵ�˵��
    //����DDR��ߴ磬������С��
    size =    4                                     //ECCF ʹ��eccУ���־
            + 4                                     //ADBĥ���������
            + 2*DBX_flash_tag->block_sum            //ARTĥ�������
            + 4                                     //FDSB,fdt���׿���
            + 4*2*DBX_flash_tag->block_sum;         //mat��
    //��ʽ�˷��������32λ�������������64λ����
    u64i = chip->block_size * size + 4 * (chip->block_size-1);
    size = (uint32_t)(u64i / (chip->block_size-4));
    //��ʽ�õ���result���ܻ��ʵ����Ҫ��(0~3),�������֮
    size = align_down(4,size);
    if(size > chip->block_size)       //����1�飬�迼�ǵ�����ĿԽ��߽�����
    {//DDR������һ�����������һ����Ŀ���ܿ��߽磬�������֮��
     //�����Ĺ����п�������DDR_size�仯������������DBL���ȱ仯
     //1��DBL�����ʼƫ��Ϊ0������̶�Ϊ4�ֽڣ�������DBL��ռ���ٿ飬
     //   �϶������п�߽���
     //2��DBL�������4�ֽڶ����ַ��FDSB�����ܿ�߽硣
     //2��FDSB������4�ֽڶ����ַ��ĥ��������������ܿ�߽硣
     //3��ĥ���������������4�ֽڶ����ַ��ĥ����������ܿ��
     //4��ĥ�������������ֽڶ����ַ����MATҪ��8�ֽڶ��룬����Ҫ������
        blocks = (size +chip->block_size -1 ) / chip->block_size;   //����
        flash_MAT = 4*blocks                        //DBL
                    + 4                             //ECCF
                    + 4 + 4                         //ARB+FDSB
                    + 2*DBX_flash_tag->block_sum;   //ART
        fill = align_up_sys(flash_MAT) - flash_MAT;  //��������
        if((fill + size)/chip->block_size != size/chip->block_size)
        {//����������˿���
            size += fill;
            if(fill < 4)   //�������µĿ�λ����һ��DBL��
                size = size + 8;
        }else
        {
            size += fill;
        }
    }
    return size;
}

#endif //#if ((cfg_djyfs == 1) && (cn_run_mode != cn_mode_tiny) && (cfg_djyfs_flash == 1))
