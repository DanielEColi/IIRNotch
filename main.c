#include <stdio.h>
#include <stdlib.h>
#include "iirnotch.h"

#define		AZ_SAMPLE_RATE		(1024)		//采样率Hz
#define		AZ_NOTCH_RATE		(60)		//陷波频率Hz
#define     AZ_FREQ_NORM(f,fs)	((double)f/(fs/2.0))							//归一化频率
#define		AZ_NOTCH_W0			(AZ_FREQ_NORM(AZ_NOTCH_RATE,AZ_SAMPLE_RATE))	//归一化陷波频率
#define		AZ_PI				(3.1415926f)

FILE *pFile;
void main(void)
{
	int i;
	int val,y1,y2,y3;

	pFile = fopen("testSignal.txt","wt");
	if (pFile==NULL)
	{
		printf("Fail!\n");
		return;
	}
	else printf("PASS!\n");

// 	AzInitNotchByChannel(0,AZ_NOTCH_W0,(float)AZ_FREQ_NORM(2.0,AZ_SAMPLE_RATE));
// 	AzInitNotchByChannel(1,AZ_NOTCH_W0,(float)AZ_FREQ_NORM(2.0,AZ_SAMPLE_RATE));
// 	AzInitNotchByChannel(11,AZ_NOTCH_W0,(float)AZ_FREQ_NORM(2.0,AZ_SAMPLE_RATE));
	for (i=0;i<AZ_SAMPLE_RATE*5;i++)
	{
		if (i==0)
		{
			val = (int)(2e4*cos((double)(2*AZ_PI*1*i)/AZ_SAMPLE_RATE)+1e4*cos((double)(2*AZ_PI*59.8*i)/AZ_SAMPLE_RATE));
			y1 = AzIIRNotchFilter(0,val,AZ_NOTCH_RATE,2,AZ_SAMPLE_RATE,1);
			val = (int)(2e4*cos((double)(2*AZ_PI*1*i)/AZ_SAMPLE_RATE)+1e4*cos((double)(2*AZ_PI*60*i)/AZ_SAMPLE_RATE));
			y2 = AzIIRNotchFilter(1,val,AZ_NOTCH_RATE,2,AZ_SAMPLE_RATE,1);
			val = (int)(2e4*cos((double)(2*AZ_PI*1*i)/AZ_SAMPLE_RATE)+1e4*cos((double)(2*AZ_PI*60.2*i)/AZ_SAMPLE_RATE));
			y3 = AzIIRNotchFilter(11,val,AZ_NOTCH_RATE,2,AZ_SAMPLE_RATE,1);
		}
		else
		{
			val = (int)(2e4*cos((double)(2*AZ_PI*1*i)/AZ_SAMPLE_RATE)+1e4*cos((double)(2*AZ_PI*59.8*i)/AZ_SAMPLE_RATE));
			y1 = AzIIRNotchFilter(0,val,AZ_NOTCH_RATE,2,AZ_SAMPLE_RATE,0);
			val = (int)(2e4*cos((double)(2*AZ_PI*1*i)/AZ_SAMPLE_RATE)+1e4*cos((double)(2*AZ_PI*60*i)/AZ_SAMPLE_RATE));
			y2 = AzIIRNotchFilter(1,val,AZ_NOTCH_RATE,2,AZ_SAMPLE_RATE,0);
			val = (int)(2e4*cos((double)(2*AZ_PI*1*i)/AZ_SAMPLE_RATE)+1e4*cos((double)(2*AZ_PI*60.2*i)/AZ_SAMPLE_RATE));
			y3 = AzIIRNotchFilter(11,val,AZ_NOTCH_RATE,2,AZ_SAMPLE_RATE,0);
		}

		//fprintf(pFile,"%f\t%f\t%f\t%f\n",val,y1,y2,y3);
		fprintf(pFile,"%d\t%d\t%d\t%d\n",val,y1,y2,y3);
	}

	fclose(pFile);
// 	getchar();

}
