#include "imagehelper.h"

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>

extern int parallelism_enabled;

double getWallTime();
double analyzeArrayS(ImageData *src, ImageData *dest);
double analyzeArrayM1(ImageData *src, ImageData *dest);
double analyzeArrayM1wC(ImageData *src, ImageData *dest);
double analyzeArrayM2(ImageData *src, ImageData *dest);

int width = 640;
int height = 512;
int thread_count = 6;
int iterations = 20;
int run_single = 0;

int main(int argc, char *argv[])
{
	if(argc == 2) {
		thread_count = atoi(argv[1]);
		printf("setting thread num to %d\n", thread_count);	
	} else if(argc == 4) {
		thread_count = atoi(argv[1]);
		iterations = atoi(argv[2]);
		run_single = atoi(argv[3]);
		printf("setting thread num to %d\n", thread_count);
	} else if(argc > 5) {
		thread_count = atoi(argv[1]);
		iterations = atoi(argv[2]);
		run_single = atoi(argv[3]);
		height = atoi(argv[4]);
		width = atoi(argv[5]);
		printf("standard dimensions updated to %dx%d\n",
			height, width);
		printf("setting thread num to %d\n", thread_count);	
	} else {
		printf("running with standard 512x640 image generation\n");	
		printf("setting thread num to %d\n", thread_count);	
	}
	// printf("parallelism_enabled = %d\n", parallelism_enabled);
	ImageData *src = initializeImageData(height, width);
	ImageData *dest = initializeEmptyImageData(height, width);
	double avg = 0;

	if (run_single)
	{
		for (int i = 0; i < iterations; ++i)
		{
			avg = (avg * i + analyzeArrayS(src, dest)) / (i + 1);
		}
		printf("Single tests run for %d times\n", iterations);
		printResult(avg);
	}

	avg = 0;
	for (int i = 0; i < iterations; ++i)
	{
		avg = (avg * i + analyzeArrayM1(src, dest)) / (i + 1);
	}
	printf("Multiple1 tests run for %d times\n", iterations);
	printResult(avg);

	avg = 0;
	for (int i = 0; i < iterations; ++i)
	{
		avg = (avg * i + analyzeArrayM1wC(src, dest)) / (i + 1);
	}
	printf("Multiple1 with collapse(2) tests run for %d times\n", iterations);
	printResult(avg);

	avg = 0;
	for (int i = 0; i < iterations; ++i)
	{
		avg = (avg * i + analyzeArrayM2(src, dest)) / (i + 1);
	}
	printf("Multiple2 tests run for %d times\n", iterations);
	printResult(avg);

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

double analyzeArrayS(ImageData *src, ImageData *dest)
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
			dest->buf[i + j * width] = assignDirection(src, i, j);
		}
	}

	double end = getWallTime();

	return end - start;
}

double analyzeArrayM1(ImageData *src, ImageData *dest)
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
				dest->buf[i + j * width] = assignDirection(src, i, j);
			}
		}
	}
	double end = omp_get_wtime();

	return end - start;
}

double analyzeArrayM1wC(ImageData *src, ImageData *dest)
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
				dest->buf[i + j * width] = assignDirection(src, i, j);
			}
		}
	}
	double end = omp_get_wtime();

	return end - start;
}

double analyzeArrayM2(ImageData *src, ImageData *dest)
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
				dest->buf[i + j * width] = assignDirection(src, i, j);
			}
		}
	}
	double end = omp_get_wtime();

	return end - start;
}