/*
 * stm32_ad.h
 *
 *  Created on: 2012-5-4
 *      Author: Administrator
 */

#ifndef STM32_AD_H_
#define STM32_AD_H_


#define PowerAD_GPIOA7        	GPIO_Pin_7
#define GmAD_GPIOC5        	    GPIO_Pin_5
#define ShiDuAD_GPIOF6        	    GPIO_Pin_6
#define ShiDuH_GPIOG13        	    GPIO_Pin_13
#define ShiDuL_GPIOG14        	    GPIO_Pin_14

#define b_ShiDu_H 		*((volatile unsigned long *) (PERIPH_BB_BASE + (GPIOG_BASE-PERIPH_BASE)*8*4 + 0x0C*8*4 + 13*4))				//GPIOG_ODR13
#define b_ShiDu_L 		*((volatile unsigned long *) (PERIPH_BB_BASE + (GPIOG_BASE-PERIPH_BASE)*8*4 + 0x0C*8*4 + 14*4))				//GPIOG_ODR14


extern void ad_init(void);
extern void ad_task(void);
extern void ad_cofg(void);
extern void ad_gpio(void);
extern void power_ad_cofg(void);
extern void gm_ad_cofg(void);
//extern void shidu_ad_cofg(void);


#endif /* STM32_AD_H_ */
