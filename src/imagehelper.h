#ifndef __IMAGE_HELPER__
#define __IMAGE_HELPER__

typedef struct _imageData {
	unsigned char *buf;
	int width;
	int height;
} ImageData;

ImageData * initializeImageData(int height, int width);
ImageData * initializeEmptyImageData(int height, int width);
void destroyImageData(ImageData * img);

void printResult(double val);

int compareImages(ImageData *img1, ImageData *img2);

#endif