/**
* @file image_correct_protanopia.c
* @brief C program to correct the image colors, using convolution
* @author Priya Shah
* @version v1
* @date 2018-01-10
*/
#include <stdio.h>
#include <time.h>
#include "color_blindess.c"

int main()
{
	clock_t start, stop;

	start = clock();						// Note the start time for profiling purposes.

	FILE *fIn = fopen("images/nature-4080511_960_720.bmp","r");			// Input File name
	FILE *fOut = fopen("nature-4080511_960_720_Protanopia_color.bmp","w+");		    	// Output File name

	int i,j,y, x;
	unsigned char byte[54];

	if(fIn==NULL)							// check if the input file has not been opened succesfully.
	{											
		printf("File does not exist.\n");
		return -1;
	}

	for(i=0;i<54;i++)						// read the 54 byte header from fIn
	{									
		byte[i] = getc(fIn);								
	}

	fwrite(byte,sizeof(unsigned char),54,fOut);			// write the header back

	// extract image height, width and bitDepth from imageHeader 
	int height = *(int*)&byte[18];
	int width = *(int*)&byte[22];
	int bitDepth = *(int*)&byte[28];

	printf("width: %d\n",width);
	printf("height: %d\n",height );

	int size = height*width;					// calculate the image size

	unsigned char buffer[size][3];					// store the input image data
	unsigned char out[size][3];					// store the output image data

	for(i=0;i<size;i++)						// read image data character by character
	{
		buffer[i][2]=getc(fIn);					// blue
		buffer[i][1]=getc(fIn);					// green
		buffer[i][0]=getc(fIn);					// red
	}

	// float lms_buffer[size][3];
	unsigned char r, g, b;
	float rr, gg, bb;
	float l, m, s; // original
	float ll, mm, ss; // updated
	unsigned int index;


	for(x=0;x<height;x++)
	{					
		for(y=0;y<width;y++)
		{
			index = (x)*width+(y);
			// matrix multiplication with kernel with every color plane
			r = buffer[index][0]; //red
			g = buffer[index][1]; //green
			b = buffer[index][2]; //blue

			// convert to lms
			l = (17.8824 * r) 	+ (43.5161 * g) 	+ (4.11935 * b); // long
			m = (3.45565 * r) 	+ (27.1554 * g) 	+ (3.86714 * b); // medium
			s = (0.0299566 * r)	+ (0.184309 * g) 	+ (1.46709 * b); // short
		
			correct_protanopia(l, m, s, &ll, &mm, &ss);
			// correct_deuteranopia(l, m, s, &ll, &mm, &ss);
			// correct_tritanopia(l, m, s, &ll, &mm, &ss);


			// convert back to rgb
			rr = (0.0809444479 * ll) 		+ (-0.130504409 * mm) 	+ (0.116721066 * ss);
			gg = (-0.0102485335 * ll)		+ (0.0540193266 * mm) 	+ (-0.113614708 * ss);
			bb = (-0.000365296938 * ll) 	+ (-0.00412161469 * mm) + (0.693511405 * ss);

			// calculate error
			rr = r - rr;
			gg = g - gg;
			bb = b - bb;

			// shift the color towards visible spectrum and add compensation
			rr = r;
			gg = ((0.7 * rr) + (1.0 * gg)) + g;
			bb = ((0.7 * rr) + (1.0 * bb)) + b;

			// clamp values towards unsigned char
			r = (rr > 255)? 255:((rr < 0)? 0:rr);
			g = (gg > 255)? 255:((gg < 0)? 0:gg);
			b = (bb > 255)? 255:((bb < 0)? 0:bb);


			// save resulted color
			out[index][0]=r;
			out[index][1]=g;
			out[index][2]=b;
		}
	}

	//write image data back to the file
	for(i=0;i<size;i++)
	{
		putc(out[i][2],fOut);
		putc(out[i][1],fOut);
		putc(out[i][0],fOut);
	}
		
	fclose(fOut);
	fclose(fIn);

	stop = clock();
	printf("\nCLOCKS_PER_SEC = %ld\n",stop-start); //1000000
	printf("%lf ms\n",((double)(stop-start) * 1000.0)/CLOCKS_PER_SEC );
	return 0;
}
