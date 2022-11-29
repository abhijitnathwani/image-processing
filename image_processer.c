
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

// #include "image_bright.c"
// #include "image_dark.c"
// #include "image_colortosepia.c"
// //#include "image_simulate_cvd.c"
// #include "image_correct_cvd.c"
#include "black_white.c"

int colored() {
	//FILE *fIn = fopen("images/nature-4080511_960_720.bmp","r");			// Input File name
	FILE *fIn = fopen("images/img4.bmp","r");			// Input File name
	unsigned char header[54];
	int i;

    if(fIn==NULL)							// check if the input file has not been opened succesfully.
	{											
		printf("File does not exist.\n");
	}
#pragma omp parallel for num_threads(1) 
	for(i=0;i<54;i++)						// read the 54 byte header from fIn
	{								
		header[i] = getc(fIn);								
	}

    int height = *(int*)&header[18];
	int width = *(int*)&header[22];
	int bitDepth = *(int*)&header[28];

	int size = height*width;	
									//calculate image size
	unsigned char buffer[size][3];								//to store the image data

#pragma omp parallel for num_threads(1) 
	for(i=0;i<size;i++){

		buffer[i][2] = getc(fIn);									//blue
		buffer[i][1] = getc(fIn);									//green
		buffer[i][0] = getc(fIn);									//red

	}
	printf("height: %d\n",height);
	printf("width: %d\n",width);
printf("sizw: %d\n",size);
#pragma omp parallel sections
{
    // #pragma omp section
    // image_colortosepia(header, size, buffer);

    // #pragma omp section 
	// simulate_cvd_protanopia(header, size, buffer);

    // #pragma omp section
	// simulate_cvd_deuteranopia(header, size, buffer);

    // #pragma omp section
	// simulate_cvd_tritanopia(header, size, buffer);
	
    // #pragma omp section
	// correct_cvd_protanopia(header, size, buffer);

    // #pragma omp section
	// correct_cvd_deuteranopia(header, size, buffer);

    // #pragma omp section
	// correct_cvd_tritanopia(header, size, buffer);

	 #pragma omp section
	black_and_white(header, size, buffer);

}

   	fclose(fIn);
	return 0;
}
int C = 4;

int main(int argc, char *argv[]) {

    // if(argc > 1)
    //     C = atoi(argv[1]);

    omp_set_num_threads(C);

    // Load image uncolored
   //	FILE *fIn = fopen("images/lena512.bmp","r");			//Input File name

    double start, stop;
    start = omp_get_wtime();

	// int i;
	// unsigned char header[54];
    // unsigned char colorTable[1024];
	
    // if(fIn==NULL)							// check if the input file has not been opened succesfully.
	// {											
	// 	printf("File does not exist.\n");
	// 	return -1;
	// }

	// for(i=0;i<54;i++)						// read the 54 byte header from fIn
	// {									
	// 	header[i] = getc(fIn);								
	// }


	// // extract image height, width and bitDepth from imageHeader 
	// int height = *(int*)&header[18];
	// int width = *(int*)&header[22];
	// int bitDepth = *(int*)&header[28];

	// if(bitDepth <= 8)					//if ColorTable present, extract it.
	// {
	// 	fread(colorTable,sizeof(unsigned char),1024,fIn);
	// }

	// int size = height * width;				//calculate image size
	// unsigned char buffer[size];				//to store the image data

	// fread(buffer,sizeof(unsigned char),size,fIn);		//read image data

// #pragma omp parallel sections
// {
    // #pragma omp section 
    // image_dark(header, colorTable, size, buffer);

    // #pragma omp section
    // image_bright(header, colorTable, size, buffer);

    // #pragma omp section
    colored();
// }

    stop = omp_get_wtime();

	printf("Time: %lf ms\n",((double)(stop - start)*1000));
	//fclose(fIn);
}