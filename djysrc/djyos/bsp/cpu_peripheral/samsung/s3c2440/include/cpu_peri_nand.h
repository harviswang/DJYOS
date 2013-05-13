//----------------------------------------------------
//Copyright (C), 2004-2011,  djyos team.
//��Ȩ���� (C), 2004-2011,   djyos�Ŷ�.
//���ߣ�djyos�Ŷ�
//�汾��V1.0.0
//�ļ�����: s3c2440 nand�������Ĵ�������
//����˵��:
//�޶���ʷ:
// 1. ����: 2011-11-29
//   ����: djyos�Ŷ�
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __cpu_peri_nand__
#define __cpu_peri_nand__

#ifdef __cplusplus
extern "C" {
#endif

struct nand_reg         //��ַ��0x4e000000
{
    uint16_t NFCONF;    //  0x4e000000 NAND Flash configuration
    uint16_t revs0;
    uint16_t NFCONT;    //  0x4e000004
    uint16_t revs1;
    uint16_t NFCMD ;    //  0x4e000008 NADD Flash command
    uint16_t revs2;
    uint16_t NFADDR;    //  0x4e00000c NAND Flash address
    uint16_t revs3;
    u8 NFDATA;     //  0x4e000010 NAND Flash data
    u8 revs30;
    u8 revs31;
    u8 revs32;
    u32 NFMECCD0 ; //  0x4e000014 NAND Flash ECCD0
    u32 NFMECCD1 ; //  0x4e000018 NAND Flash ECCD1
    u32 NFSECCD ;  //  0x4e00001C NAND Flash SPARE AREA ECC REGISTER
    uint16_t NFSTAT;    //  0x4e000020 NAND Flash operation status
    uint16_t revs4;
    u32 NFESTAT0;  //  0x4e000024 NAND flash ECC status for I/O[7:0]
    u32 NFESTAT1;  //  0x4e000028 NAND flash ECC status for I/O[15:8]
    u32 NFMECC0;   //  0x4e00002c NAND flash main area ECC0 status
    u32 NFMECC1;   //  0x4e000030 NAND flash main area ECC1 status
    u32 NFSECC;    //  0x4e000034 NAND flash spare area ECC status
    u32 NFSBLK;    //  0x4e000038 NAND flash start block address
    u32 NFEBLK;    //  0x4e00003c NAND flash end block address
};


#ifdef __cplusplus
}
#endif

#endif //__cpu_peri_nand__
