#include <stdio.h>
#include <stdlib.h>
#include "iirnotch.h"


FILE *pFile;
void main(void)
{
	int i;
	float val,valFilt;

	pFile = fopen("testSignal.txt","wt");
	if (pFile==NULL)
	{
		printf("Fail!\n");
		return;
	}
	else printf("PASS!\n");
#if 1
	InitNotchByChannel(0,(float)__NOTCH_W0);
	for (i=0;i<__SAMPLE_RATE*5;i++)
	{
		val = (float)(10*cos((double)(2*__PI__*1*i)/__SAMPLE_RATE)+5*cos((double)(2*__PI__*49.8*i)/__SAMPLE_RATE));
		valFilt = IIRNotchFilter(0,val);
		fprintf(pFile,"%f\t%f\n",val,valFilt);
	}
#else 
	InitNotchByChannel(0,(float)__NOTCH_W0);
	for (i=0;i<__SAMPLE_RATE*5;i++)
	{
		val = (float)(10*cos((double)(2*__PI__*1*i)/__SAMPLE_RATE)+5*cos((double)(2*__PI__*49.8*i)/__SAMPLE_RATE));
		if (i==0)valFilt=IIRNotchFilter(0,val,1);
		else valFilt=IIRNotchFilter(0,val,0);
	}
#endif
	fclose(pFile);
// 	getchar();

}
