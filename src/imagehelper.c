#include "imagehelper.h"

#include <stdlib.h>
#include <time.h>

ImageData * initializeImageData(int height, int width)
{
	srand(time(0));
	ImageData *img = calloc(1, sizeof(ImageData));
	img->width = width;
	img->height = height;
	img->buf = calloc(width * height, sizeof(unsigned char));
	for(int i = 0; i < width * height; ++i) {
		img->buf[i] = rand() % 256;
	}
	return img;
}

void destroyImageData(ImageData * img)
{
	free(img->buf);
	free(img);
}