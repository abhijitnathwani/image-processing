
#include <stdio.h>

#include "image_bright.c"
#include "image_dark.c"
#include "image_colortosepia.c"

// include function

int colored() {
	FILE *fIn = fopen("images/lena_color.bmp","r");			// Input File name

	unsigned char header[54];
	int i;

    if(fIn==NULL)							// check if the input file has not been opened succesfully.
	{											
		printf("File does not exist.\n");
	}

	for(i=0;i<54;i++)						// read the 54 byte header from fIn
	{									
		header[i] = getc(fIn);								
	}

    int height = *(int*)&header[18];
	int width = *(int*)&header[22];
	int bitDepth = *(int*)&header[28];

	int size = height*width;									//calculate image size
	unsigned char buffer[size][3];								//to store the image data

	for(i=0;i<size;i++){
		buffer[i][2] = getc(fIn);									//blue
		buffer[i][1] = getc(fIn);									//green
		buffer[i][0] = getc(fIn);									//red
    }

    image_colortosepia(header, size, buffer);
    //      * image simulate
    //      * image correct

   	fclose(fIn);
}

int main(int argc, char *argv[]) {

    // Load image uncolored
   	FILE *fIn = fopen("images/lena512.bmp","r");			//Input File name

	int i;
	unsigned char header[54];
    unsigned char colorTable[1024];
	
    if(fIn==NULL)							// check if the input file has not been opened succesfully.
	{											
		printf("File does not exist.\n");
		return -1;
	}

	for(i=0;i<54;i++)						// read the 54 byte header from fIn
	{									
		header[i] = getc(fIn);								
	}


	// extract image height, width and bitDepth from imageHeader 
	int height = *(int*)&header[18];
	int width = *(int*)&header[22];
	int bitDepth = *(int*)&header[28];

	printf("width: %d\n",width);
	printf("height: %d\n",height );

	if(bitDepth <= 8)					//if ColorTable present, extract it.
	{
		fread(colorTable,sizeof(unsigned char),1024,fIn);
	}

	int size = height * width;				//calculate image size
	unsigned char buffer[size];				//to store the image data

	fread(buffer,sizeof(unsigned char),size,fIn);		//read image data

    // Perform functions
    //      * image dark
    image_dark(header, colorTable, size, buffer);
    //      * image bright
    image_bright(header, colorTable, size, buffer);

	fclose(fIn);

    colored();
}