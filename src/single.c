#include "imagehelper.h"
#include <stdlib.h>
#include <time.h>

void analyzeArray(ImageData *img, int y1, int y2, int x1, int x2);
unsigned char assignDirection(ImageData *img, int y, int x);

int main(int argc, char *argv[])
{
	srand(time(0));
	ImageData *img = initializeImageData(512, 640);
	analyzeArray(img, 0, img->height, 0, img->width);
	return 0;
}

void analyzeArray(ImageData *img, int y1, int y2, int x1, int x2)
{
	int max = 0;
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
	unsigned char *buf = img->buf;

	int xMin = x > 1 ? x - 1 : 0;
	int yMin = y > 1 ? y - 1 : 0;
	int xMax = x < width - 2 ? x + 2 : width;
	int yMax = y < height - 2 ? y + 2 : height;

	// std::cout << "xMin = " << xMin << " xMax = " << xMax << std::endl;
	// std::cout << "yMin = " << yMin << " yMax = " << yMax << std::endl;

	unsigned char maxValue = buf[y * width + x];
	int maxXLoc = x;
	int maxYLoc = y;

	for (int i = yMin; i < yMax; ++i) {
		for (int j = xMin; j < xMax; ++j) {
			// std::cout << i << ", " << j << std::endl;
			if(buf[i * width + j] > maxValue) {
				maxValue = buf[i * width + j];
				maxYLoc = i;
				maxXLoc = j;
			}
		}
	}

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