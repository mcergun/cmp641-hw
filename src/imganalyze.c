#include "imagehelper.h"

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>

extern int parallelism_enabled;

double getWallTime();
double analyzeArrayS(ImageData *img);
double analyzeArrayM1(ImageData *img);
double analyzeArrayM2(ImageData *img);

int width = 640;
int height = 512;
int thread_count = 6;

int main(int argc, char *argv[])
{
	if(argc == 2) {
		thread_count = atoi(argv[1]);
		printf("setting thread num to %d\n", thread_count);	
	} else if(argc > 3) {
		thread_count = atoi(argv[1]);
		height = atoi(argv[2]);
		width = atoi(argv[3]);
		printf("standard dimensions updated to %dx%d\n",
			height, width);
		printf("setting thread num to %d\n", thread_count);	
	} else {
		printf("running with standard 512x640 image generation\n");	
		printf("setting thread num to %d\n", thread_count);	
	}
	// printf("parallelism_enabled = %d\n", parallelism_enabled);
	ImageData *img = initializeImageData(height, width);
	double avg = 0;

	for (int i = 0; i < 50; ++i)
	{
		avg = (avg * i + analyzeArrayS(img)) / (i + 1);
	}
	printf("Single tests run for 50 times\n");
	printResult(avg);

	return 0;
	for (int i = 0; i < 50; ++i)
	{
		avg = (avg * i + analyzeArrayM1(img)) / (i + 1);
	}
	printf("Multiple1 tests run for 50 times\n");
	printResult(avg);

	for (int i = 0; i < 50; ++i)
	{
		avg = (avg * i + analyzeArrayM2(img)) / (i + 1);
	}
	printf("Multiple2 tests run for 50 times\n");
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

double analyzeArrayS(ImageData *img)
{
	int y1 = 0;
	int y2 = img->height;
	int x1 = 0;
	int x2 = img->width;

	double start = getWallTime();

	for (int i = y1; i < y2; ++i)
	{
		for (int j = x1; j < x2; ++j)
		{
			assignDirection(img, i, j);
		}
	}

	double end = getWallTime();

	return end - start;
}

double analyzeArrayM1(ImageData *img)
{
	int y1 = 0;
	int y2 = img->height;
	int x1 = 0;
	int x2 = img->width;

	double start = omp_get_wtime();

	#pragma omp parallel num_threads(thread_count) 
	{
		#pragma omp for
		for (int i = y1; i < y2; ++i)
		{
			for (int j = x1; j < x2; ++j)
			{
				assignDirection(img, i, j);
			}
		}
	}
	double end = omp_get_wtime();

	return end - start;
}

double analyzeArrayM2(ImageData *img)
{
	int y1 = 0;
	int y2 = img->height;
	int x1 = 0;
	int x2 = img->width;

	double start = omp_get_wtime();

	#pragma omp parallel num_threads(thread_count) 
	{
		#pragma omp for
		for (int i = y1; i < y2; ++i)
		{
			for (int j = x1; j < x2; ++j)
			{
				assignDirection(img, i, j);
			}
		}
	}
	double end = omp_get_wtime();

	return end - start;
}