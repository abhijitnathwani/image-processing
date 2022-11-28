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
#define THRESHOLD 128
#define WHITE 255
#define BLACK 0

int main(){

	 FILE *fIn = fopen("extr-resurces/image-processing-images/img4.bmp","r");				//Input File name
	//FILE *fIn = fopen("images/lena512.bmp","r");
	FILE *fOut = fopen("results_imgs/b_w.bmp","w+");		           						 //Output File name

	clock_t start, end;
    double cpu_time_used;
	int i, y;
	unsigned char byte[54];								//to get the image header
	unsigned char colorTable[1024];						//to get the colortable

	if(fIn==NULL)										// check if the input file has not been opened succesfully.
	{										
		printf("File does not exist.\n");
	}

	for(i=0;i<54;i++)									//read the 54 byte header from fIn
	{									
		byte[i]=getc(fIn);								
	}

	fwrite(byte,sizeof(unsigned char),54,fOut);			//write the header back

	// extract image height, width and bitDepth from imageHeader 
	int height = *(int*)&byte[18];
	int width = *(int*)&byte[22];
	int bitDepth = *(int*)&byte[28];

	printf("width: %d\n",width);
	printf("height: %d\n",height );
	printf("bitDepth: %d\n",bitDepth );
	int size=height*width;								//calculate image size

	if(bitDepth<=8)										//if ColorTable present, extract it.
	{
		
		fread(colorTable,sizeof(unsigned char),1024,fIn);
		fwrite(colorTable,sizeof(unsigned char),1024,fOut);
	}

	unsigned char buffer[size][3];							//to store the image data

	//fread(buffer,sizeof(unsigned char),[size][3],fIn);		//read image data

 	start = clock();
#pragma omp parallel for num_threads(4) private(y) 
{



	for(i=0;i<size;i++)									//store 0(black) and 255(white) values to buffer 
		{
		buffer[i][2]=getc(fIn);									
		buffer[i][1]=getc(fIn);									
		buffer[i][0]=getc(fIn);									
		if((buffer[i][0] > THRESHOLD && buffer[i][1] > THRESHOLD)||(buffer[i][0] > THRESHOLD && buffer[i][2] > THRESHOLD)||(buffer[i][1] > THRESHOLD && buffer[i][2] > THRESHOLD)){
		y= WHITE ;
		}else{
		y= BLACK ;	
		}
		#pragma omp critical
		{
		putc(y,fOut);
		putc(y,fOut);
		putc(y,fOut);
		}
		}
}
	end = clock();
 	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("The excution time is %f\n", cpu_time_used);
	fwrite(buffer,sizeof(unsigned char),size,fOut);		//write back to the output image

	fclose(fIn);
	fclose(fOut);

	return 0;
}
