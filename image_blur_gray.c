/**
* @file image_blur_gray.c
* @brief C program to blur the grayscale image, using convolution
* @author Priya Shah
* @version v1
* @date 2018-01-10
*/
#include <stdio.h>
#include <time.h>

int main()
{
	clock_t start, stop;

	start = clock();											// Note the start time for profiling purposes.

	FILE *fIn = fopen("lena_color.bmp","r");					//Input File name
	FILE *fOut = fopen("lena_gray.bmp","w+");		    		//Output File name

	int i,j,y;
	unsigned char byte[54];
	
	if(fIn==NULL)												// check if the input file has not been opened succesfully.
	{											
		printf("File does not exist.\n");
	}

	for(i=0;i<54;i++)											//read the 54 byte header from fIn
	{									
		byte[i] = getc(fIn);								
	}

	fwrite(byte,sizeof(unsigned char),54,fOut);					//write the header back

	// extract image height, width and bitDepth from imageHeader 
	int height = *(int*)&byte[18];
	int width = *(int*)&byte[22];
	int bitDepth = *(int*)&byte[28];

	printf("width: %d\n",width);
	printf("height: %d\n",height );

	int size = height*width;									//calculate image size

	unsigned char buffer[size],out[size];						//to store the image data
	
	float v=1.0 / 9.0;											
	float kernel[3][3]={{v,v,v},								//initialize the blurrring kernel
						{v,v,v},
						{v,v,v}};
	
	fread(buffer,sizeof(unsigned char),size,fIn);				//read the image data into buffer

	for(i=0;i<size;i++)
	{
		out[i] = buffer[i];										//copy image data to out bufer
	}

	for(x=1;x<height-1;x++)
	{
		for(y=1;y<width-1;y++)
		{
			float sum= 0.0;
			for(i=-1;i<=1;++i)									
			{
				for(j=-1;j<=1;++j)
				{
					sum=sum+(float)kernel[i+1][j+1]*buffer[(x+i)*width+(y+j)];	//matrix multiplication with kernel
				}
			}
			out[(x)*width+(y)]=sum;
		}
	}

	fwrite(out,sizeof(unsigned char),size,fOut);				//write image data back to the file

	fclose(fIn);
	fclose(fOut);
	return 0;
}

