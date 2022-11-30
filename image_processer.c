
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include "image_bright.c"
#include "image_dark.c"
#include "image_colortosepia.c"
#include "image_simulate_cvd.c" //there is an issue related to this piece of code #To be reviewed 
#include "image_correct_cvd.c"
#include "image_rgbtogray.c"
#include "black_white.c"
#include "image_blur_color.c"
#include "image_blur_gray.c"
#include "image_rotate.c"
#include "negative_image.c"

int number_of_threads = 1;

int colored() {
	omp_set_num_threads(14);

	printf("******** This code is executing the colored image processing applications ****** \n");
	FILE *fIn = fopen("images/airplane.bmp","r");			// Input File name
	FILE *fIn3D = fopen("images/airplane.bmp", "r");		// Input File name
	unsigned char header[54];
	unsigned char colorTable[1024];
	int i, j;
	if(fIn==NULL)							// check if the input file has not been opened succesfully.
	{											
		printf("File does not exist.\n");
	}
	#pragma omp parallel for num_threads(1) //ordered
	for(i=0;i<54;i++)						// read the 54 byte header from fIn
	{			
		// #pragma omp ordered
		{
			header[i] = getc(fIn);
			getc(fIn3D);
		}												
	}
	
    int height = *(int*)&header[18];
	int width = *(int*)&header[22];
	int bitDepth = *(int*)&header[28];
	if (bitDepth <= 8) // if ColorTable present, extract it.
	{
		fread(colorTable, sizeof(unsigned char), 1024, fIn);
	}

	int size = height*width;		//calculate image size
	unsigned char D3buffer[width][height][3]; // to store the image data
	
	#pragma omp parallel for private(j) num_threads(1) //ordered
	for(int i=0;i<width;i++)											
	{
		// #pragma omp ordered
		for (j=0;j<height;j++){
            D3buffer[i][j][2]=getc(fIn3D);									//blue
            D3buffer[i][j][1]=getc(fIn3D);									//green
            D3buffer[i][j][0]=getc(fIn3D);									//red
        }
	}
	unsigned char buffer[size][3];								//to store the image data

	#pragma omp parallel for num_threads(1) //ordered 
	for(i=0;i<size;i++){
		// #pragma omp ordered
		{
			buffer[i][2] = getc(fIn); // blue
			buffer[i][1] = getc(fIn); // green
			buffer[i][0] = getc(fIn); // red
		}
	}
	printf("height: %d\n",height);
	printf("width: %d\n",width);
	printf("size: %d\n",size);
	#pragma omp parallel sections
	{
		#pragma omp section
		image_colortosepia(header, size, buffer);

		#pragma omp section
		simulate_cvd_protanopia(header, size, buffer);

		#pragma omp section
		simulate_cvd_deuteranopia(header, size, buffer);

		#pragma omp section
		simulate_cvd_tritanopia(header, size, buffer);

		#pragma omp section
		correct_cvd_protanopia(header, size, buffer);

		#pragma omp section
		correct_cvd_deuteranopia(header, size, buffer);

		#pragma omp section
		correct_cvd_tritanopia(header, size, buffer);

		#pragma omp section
		black_and_white(header, size, buffer, bitDepth, colorTable);

		#pragma omp section
		image_bluring_color(header, size, height, width, buffer , bitDepth, colorTable);


		#pragma omp section
		image_rgb_rotate_right(number_of_threads, header, height, width, D3buffer, colorTable);

		#pragma omp section
		image_rgb_rotate_left(number_of_threads, header, height, width, D3buffer, colorTable);

		#pragma omp section
		image_rgb_rotate_180(number_of_threads, header, height, width, D3buffer, colorTable);

		#pragma omp section
		image_negative(number_of_threads, header, height, width, D3buffer, colorTable);

		#pragma omp section
		image_rgbtogray(number_of_threads, header, height, width, D3buffer, colorTable);
	}

	fclose(fIn);
	// fclose(fOut);
	return 0;
}


int nonColored() {
	omp_set_num_threads(3);

	printf("******** This code is executing the non-colored image processing applications ****** \n");


	FILE *fIn = fopen("images/lena512.bmp","r");			// Input File name
	unsigned char header[54];
	unsigned char colorTable[1024];
	int i;

    if(fIn==NULL)							// check if the input file has not been opened succesfully.
	{											
		printf("File does not exist.\n");
	}
	#pragma omp parallel for num_threads(1) //ordered
	for(i=0;i<54;i++)						// read the 54 byte header from fIn
	{					
		// #pragma omp ordered
		{
			header[i] = getc(fIn);
		}			
	}

    int height = *(int*)&header[18];
	int width = *(int*)&header[22];
	int bitDepth = *(int*)&header[28];

	if(bitDepth<=8)										//if ColorTable present, extract it.
	{
		fread(colorTable,sizeof(unsigned char),1024,fIn);
	}

	int size = height*width;	//calculate image size					
	unsigned char buffer[size];	//to store the image data

	#pragma omp parallel for num_threads(1) 
	for(i=0;i<size;i++){
		buffer[i] = getc(fIn);			
	}
	printf("height: %d\n",height);
	printf("width: %d\n",width);
	printf("size: %d\n",size);
	#pragma omp parallel sections
	{
	 #pragma omp section
	image_bluring_gray(header, size, height, width, buffer , bitDepth, colorTable);//lena512.bmp
	
	#pragma omp section 
    image_dark(header, colorTable, size, buffer);

    #pragma omp section
    image_bright(header, colorTable, size, buffer);
}
   	fclose(fIn);
	return 0;
}


int C = 1;

int main(int argc, char *argv[]) {

    

    double CStart, CStop,NCStart, NCStop;

	#pragma omp parallel num_threads(1)
	{
		#pragma omp task
		{
			CStart = omp_get_wtime();
			colored();
			CStop = omp_get_wtime();
		}
		#pragma omp task
		{
			NCStart = omp_get_wtime();
			nonColored();
			NCStop = omp_get_wtime();
		}
	}

	printf("colored excution Time: %lf ms\n",((double)(CStop-CStart)*1000));
	printf("non-colored excution Time: %lf ms\n",((double)(NCStop-NCStart)*1000));
	printf("overall excution Time: %lf ms\n",((double)((NCStop+CStop)-(NCStart+CStart))*1000));
}