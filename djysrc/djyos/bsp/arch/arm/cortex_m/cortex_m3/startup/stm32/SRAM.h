/*
 * SRAM.h
 *
 *  Created on: 2012-3-22
 *      Author: Administrator
 */

#ifndef SRAM_H_
#define SRAM_H_

#define SRAM_D0_GPIOD14        	GPIO_Pin_14
#define SRAM_D1_GPIOD15        	GPIO_Pin_15
#define SRAM_D2_GPIOD0         	GPIO_Pin_0
#define SRAM_D3_GPIOD1         	GPIO_Pin_1
#define SRAM_D4_GPIOE7        	GPIO_Pin_7
#define SRAM_D5_GPIOE8        	GPIO_Pin_8
#define SRAM_D6_GPIOE9        	GPIO_Pin_9
#define SRAM_D7_GPIOE10        	GPIO_Pin_10
#define SRAM_D8_GPIOE11        	GPIO_Pin_11
#define SRAM_D9_GPIOE12        	GPIO_Pin_12
#define SRAM_D10_GPIOE13       GPIO_Pin_13
#define SRAM_D11_GPIOE14       GPIO_Pin_14
#define SRAM_D12_GPIOE15       GPIO_Pin_15
#define SRAM_D13_GPIOD8        	GPIO_Pin_8
#define SRAM_D14_GPIOD9        	GPIO_Pin_9
#define SRAM_D15_GPIOD10       GPIO_Pin_10

#define SRAM_WE_GPIOD5        	GPIO_Pin_5
#define SRAM_RE_GPIOD4        	GPIO_Pin_4
#define SRAM_CS_GPIOD7        	GPIO_Pin_7
#define SRAM_LB_GPIOE0        	GPIO_Pin_0
#define SRAM_UB_GPIOE1        	GPIO_Pin_1

#define SRAM_A0_GPIOF0        	GPIO_Pin_0
#define SRAM_A1_GPIOF1        	GPIO_Pin_1
#define SRAM_A2_GPIOF2        	GPIO_Pin_2
#define SRAM_A3_GPIOF3       	GPIO_Pin_3
#define SRAM_A4_GPIOF4       	GPIO_Pin_4
#define SRAM_A5_GPIOF5        	GPIO_Pin_5
#define SRAM_A6_GPIOF12        	GPIO_Pin_12
#define SRAM_A7_GPIOF13        	GPIO_Pin_13
#define SRAM_A8_GPIOF14       	GPIO_Pin_14
#define SRAM_A9_GPIOF15        	GPIO_Pin_15
#define SRAM_A10_GPIOG0        	GPIO_Pin_0
#define SRAM_A11_GPIOG1        	GPIO_Pin_1
#define SRAM_A12_GPIOG2        	GPIO_Pin_2
#define SRAM_A13_GPIOG3        	GPIO_Pin_3
#define SRAM_A14_GPIOG4        	GPIO_Pin_4
#define SRAM_A15_GPIOG5        	GPIO_Pin_5
#define SRAM_A16_GPIOD11       GPIO_Pin_11
#define SRAM_A17_GPIOD12       GPIO_Pin_12
#define SRAM_A18_GPIOD13       GPIO_Pin_13
#define SRAM_A19_GPIOE3        	GPIO_Pin_3
#define SRAM_A20_GPIOE4        	GPIO_Pin_4
#define SRAM_A21_GPIOE5        	GPIO_Pin_5


#define PSRAM_BASE       (uint8_t*)(uint32_t)(0x60000000)

#if !defined (PSRAM)
#define PSRAM	                	1
//#define EM7324SU16SP		1	                    // 4M字节的PSRAM
#define EM7644SU16ANP		1	                    // 8M字节的PSRAM
#endif


#if( PSRAM == EM7644SU16ANP )
    #define PSRAM_32TEST_PAGE	    	0x800000

    #define PSRAM_16TEST_PAGE	    	0x10000
    #define PSRAM_16TEST_nPAGE		0x80

    #define PSRAM_8TEST_PAGE	    	0x100
    #define PSRAM_8TEST_nPAGE	    	0x8000
#elif( PSRAM == EM7324SU16SP )
    #define PSRAM_32TEST_PAGE	    	0x400000

    #define PSRAM_16TEST_PAGE	    	0x10000
    #define PSRAM_16TEST_nPAGE		0x40

    #define PSRAM_8TEST_PAGE	    	0x100
    #define PSRAM_8TEST_nPAGE	    	0x4000
#else
    #define PSRAM_32TEST_PAGE	    	0x0
    #define PSRAM_32TEST_nPAGE		0x0

    #define PSRAM_16TEST_PAGE	    	0x0
    #define PSRAM_16TEST_nPAGE		0x0

    #define PSRAM_8TEST_PAGE	    	0x0
    #define PSRAM_8TEST_nPAGE	    	0x0
#endif

#endif /* SRAM_H_ */
