#ifndef __IIR_NOTCH_C__
#define __IIR_NOTCH_C__

#include "iirnotch.h"

#if 1

/************************************************************************
*  Design a 2nd-order notch digital filter.
*  �����б�
*		float Wo:��һ���ݲ�Ƶ��(normalized by pi),����__FREQ_NORM������
*		float BW:the bandwidth at the -Ab dB point set to BW,BW = Wo/Q
*				(eg:if -3db is Wo��dw��BW=2*dw)
*		float Ab:-Ab dB
*		[out]float *num:numerator coefficient b
*		[out]float *den:denominator coefficient a
************************************************************************/
void InitNotchCoef(float Wo,float BW,float Ab,float *num,float *den )
{
	double Gb,beta,gain;

	BW = BW*__PI__;
	Wo = Wo*__PI__;

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
*  �����б�
*		short ch:�˲�ͨ����0-JR_CH_NUM
*		float fn:��һ���ݲ�Ƶ��,����__FREQ_NORM������
************************************************************************/
void InitNotchByChannel(short ch,float fn)
{
// 	InitNotchCoef(fn,(float)(__NOTCH_W0/35.0), 3, fCoefNum[ch],fCoefDen[ch]);
	InitNotchCoef(fn,(float)(__FREQ_NORM(2.0,__SAMPLE_RATE)), 3, fCoefNum[ch],fCoefDen[ch]);
	memset(fNotchBuf[ch],0,sizeof(fNotchBuf[ch]));
}
/************************************************************************
*  �����б�
*		short ch:�˲�ͨ����0-JR_CH_NUM
*		float xn:�˲�����
*  �����
*				�˲����
************************************************************************/
float IIRNotchFilter(short ch,float xn)
{
	float y0;

	fNotchBuf[ch][0]=fCoefDen[ch][0]*xn-fCoefDen[ch][1]*fNotchBuf[ch][1]-fCoefDen[ch][2]*fNotchBuf[ch][2];
	y0=fCoefNum[ch][0]*fNotchBuf[ch][0]+fCoefNum[ch][1]*fNotchBuf[ch][1]+fCoefNum[ch][2]*fNotchBuf[ch][2];
	fNotchBuf[ch][2]=fNotchBuf[ch][1];
	fNotchBuf[ch][1]=fNotchBuf[ch][0];

	return y0;
}

#else
// a digital notching filter with the notch located at w0, 
// and with the bandwidth at the -3 dB point set to bw.
// % Define default values.
// Ab = abs(10*log10(.5)); % 3-dB width
// The quality factor (Q factor) q for the filter is related 
// to the filter bandwidth by q=w0/bw 
// where ��0 is w0, the frequency to remove from the signal.
void InitNotch(float Wo,float BW,float Ab,float *num,float *den )
{
	// % Design a 2nd-order notch digital filter.
	// % Inputs are normalized by pi.
	double Gb,beta,gain;

	BW = BW*__PI__;
	Wo = Wo*__PI__;

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

// *sum(a(k)*y(n-k)) = sum(b(k)*x(n-k))
float IIRNotchFilter(short ch,float xn,short isInit)
{
	float temp_y;
	int i,pos;

	if (isInit)
	{
		InitNotch((float)__NOTCH_W0,(float)(__NOTCH_W0/35.0), 3, fCoefNum[ch],fCoefDen[ch]);
		notch_posX[ch] = 0;
		notch_posY[ch] = 0;
		memset(notch_bufX[ch],0,sizeof(notch_bufX[ch]));
		memset(notch_bufY[ch],0,sizeof(notch_bufY[ch]));
	}


	/*����x(n)ֵ��notch_buf_x*/
	notch_bufX[ch][notch_posX[ch]] = xn;

	temp_y = 0.0f;
	pos = notch_posX[ch];
	for (i=0;i<JR_NOTCH_SIZE;i++)
	{//sum(b(k)*x(n-k))
		temp_y += notch_bufX[ch][pos]*fCoefNum[ch][i] ;
		pos = JrCIR_DEC(pos,JR_NOTCH_SIZE);
	}
	pos = JrCIR_DEC(notch_posY[ch],JR_NOTCH_SIZE);
	for (i=1;i<JR_NOTCH_SIZE;i++)
	{	
		temp_y -= notch_bufY[ch][pos]*fCoefDen[ch][i] ;
		pos = JrCIR_DEC(pos,JR_NOTCH_SIZE);
	}
	/*����y(n)ֵ��notch_bufY*/
	notch_bufY[ch][notch_posY[ch]] = temp_y;
	/*�����±�*/
	notch_posX[ch] = JrCIR_INC(notch_posX[ch],NOTCH_BUF_X_SIZE);
	notch_posY[ch] = JrCIR_INC(notch_posY[ch],NOTCH_BUF_Y_SIZE);

	return temp_y;
}
#endif




#if 0
#define FIRNUMBER 25
#define SIGNAL1F 1000
#define SIGNAL2F 4500
#define SAMPLEF  10000
#define PI 3.1415926

float InputWave();
float FIR();

float fHn[FIRNUMBER]={ 0.0,0.0,0.001,-0.002,-0.002,0.01,-0.009,
                       -0.018,0.049,-0.02,0.11,0.28,0.64,0.28,
                       -0.11,-0.02,0.049,-0.018,-0.009,0.01,
                       -0.002,-0.002,0.001,0.0,0.0
                     };
float fXn[FIRNUMBER]={ 0.0 };
float fInput,fOutput;
float fSignal1,fSignal2;
float fStepSignal1,fStepSignal2;
float f2PI;
int i;
float fIn[256],fOut[256];
int nIn,nOut;

main()
{
    nIn=0; nOut=0;
    f2PI=2*PI;
    fSignal1=0.0;
    fSignal2=PI*0.1;
    fStepSignal1=2*PI/30;
    fStepSignal2=2*PI*1.4;
    while ( 1 )
    {
        fInput=InputWave();
        fIn[nIn]=fInput;
        nIn++; nIn%=256;
        fOutput=FIR();
        fOut[nOut]=fOutput;
        nOut++;
        if ( nOut>=256 )
        {
            nOut=0;        /* 请在此句上设置软件断�?*/
        }
    }
}

float InputWave()
{
    for ( i=FIRNUMBER-1;i>0;i-- )
        fXn[i]=fXn[i-1];
    fXn[0]=sin(fSignal1)+cos(fSignal2)/6.0;
    fSignal1+=fStepSignal1; 
    if ( fSignal1>=f2PI )    fSignal1-=f2PI;
    fSignal2+=fStepSignal2;
    if ( fSignal2>=f2PI )    fSignal2-=f2PI;
    return(fXn[0]);
}

float FIR()
{
    float fSum;
    fSum=0;
    for ( i=0;i<FIRNUMBER;i++ )
    {
        fSum+=(fXn[i]*fHn[i]);
    }
    return(fSum);
}

#endif
#endif