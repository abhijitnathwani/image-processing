#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int image_negative(int threads, unsigned char header[54], int height, int width, unsigned char buffer[width][height][3], unsigned char colorTable[1024])
{
	FILE *fOut = fopen("out/negative_image.bmp", "w+"); // Output File name
	int i,j; // loop counter variables
	unsigned char out_buffer[width][height][3];
	int bitDepth = *(int*)&header[28];

	//Calculate the mean of the image
	#pragma omp parallel for private(j) //num_threads(threads)
	for(i = 0; i < width; i++){
	      for(j = 0; j < height; j++){
			  out_buffer[i][j][0] = 0xFF - buffer [i][j][0];
			  out_buffer[i][j][1] = 0xFF - buffer [i][j][1];
			  out_buffer[i][j][2] = 0xFF - buffer [i][j][2];
	      }
	}

	fwrite(header, sizeof(unsigned char), 54, fOut); // write the header back
	if (bitDepth <= 8)								 // if ColorTable present, extract it.
	{
		fwrite(colorTable, sizeof(unsigned char), 1024, fOut);
	}

	// #pragma omp parallel for num_threads(threads) private(j) ordered
	for (i = 0; i < width; i++)
	{
		// #pragma omp ordered
		for (j = 0; j < height; j++)
		{
			putc(out_buffer[i][j][2], fOut);
			putc(out_buffer[i][j][1], fOut);
			putc(out_buffer[i][j][0], fOut);
		}
	}
	fclose(fOut);
	return 0;
}
