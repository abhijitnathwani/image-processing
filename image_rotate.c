/**
* @file image_rotate.c
* @brief C program to rotate the image by 180-degree. ( Flipping)
* @author Priya Shah
* @version v1
* @date 2018-01-10
*/
#include <stdio.h>

int image_rotate(unsigned char header[54], int height, int width, unsigned char buffer[width][height][3], unsigned char colorTable[1024])
{
	FILE *fOut = fopen("rgb_rotate.bmp","w+");		    	//Output File name
	int i,j,choice;
	int temp;
	unsigned char heightA[4];
	unsigned char widthA[4];


	// extract image height, width and bitDepth from imageHeader
	// int height = *(int *)&header[18];
	// int width = *(int *)&header[22];
	int bitDepth = *(int *)&header[28];

	printf("width: %d\n",width);
	printf("height: %d\n",height );

	int size = height*width;								//calculate image size



	// fread(buffer,sizeof(unsigned char),size,fIn);			//read the image data

	unsigned char out_buffer[width][height][3];
	// unsigned char F_out_buffer[height][width];



	printf("Enter your choice :\n");
	printf("1. Rotate right\n");
	printf("2. Rotate left\n");
	printf("3. Rotate 180\n");

	scanf("%d",&choice);

	switch(choice)												//to rotate image in 3 direction
	{												
		case 1:
			temp=height;
			height=width;
			width=temp;
			for(i=0;i<width;i++)								//to rotate right
			{
				for(j=0;j<height;j++)
				{
					out_buffer[j][height-1-i][2]=buffer[j][i][2];
                	out_buffer[j][height-1-i][1]=buffer[j][i][1];
                	out_buffer[j][height-1-i][0]=buffer[j][i][0];
				}
			}

			for (i=0;i<4;i++){
				heightA[i]=header[18+i];
				widthA[i]=header[22+i];
			}
			for (i=0;i<4;i++){
				header[18+i]=widthA[i];
				header[22+i]=heightA[i];
			}
			fwrite(header, sizeof(unsigned char), 54, fOut); // write the header back
			if (bitDepth <= 8)								 // if ColorTable present, extract it.
			{
				fwrite(colorTable, sizeof(unsigned char), 1024, fOut);
			}
			for (i = 0; i < height; i++)
			{
				for (j = 0; j < width; j++)
				{
					putc(out_buffer[i][j][2], fOut);
					putc(out_buffer[i][j][1], fOut);
					putc(out_buffer[i][j][0], fOut);
				}
			}
			break;
		// case 2:
		// 	for(i=0;i<width;i++)								//to rotate left
		// 	{
		// 		for(j=0;j<height;j++)
		// 		{
		// 			out_buffer[j][i]=buffer[i][j];
		// 		}
		// 	}
		// 	break;
		// case 3:
		// 	for(i=0;i<width;i++)								//to rotate 180 degree
		// 	{
		// 		for(j=0;j<height;j++)
		// 		{
		// 			out_buffer[width-i][j]=buffer[i][j];
		// 		}
		// 	}
		// 	break;
		default:
			break;
	}
	
	// fwrite(out_buffer,sizeof(unsigned char),size,fOut);		//write back to the output image

	// fclose(fIn);
	fclose(fOut);

	return 0;
}
