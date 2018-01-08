/**
* @file image_bright.c
* @brief C program to make the image brighter by increasing pixel values.
* @author Priya Shah
* @version v1
* @date 2018-01-06
*/
#include <stdio.h>
#include <stdlib.h>
#define MAX_COLOR 255
#define BRIGHTNESS_FACTOR 25


int main()
{
	FILE *fIn = fopen("lena512.bmp","r");			//Input File name
	FILE *fOut = fopen("lena_bright.bmp","w+");	    	//Output File name

	int i;
	unsigned char byte[54],colorTable[1024];
	
	if(fIn==NULL)						// check if the input file has not been opened succesfully.
	{											
		printf("File does not exist.\n");
		exit(1);
	}

	for(i=0;i<54;i++)					//read the 54 byte header from fIn
	{									
		byte[i] = getc(fIn);								
	}

	fwrite(byte,sizeof(unsigned char),54,fOut);		//write the header back

	// extract image height, width and bitDepth from imageHeader 
	int height = *(int*)&byte[18];
	int width = *(int*)&byte[22];
	int bitDepth = *(int*)&byte[28];

	printf("width: %d\n",width);
	printf("height: %d\n",height );

	int size = height*width;				//calculate image size

	if(bitDepth <= 8)					//if ColorTable present, extract it.
	{
		fread(colorTable,sizeof(unsigned char),1024,fIn);
		fwrite(colorTable,sizeof(unsigned char),1024,fOut);
	}

	
	unsigned char buffer[size];				//to store the image data
	int temp;

	fread(buffer,sizeof(unsigned char),size,fIn);		//read image data

	for(i=0;i<size;i++)					//increasing pixel values to get image bright
	{
		temp = buffer[i] + BRIGHTNESS_FACTOR;
		buffer[i] = (temp > MAX_COLOR) ? MAX_COLOR : temp;
	}
	
	fwrite(buffer,sizeof(unsigned char),size,fOut);		//write back to the output image

	fclose(fIn);
	fclose(fOut);

	return 0;
}
