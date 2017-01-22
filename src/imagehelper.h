#ifndef __IMAGE_HELPER__
#define __IMAGE_HELPER__

typedef struct _imageData {
	unsigned char *buf;
	int width;
	int height;
} ImageData;

// used for algorithm's 2nd step
static int xx[] = {0, 1, 1, 1, 0, -1, -1, -1};
static int yy[] = {-1, -1, 0, 1, 1, 1, 0, -1};

ImageData * initializeImageData(int height, int width);
ImageData * initializeEmptyImageData(int height, int width);
void destroyImageData(ImageData * img);

void assignDirection(ImageData *img, ImageData *dest, int y, int x);
void sumAndCompare(ImageData *img, ImageData *dest, int y, int x, int limit);

void printResult(double val);

double analyzeArrayS(ImageData *src, ImageData *dest, ImageData *dest2);
double analyzeArrayM1(ImageData *src, ImageData *dest, ImageData *dest2);
double analyzeArrayM1wC(ImageData *src, ImageData *dest, ImageData *dest2);
double analyzeArrayM2(ImageData *src, ImageData *dest, ImageData *dest2);
int compareImages(ImageData *img1, ImageData *img2);

#endif