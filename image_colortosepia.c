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

int image_colortosepia(char * header, int size, unsigned char * buffer)
{
	FILE *fOut = fopen("out/lena_sepia.bmp","w+");		    		//Output File name

	int i,r,g,b;

	fwrite(header,sizeof(unsigned char),54,fOut);					//write the header back

	// extract image height, width and bitDepth from imageHeader 
	int bitDepth = *(int*)&header[28];

	// Parallelizable segment
	for(i=0;i<size;i++){
		r = 0;
		g = 0;
		b = 0;

		//conversion formula of rgb to sepiabuffer
		r = (buffer[i*3 + 2]*0.393) + (buffer[i*3 + 1]*0.769)	+ (buffer[i*3 + 0]*0.189);
		g = (buffer[i*3 + 2]*0.349) + (buffer[i*3 + 1]*0.686)	+ (buffer[i*3 + 0]*0.168);
		b = (buffer[i*3 + 2]*0.272) + (buffer[i*3 + 1]*0.534)	+ (buffer[i*3 + 0]*0.131);

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

	return 0;
}
