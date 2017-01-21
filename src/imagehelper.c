#include "imagehelper.h"

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <math.h>

static int width = 640;
static int height = 512;
static int thread_count = 6;

static int xx[] = {0, 1, 1, 1, 0, -1, -1, -1};
static int yy[] = {-1, -1, 0, 1, 1, 1, 0, -1};

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

void singleImpl(ImageData *src, ImageData *intmd, ImageData *result, int limit)
{
	int width = src->width;
	int height = src->height;
	unsigned char *imgbuf = src->buf;
	unsigned char *intmdbuf = intmd->buf;
	unsigned char *resultbuf = result->buf;

	for(int row = 1; row < height - 1; row++) {
		for(int col = 1; col < width - 1; col++) {
			int pos = row * width + col;

			unsigned char maxValue = imgbuf[pos];
			int direction = 10;

			// don't want to use a for loop for a 3x3 grid
			// it may reduce the performance as it will get called so often
			if(maxValue < imgbuf[pos - width - 1]) {
				maxValue = imgbuf[pos - width - 1];
				direction = 8;
			}
			if(maxValue < imgbuf[pos - width]) {
				maxValue = imgbuf[pos - width];
				direction = 1;
			}
			if(maxValue < imgbuf[pos - width + 1]) {
				maxValue = imgbuf[pos - width + 1];
				direction = 2;
			}
			if(maxValue < imgbuf[pos - 1]) {
				maxValue = imgbuf[pos - 1];
				direction = 7;
			}
			if(maxValue < imgbuf[pos + 1]) {
				maxValue = imgbuf[pos + 1];
				direction = 3;
			}
			if(maxValue < imgbuf[pos + width - 1]) {
				maxValue = imgbuf[pos - width - 1];
				direction = 6;
			}
			if(maxValue < imgbuf[pos + width]) {
				maxValue = imgbuf[pos - width];
				direction = 5;
			}
			if(maxValue < imgbuf[pos + width + 1]) {
				maxValue = imgbuf[pos - width + 1];
				direction = 4;
			}

			intmdbuf[pos] = direction;
		}
	}

	for(int row = 2; row < width - 2; row++) {
		for(int col = 2; col < height - 2; col++) {

			int posOrig = (row - 2) * width + col;
			int pos = posOrig;

			int xtop = 0;
			int ytop = 0;

			for (int i = 0; i < 5; ++i)
			{
				pos = pos + width * i;
				for (int j = 0; j < 5; ++j)
				{
					if(imgbuf[pos + j]) {
						xtop = xtop + xx[intmdbuf[pos + j]];
						ytop = ytop + yy[intmdbuf[pos + j]];
					}
				}
			}

			int result = sqrt(pow(xtop, 2) + pow(ytop, 2));

			if(result < limit)
				result = 0;

			resultbuf[posOrig] = result;
		}
	}
	
}

// 8 1 2
// 7 0 3
// 6 5 4
void assignDirection(ImageData *img, ImageData *dest, int y, int x)
{
	int width = img->width;
	unsigned char *buf = img->buf;

	int pos = y * width + x;

	// printf("xMin = %u, xMax = %u\n"
	// 	"yMin = %u, yMax = %u\n", xMin, xMax, yMin, yMax);

	unsigned char maxValue = buf[pos];
	int direction = 10;

	// don't want to use a for loop for a 3x3 grid
	// it may reduce the performance as it will get called so often
	if(maxValue < buf[pos - width - 1]) {
		maxValue = buf[pos - width - 1];
		direction = 8;
	}
	if(maxValue < buf[pos - width]) {
		maxValue = buf[pos - width];
		direction = 1;
	}
	if(maxValue < buf[pos - width + 1]) {
		maxValue = buf[pos - width + 1];
		direction = 2;
	}
	if(maxValue < buf[pos - 1]) {
		maxValue = buf[pos - 1];
		direction = 7;
	}
	if(maxValue < buf[pos + 1]) {
		maxValue = buf[pos + 1];
		direction = 3;
	}
	if(maxValue < buf[pos + width - 1]) {
		maxValue = buf[pos - width - 1];
		direction = 6;
	}
	if(maxValue < buf[pos + width]) {
		maxValue = buf[pos - width];
		direction = 5;
	}
	if(maxValue < buf[pos + width + 1]) {
		maxValue = buf[pos - width + 1];
		direction = 4;
	}

	dest->buf[pos] = direction;
}

void sumAndCompare(ImageData *img, ImageData *dest, int y, int x, int limit)
{	
	int width = img->width;
	int height = img->height;
	unsigned char *buf = img->buf;

	int posOrig = (y - 2) * width + x;
	int pos = posOrig;

	unsigned char *imgbuf = img->buf;
	unsigned char *destbuf = dest->buf;

	int xtop = 0;
	int ytop = 0;

	for (int i = 0; i < 5; ++i)
	{
		pos = pos + width * i;
		for (int j = 0; j < 5; ++j)
		{
			if(imgbuf[pos + j]) {
				xtop = xtop + xx[imgbuf[pos + j]];
				ytop = ytop + yy[imgbuf[pos + j]];
			}
		}
	}

	int result = sqrt(pow(xtop, 2) + pow(ytop, 2));

	if(result < limit)
		result = 0;

	dest->buf[posOrig] = result;
}

void printResult(double val)
{
	printf("\n########################################\n\n"
		"Time Difference = %.4fms\n\n"
		"########################################\n\n\n", val * 1000);
}

double analyzeArrayS(ImageData *src, ImageData *dest, ImageData *dest2)
{
	int height = src->height;
	int width = src->width;
	int y1 = 1;
	int y2 = height - 1;
	int x1 = 1;
	int x2 = width - 1;

	double start = getWallTime();

	for (int i = y1; i < y2; ++i)
	{
		for (int j = x1; j < x2; ++j)
		{
			assignDirection(src, dest, i, j);
		}
	}

	y1 = 2;
	y2 = height - 2;
	x1 = 2;
	x2 = width -2;

	for (int i = y1; i < y2; ++i)
	{
		for (int j = x1; j < x2; ++j)
		{
			sumAndCompare(dest, dest2, i, j, 4);
		}
	}

	double end = getWallTime();

	return end - start;
}

double analyzeArrayM1(ImageData *src, ImageData *dest, ImageData *dest2)
{
	int height = src->height;
	int width = src->width;
	int y1 = 1;
	int y2 = height - 1;
	int x1 = 1;
	int x2 = width - 1;

	double start = omp_get_wtime();

	#pragma omp parallel num_threads(thread_count) 
	{
		#pragma omp for
		for (int i = y1; i < y2; ++i)
		{
			for (int j = x1; j < x2; ++j)
			{
				assignDirection(src, dest, i, j);
			}
		}
	}

	y1 = 2;
	y2 = height - 2;
	x1 = 2;
	x2 = width -2;

	for (int i = y1; i < y2; ++i)
	{
		for (int j = x1; j < x2; ++j)
		{
			sumAndCompare(dest, dest2, i, j, 4);
		}
	}

	double end = omp_get_wtime();

	return end - start;
}

double analyzeArrayM1wC(ImageData *src, ImageData *dest, ImageData *dest2)
{
	int height = src->height;
	int width = src->width;
	int y1 = 1;
	int y2 = height - 1;
	int x1 = 1;
	int x2 = width - 1;

	double start = omp_get_wtime();

	#pragma omp parallel num_threads(thread_count) 
	{
		#pragma omp for collapse(2)
		for (int i = y1; i < y2; ++i)
		{
			for (int j = x1; j < x2; ++j)
			{
				assignDirection(src, dest, i, j);
			}
		}
	}

	y1 = 2;
	y2 = height - 2;
	x1 = 2;
	x2 = width -2;

	for (int i = y1; i < y2; ++i)
	{
		for (int j = x1; j < x2; ++j)
		{
			sumAndCompare(dest, dest2, i, j, 4);
		}
	}

	double end = omp_get_wtime();

	return end - start;
}

double analyzeArrayM2(ImageData *src, ImageData *dest, ImageData *dest2)
{
	int height = src->height;
	int width = src->width;
	int y1 = 1;
	int y2 = height - 1;
	int x1 = 1;
	int x2 = width - 1;

	double start = omp_get_wtime();

	#pragma omp parallel num_threads(thread_count) 
	{
		#pragma omp for
		for (int i = y1; i < y2; ++i)
		{
			for (int j = x1; j < x2; ++j)
			{
				assignDirection(src, dest, i, j);
			}
		}
	}

	y1 = 2;
	y2 = height - 2;
	x1 = 2;
	x2 = width -2;

	for (int i = y1; i < y2; ++i)
	{
		for (int j = x1; j < x2; ++j)
		{
			sumAndCompare(dest, dest2, i, j, 4);
		}
	}

	double end = omp_get_wtime();

	return end - start;
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