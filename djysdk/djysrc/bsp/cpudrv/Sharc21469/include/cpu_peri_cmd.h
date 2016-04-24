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


#ifndef __CPU_PERI_CMD_H__
#define __CPU_PERI_CMD_H__
#ifdef __cplusplus
extern "C" {
#endif

#define     cn_uart_connect_recv_evtt    0
#define     cn_uart_disconnect_recv_evtt 1

u32 cmd_int(ufast_t uart_int_line);
ptu32_t module_init_uart0(ptu32_t para);
ptu32_t cmd_hard_write(struct  tagPanDevice *cmd_dev,ptu32_t buf,
                            ptu32_t res2,ptu32_t len);
ptu32_t cmd_app_write(struct  tagPanDevice *cmd_dev,ptu32_t src_buf,
                            ptu32_t res,ptu32_t len);
ptu32_t cmd_app_read(struct  tagPanDevice *uart_dev,ptu32_t res,
                                        ptu32_t dst_buf,ptu32_t len);
ptu32_t cmd_ctrl(struct tagPanDevice *uart_dev,u32 cmd,
                   u32 data1,u32 data2);
/**
 * @brief Disables interrupts specified by the Mask parameter.
 * @param PeripheralBase Peripheral base address.
 * @param Mask Interrupt mask. Use constants from group "Interrupt masks". This
 *        parameter is 8 bits wide.
 * @return Returns a value of void type.
 * @remarks The macro accesses the following registers: UART0_C2, UART0_C3,
 *          UART1_C2, UART1_C3, UART2_C2, UART2_C3, UART3_C2, UART3_C3, UART4_C2,
 *          UART4_C3, UART5_C2, UART5_C3 (depending on the peripheral).
 * @par Example:
 *      @code
 *      UART_PDD_DisableInterrupt(deviceID, UART_PDD_INTERRUPT_TRANSMITTER);
 *      @endcode
 */
#define UART_PDD_DisableInterrupt(PeripheralBase, Mask) ( \
    (UART_C2_REG(PeripheralBase) &= \
     (uint8_t)(~(uint8_t)((uint8_t)(Mask) & (uint8_t)(~(uint8_t)0xFU)))), \
    (UART_C3_REG(PeripheralBase) &= \
     (uint8_t)(~(uint8_t)((uint8_t)(Mask) & 0xFU))) \
  )

/**
 * @brief Enables interrupts specified by the Mask parameter.
 * @param PeripheralBase Peripheral base address.
 * @param Mask Interrupt mask. Use constants from group "Interrupt masks". This
 *        parameter is 8 bits wide.
 * @return Returns a value of void type.
 * @remarks The macro accesses the following registers: UART0_C2, UART0_C3,
 *          UART1_C2, UART1_C3, UART2_C2, UART2_C3, UART3_C2, UART3_C3, UART4_C2,
 *          UART4_C3, UART5_C2, UART5_C3 (depending on the peripheral).
 * @par Example:
 *      @code
 *      UART_PDD_EnableInterrupt(deviceID, UART_PDD_INTERRUPT_TRANSMITTER);
 *      @endcode
 */
#define UART_PDD_EnableInterrupt(PeripheralBase, Mask) ( \
    (UART_C2_REG(PeripheralBase) |= \
     (uint8_t)((uint8_t)(Mask) & (uint8_t)(~(uint8_t)0xFU))), \
    (UART_C3_REG(PeripheralBase) |= \
     (uint8_t)((uint8_t)(Mask) & 0xFU)) \
  )
#define PDD_DISABLE 0u
#define PDD_ENABLE  1u
#define UART_PDD_INTERRUPT_RECEIVER  (0x20u)
#define UART_PDD_INTERRUPT_TRANSMITTER          (UART_C2_TIE_MASK)
#ifdef __cplusplus
}
#endif
#endif
