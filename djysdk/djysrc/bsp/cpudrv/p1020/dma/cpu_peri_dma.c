//----------------------------------------------------
//Copyright (C), 2013-2020,  luoshitian.
//��Ȩ���� (C), 2013-2020,   ������.
//����ģ��: dma����ģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����: dma�����ļ���ʹ��dmaֱ�Ӱ���
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2013-11-14
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include <cpu_peri_dma.h>
#include <asm/io.h>
#include <stdio.h>
/* Controller can only transfer 2^26 - 1 bytes at a time */
#define FSL_DMA_MAX_SIZE    (0x3ffffff)


#define FSL_DMA_MR_DEFAULT (FSL_DMA_MR_BWC_DIS | FSL_DMA_MR_CTM_DIRECT)

ccsr_dma_t *dma_base = (void *)(CONFIG_SYS_MPC85xx_DMA_ADDR);


#define max(x, y)    (((x) < (y)) ? (y) : (x))
#define min(x, y)    (((x) < (y)) ? (x) : (y))

static void dma_sync(void)
{
    __asm__("sync; isync; msync");
}

static void out_dma32(volatile unsigned *addr, int val)
{
    out_be32(addr, val);
}

static unsigned int in_dma32(volatile unsigned *addr)
{
    return in_be32(addr);
}
//----���dma����״̬-----------------------------------------------------------------
//���ܣ����dma����״̬�����DMA��ͨ��æ�Ĵ������ȵ����Ϊ0�����ʾ������ɣ�Ȼ���start DMA cs���㡣
//��������
//���أ�����dma sr״̬�Ĵ���ֵ
//-----------------------------------------------------------------------------
static unsigned int dma_check(void) {
    volatile fsl_dma_t *dma = &dma_base->dma[3];
    unsigned int status;

    /* While the channel is busy, spin */
    do {
        status = in_dma32(&dma->sr);
    } while (status & FSL_DMA_SR_CB);

    /* clear MR[CS] channel start bit */
    out_dma32(&dma->mr, in_dma32(&dma->mr) & ~FSL_DMA_MR_CS);
    dma_sync();

    if (status != 0)
        printf ("DMA Error: status = %x\r\n", status);

    return status;
}
//----dma��ʼ��-----------------------------------------------------------------
//���ܣ���ʼ��dma�Ĵ��������error״̬
//��������
//���أ�����dma sr״̬�Ĵ���ֵ
//-----------------------------------------------------------------------------
void dma_init(void) {
    volatile fsl_dma_t *dma = &dma_base->dma[3];

    out_dma32(&dma->satr, FSL_DMA_SATR_SREAD_SNOOP);
    out_dma32(&dma->datr, FSL_DMA_DATR_DWRITE_SNOOP);
    out_dma32(&dma->sr, 0xffffffff); /* clear any errors */
    dma_sync();
}
//----dma���˺���-----------------------------------------------------------------
//���ܣ�ʹ��dma�������ݣ����dma�����Ƿ������ɡ�
//������phys_addr_t dest, ���˵�Ŀ���ַ��phys_addr_t src, ���˵�Դ��ַ��phys_size_t count�����˵����ݳ���
//���أ�0��ʾ���˳ɹ���-1��ʾ����ʧ��
//-----------------------------------------------------------------------------
int dmacpy(unsigned long dest, unsigned long src, unsigned long count) {
    volatile fsl_dma_t *dma = &dma_base->dma[3];
    unsigned int xfer_size;
    //printf("dest = %8x,src = %8x\r\n",dest,src);
    while (count) {
        xfer_size = min(FSL_DMA_MAX_SIZE, count);

        out_dma32(&dma->dar, (u32) (dest & 0xFFFFFFFF));
        out_dma32(&dma->sar, (u32) (src & 0xFFFFFFFF));

        out_dma32(&dma->satr,
            in_dma32(&dma->satr) | (u32)((u64)src >> 32));
        out_dma32(&dma->datr,
            in_dma32(&dma->datr) | (u32)((u64)dest >> 32));

        out_dma32(&dma->bcr, xfer_size);
        dma_sync();

        /* Prepare mode register */
        out_dma32(&dma->mr, FSL_DMA_MR_DEFAULT);
        dma_sync();

        /* Start the transfer */
        out_dma32(&dma->mr, FSL_DMA_MR_DEFAULT | FSL_DMA_MR_CS);

        count -= xfer_size;
        src += xfer_size;
        dest += xfer_size;

        dma_sync();

        if (dma_check())
            return -1;
    }

    return 0;
}
//----dma���˺�����������⣩-----------------------------------------------------------------
//���ܣ�ʹ��dma�������ݣ������dma�����Ƿ������ɡ�
//������phys_addr_t dest, ���˵�Ŀ���ַ��phys_addr_t src, ���˵�Դ��ַ��phys_size_t count�����˵����ݳ���
//���أ�0��ʾ���˳ɹ���-1��ʾ����ʧ��
//-----------------------------------------------------------------------------
int dmacpy_nocheck(unsigned long dest, unsigned long src, unsigned long count) {
    volatile fsl_dma_t *dma = &dma_base->dma[3];
    unsigned int xfer_size;
    //printf("dest = %8x,src = %8x\r\n",dest,src);
    while (count) {
        xfer_size = min(FSL_DMA_MAX_SIZE, count);

        out_dma32(&dma->dar, (u32) (dest & 0xFFFFFFFF));
        out_dma32(&dma->sar, (u32) (src & 0xFFFFFFFF));

        out_dma32(&dma->satr,
            in_dma32(&dma->satr) | (u32)((u64)src >> 32));
        out_dma32(&dma->datr,
            in_dma32(&dma->datr) | (u32)((u64)dest >> 32));

        out_dma32(&dma->bcr, xfer_size);
        dma_sync();

        /* Prepare mode register */
        out_dma32(&dma->mr, FSL_DMA_MR_DEFAULT);
        dma_sync();

        /* Start the transfer */
        out_dma32(&dma->mr, FSL_DMA_MR_DEFAULT | FSL_DMA_MR_CS);

        count -= xfer_size;
        src += xfer_size;
        dest += xfer_size;

        dma_sync();

        //if (dma_check())
            //return -1;
    }

    return 0;
}
/*
 * 85xx/86xx use dma to initialize SDRAM when !CONFIG_ECC_INIT_VIA_DDRCONTROLLER
 * while 83xx uses dma to initialize SDRAM when CONFIG_DDR_ECC_INIT_VIA_DMA
 */
#if 0
#if ((!defined CONFIG_MPC83xx && defined(CONFIG_DDR_ECC) &&    \
    !defined(CONFIG_ECC_INIT_VIA_DDRCONTROLLER)) ||        \
    (defined(CONFIG_MPC83xx) && defined(CONFIG_DDR_ECC_INIT_VIA_DMA)))
void dma_meminit(uint val, uint size)
{
    uint *p = 0;
    uint i = 0;

    for (*p = 0; p < (uint *)(8 * 1024); p++) {
        if (((uint)p & 0x1f) == 0)
            ppcDcbz((ulong)p);

        *p = (uint)CONFIG_MEM_INIT_VALUE;

        if (((uint)p & 0x1c) == 0x1c)
            ppcDcbf((ulong)p);
    }

    dmacpy(0x002000, 0, 0x002000); /* 8K */
    dmacpy(0x004000, 0, 0x004000); /* 16K */
    dmacpy(0x008000, 0, 0x008000); /* 32K */
    dmacpy(0x010000, 0, 0x010000); /* 64K */
    dmacpy(0x020000, 0, 0x020000); /* 128K */
    dmacpy(0x040000, 0, 0x040000); /* 256K */
    dmacpy(0x080000, 0, 0x080000); /* 512K */
    dmacpy(0x100000, 0, 0x100000); /* 1M */
    dmacpy(0x200000, 0, 0x200000); /* 2M */
    dmacpy(0x400000, 0, 0x400000); /* 4M */

    for (i = 1; i < size / 0x800000; i++)
        dmacpy((0x800000 * i), 0, 0x800000);
}
#endif
#endif
