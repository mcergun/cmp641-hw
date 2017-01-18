#include "imagehelper.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

void analyzeArray(ImageData *img, int y1, int y2, int x1, int x2);
void analyzeArray2(ImageData *img);
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
	analyzeArray(img, 0, img->height, 0, img->width);
	analyzeArray2(img);
	return 0;
}

void analyzeArray2(ImageData *img)
{
	int width = img->width;
	int height = img->height;

	int size = width * height;
	int ypos = 0;
	int xpos = 0;

	double start = getWallTime();

	for (int i = 0; i < size; ++i) {
		xpos = i % width;
		ypos = i / width;
		assignDirection(img, xpos, ypos);
	}

	double end = getWallTime();

	printf("\n########################################\n\n"
		"Time Difference = %.8f\n\n"
		"########################################\n\n\n", end - start);
}

void analyzeArray(ImageData *img, int y1, int y2, int x1, int x2)
{
	double start = getWallTime();

	for (int i = y1; i < y2; ++i)
	{
		for (int j = x1; j < x2; ++j)
		{
			assignDirection(img, i, j);
		}
	}

	double end = getWallTime();

	printf("\n########################################\n\n"
		"Time Difference = %.8f\n\n"
		"########################################\n\n\n", end - start);
}

double getWallTime(){
    struct timeval time;
    if (gettimeofday(&time,NULL)){
        //  Handle error
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}