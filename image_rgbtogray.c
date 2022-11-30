/**
* @file image_rgbtogray.c
* @brief C program to convert an RGB image to grayscale.
* @author Priya Shah
* @version v1
* @date 2018-01-10
*/
#include <stdio.h>
#include <time.h>

int image_rgbtogray(int threads, unsigned char header[54], int height, int width, unsigned char buffer[width][height][3], unsigned char colorTable[1024])
{
	FILE *fOut = fopen("out/image_gray.bmp", "w+"); // Output File name
	int i, j;
	int tempH, tempW;
	unsigned char heightA[4];
	unsigned char widthA[4];
	int bitDepth = *(int *)&header[28];
	int size = height * width; // calculate image size

	double out_buffer[width][height];

	#pragma omp parallel for private(j) //num_threads(threads)
	for (i = 0; i < width; i++) // to rotate right
	{
		for (j = 0; j < height; j++)
		{
			out_buffer[i][j] = (buffer[i][j][0] * 0.3 + buffer[i][j][1] * 0.59 + buffer[i][j][2] * 0.11);
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
			putc(out_buffer[i][j], fOut);
			putc(out_buffer[i][j], fOut);
			putc(out_buffer[i][j], fOut);
		}
	}
	fclose(fOut);
	return 0;
}
