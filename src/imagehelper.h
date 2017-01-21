#ifndef __IMAGE_HELPER__
#define __IMAGE_HELPER__

typedef struct _imageData {
	unsigned char *buf;
	int width;
	int height;
} ImageData;

ImageData * initializeImageData(int height, int width);
void destroyImageData(ImageData * img);

unsigned char assignDirection(ImageData *img, int y, int x);

void printResult(double val);

#endif