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
// ģ������: NORFLASH�쳣�洢����
// ģ��汾: V1.00
// ������Ա: Administrator
// ����ʱ��: 1:59:10 PM/May 9, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
// ��ע:�����쳣�Ĳ������ǽ�ֹ���ȵģ���˸÷��������ٽ�����Դ�����ı���
#include "stdint.h"
#include "stddef.h"
#include "stdio.h"
#include "endian.h"

#include "os.h"

#include "exp_api.h"

#define CN_RECORD_ADDRBASE   0XFFB00000
#define CN_RECORD_SIZE       0X100000


//���ļ�ʹ�õ�һЩ�궨��
#define CN_EXP_RECORD_INVALID_ADDR 0xFFFFFFFF
#define CN_EXP_RECORD_BLANKFLAG    0xFFFFFFFF
#define CN_EXP_RECORD_MAGICNUMBER  0x33333333

//���ļ����е����ݽṹ
//�洢�쳣��Ϣ��ͷ���洢�ķ�ʽ���洢ͷ+��Ϣ�ķ�ʽ������ʽ
 struct tagExpNorRecordItemHead
{
    u32 recordendian;   //�洢�Ĵ�С��
    u32 magicnumber;    //�Լ�����������
    u32 expinfoaddr;    //�쳣��Ϣ��ַ
    u32 expinfolen;     //�쳣��Ϣ����
    u32 itemnextaddr;   //��һ����ַ
    //��¼�������ֵ���Ϣ,�����Ժ����Ϣ���ˣ���ַ��ָ���Ǵ洢��ַ
    u32 exp_headinfoaddr;       //ͷ��Ϣ��ַ
    u32 exp_headinfolen;        //ͷ��Ϣ����
    u32 exp_osstateinfoaddr;    //OS״̬��Ϣ��ַ
    u32 exp_osstateinfolen;     //OS״̬��Ϣ����
    u32 exp_hookinfoaddr;       //HOOK��Ϣ��ַ
    u32 exp_hookinfolen;        //HOOK��Ϣ����
    u32 Exp_Throwinfoaddr;      //�׳���Ϣ��ַ
    u32 Exp_Throwinfolen;       //�׳���Ϣ����
};
static u32  s_u32ExpRecordValidAddrNor = 0;//���д洢����ַ
static u32  s_u32ExpRecordValidLenNor = 0; //���д洢������
static u32  s_u32ExpRecordItemNumNor = 0;  //�Ѿ��洢���쳣��Ŀ��

//flash��д
extern uint8_t Flash_EraseAddr(u32 addr, u32 len);
extern uint32_t Flash_ReadData(uint32_t addr, uint8_t *buf, uint32_t len);
extern uint32_t Flash_WriteData(uint32_t addr, uint8_t *buf, uint32_t len);

// =============================================================================
// ��������:_exp_norrecordheadswap
//         �洢��Ϣͷ�Ĵ�С��ת��
// �������:itemhead,�洢��Ϣͷ
// �������:itemhead
// ����ֵ  :��
// ˵��    :
// =============================================================================
void _exp_norrecordheadswap(struct tagExpNorRecordItemHead *itemhead)
{
    itemhead->expinfoaddr = swapl(itemhead->expinfoaddr);
    itemhead->expinfolen = swapl(itemhead->expinfolen);
    itemhead->itemnextaddr = swapl(itemhead->itemnextaddr);
    itemhead->magicnumber = swapl(itemhead->magicnumber);
    itemhead->recordendian = swapl(itemhead->recordendian);
}
// =============================================================================
// ��������:Exp_Recordscan_nor
//         ɨ���쳣�洢�������¿��õ�ַ�Լ��Ѿ����ڵ���Ϣ��Ŀ
// �������:��
// �������:��
// ����ֵ  :��
// ˵��    :CN_RECORD_SIZE�����õĴ洢�����ȣ�
//        CN_RECORD_ADDRBASE�����õĴ洢������ַ�������NORFLASH��ƫ�Ƶ�ַ��
// =============================================================================
void Exp_Recordscan_nor(void)
{
    bool_t continuescan;
    u32 readaddr;
    u32 readlen;
    u32 hasread;
    u32 itemnum;
    struct tagExpNorRecordItemHead  itemhead;

    readaddr = CN_RECORD_ADDRBASE;
    continuescan = true;
    readlen = sizeof(itemhead);
    itemnum = 0;
    while(continuescan)  //ɨ��ֱ�����ֿ���������ߵ���洢�����β
    {
        if((readaddr)>= (CN_RECORD_ADDRBASE + CN_RECORD_SIZE - readlen))
        {
           //�洢��ɨ�����
            s_u32ExpRecordValidAddrNor = readaddr;
            s_u32ExpRecordValidLenNor = CN_RECORD_ADDRBASE + CN_RECORD_SIZE -readaddr;
            s_u32ExpRecordItemNumNor =itemnum ;
            continuescan = false;
        }
        else
        {
            hasread = Flash_ReadData(readaddr, (u8 *)&itemhead,readlen);
            if(readlen != hasread)
            {
                printk("__exp_norrecordscan:scanerr:readaddr = 0x%08x\n\r",readaddr);
                printk("__exp_norrecordscan:readlen = 0x%08x,hasread = 0x%08x\n\r",\
                            readlen, hasread);
                //ɨ��ʧ��
                s_u32ExpRecordValidAddrNor = CN_RECORD_ADDRBASE + CN_RECORD_SIZE;
                s_u32ExpRecordValidLenNor = 0;
                s_u32ExpRecordItemNumNor = 0xFFFFFFFF;
                continuescan = false;
            }
            else
            {
                //��С����Ϊ��׼��Ҫ����ΪС����0����С�˶�һ��
                if(((itemhead.recordendian == CN_CFG_LITTLE_ENDIAN)&&(CN_CFG_BYTE_ORDER != CN_CFG_LITTLE_ENDIAN))||\
                   ((itemhead.recordendian != CN_CFG_LITTLE_ENDIAN)&&(CN_CFG_BYTE_ORDER == CN_CFG_LITTLE_ENDIAN)))
                {
                    //��Ҫ��С��ת��
                    _exp_norrecordheadswap(&itemhead);
                }
                if(CN_EXP_RECORD_MAGICNUMBER != itemhead.magicnumber)//���е�
                {
                    s_u32ExpRecordValidAddrNor = readaddr;
                    s_u32ExpRecordValidLenNor = CN_RECORD_ADDRBASE + CN_RECORD_SIZE -readaddr;
                    s_u32ExpRecordItemNumNor =itemnum ;
                    continuescan = false;
                }
                else
                {
                    itemnum ++;
                    readaddr = itemhead.itemnextaddr;
                    continuescan = true;
                }
            }
        }
    }
}
// =============================================================================
// �������ܣ�__exp_checkrecordpara_nor
//           ���洢����
// ���������recordpara���洢����
// ���������
// ����ֵ  ��true,�Ϸ�������false�����Ϸ�����
// =============================================================================
bool_t  __exp_checkrecordpara_nor(struct tagExpRecordPara *recordpara)
{
    bool_t result;

    result = true;
    if((NULL == recordpara) ||\
       (recordpara->headinfolen > s_u32ExpRecordValidLenNor)||\
       (recordpara->hookinfolen > s_u32ExpRecordValidLenNor)||\
       (recordpara->osstateinfolen > s_u32ExpRecordValidLenNor)||\
       (recordpara->throwinfolen > s_u32ExpRecordValidLenNor))
    {
        result = false;
    }
    else
    {
        if((recordpara->headinfoaddr == 0) &&(recordpara->headinfolen != 0))//��һ��
        {
            result = false;
        }
        if((recordpara->hookinfoaddr == 0) &&(recordpara->hookinfolen != 0))//��һ��
        {
            result = false;
        }
        if((recordpara->osstateinfoaddr== 0) &&(recordpara->osstateinfolen != 0))//��һ��
        {
            result = false;
        }
        if((recordpara->throwinfoaddr == 0) &&(recordpara->throwinfolen!= 0))//��һ��
        {
            result = false;
        }
    }
    return result;
}
// =============================================================================
// �������ܣ�Exp_Record_nor
//           ��¼һ֡�쳣��Ϣ
// ���������recordpara����Ҫ��¼���쳣��Ϣ
// ���������
// ����ֵ  ����en_sysExp_Record_result����
// =============================================================================
u32 Exp_Record_nor(struct tagExpRecordPara *recordpara)
{
    u32 infolen = 0;
    u32 itemlen = 0;
    u32 recordaddr = 0;
    u32 recordlen = 0;
    u32 haswrite = 0;
    u8* infobuf;
    u32 result;
    struct tagExpNorRecordItemHead  itemhead;
    if(false == __exp_checkrecordpara_nor(recordpara))
    {
        printk("exp_norrecord:invalid recordpara\n\r");
        result =EN_EXP_DEAL_RECORD_PARAERR;
    }
    else
    {
        infolen = recordpara->headinfolen + recordpara->hookinfolen + \
                  recordpara->osstateinfolen + recordpara->throwinfolen;
        itemlen = sizeof(itemhead) + infolen;
        if(itemlen > s_u32ExpRecordValidLenNor)
        {
            result = EN_EXP_DEAL_RECORD_NOSPACE;
        }
        else
        {
            result = EN_EXP_DEAL_RECORD_SUCCESS;

            infobuf = (u8*)&itemhead;
            recordaddr = s_u32ExpRecordValidAddrNor;
            recordlen = sizeof(itemhead);
            //�洢����ṹ(���洢����ͷ)
            itemhead.recordendian = CN_CFG_BYTE_ORDER;
            itemhead.magicnumber = CN_EXP_RECORD_MAGICNUMBER;
            itemhead.expinfolen = infolen;
            itemhead.expinfoaddr = recordaddr+sizeof(itemhead);
            itemhead.itemnextaddr = recordaddr + itemlen;
            itemhead.exp_headinfoaddr = itemhead.expinfoaddr;
            itemhead.exp_headinfolen = recordpara->headinfolen;
            itemhead.exp_osstateinfoaddr = itemhead.exp_headinfoaddr+ itemhead.exp_headinfolen;
            itemhead.exp_osstateinfolen = recordpara->osstateinfolen;
            itemhead.exp_hookinfoaddr = itemhead.exp_osstateinfoaddr + itemhead.exp_osstateinfolen ;
            itemhead.exp_hookinfolen = recordpara->hookinfolen;
            itemhead.Exp_Throwinfoaddr = itemhead.exp_hookinfoaddr+ itemhead.exp_hookinfolen;
            itemhead.Exp_Throwinfolen= recordpara->throwinfolen;

            haswrite = Flash_WriteData(recordaddr, infobuf, recordlen);
            if(haswrite != recordlen)
            {
                result = EN_EXP_DEAL_RECORD_HARDERR;
            }
            recordaddr = recordaddr + recordlen;

            //���¾�̬����
            s_u32ExpRecordValidAddrNor += itemlen;
            s_u32ExpRecordValidLenNor  -= itemlen;
            s_u32ExpRecordItemNumNor++;

            //��ʼ��¼INFOHEAD
            infobuf = (u8*)recordpara->headinfoaddr;
            recordlen = recordpara->headinfolen;
            haswrite = Flash_WriteData(recordaddr, infobuf, recordlen);
            if(haswrite != recordlen)
            {
                result = EN_EXP_DEAL_RECORD_HARDERR;
            }
            recordaddr = recordaddr + recordlen;
            //��¼OSSTATEINFO
            infobuf = (u8*)recordpara->osstateinfoaddr;
            recordlen = recordpara->osstateinfolen;
            haswrite = Flash_WriteData(recordaddr, infobuf, recordlen);
            if(haswrite != recordlen)
            {
                result = EN_EXP_DEAL_RECORD_HARDERR;
            }
            recordaddr = recordaddr + recordlen;

            //��¼HOOKINFO
            infobuf = (u8*)recordpara->hookinfoaddr;
            recordlen = recordpara->hookinfolen;
            haswrite = Flash_WriteData(recordaddr, infobuf, recordlen);
            if(haswrite != recordlen)
            {
                result = EN_EXP_DEAL_RECORD_HARDERR;
            }
            recordaddr = recordaddr + recordlen;
            //��¼THROWINFO
            infobuf = (u8*)recordpara->throwinfoaddr;
            recordlen = recordpara->throwinfolen;
            haswrite = Flash_WriteData(recordaddr, infobuf, recordlen);
            extern bool_t Flash_OpetionIsOK(void);
            Flash_OpetionIsOK();
            if(haswrite != recordlen)
            {
                result = EN_EXP_DEAL_RECORD_HARDERR;
            }
            recordaddr = recordaddr + recordlen;

            if(recordaddr != s_u32ExpRecordValidAddrNor)
            {
                printk("exp_norrecord:logic err\n\r");
            }
        }
    }
    return result;
}
// =============================================================================
// �������ܣ�Exp_RecordClear_nor
//          ������е��쳣��Ϣ������쳣��Ϣ�洢����
// �����������
// �����������
// ����ֵ  ��true �ɹ��� falseʧ��
// =============================================================================
bool_t Exp_RecordClear_nor(void)
{
    bool_t result;
    Int_SaveAsynSignal();
    result = Flash_EraseAddr(CN_RECORD_ADDRBASE,CN_RECORD_SIZE);
    Exp_Recordscan_nor();
    Int_RestoreAsynSignal();
    return result;
}
// =============================================================================
// �������ܣ�Exp_RecordCheckNum_nor
//          �鿴һ���洢�˶������쳣��Ϣ
// �����������
// ���������recordnum,���ص��쳣��Ϣ��Ŀ��
// ����ֵ     ��false,ʧ�� true�ɹ�
// =============================================================================
bool_t Exp_RecordCheckNum_nor(u32 *recordnum)
{
    *recordnum = s_u32ExpRecordItemNumNor;
    return true;
}

// =============================================================================
// �������ܣ�Exp_RecordCheckLen_nor
//          �鿴ָ����Ŀ���쳣��Ϣ�ĳ���
// ���������assignedno,ָ�����쳣֡��Ŀ(��1��ʼ)
// ���������recordlen,���ڴ洢ָ���쳣��Ŀ�ĳ���
// ����ֵ     ��false,ʧ�� true�ɹ�
// =============================================================================
bool_t Exp_RecordCheckLen_nor(u32 assignedno, u32 *recordlen)
{
    bool_t result;
    bool_t continuescan;
    u32 readaddr;
    u32 readlen;
    u32 hasread;
    u32 itemnum;
    struct tagExpNorRecordItemHead  itemhead;

    if((assignedno > s_u32ExpRecordItemNumNor)||(NULL == recordlen))
    {
        result = false;
        printk("checklen:assignedno = %d  recorditemnum = %d lenrecord = 0x%08x\n\r",\
                assignedno, s_u32ExpRecordItemNumNor, (u32)recordlen);
    }
    else//��ʼɨ��
    {
        readaddr = CN_RECORD_ADDRBASE;
        continuescan = true;
        readlen = sizeof(itemhead);
        itemnum = 0;
        result = false;

        while(continuescan)  //ɨ��ֱ�����ֿ���������ߵ���洢�����β
        {
            if((readaddr)>= (CN_RECORD_ADDRBASE + CN_RECORD_SIZE - readlen))
            {
               //�洢��ɨ�����
                s_u32ExpRecordValidAddrNor = readaddr;
                s_u32ExpRecordValidLenNor = CN_RECORD_ADDRBASE + CN_RECORD_SIZE -readaddr;
                s_u32ExpRecordItemNumNor =itemnum ;
                result = false;
                continuescan = false;
            }
            else
            {
                hasread = Flash_ReadData(readaddr, (u8 *)&itemhead,readlen);
                if(readlen != hasread)
                {
                    printk("__exp_norrecordscan:scanerr:readaddr = 0x%08x\n\r",readaddr);
                    printk("__exp_norrecordscan:readlen = 0x%08x,hasread = 0x%08x\n\r",\
                                readlen, hasread);
                    //ɨ��ʧ��
                    result = false;
                    continuescan = false;
                }
                else
                {
                    //��С����Ϊ��׼��Ҫ����ΪС����0����С�˶�һ��
                    if(((itemhead.recordendian == CN_CFG_LITTLE_ENDIAN)&&(CN_CFG_BYTE_ORDER != CN_CFG_LITTLE_ENDIAN))||\
                       ((itemhead.recordendian != CN_CFG_LITTLE_ENDIAN)&&(CN_CFG_BYTE_ORDER == CN_CFG_LITTLE_ENDIAN)))
                    {
                        //��Ҫ��С��ת��
                        _exp_norrecordheadswap(&itemhead);
                    }
                    if(CN_EXP_RECORD_MAGICNUMBER != itemhead.magicnumber)//���е�
                    {
                        result = false;
                        continuescan = false;
                    }
                    else
                    {
                        itemnum ++;
                        if(itemnum == assignedno)
                        {
                            continuescan = false;
                            result = true;
                        }
                        else
                        {
                            readaddr = itemhead.itemnextaddr;
                            continuescan = true;
                            result = false;
                        }
                    }
                }
            }
        }
        if(true == result)
        {
            *recordlen = itemhead.expinfolen;
        }
    }
    return result;
}
// =============================================================================
// �������ܣ�Exp_RecordGet_nor
//          �Ӵ洢�����л�ȡָ����Ŀ���쳣֡��Ϣ
// ���������assignedno,ָ�����쳣֡��Ŀ
//          buflenlimit,ϵͳ�쳣֡�ĳ��ȣ����Ƴ�������Ϊ���⿽��Խ��
// ���������buf,���ڴ洢��ȡָ����Ŀ���쳣��Ϣ
//          recordpara,�쳣��Ϣ�洢ʱ�Ĳ������ڴ��Ƕ�buf�ĸ������ֵĶ���
// ����ֵ     ��true�ɹ� falseʧ��
// =============================================================================
bool_t Exp_RecordGet_nor(u32 assignedno, u32 buflenlimit, u8 *buf, \
                         struct tagExpRecordPara *recordpara)
{
    bool_t result_bool = false;
    bool_t continuescan;
    u32 readaddr;
    u32 readlen;
    u32 hasread;
    u32 itemnum;
    struct tagExpNorRecordItemHead  itemhead;

    if((assignedno > s_u32ExpRecordItemNumNor)||(NULL == buf) ||(NULL == recordpara))
    {
        readlen = 0;
        printk("checklen:assignedno = %d  recorditemnum = %d buf = 0x%08\n\r",\
                assignedno, s_u32ExpRecordItemNumNor, (u32)buf);
    }
    else//��ʼɨ��
    {
        readaddr = CN_RECORD_ADDRBASE;
        continuescan = true;
        readlen = sizeof(itemhead);
        itemnum = 0;
        result_bool = false;

        while(continuescan)  //ɨ��ֱ�����ֿ���������ߵ���洢�����β
        {
            if((readaddr)>= (CN_RECORD_ADDRBASE + CN_RECORD_SIZE - readlen))
            {
               //�洢��ɨ�����
                s_u32ExpRecordValidAddrNor = readaddr;
                s_u32ExpRecordValidLenNor = CN_RECORD_ADDRBASE + CN_RECORD_SIZE -readaddr;
                s_u32ExpRecordItemNumNor =itemnum ;
                result_bool = false;
                continuescan = false;
            }
            else
            {
                hasread = Flash_ReadData(readaddr, (u8 *)&itemhead,readlen);
                if(readlen != hasread)
                {
                    printk("__exp_norrecordscan:scanerr:readaddr = 0x%08x\n\r",readaddr);
                    printk("__exp_norrecordscan:readlen = 0x%08x,hasread = 0x%08x\n\r",\
                                readlen, hasread);
                    //ɨ��ʧ��
                    result_bool = false;
                    continuescan = false;
                }
                else
                {
                    //��С����Ϊ��׼��Ҫ����ΪС����0����С�˶�һ��
                    if(((itemhead.recordendian == CN_CFG_LITTLE_ENDIAN)&&(CN_CFG_BYTE_ORDER != CN_CFG_LITTLE_ENDIAN))||\
                       ((itemhead.recordendian != CN_CFG_LITTLE_ENDIAN)&&(CN_CFG_BYTE_ORDER == CN_CFG_LITTLE_ENDIAN)))
                    {
                        //��Ҫ��С��ת��
                        _exp_norrecordheadswap(&itemhead);
                    }
                    if(CN_EXP_RECORD_MAGICNUMBER != itemhead.magicnumber)//���е�
                    {
                        result_bool = false;
                        continuescan = false;
                    }
                    else
                    {
                        itemnum ++;
                        if(itemnum == assignedno)
                        {
                            continuescan = false;
                            result_bool = true;
                        }
                        else
                        {
                            readaddr = itemhead.itemnextaddr;
                            continuescan = true;
                            result_bool = false;
                        }
                    }
                }
            }
        }
        if(true == result_bool)
        {
            readlen = itemhead.expinfolen;
            if(readlen > buflenlimit)
            {
                printk("readlen(%d) > buflenlimit(%d)",readlen, buflenlimit);
                result_bool = false;
            }
            else
            {
                readaddr = itemhead.expinfoaddr;
                readlen = Flash_ReadData(readaddr, buf, readlen);

                recordpara->headinfoaddr = (u32)buf;
                recordpara->headinfolen = itemhead.exp_headinfolen;
                recordpara->osstateinfoaddr = recordpara->headinfoaddr + \
                                             recordpara->headinfolen;
                recordpara->osstateinfolen = itemhead.exp_osstateinfolen;
                recordpara->hookinfoaddr = recordpara->osstateinfoaddr + \
                                             recordpara->osstateinfolen;
                recordpara->hookinfolen = itemhead.exp_hookinfolen;
                recordpara->throwinfoaddr = recordpara->hookinfoaddr + \
                                             recordpara->hookinfolen;
                recordpara->throwinfolen = itemhead.Exp_Throwinfolen;
            }
        }
    }
    return result_bool;
}

//norflash�쳣����
struct tagExpRecordOperate sgexprecordopt_norflash = {
    .fnexprecord = Exp_Record_nor,
    .fnexprecordchecklen = Exp_RecordCheckLen_nor,
    .fnexprecordchecknum = Exp_RecordCheckNum_nor,
    .fnexprecordclear = Exp_RecordClear_nor,
    .fnexprecordget = Exp_RecordGet_nor,
    .fnexprecordscan = Exp_Recordscan_nor,
};

// =============================================================================
// �������ܣ�ע��NORFLASH��Ϊ�쳣�洢����
// �����������
// ����ֵ     ��true�ɹ� falseʧ��
// =============================================================================
bool_t Exp_RecordNorInit(void)
{
    bool_t result;
    result = Exp_RegisterRecordOpt(&sgexprecordopt_norflash);
    return result;
}

