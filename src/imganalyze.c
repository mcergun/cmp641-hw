#include "imagehelper.h"

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

#define CALL_IMPL(function_p, src_p, intmd_p, result_p) \
{\
	double avg = 0.0; \
	for(int i = 0; i < 200; i++) { \
		avg = (avg * i + function_p(src_p, intmd_p, result_p, 4)) / (i + 1); \
	} \
	printResult(avg); \
}

extern int parallelism_enabled;

double getWallTime();

double singleCoreImpl(ImageData *src, ImageData *intmd, ImageData *result, int limit);
double multipleCoreImpl(ImageData *src, ImageData *intmd, ImageData *result, int limit);

int width = 1000;
int height = 1000;
int thread_count = 6;
int iterations = 20;
int run_single = 0;

int main(int argc, char *argv[])
{
	ImageData *src = initializeImageData(height, width);
	ImageData *intmd = initializeEmptyImageData(height, width);
	ImageData *result = initializeEmptyImageData(height, width);

	CALL_IMPL(singleCoreImpl, src, intmd, result);


	return 0;
}

double getWallTime() 
{
    struct timeval time;
    if (gettimeofday(&time,NULL)){
        //  Handle error
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

double singleCoreImpl(ImageData *src, ImageData *intmd, ImageData *result, int limit)
{
	int width = src->width;
	int height = src->height;
	unsigned char *imgbuf = src->buf;
	unsigned char *intmdbuf = intmd->buf;
	unsigned char *resultbuf = result->buf;

	double start = getWallTime();

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

	double end = getWallTime();

	return end - start;
}

double multipleCoreImpl(ImageData *src, ImageData *intmd, ImageData *result, int limit)
{
	int width = src->width;
	int height = src->height;
	unsigned char *imgbuf = src->buf;
	unsigned char *intmdbuf = intmd->buf;
	unsigned char *resultbuf = result->buf;

	double start = getWallTime();

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

	double end = getWallTime();

	return end - start;
}