#ifndef __IIR_NOTCH_C__
#define __IIR_NOTCH_C__

#include "iirnotch.h"



/* ============= Filter declaration code=============*/
#define		AZ_PI				(3.1415926f)
#define		AZ_CH_NUM			(18)		//滤波通道数目
#define		AZ_NOTCH_SIZE		(3)			//滤波器阶数+1
static float fCoefB[AZ_CH_NUM][AZ_NOTCH_SIZE];
static float fCoefA[AZ_CH_NUM][AZ_NOTCH_SIZE];
static float fNotchBuf[AZ_CH_NUM][AZ_NOTCH_SIZE];

/************************************************************************
*  Design a 2nd-order notch digital filter.
*  参数列表：
*		float Wo:归一化陷波频率(normalized by pi),可用AZ_FREQ_NORM宏生成
*		float BW:the bandwidth at the -Ab dB point set to BW,BW = Wo/Q
*				(eg:if -3db is Wo±dw，BW=2*dw)
*		float Ab:-Ab dB
*		[out]float *num:numerator coefficient b
*		[out]float *den:denominator coefficient a
************************************************************************/
static void AzDesignNotch(float Wo,float BW,float Ab,float *num,float *den )
{
	double Gb,beta,gain;

	BW = BW*AZ_PI;
	Wo = Wo*AZ_PI;

	// Gb   = 10^(-Ab/20);
	// beta = (sqrt(1-Gb.^2)/Gb)*tan(BW/2);
	// gain = 1/(1+beta);
	Gb   = pow(10,-Ab/20);
	beta = (sqrt(1-Gb*Gb)/Gb)*tan(BW/2);
	gain = 1/(1+beta);

	// num  = gain*[1 -2*cos(Wo) 1];
	// den  = [1 -2*gain*cos(Wo) (2*gain-1)];
	*num	 = (float)gain;
	*(num+1) = (float)(-2*gain*cos(Wo));
	*(num+2) = (float)gain;
	*den	 = 1;							//y(n)
	*(den+1) = (float)(-2*gain*cos(Wo));	//y(n-1)
	*(den+2) = (float)(2*gain-1);			//y(n-2)

}



/************************************************************************
*  按通道初始化陷波器
*  参数列表：
*		short ch:滤波通道，0-17
*		float Fn:归一化陷波频率,(eg:50Hz±1Hz，Fn=50/(fs/2) )
*		float BW:归一化带宽,(eg:50Hz±1Hz，BW=2/(fs/2) )			
* 说明: 陷波器个数由ch决定，每个陷波器相互独立，因此需要单独初始化；
		初始化只需执行1次，除非带宽BW发生了改变
************************************************************************/
void AzInitNotchByChannel(short ch,float Fn,float BW)
{
	if (ch<0 || ch>=AZ_CH_NUM)return;

	AzDesignNotch(Fn, BW, 3, fCoefB[ch],fCoefA[ch]);
	memset(fNotchBuf[ch],0,sizeof(fNotchBuf[ch]));
}

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
int AzIIRNotchFilter(short ch,int xn, float fnotch,float bw, float fs, short init)
{
	float y0;
	float Fn;
	float BW;

	if (ch<0 || ch>=AZ_CH_NUM)return -1;
	if (bw<=0 || fs<=0 || fnotch<=0 || fs<=fnotch*2 || fnotch-bw/2<=0)return -1;

	Fn = fnotch/(fs/2);
	BW = bw/(fs/2);
	if (init)
	{
		AzInitNotchByChannel(ch,Fn,BW);
	}

// 	fNotchBuf[ch][0]=(float)xn*fCoefA[ch][0]-fCoefA[ch][1]*fNotchBuf[ch][1]-fCoefA[ch][2]*fNotchBuf[ch][2];
	fNotchBuf[ch][0]=(float)xn-fCoefA[ch][1]*fNotchBuf[ch][1]-fCoefA[ch][2]*fNotchBuf[ch][2];
	y0=fCoefB[ch][0]*fNotchBuf[ch][0]+fCoefB[ch][1]*fNotchBuf[ch][1]+fCoefB[ch][2]*fNotchBuf[ch][2];
	fNotchBuf[ch][2]=fNotchBuf[ch][1];
	fNotchBuf[ch][1]=fNotchBuf[ch][0];

	return (int)y0;
}

#endif