
#include <stdio.h>
#include <time.h>

int image_bluring_color(unsigned char header[54], int size , int height , int width, unsigned char buffer[size][3],int bitDepth,unsigned char colorTable[1024])
{
	clock_t start, stop;

	FILE *fOut = fopen("out/bluring_color.bmp","w+");		    	// Output File name

	int i,j,y,x;

	fwrite(header,sizeof(unsigned char),54,fOut);			// write the header back

	
	if(bitDepth<=8)										//if ColorTable present, extract it.
	{
		fwrite(colorTable,sizeof(unsigned char),1024,fOut);
	}


	unsigned char out[size][3];					// store the output image data
	float v=1.0 / 9.0;						// initialize the blurrring kernel
	float kernel[3][3]={{v,v,v},
						{v,v,v},
						{v,v,v}};
	#pragma omp parallel for num_threads(1)
	for(x=1;x<height-1;x++)
	{					
		for(y=1;y<width-1;y++)
		{
			float sum0= 0.0;
			float sum1= 0.0;
			float sum2= 0.0;
			for(i=-1;i<=1;++i)
			{
				for(j=-1;j<=1;++j)
				{	
					// matrix multiplication with kernel with every color plane
					sum0=sum0+(float)kernel[i+1][j+1]*buffer[(x+i)*width+(y+j)][0];
					sum1=sum1+(float)kernel[i+1][j+1]*buffer[(x+i)*width+(y+j)][1];
					sum2=sum2+(float)kernel[i+1][j+1]*buffer[(x+i)*width+(y+j)][2];
				}
			}	
			out[(x)*width+(y)][0]=sum0;
			out[(x)*width+(y)][1]=sum1;
			out[(x)*width+(y)][2]=sum2;	
		}
	}
	#pragma omp parallel for num_threads(1)
	for(i=0;i<size;i++)						//write image data back to the file
	{
		putc(out[i][2],fOut);
		putc(out[i][1],fOut);
		putc(out[i][0],fOut);
	}
		
	fclose(fOut);
	return 0;
}
