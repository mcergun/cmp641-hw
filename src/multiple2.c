#include "imagehelper.h"

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

extern int parallelism_enabled;

void analyzeArray(ImageData *img, int y1, int y2, int x1, int x2);

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
	analyzeArray(img, 0, img->height, 0, img->width);
	return 0;
}

void analyzeArray(ImageData *img, int y1, int y2, int x1, int x2)
{
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

	printf("\n########################################\n\n"
		"Time Difference = %.8f\n\n"
		"########################################\n\n\n", end - start);
}