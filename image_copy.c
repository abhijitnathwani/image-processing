/**
* @file image_copy.c
* @brief This programs opens a BMP image, reads the header, colortable, and image data
	and stores it in a new file. Makes a copy of the image.
* @author Abhijit Nathwani
* @version v0.1
* @date 2017-12-19
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc,char *argv[])
{
	clock_t start, stop; 

	start=clock(); // Note the start time for profiling purposes.

	FILE *fo = fopen("images/lena_copy.bmp","wb"); // Output File name

	int i;

	FILE *streamIn; 
        streamIn = fopen("images/lena512.bmp", "r"); // Input file name
   
        if (streamIn == (FILE *)0) // check if the input file has not been opened succesfully.
	{
            printf("File opening error ocurred. Exiting program.\n");
            exit(0);
 	}

 	unsigned char header[54]; // to store the image header
	unsigned char colorTable[1024]; // to store the colorTable, if it exists.
	
 	int count = 0;
 	for(i=0;i<54;i++) 
 	{
 		header[i] = getc(streamIn);  // strip out BMP header
 		
 	}
 	int width = *(int*)&header[18]; // read the width from the image header
 	int height = *(int*)&header[22]; // read the height from the image header
	int bitDepth = *(int*)&header[28]; // read the bitDepth from the image header

	if(bitDepth <= 8)
		fread(colorTable, sizeof(unsigned char), 1024, streamIn);


	printf("width: %d\n",width);
	printf("height: %d\n",height );

	fwrite(header, sizeof(unsigned char), 54, fo); // write the image header to output file
  	
 	unsigned char buf[height * width]; // to store the image data

	fread(buf, sizeof(unsigned char), (height * width), streamIn);
	
	if(bitDepth <= 8)
		fwrite(colorTable, sizeof(unsigned char), 1024, fo);	

	fwrite(buf, sizeof(unsigned char), (height * width), fo);
 	
	fclose(fo);
 	fclose(streamIn);

	stop = clock(); 
	
	printf("Time: %lf ms\n",((double)(stop - start) * 1000.0 )/ CLOCKS_PER_SEC);

}
