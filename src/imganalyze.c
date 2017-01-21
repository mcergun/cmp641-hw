#include "imagehelper.h"

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>

extern int parallelism_enabled;

double getWallTime();

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
	ImageData *dests1;
	ImageData *dests2;
	if(run_single) {
		dests1 = initializeEmptyImageData(height, width);
		dests2 = initializeEmptyImageData(height, width);
	}
	ImageData *destm1 = initializeEmptyImageData(height, width);
	ImageData *destm2 = initializeEmptyImageData(height, width);
	double avg = 0;

	if (run_single)
	{
		for (int i = 0; i < iterations; ++i)
		{
			avg = (avg * i + analyzeArrayS(src, dests1, dests2)) / (i + 1);
		}
		printf("Single tests run for %d times\n", iterations);
		printResult(avg);
	}

	avg = 0;
	for (int i = 0; i < iterations; ++i)
	{
		avg = (avg * i + analyzeArrayM1wC(src, destm1, destm2)) / (i + 1);
	}
	printf("Multiple1 tests run for %d times\n", iterations);
	if(run_single) {
		printf("Comparing resulting arrays\n");
		printf("Comparison Result = %d\n", compareImages(dests1, destm1));
		printf("Comparison Result = %d\n", compareImages(dests2, destm2));
	}
	printResult(avg);

	avg = 0;
	for (int i = 0; i < iterations; ++i)
	{
		avg = (avg * i + analyzeArrayM1wC(src, destm1, destm2)) / (i + 1);
	}
	printf("Multiple1 with collapse(2) tests run for %d times\n", iterations);
	if(run_single) {
		printf("Comparing resulting arrays\n");
		printf("Comparison Result = %d\n", compareImages(dests1, destm1));
		printf("Comparison Result = %d\n", compareImages(dests2, destm2));
	}
	printResult(avg);

	avg = 0;
	for (int i = 0; i < iterations; ++i)
	{
		avg = (avg * i + analyzeArrayM1wC(src, destm1, destm2)) / (i + 1);
	}
	printf("Multiple2 tests run for %d times\n", iterations);
	if(run_single) {
		printf("Comparing resulting arrays\n");
		printf("Comparison Result = %d\n", compareImages(dests1, destm1));
		printf("Comparison Result = %d\n", compareImages(dests2, destm2));
	}
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