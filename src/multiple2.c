#include "imagehelper.h"

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

extern int parallelism_enabled;

double analyzeArray(ImageData *img);

int thread_count = 6;
int width = 640;
int height = 512;

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

	for (int i = 0; i < 20; ++i)
	{
		avg = (avg * i + analyzeArray(img)) / (i + 1);
	}
	printf("Multiple1 tests run for 20 times\n");
	printResult(avg);
	
	return 0;
}

double analyzeArray(ImageData *img)
{
	int y1 = 0;
	int y2 = img->height;
	int x1 = 0;
	int x2 = img->width;

	double start = omp_get_wtime();

	#pragma omp parallel num_threads(thread_count) 
	for (int i = y1; i < y2; ++i)
	{
		{ 
			#pragma omp for 
			for (int j = x1; j < x2; ++j)
			{
				assignDirection(img, i, j);
			}
		}
	}

	double end = omp_get_wtime();

	return end - start;
}