#include "imagehelper.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

void analyzeArray(ImageData *img);
double getWallTime();

int main(int argc, char *argv[])
{
	int width = 640;
	int height = 512;
	if(argc < 3)
		printf("running with standard 512x640 image generation\n");
	else 
	{
		height = atoi(argv[1]);
		width = atoi(argv[2]);
		printf("standard dimensions updated to %dx%d\n",
			height, width);
	}
	ImageData *img = initializeImageData(height, width);
	analyzeArray(img);
	return 0;
}

void analyzeArray(ImageData *img)
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

	printResult(end - start);
}

double getWallTime(){
    struct timeval time;
    if (gettimeofday(&time,NULL)){
        //  Handle error
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}