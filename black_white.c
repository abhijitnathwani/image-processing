/**
* @file black_white.c
* @brief C program to convert image to pure black and white, without using any libraries.
* @author Priya Shah
* @version v1
* @date 2018-01-10
*/
#include <omp.h>
#include <stdio.h>
#include <time.h>
#define BW_THRESHOLD 128
#define WHITE 255
#define BLACK 0

int black_and_white(unsigned char header[54], int size, unsigned char buffer[size][3])
{

	FILE *fOut = fopen("out/black_and_white.bmp","w+");		           						 //Output File name


	int i, y;
	// unsigned char byte[54];								//to get the image header
	// unsigned char colorTable[1024];						//to get the colortable

	

	

	fwrite(header,sizeof(unsigned char),54,fOut);			//write the header back

	// extract image height, width and bitDepth from imageHeader 
	// int height = *(int*)&byte[18];
	// int width = *(int*)&byte[22];
	// int bitDepth = *(int*)&byte[28];

	// printf("width: %d\n",width);
	// printf("height: %d\n",height );
	// printf("bitDepth: %d\n",bitDepth );

	// if(bitDepth<=8)										//if ColorTable present, extract it.
	// {
		
	// 	fread(colorTable,sizeof(unsigned char),1024,fIn);
	// 	fwrite(colorTable,sizeof(unsigned char),1024,fOut);
	// }


	//fread(buffer,sizeof(unsigned char),[size][3],fIn);		//read image data

//#pragma omp parallel for num_threads(4) private(y) 
//{


printf("size2: %d\n",size);
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
//}

	fwrite(buffer,sizeof(unsigned char),size,fOut);		//write back to the output image

	fclose(fOut);

	return 0;
}
