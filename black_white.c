
#include <omp.h>
#include <stdio.h>
#include <time.h>
#define BW_THRESHOLD 128
#define WHITE 255
#define BLACK 0

int black_and_white(unsigned char header[54], int size, unsigned char buffer[size][3], int bitDepth, unsigned char colorTable[1024])
{

	FILE *fOut = fopen("out/black_and_white.bmp","w+");		           						 //Output File name


	int i, y;

	fwrite(header,sizeof(unsigned char),54,fOut);			//write the header back

	if(bitDepth<=8)										//if ColorTable present, extract it.
	{
		fwrite(colorTable,sizeof(unsigned char),1024,fOut);
	}

	#pragma omp parallel for num_threads(4) ordered
	for(i=0;i<size;i++)									//store 0(black) and 255(white) values to buffer 
		{								
		if((buffer[i][0] > BW_THRESHOLD && buffer[i][1] > BW_THRESHOLD)||(buffer[i][0] > BW_THRESHOLD && buffer[i][2] > BW_THRESHOLD)||(buffer[i][1] > BW_THRESHOLD && buffer[i][2] > BW_THRESHOLD)){
		y= WHITE ;
		}else{
		y= BLACK ;	
		}
		#pragma omp ordered
			{
			putc(y,fOut);
			putc(y,fOut);
			putc(y,fOut);
			}
		}

	fwrite(buffer,sizeof(unsigned char),size,fOut);		//write back to the output image

	fclose(fOut);

	return 0;
}
