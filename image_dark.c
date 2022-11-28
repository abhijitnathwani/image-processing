/**
* @file image_dark.c
* @brief C program to darken the image by reducing the pixel values.
* @author Priya Shah
* @version v1
* @date 2018-01-10
*/
#include <stdio.h>
#define THRESHOLD 40

int image_dark(unsigned char * header, unsigned char * colorTable ,int size, unsigned char * buffer)
{
	FILE *fOut = fopen("out/lena_dark.bmp","w+");		    	//Output File name

	int i;

	fwrite(header,sizeof(unsigned char),54,fOut);				//write the header back

	// extract image height, width and bitDepth from imageHeader 
	int bitDepth = *(int*)&header[28];

	if(bitDepth <= 8)										//if ColorTable present, extract it.
	{
		fwrite(colorTable,sizeof(unsigned char),1024,fOut);
	}

	unsigned char image_buffer[size];
	// Parallelizable segment
	for(i=0;i<size;i++)										//decreasing pixel values to get image bright
	{
		if (buffer[i] > THRESHOLD) 
			image_buffer[i] = buffer[i] - THRESHOLD;
		else
			image_buffer[i] = 0;
		// image_buffer[i] = buffer[i];
	}
	
	fwrite(image_buffer,sizeof(unsigned char),size,fOut);			//write back to the output image
	fclose(fOut);
	
	return 0;
}
