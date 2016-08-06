#ifndef PNG_JPEG_H
#define PNG_JPEG_H

#include <png.h>
#include <zlib.h>
#include <jpeglib.h>

typedef struct
{
	unsigned int width, height;
	unsigned char* data;
} imageData;

typedef struct
{
	png_uint_32 width;
	png_uint_32 height;
	int bit_depth;
	int color_type;
	int interlace_type;
	int compression_type;
	int filter_method;
	int channels;
} imageHeaders;

imageData	loadPNG(const char* filename);
imageData	loadJPEG(const char* filename);
void		savePNG(const char* filename, imageData input);
//void		saveJPEG(const char* filename, imageData input);

#endif