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
//Copyright (C), 2004-2009,  lst.
//��Ȩ���� (C), 2004-2009,   lst.
//����ģ��:�ж�ģ��
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: �����жϺ�
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009--9-28
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __CPU_PERI_INT_LINE__
#define __CPU_PERI_INT_LINE__

#ifdef __cplusplus
extern "C" {
#endif

#define CN_INT_LINE_WDT       		 0        //!< Watchdog Timer Interrupt                         */
#define CN_INT_LINE_TIMER0           1        //!< Timer0 Interrupt                                 */
#define CN_INT_LINE_TIMER1           2        //!< Timer1 Interrupt                                 */
#define CN_INT_LINE_TIMER2           3        //!< Timer2 Interrupt                                 */
#define CN_INT_LINE_TIMER3           4        //!< Timer3 Interrupt                                 */
#define CN_INT_LINE_UART0            5        //!< UART0 Interrupt                                  */
#define CN_INT_LINE_UART1            6        //!< UART1 Interrupt                                  */
#define CN_INT_LINE_UART2            7        //!< UART2 Interrupt                                  */
#define CN_INT_LINE_UART3            8        //!< UART3 Interrupt                                  */
#define CN_INT_LINE_PWM1             9        //!< PWM1 Interrupt                                   */
#define CN_INT_LINE_I2C0             10       //!< I2C0 Interrupt                                   */
#define CN_INT_LINE_I2C1             11       //!< I2C1 Interrupt                                   */
#define CN_INT_LINE_I2C2             12       //!< I2C2 Interrupt                                   */
#define CN_INT_LINE_SPI              13       //!< SPI Interrupt                                    */
#define CN_INT_LINE_SSP0             14       //!< SSP0 Interrupt                                   */
#define CN_INT_LINE_SSP1             15       //!< SSP1 Interrupt                                   */
#define CN_INT_LINE_PLL0             16       //!< PLL0 Lock (Main PLL) Interrupt                   */
#define CN_INT_LINE_RTC              17       //!< Real Time Clock Interrupt                        */
#define CN_INT_LINE_EINT0            18       //!< External Interrupt 0 Interrupt                   */
#define CN_INT_LINE_EINT1            19       //!< External Interrupt 1 Interrupt                   */
#define CN_INT_LINE_EINT2            20       //!< External Interrupt 2 Interrupt                   */
#define CN_INT_LINE_EINT3            21       //!< External Interrupt 3 Interrupt                   */
#define CN_INT_LINE_ADC              22       //!< A/D Converter Interrupt                          */
#define CN_INT_LINE_BOD              23       //!< Brown-Out Detect Interrupt                       */
#define CN_INT_LINE_USB              24       //!< USB Interrupt                                    */
#define CN_INT_LINE_CAN              25       //!< CAN Interrupt                                    */
#define CN_INT_LINE_DMA              26       //!< General Purpose DMA Interrupt                    */
#define CN_INT_LINE_I2S              27       //!< I2S Interrupt                                    */
#define CN_INT_LINE_ENET             28       //!< Ethernet Interrupt                               */
#define CN_INT_LINE_RIT              29       //!< Repetitive Interrupt Timer Interrupt             */
#define CN_INT_LINE_MCPWM            30       //!< Motor Control PWM Interrupt                      */
#define CN_INT_LINE_QEI              31       //!< Quadrature Encoder Interface Interrupt           */
#define CN_INT_LINE_PLL1             32       //!< PLL1 Lock (USB PLL) Interrupt                    */
#define CN_INT_LINE_USBActivity      33       //!< USB Activity Interrupt 						  */
#define CN_INT_LINE_CANActivity      34       //!< CAN Activity Interrupt
#define CN_INT_LINE_LAST             34

#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_INT_LINE__

