/******************************************************************************/
/* TS_I2C.c: Touch-Screen STMPE811 controller routines                        */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2005-2009 Keil Software. All rights reserved.                */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/

#include "I2C2.h"
#include "TS_I2C.h"

/************************ Local auxiliary functions ***************************/

/************************ Exported functions **********************************/


/*******************************************************************************
* Write value to Touch-screen controller register                              *
*   Parameter:    reg:    register address                                     *
*                 num:    number of bytes to be written                        *
*                 val:    value to be written                                  *
*   Return:                                                                    *
*******************************************************************************/

void TS_Write (unsigned char reg, unsigned char num, unsigned int val) {

  I2C2_Start();
  I2C2_Addr(0x82 | 0);
  I2C2_Write(reg);                       /* Register to be written             */
  while (num--) {
    I2C2_Write(val & 0xFF);
    val >>= 8;
  }
  I2C2_Stop();
}


/*******************************************************************************
* Read data from Touch-screen controller register                              *
*   Parameter:    reg:    register address                                     *
*                 num:    number of bytes to be read                           *
*   Return:               read value                                           *
*******************************************************************************/

unsigned int TS_Read (unsigned char reg, unsigned char num) {
  unsigned int res = 0;

  I2C2_Start();
  I2C2_Addr(0x82 | 0);
  I2C2_Write(reg);                       /* Register to be read                */
  I2C2_Start();
  I2C2_Addr(0x82 | 1);
  while (num--) {
    res <<= 8;
    res  |= I2C2_Read ((num >= 1));
  }
  I2C2_Stop();

  return(res);
}

/******************************************************************************/
