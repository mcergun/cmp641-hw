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
	for(int i = 0; i < 10; i++) { \
		avg = (avg * i + function_p(src_p, intmd_p, result_p, 4)) / (i + 1); \
	} \
	printResult(avg); \
}

// used for algorithm's 2nd step
static int xx[] = {0, 1, 1, 1, 0, -1, -1, -1};
static int yy[] = {-1, -1, 0, 1, 1, 1, 0, -1};

extern int parallelism_enabled;

double getWallTime();

double singleCoreImpl(ImageData *src, ImageData *intmd, ImageData *result, int limit);
double multiCoreImpl(ImageData *src, ImageData *intmd, ImageData *result, int limit);
double multiCoreImpl2(ImageData *src, ImageData *intmd, ImageData *result, int limit);

int width = 1920;
int height = 1080;
int thread_count = 6;
int iterations = 20;
int run_single = 0;

int main(int argc, char *argv[])
{
	ImageData *src = initializeImageData(height, width);
	ImageData *intmd = initializeEmptyImageData(height, width);
	ImageData *result = initializeEmptyImageData(height, width);

	// to check if result of OpenMP algorithms are the same as serial one
	ImageData *intmdmp = initializeEmptyImageData(height, width);
	ImageData *resultmp = initializeEmptyImageData(height, width);

	if(argc == 2) {
		thread_count = atoi(argv[1]);
	} else if(argc == 3) {
		thread_count = atoi(argv[1]);
		run_single = atoi(argv[2]);
	}

	printf("Running with %d threads\n", thread_count);

	if(run_single)
		CALL_IMPL(singleCoreImpl, src, intmd, result);
	
	CALL_IMPL(multiCoreImpl, src, intmdmp, resultmp);
	if(run_single) {
		if(compareImages(intmd, intmdmp))
			printf("Method1's intermediate results are the same as serial algorithm\n");
		else
			printf("Method1's intermediate results are different than serial algorithm\n");
		if(compareImages(result, resultmp))
			printf("Method1's final results are the same as serial algorithm\n");
		else
			printf("Method1's final results are different than serial algorithm\n");
		printf("\n");
	}

	CALL_IMPL(multiCoreImpl2, src, intmdmp, resultmp);
	if(run_single) {
		if(compareImages(intmd, intmdmp))
			printf("Method2's intermediate results are the same as serial algorithm\n");
		else
			printf("Method2's intermediate results are different than serial algorithm\n");
		if(compareImages(result, resultmp))
			printf("Method2's final results are the same as serial algorithm\n");
		else
			printf("Method2's final results are different than serial algorithm\n");
		printf("\n");
	}


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
				maxValue = imgbuf[pos + width - 1];
				direction = 6;
			}
			if(maxValue < imgbuf[pos + width]) {
				maxValue = imgbuf[pos + width];
				direction = 5;
			}
			if(maxValue < imgbuf[pos + width + 1]) {
				maxValue = imgbuf[pos + width + 1];
				direction = 4;
			}

			intmdbuf[pos] = direction;
		}
	}

	for(int row = 2; row < width - 2; row++) {
		for(int col = 2; col < height - 2; col++) {

			int pos = row * width + col;

			int xtop = 0;
			int ytop = 0;

			for (int i = -2 * width; i < 3 * width; i += width)
			{
				for (int j = -2; j < 3; ++j)
				{
					if(imgbuf[pos + i + j]) {
						xtop = xtop + xx[intmdbuf[pos + i + j]];
						ytop = ytop + yy[intmdbuf[pos + i + j]];
					}
				}
			}

			int result = sqrt(pow(xtop, 2) + pow(ytop, 2));

			if(result < limit)
				result = 0;

			resultbuf[pos] = result;
		}
	}

	double end = getWallTime();

	return end - start;
}

double multiCoreImpl(ImageData *src, ImageData *intmd, ImageData *result, int limit)
{
	int width = src->width;
	int height = src->height;
	unsigned char *imgbuf = src->buf;
	unsigned char *intmdbuf = intmd->buf;
	unsigned char *resultbuf = result->buf;

	double start = omp_get_wtime();
	#pragma omp parallel for num_threads(thread_count)
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
				maxValue = imgbuf[pos + width - 1];
				direction = 6;
			}
			if(maxValue < imgbuf[pos + width]) {
				maxValue = imgbuf[pos + width];
				direction = 5;
			}
			if(maxValue < imgbuf[pos + width + 1]) {
				maxValue = imgbuf[pos + width + 1];
				direction = 4;
			}

			intmdbuf[pos] = direction;
		}
	}
	#pragma omp parallel for num_threads(thread_count)
	for(int row = 2; row < width - 2; row++) {
		for(int col = 2; col < height - 2; col++) {

			int pos = row * width + col;

			int xtop = 0;
			int ytop = 0;

			for (int i = -2 * width; i < 3 * width; i += width)
			{
				for (int j = -2; j < 3; ++j)
				{
					if(imgbuf[pos + i + j]) {
						xtop = xtop + xx[intmdbuf[pos + i + j]];
						ytop = ytop + yy[intmdbuf[pos + i + j]];
					}
				}
			}

			int result = sqrt(pow(xtop, 2) + pow(ytop, 2));

			if(result < limit)
				result = 0;

			resultbuf[pos] = result;
		}
	}

	double end = omp_get_wtime();

	return end - start;
}

double multiCoreImpl2(ImageData *src, ImageData *intmd, ImageData *result, int limit)
{
	int width = src->width;
	int height = src->height;
	unsigned char *imgbuf = src->buf;
	unsigned char *intmdbuf = intmd->buf;
	unsigned char *resultbuf = result->buf;

	double start = omp_get_wtime();
	#pragma omp parallel num_threads(thread_count)
	for(int row = 1; row < height - 1; row++) {
		#pragma omp for
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
				maxValue = imgbuf[pos + width - 1];
				direction = 6;
			}
			if(maxValue < imgbuf[pos + width]) {
				maxValue = imgbuf[pos + width];
				direction = 5;
			}
			if(maxValue < imgbuf[pos + width + 1]) {
				maxValue = imgbuf[pos + width + 1];
				direction = 4;
			}

			intmdbuf[pos] = direction;
		}
	}

	#pragma omp parallel num_threads(thread_count)
	for(int row = 2; row < width - 2; row++) {
		#pragma omp for nowait
		for(int col = 2; col < height - 2; col++) {

			int pos = row * width + col;

			int xtop = 0;
			int ytop = 0;

			for (int i = -2 * width; i < 3 * width; i += width)
			{
				if(imgbuf[pos + i - 2]) {
					xtop = xtop + xx[intmdbuf[pos + i - 2]];
					ytop = ytop + yy[intmdbuf[pos + i - 2]];
				}
				if(imgbuf[pos + i - 1]) {
					xtop = xtop + xx[intmdbuf[pos + i - 1]];
					ytop = ytop + yy[intmdbuf[pos + i - 1]];
				}
				if(imgbuf[pos + i]) {
					xtop = xtop + xx[intmdbuf[pos + i]];
					ytop = ytop + yy[intmdbuf[pos + i]];
				}
				if(imgbuf[pos + i + 1]) {
					xtop = xtop + xx[intmdbuf[pos + i + 1]];
					ytop = ytop + yy[intmdbuf[pos + i + 1]];
				}
				if(imgbuf[pos + i + 2]) {
					xtop = xtop + xx[intmdbuf[pos + i + 2]];
					ytop = ytop + yy[intmdbuf[pos + i + 2]];
				}
			}

			int result = sqrt(pow(xtop, 2) + pow(ytop, 2));

			if(result < limit)
				result = 0;

			resultbuf[pos] = result;
		}
	}

	double end = omp_get_wtime();

	return end - start;
}