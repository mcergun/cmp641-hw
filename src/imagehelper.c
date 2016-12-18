#include "imagehelper.h"

#include <stdlib.h>
#include <time.h>

ImageData * initializeImageData(int height, int width)
{
	srand(time(0));
	ImageData *img = calloc(1, sizeof(ImageData));
	img->width = width;
	img->height = height;
	img->buf = allocate2dBuffer(height, width);
	for(int i = 0; i < height; ++i) {
		for(int j = 0; j < width; j++) {
			img->buf[i][j] = rand() % 256;
		}
	}
	return img;
}

void destroyImageData(ImageData * img)
{
	//free each row buffer
	for(int i = 0; i < img->height; ++i) {
		free(img->buf[i]);
	}

	//free row buffer holders
	free(img->buf);

	//free struct
	free(img);
}

unsigned char ** allocate2dBuffer(int rows, int cols)
{
	unsigned char **buf = malloc(sizeof(unsigned char *) * rows);
	if(buf) {
		for(int i = 0; i < rows; ++i) {
			buf[i] = malloc(sizeof(unsigned char) * cols);
		}
	}

	return buf;
}