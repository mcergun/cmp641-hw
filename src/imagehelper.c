#include "imagehelper.h"

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <math.h>

ImageData * initializeImageData(int height, int width)
{
	srand(time(0));
	ImageData *img = calloc(1, sizeof(ImageData));
	img->width = width;
	img->height = height;
	img->buf = malloc(sizeof(unsigned char *) * width * height);
	for(int i = 0; i < height * width; ++i) {
		img->buf[i] = rand() % 256;
	}
	return img;
}

ImageData * initializeEmptyImageData(int height, int width)
{
	srand(time(0));
	ImageData *img = calloc(1, sizeof(ImageData));
	img->width = width;
	img->height = height;
	img->buf = calloc(1, sizeof(unsigned char *) * width * height);
	return img;
}

void destroyImageData(ImageData * img)
{
	//free row buffer holders
	free(img->buf);

	//free struct
	free(img);
}

void printResult(double val)
{
	printf("\n########################################\n\n"
		"Time Difference = %.4fms\n\n"
		"########################################\n\n\n", val * 1000);
}

int compareImages(ImageData *img1, ImageData *img2)
{
	if((img1->width != img2->width) ||
		(img1->height != img2->height))
		return -1;

	int width = img1->width;
	int height = img1->height;
	int bufSize = width * height;

	unsigned char *buf1 = img1->buf;
	unsigned char *buf2 = img2->buf;

	int isEqual = 1;
	for (int i = 0; i < bufSize && isEqual; ++i)
	{
		isEqual = buf1[i] == buf2[i];
	}

	return isEqual;
}