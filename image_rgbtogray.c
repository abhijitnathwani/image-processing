/**
* @file image_rgbtogray.c
* @brief C program to convert an RGB image to grayscale.
* @author Priya Shah
* @version v1
* @date 2018-01-10
*/
#include <stdio.h>
#include <time.h>

int image_rgbtogray(unsigned char header[54], int size, unsigned char buffer[size][3])
{
	double start, stop;

	// start = omp_get_wtime();												// Note the start time for profiling purposes.
	FILE *fOut = fopen("results_imgs/lena_gray.bmp","w+");		    		//Output File name

	int i,j,y;
	fwrite(header, sizeof(unsigned char), 54, fOut); // write the header back

	// extract image height, width and bitDepth from imageHeader
	int height = *(int *)&header[18];
	int width = *(int *)&header[22];
	int bitDepth = *(int *)&header[28];

	// printf("inside rgbtogray : size: %d\n", size);
	// printf("inside rgbtogray : width: %d\n", width);
	// printf("inside rgbtogray : height: %d\n", height);

	// #pragma omp parallel for private(y) ordered		
	for(i=0;i<size;i++)											//RGB to gray
	{

		// #pragma omp ordered
		// {
			y = 0;
			y = (buffer[i][0] * 0.3) + (buffer[i][1] * 0.59) + (buffer[i][2] * 0.11); // conversion formula of rgb to gray
			putc(y, fOut);
			putc(y, fOut);
			putc(y, fOut);
		// }


	}
	
	fclose(fOut);
	// stop = omp_get_wtime();
	// printf("time to convert rgb to gray (ms) = %lf\n", ((double)(stop - start) * 1000.0));
	return 0;
}
