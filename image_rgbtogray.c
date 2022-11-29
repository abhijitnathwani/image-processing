/**
* @file image_rgbtogray.c
* @brief C program to convert an RGB image to grayscale.
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

	FILE *fIn = fopen("extr-resurces/image-processing-images/img4.bmp","r");					//Input File name
	FILE *fOut = fopen("results_imgs/lena_gray.bmp","w+");		    		//Output File name

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

	unsigned char buffer[size][3];								//to store the image data
	
			
	for(i=0;i<size;i++)											//RGB to gray
	{
		y=0;
		buffer[i][2]=getc(fIn);									//blue
		buffer[i][1]=getc(fIn);									//green
		buffer[i][0]=getc(fIn);									//red
			
		y=(buffer[i][0]*0.3) + (buffer[i][1]*0.59)	+ (buffer[i][2]*0.11);			//conversion formula of rgb to gray

		putc(y,fOut);
		putc(y,fOut);
		putc(y,fOut);
	}
	
	fclose(fOut);
	fclose(fIn);

	stop = clock();
	printf("\nCLOCKS_PER_SEC = %ld\n",stop-start); 
	printf("%lf ms\n",((double)(stop-start) * 1000.0)/CLOCKS_PER_SEC );
	return 0;
}
