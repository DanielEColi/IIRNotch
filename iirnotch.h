#ifndef __IIR_NOTCH_H__
#define __IIR_NOTCH_H__

#include <math.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
*  IIR陷波器(直接II型/典范型)
*  参数列表：
*		short ch:滤波通道，0-17
*		int   xn:滤波输入
*		float fnotch:陷波频率,Hz
*		float bw:    带宽,Hz(eg:50Hz±1Hz，BW=2Hz )	
*		float fs:    采样率,Hz
*		short init:  是否初始化陷波器（1:初始化,0:不初始化）
*  输出：
*		滤波输出(若ch超出范围，或者fs,bw,fnotch不合理，返回-1)
* 说明: 陷波器个数由ch决定，每个陷波器相互独立，因此需要单独初始化；
************************************************************************/
__declspec(dllexport) int AzIIRNotchFilter(short ch,int xn, float fnotch,float bw, float fs, short init);

#ifdef __cplusplus
}

#endif

#endif
