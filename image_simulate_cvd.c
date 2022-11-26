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

int simulate_cvd_protanopia(char * header, int size, unsigned char * buffer) {
	FILE *fOut = fopen("out/natur_simu_protanopia.bmp","w+");		    	// Output File name

    if(fOut==NULL)							// check if the input file has not been opened succesfully.
	{											
		printf("File did not open.\n");
	}

	int i,j,y, x;
	float l, m, s; // original
	unsigned char r, g, b;
	float ll, mm, ss; // updated

	fwrite(header,sizeof(unsigned char),54,fOut);			// write the header back
	unsigned char out[size][3];					// store the output image data

	for (i = 0; i < size ; i++) {
		b = buffer[i * 3]; //blue
		g = buffer[i * 3 + 1]; //green
		r = buffer[i * 3 + 2]; //red

		rgb2lms(r, g, b, &l, &m, &s);

		simulate_protanopia(l, m, s, &ll, &mm, &ss);
    	
		lms2rgb(ll, mm, ss, &r, &g, &b);

		out[i][0]=r;
		out[i][1]=g;
		out[i][2]=b;
	}

	for(i=0;i<size;i++)
	{
		putc(out[i][2],fOut);
		putc(out[i][1],fOut);
		putc(out[i][0],fOut);
	}

	fclose(fOut);
	return 0;
}
int simulate_cvd_deuteranopia(char * header, int size, unsigned char * buffer) {
	FILE *fOut = fopen("out/natur_simu_deuteranopia.bmp","w+");		    	// Output File name

	int i,j,y, x;
	float l, m, s; // original
	unsigned char r, g, b;
	float ll, mm, ss; // updated

	fwrite(header,sizeof(unsigned char),54,fOut);			// write the header back
	unsigned char out[size][3];					// store the output image data

	for (i = 0; i < size ; i++) {
		b = buffer[i * 3]; //blue
		g = buffer[i * 3 + 1]; //green
		r = buffer[i * 3 + 2]; //red

		rgb2lms(r, g, b, &l, &m, &s);

		simulate_deuteranopia(l, m, s, &ll, &mm, &ss);
    	
		lms2rgb(ll, mm, ss, &r, &g, &b);

		out[i][0]=r;
		out[i][1]=g;
		out[i][2]=b;
	}

	for(i=0;i<size;i++)
	{
		putc(out[i][2],fOut);
		putc(out[i][1],fOut);
		putc(out[i][0],fOut);
	}

	fclose(fOut);
	return 0;
}
int simulate_cvd_tritanopia(char * header, int size, unsigned char * buffer) {
	FILE *fOut = fopen("out/nature_sim_tritanopia.bmp","w+");		    	// Output File name

	int i,j,y, x;
	float l, m, s; // original
	unsigned char r, g, b;
	float ll, mm, ss; // updated

	fwrite(header,sizeof(unsigned char),54,fOut);			// write the header back
	unsigned char out[size][3];					// store the output image data

	for (i = 0; i < size ; i++) {
		b = buffer[i * 3]; //blue
		g = buffer[i * 3 + 1]; //green
		r = buffer[i * 3 + 2]; //red

		rgb2lms(r, g, b, &l, &m, &s);

		simulate_tritanopia(l, m, s, &ll, &mm, &ss);
    	
		lms2rgb(ll, mm, ss, &r, &g, &b);

		out[i][0]=r;
		out[i][1]=g;
		out[i][2]=b;
	}
	
	for(i=0;i<size;i++)
	{
		putc(out[i][2],fOut);
		putc(out[i][1],fOut);
		putc(out[i][0],fOut);
	}

	fclose(fOut);
}

int simulate_cvd(char * header, int size, unsigned char * buffer) {

    #pragma omp task
	simulate_cvd_protanopia(header, size, buffer);

    #pragma omp task
	simulate_cvd_deuteranopia(header, size, buffer);

    #pragma omp task
	simulate_cvd_tritanopia(header, size, buffer);

	return 0;
}