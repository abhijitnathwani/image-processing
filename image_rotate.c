/**
* @file image_rotate.c
* @brief C program to rotate the image by 180-degree. ( Flipping)
* @author Priya Shah
* @version v1
* @date 2018-01-10
*/
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

int image_rgb_rotate_right(int threads, unsigned char header[54], int height, int width, unsigned char buffer[width][height][3], unsigned char colorTable[1024])
{
	FILE *fOut = fopen("out/rotate_right.bmp","w+");		    	//Output File name
	int i,j;
	int tempH, tempW;
	unsigned char heightA[4];
	unsigned char widthA[4];
	int bitDepth = *(int *)&header[28];
	int size = height*width;								//calculate image size
	unsigned char out_buffer[height][width][3];

	#pragma omp parallel for private(j, tempH) //num_threads(threads)
	for (i = 0; i < width; i++) // to rotate right
	{
		tempH = height;
		for (j = 0; j < height; j++)
		{
			tempH--;
			out_buffer[tempH][i][2] = buffer[i][j][2];
			out_buffer[tempH][i][1] = buffer[i][j][1];
			out_buffer[tempH][i][0] = buffer[i][j][0];
		}
	}

	#pragma omp parallel for //num_threads(threads)
	for (i=0;i<4;i++){
		heightA[i]=header[18+i];
		widthA[i]=header[22+i];
		header[18 + i] = widthA[i];
		header[22 + i] = heightA[i];
	}

	fwrite(header, sizeof(unsigned char), 54, fOut); // write the header back
	if (bitDepth <= 8)								 // if ColorTable present, extract it.
	{
		fwrite(colorTable, sizeof(unsigned char), 1024, fOut);
	}

// #pragma omp parallel for num_threads(threads) private(j) ordered
	for (i = 0; i < height; i++)
	{
		// #pragma omp ordered
		for (j = 0; j < width; j++)
		{
			putc(out_buffer[i][j][2], fOut);
			putc(out_buffer[i][j][1], fOut);
			putc(out_buffer[i][j][0], fOut);
		}
	}
	fclose(fOut);
	return 0;
}

int image_rgb_rotate_left(int threads, unsigned char header[54], int height, int width, unsigned char buffer[width][height][3], unsigned char colorTable[1024])
{
	FILE *fOut = fopen("out/rotate_left.bmp","w+");		    	//Output File name
	int i,j,choice;
	int tempH, tempW;
	unsigned char heightA[4];
	unsigned char widthA[4];
	int bitDepth = *(int *)&header[28];
	int size = height*width;								//calculate image size
	unsigned char out_buffer[height][width][3];

	#pragma omp parallel for private(j, tempH) //num_threads(threads)
	for (i = 0; i < height; i++) // to rotate left
	{
		tempW = width;
		for (j = 0; j < width; j++)
		{
			tempW--;
			out_buffer[i][tempW][2] = buffer[j][i][2];
			out_buffer[i][tempW][1] = buffer[j][i][1];
			out_buffer[i][tempW][0] = buffer[j][i][0];
		}
	}

	#pragma omp parallel for //num_threads(threads)
	for (i = 0; i < 4; i++)
	{
		heightA[i] = header[18 + i];
		widthA[i] = header[22 + i];
		header[18 + i] = widthA[i];
		header[22 + i] = heightA[i];
	}
	fwrite(header, sizeof(unsigned char), 54, fOut); // write the header back
	if (bitDepth <= 8)								 // if ColorTable present, extract it.
	{
		fwrite(colorTable, sizeof(unsigned char), 1024, fOut);
	}

	// #pragma omp parallel for num_threads(threads) private(j, tempH) ordered
	for (i = 0; i < height; i++)
	{
		// #pragma omp ordered
		for (j = 0; j < width; j++)
		{
			putc(out_buffer[i][j][2], fOut);
			putc(out_buffer[i][j][1], fOut);
			putc(out_buffer[i][j][0], fOut);
		}
	}
	fclose(fOut);
	return 0;
}

int image_rgb_rotate_180(int threads, unsigned char header[54], int height, int width, unsigned char buffer[width][height][3], unsigned char colorTable[1024])
{
	FILE *fOut = fopen("out/rotate_180.bmp","w+");		    	//Output File name
	int i,j,choice;
	int tempH, tempW;
	unsigned char heightA[4];
	unsigned char widthA[4];

	int bitDepth = *(int *)&header[28];

	int size = height*width;								//calculate image size
	unsigned char out_buffer[width][height][3];
	
	#pragma omp parallel for private(j, tempH) //num_threads(threads)
	for (i = width-1; i >= 0; i--) // to rotate left
	{
		for (j = height-1; j >= 0; j--)
		{
			out_buffer[i][j][2] = buffer[width - 1 - i][j][2];
			out_buffer[i][j][1] = buffer[width - 1 - i][j][1];
			out_buffer[i][j][0] = buffer[width - 1 - i][j][0];
		}
	}

	fwrite(header, sizeof(unsigned char), 54, fOut); // write the header back
	if (bitDepth <= 8)								 // if ColorTable present, extract it.
	{
		fwrite(colorTable, sizeof(unsigned char), 1024, fOut);
	}

	// #pragma omp parallel for num_threads(threads) private(j, tempH) ordered
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

