#include "imagehelper.h"

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

extern int parallelism_enabled;

void analyzeArray(ImageData *img, int y1, int y2, int x1, int x2);
unsigned char assignDirection(ImageData *img, int y, int x);

int width = 640;
int height = 512;
int thread_count = 6;

int main(int argc, char *argv[])
{
	printf("asda\n");
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
	#pragma omp parallel for num_threads(thread_count)
	for (int i = y1; i < y2; ++i)
	{
		for (int j = x1; j < x2; ++j)
		{
			assignDirection(img, i, j);
		}
	}
}

// 8 1 2
// 7 0 3
// 6 5 4
unsigned char assignDirection(ImageData *img, int y, int x)
{
	int width = img->width;
	int height = img->height;
	unsigned char **buf = img->buf;

	int xMin = x > 1 ? x - 1 : 0;
	int yMin = y > 1 ? y - 1 : 0;
	int xMax = x < width - 2 ? x + 2 : width;
	int yMax = y < height - 2 ? y + 2 : height;

	// printf("xMin = %u, xMax = %u\n"
	// 	"yMin = %u, yMax = %u\n", xMin, xMax, yMin, yMax);

	unsigned char maxValue = buf[y][x];
	int maxXLoc = x;
	int maxYLoc = y;

	for (int i = yMin; i < yMax; ++i) {
		for (int j = xMin; j < xMax; ++j) {
			// printf("%u, %u\n", i, j);
			// std::cout << i << ", " << j << std::endl;
			if(buf[i][j] > maxValue) {
				maxValue = buf[i][j];
				maxYLoc = i;
				maxXLoc = j;
			}
		}
	}
	// printf("\n");

	unsigned char direction = 0;

	switch(maxXLoc - x) {
	case -1:
		direction = 7 + y - maxYLoc;
		break;
	case 0:
		direction = maxYLoc - y ? (maxYLoc - y > 0 ? 5 : 1) : 0;
		break;
	case 1:
		direction = 3 + maxYLoc - y;
		break;
	default:
		// std::cout << "something is going wrong" << std::endl;
		break;
	}

	// std::cout << (int)maxValue << "  " << y << ", " << x << " " 
	// 	<< " (" << maxYLoc << ", " << maxXLoc << ") " << (int)direction << std::endl;
	return direction;
}