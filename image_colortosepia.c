/**
* @file image_colortosepia.c
* @brief C Program to apply Sepia filter to RGB image.
* @author Priya Shah
* @version v1
* @date 2018-01-10
*/
#include <stdio.h>
#include <time.h>
#define MAX_VALUE 255

int main()
{
	clock_t start, stop;

	start = clock();											// Note the start time for profiling purposes.

	FILE *fIn = fopen("lena_color.bmp","r");					//Input File name
	FILE *fOut = fopen("lena_sepia.bmp","w+");		    		//Output File name

	int i,r,g,b;
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

	for(i=0;i<size;i++){
		r = 0;
		g = 0;
		b = 0;
		buffer[i][2] = getc(fIn);									//blue
		buffer[i][1] = getc(fIn);									//green
		buffer[i][0] = getc(fIn);									//red

		//conversion formula of rgb to sepia
		r = (buffer[i][0]*0.393) + (buffer[i][1]*0.769)	+ (buffer[i][2]*0.189);
		g = (buffer[i][0]*0.349) + (buffer[i][1]*0.686)	+ (buffer[i][2]*0.168);
		b = (buffer[i][0]*0.272) + (buffer[i][1]*0.534)	+ (buffer[i][2]*0.131);

		if(r > MAX_VALUE){											//if value exceeds
			r = MAX_VALUE;
		}
		if(g > MAX_VALUE){
			g = MAX_VALUE;
		}
		if(b > MAX_VALUE){
			b = MAX_VALUE;
		}
		putc(b,fOut);
		putc(g,fOut);
		putc(r,fOut);
		
	}
	
		
	fclose(fOut);
	fclose(fIn);

	stop = clock();
	printf("\nCLOCKS_PER_SEC = %ld\n",stop-start); 
	printf("%lf ms\n",((double)(stop-start) * 1000.0)/CLOCKS_PER_SEC );
	return 0;
}
