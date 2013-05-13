//========================================================  
// 文件模块说明:                                            
// C6000中断管理                         
// 文件版本: v1.00                                          
// 开发人员: lst                                            
// 创建时间: 2011.06.01                                     
// Copyright(c) 2011-2011  深圳鹏瑞软件有限公司             
//========================================================  
// 程序修改记录(最新的放在最前面):                            
//  <版本号> <修改日期>, <修改人员>: <修改功能概述>            
//========================================================  
//例如：
// V1.02 2004-07-14, 张三: 接收函数增加对报文长度有效性校核

#ifndef __OMAPL138_DCORE_H__
#define __OMAPL138_DCORE_H__

#ifdef __cplusplus
extern "C" {
#endif

void (*Dcore_LoadTiBinCode(u8 *code_buf))(void);
void (*Dcore_LoadAisCode(u8 *code_buf))(void);
void Dcore_WakeupDsp(void (*start_address)(void));
void Dcore_AssertDspIntNmi(void);
void Dcore_AssertDspIntCi2(void);
void Dcore_AssertDspIntCi3(void);
void Dcore_AssertArmIntCi0(void);
void Dcore_AssertArmIntCi1(void);
void Dcore_AssertArmIntCi2(void);
void Dcore_AssertArmIntCi3(void);
void Dcore_ClearDspIntNmi(void);
void Dcore_ClearDspIntCi2(void);
void Dcore_ClearDspIntCi3(void);
void Dcore_ClearArmIntCi0(void);
void Dcore_ClearArmIntCi1(void);
void Dcore_ClearArmIntCi2(void);
void Dcore_ClearArmIntCi3(void);

#ifdef __cplusplus
}
#endif

#endif //__OMAPL138_DCORE_H__

