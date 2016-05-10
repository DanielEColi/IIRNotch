#ifndef __IIR_NOTCH_H__
#define __IIR_NOTCH_H__

#include <math.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif


#define		__PI__				(3.1415926f)
#define		__SAMPLE_RATE		(125)
#define		__NOTCH_RATE		(50)
#define     __FREQ_NORM(f,fs)	((double)f/(fs/2.0))
#define		__NOTCH_W0			(__FREQ_NORM(__NOTCH_RATE,__SAMPLE_RATE))


/* ============= Filter declaration code=============*/
#define		JR_CH_NUM				(12)
#define		JR_NOTCH_SIZE			(3)
static float fCoefNum[JR_CH_NUM][JR_NOTCH_SIZE];
static float fCoefDen[JR_CH_NUM][JR_NOTCH_SIZE];
static float fNotchBuf[JR_CH_NUM][JR_NOTCH_SIZE];
void InitNotchCoef(float Wo,float BW,float Ab,float *num,float *den );
void InitNotchByChannel(short ch,float fn);
float IIRNotchFilter(short ch,float xn);

// #define		JrCIR_INC(i,N)				(( (i)>=((N)-1) )? 0:((i)+1))
// #define		JrCIR_DEC(i,N)				(( (i)<=0 )?   (N)-1:((i)-1))
// #define		NOTCH_BUF_X_SIZE		(JR_NOTCH_SIZE+1) 
// #define		NOTCH_BUF_Y_SIZE		(JR_NOTCH_SIZE+1)
// static float notch_bufX[JR_CH_NUM][NOTCH_BUF_X_SIZE];
// static float notch_bufY[JR_CH_NUM][NOTCH_BUF_Y_SIZE];
// static int notch_posX[JR_CH_NUM];
// static int notch_posY[JR_CH_NUM];

// void InitNotch(float Wo,float BW,float Ab,float *num,float *den );
// float IIRNotchFilter(short ch,float xn,short isInit);
#ifdef __cplusplus
}

#endif

#endif
