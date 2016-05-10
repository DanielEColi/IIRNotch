#ifndef __IIR_NOTCH_C__
#define __IIR_NOTCH_C__

#include "iirnotch.h"



/* ============= Filter declaration code=============*/
#define		AZ_PI				(3.1415926f)
#define		AZ_CH_NUM			(18)		//�˲�ͨ����Ŀ
#define		AZ_NOTCH_SIZE		(3)			//�˲�������+1
static float fCoefB[AZ_CH_NUM][AZ_NOTCH_SIZE];
static float fCoefA[AZ_CH_NUM][AZ_NOTCH_SIZE];
static float fNotchBuf[AZ_CH_NUM][AZ_NOTCH_SIZE];

/************************************************************************
*  Design a 2nd-order notch digital filter.
*  �����б�
*		float Wo:��һ���ݲ�Ƶ��(normalized by pi),����AZ_FREQ_NORM������
*		float BW:the bandwidth at the -Ab dB point set to BW,BW = Wo/Q
*				(eg:if -3db is Wo��dw��BW=2*dw)
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
*  ��ͨ����ʼ���ݲ���
*  �����б�
*		short ch:�˲�ͨ����0-17
*		float Fn:��һ���ݲ�Ƶ��,(eg:50Hz��1Hz��Fn=50/(fs/2) )
*		float BW:��һ������,(eg:50Hz��1Hz��BW=2/(fs/2) )			
* ˵��: �ݲ���������ch������ÿ���ݲ����໥�����������Ҫ������ʼ����
		��ʼ��ֻ��ִ��1�Σ����Ǵ���BW�����˸ı�
************************************************************************/
void AzInitNotchByChannel(short ch,float Fn,float BW)
{
	if (ch<0 || ch>=AZ_CH_NUM)return;

	AzDesignNotch(Fn, BW, 3, fCoefB[ch],fCoefA[ch]);
	memset(fNotchBuf[ch],0,sizeof(fNotchBuf[ch]));
}

/************************************************************************
*  IIR�ݲ���(ֱ��II��/�䷶��)
*  �����б�
*		short ch:�˲�ͨ����0-17
*		int   xn:�˲�����
*		float fnotch:�ݲ�Ƶ��,Hz
*		float bw:    ����,Hz(eg:50Hz��1Hz��BW=2Hz )	
*		float fs:    ������,Hz
*		short init:  �Ƿ��ʼ���ݲ�����1:��ʼ��,0:����ʼ����
*  �����
*		�˲����(��ch������Χ������fs,bw,fnotch����������-1)
* ˵��: �ݲ���������ch������ÿ���ݲ����໥�����������Ҫ������ʼ����
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