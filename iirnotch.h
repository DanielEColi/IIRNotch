#ifndef __IIR_NOTCH_H__
#define __IIR_NOTCH_H__

#include <math.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

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
__declspec(dllexport) int AzIIRNotchFilter(short ch,int xn, float fnotch,float bw, float fs, short init);

#ifdef __cplusplus
}

#endif

#endif
