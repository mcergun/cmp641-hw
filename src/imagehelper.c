#include "imagehelper.h"

#include <stdlib.h>
#include <time.h>
#include <stdio.h>

ImageData * initializeImageData(int height, int width)
{
	srand(time(0));
	ImageData *img = calloc(1, sizeof(ImageData));
	img->width = width;
	img->height = height;
	img->buf = malloc(sizeof(unsigned char *) * width * height);
	for(int i = 0; i < height * width; ++i) {
		img->buf[i] = rand() % 256;
	}
	return img;
}

void destroyImageData(ImageData * img)
{
	//free row buffer holders
	free(img->buf);

	//free struct
	free(img);
}

// 8 1 2
// 7 0 3
// 6 5 4
unsigned char assignDirection(ImageData *img, int y, int x)
{
	int width = img->width;
	int height = img->height;
	unsigned char *buf = img->buf;

	int pos = y * width + x;

	int xMin = x > 1 ? x - 1 : 0;
	int yMin = y > 1 ? y - 1 : 0;
	int xMax = x < width - 2 ? x + 2 : width;
	int yMax = y < height - 2 ? y + 2 : height;

	// printf("xMin = %u, xMax = %u\n"
	// 	"yMin = %u, yMax = %u\n", xMin, xMax, yMin, yMax);

	unsigned char maxValue = buf[pos];
	int maxXLoc = x;
	int maxYLoc = y;

	for (int i = yMin; i < yMax; ++i) {
		for (int j = xMin; j < xMax; ++j) {
			// printf("%u, %u\n", i, j);
			// std::cout << i << ", " << j << std::endl;
			if(buf[pos] > maxValue) {
				maxValue = buf[pos];
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

void printResult(double val)
{
	printf("\n########################################\n\n"
		"Time Difference = %.8f\n\n"
		"########################################\n\n\n", val);
}