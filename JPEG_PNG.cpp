#include "JPEG_PNG.h"
#include <cstdlib>
#include <iostream>

/***************************************************************************************************
********************************************PNG READER**********************************************
***************************************************************************************************/

/*-----------------------------------*/
//checks the first 8 bytes of the file to see if it is a PNG file
int is_PNG_file(FILE* imgsrc)
{
	png_byte pngsig[8];
	int is_png = 0;
	fread((char*)pngsig, 1, 8, imgsrc);
	//check file
	if (!imgsrc) return 0;
	//check if png
	is_png = png_sig_cmp(pngsig, 0, 8);
	return (is_png == 0);
}
/*-----------------------------------*/
void read_PNG_file(FILE* fp, imageData* mydata)
{
	int i, j;
	//validate the data source
	if (!is_PNG_file(fp)) {
		fprintf(stderr, "ERROR: Data is not valid PNG-data\n");
		exit(EXIT_FAILURE);
		return;
	}
	std::cout << "1" << std::endl;

	//create png read struct. (NULLS are used for custom error handling functions)
	png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!pngPtr) {
		fprintf(stderr, "ERROR: Couldn't initialize png read struct\n");
		exit(EXIT_FAILURE);
		return;
	}
	std::cout << "2" << std::endl;

	//create png info struct.
	png_infop infoPtr = png_create_info_struct(pngPtr);
	if (!infoPtr) {
		fprintf(stderr, "ERROR: Couldn't initialize png info struct\n");
		//clean up read structure allocation
		png_destroy_read_struct(&pngPtr, (png_infopp)NULL, (png_infopp)NULL);
		exit(EXIT_FAILURE);
		return;
	}
	std::cout << "3" << std::endl;

	//not really sure why this is here, I think its a info 
	//structure for error functions if necessary?
	png_infop endInfo = png_create_info_struct(pngPtr);
	if (!endInfo) {
		fprintf(stderr, "ERROR: Couldn't initialize png info struct\n");
		//clean up read structure allocation
		png_destroy_read_struct(&pngPtr, &infoPtr, (png_infopp)NULL);
		exit(EXIT_FAILURE);
		return;
	}
	std::cout << "4" << std::endl;

	//when libpng encounters error, it expects to longjmp back to your routine.
	//longjmp essentially restores the environment to a previous point.
	//setjmp fills the given jmp_buf object with information of the current state
	//of the calling environment that will be used by longjmp to be restored
	if (setjmp(png_jmpbuf(pngPtr))) {
		png_destroy_read_struct(&pngPtr, &infoPtr, &endInfo);
		fclose(fp);
		exit(EXIT_FAILURE);
		return;
	}
	std::cout << "5" << std::endl;

	//set up libpng io for the source file
	png_init_io(pngPtr, fp);
	//tell libpng that we already read 8 bytes of file (stops it from trying to read
	//the header again)
	png_set_sig_bytes(pngPtr, 8);

	std::cout << "5.5" << std::endl;
	//implement low-level read interface
	png_read_info(pngPtr, infoPtr);
	std::cout << "5.5" << std::endl;
	//read info structure data
	imageHeaders a;
	std::cout << "5.5" << std::endl;
	png_get_IHDR(pngPtr, infoPtr, &a.width, &a.height, &a.bit_depth, &a.color_type,
		&a.interlace_type, &a.compression_type, &a.filter_method);
	a.channels = png_get_channels(pngPtr, infoPtr);


	std::cout << "6" << std::endl;

	//setup any necessary transformations (Want all images in RGBA 8-bit_depth format)
	if (a.color_type == PNG_COLOR_TYPE_PALETTE) {
		png_set_palette_to_rgb(pngPtr);
		printf("Changed from color pallet to rgb\n");
		a.color_type = PNG_COLOR_TYPE_RGB;
	}
	if (a.color_type == PNG_COLOR_TYPE_GRAY & a.bit_depth < 8)
		png_set_gray_1_2_4_to_8(pngPtr);
	if (png_get_valid(pngPtr, infoPtr, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(pngPtr);
	//change from 16 to 8-bit data
	if (a.bit_depth == 16) 	png_set_strip_16(pngPtr);
	//expand gray-scale to rgb
	if (a.color_type == PNG_COLOR_TYPE_GRAY || a.color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(pngPtr);
	//expand from 3-byte to 4-byte with opaque alpha value
	if (a.color_type == PNG_COLOR_TYPE_RGB) {
		png_set_filler(pngPtr, 0x00, PNG_FILLER_AFTER);
		printf("Called Filler Function\n");
	}


	std::cout << "7" << std::endl;

	//allocate data
	png_bytepp row_pointers = (png_bytepp)png_malloc(pngPtr, a.height * sizeof(png_bytep));
	for (i = 0; i < a.height; i++)
		row_pointers[i] = (png_byte*)png_malloc(pngPtr, a.width * 4);
	png_set_rows(pngPtr, infoPtr, row_pointers);

	png_read_image(pngPtr, row_pointers);


	std::cout << "8" << std::endl;

	//set data to ImageData struct
	printf("The given height and width are %i and %i\n", (int)a.height, (int)a.width);
	mydata->width = a.width;
	mydata->height = a.width;
	mydata->data = (unsigned char*)malloc(a.width * a.height * 4);
	if (a.channels != 4) {
		printf("Don't have 4 RGBA channels\n");
		printf("Only have %i\n", a.channels);
	}
	for (i = 0; i < a.height; i++) {
		//printf("%i\n",i);
		memcpy(mydata->data + i*(a.width * 4), row_pointers[a.height - i - 1], a.width * 4);
	}

	std::cout << "9" << std::endl;
}
/*-----------------------------------*/

imageData loadPNG(const char* filename)
{
	FILE* source = fopen(filename, "rb");
	imageData pngData;
	std::cout << "help?" << std::endl;
	read_PNG_file(source, &pngData);
	return pngData;
}
/*-----------------------------------*/
/***************************************************************************************************
***************************************JPEG READER**************************************************
***************************************************************************************************/

//taken from libjpeg-turbo example

struct my_error_mgr {
	struct jpeg_error_mgr pub;
	jmp_buf setjmp_buffer;
};

typedef struct my_error_mgr * my_error_ptr;

void my_error_exit(j_common_ptr cinfo)
{
	my_error_ptr myerr = (my_error_ptr)cinfo->err;
	(*cinfo->err->output_message) (cinfo);
	longjmp(myerr->setjmp_buffer, 1);
}
/*-----------------------------------*/
int read_JPEG_file(FILE* source, imageData* mydata)
{
	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr;
	unsigned char* tmpData;

	JSAMPARRAY buffer;		//output row buffer
	int row_stride;			//row width

							//Step 1: allocate and initialize JPEG decompression object

							//setup normal JPEG error routines, and XXoverride error_exitXX
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	//Establish the setjmp return context for my_error_exit to use
	if (setjmp(jerr.setjmp_buffer)) {
		jpeg_destroy_decompress(&cinfo);
		fclose(source);
		return 0;
	}

	//initialize JPEG decompression object
	jpeg_create_decompress(&cinfo);

	//Step 2: Specify Data Source
	jpeg_stdio_src(&cinfo, source);

	//Step 3: read file parameters with jpeg_read_header()
	(void)jpeg_read_header(&cinfo, TRUE);

	//Step 4: set parameters for decompression

	//Step 5: start decompressor
	(void)jpeg_start_decompress(&cinfo);

	row_stride = cinfo.output_width * cinfo.output_components;
	buffer = (*cinfo.mem->alloc_sarray) ((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);
	tmpData = (unsigned char*)malloc(cinfo.output_width*cinfo.output_height*cinfo.output_components);

	//add in corrections...
	int buffer_height = 1;
	buffer = (JSAMPARRAY)malloc(sizeof(JSAMPROW) * buffer_height);
	buffer[0] = (JSAMPROW)malloc(sizeof(JSAMPLE) * row_stride);

	printf("hello\n");

	//Step 6: read the image line by line
	int counter = 0;
	while (cinfo.output_scanline < cinfo.output_height) {
		jpeg_read_scanlines(&cinfo, buffer, 1);
		memcpy(tmpData + counter, buffer[0], row_stride);
		counter += row_stride;
	}

	//assign data to ImageData struct, adding alpha-channel to data
	mydata->width = cinfo.output_width;
	mydata->height = cinfo.output_height;
	mydata->data = (unsigned char*)malloc(cinfo.output_width*cinfo.output_height * 4);

	int i, j, row_length = mydata->width * 4, tmp_row_length = mydata->width * 3;
	for (i = 0; i < mydata->height; i++) {
		for (j = 0; j < mydata->width; j++) {
			memcpy(mydata->data + (i*row_length + j * 4), tmpData + (i*tmp_row_length + j * 3), 3);
			mydata->data[i*row_length + j * 4 + 3] = 0x00;
		}
	}


	//Step 7: Finish decompression
	(void)jpeg_finish_decompress(&cinfo);

	//Step 8: Release JPEG decompression object
	jpeg_destroy_decompress(&cinfo);
}
/*-----------------------------------*/
imageData loadJPEG(const char* filename)
{
	FILE* source = fopen(filename, "rb");
	imageData jpgData;
	read_JPEG_file(source, &jpgData);
	return jpgData;
}
/*-----------------------------------*/

/***************************************************************************************************
********************************************PNG WRITER**********************************************
***************************************************************************************************/

inline void setRGB(png_byte *ptr, float val)
{
	int v = (int)(val * 767);
	if (v < 0) v = 0;
	if (v > 767) v = 767;
	int offset = v % 256;

	if (v<256) {
		ptr[0] = 0; ptr[1] = 0; ptr[2] = offset;
	}
	else if (v<512) {
		ptr[0] = 0; ptr[1] = offset; ptr[2] = 255 - offset;
	}
	else {
		ptr[0] = offset; ptr[1] = 255 - offset; ptr[2] = 0;
	}
}

int writePNG(char* filename, unsigned int width, unsigned int height, unsigned char* buffer, char* title) {
	int code = 0;
	FILE *fp = NULL;
	png_structp	png_ptr = NULL;
	png_infop	info_ptr = NULL;
	png_bytep	row = NULL;

	fp = fopen(filename, "wb");
	if (fp == NULL) {
		fprintf(stderr, "Could not open file %s for writing\n", filename);
		code = 1;
		goto finalise;
	}

	//Initialize write structure
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		fprintf(stderr, "Could not allocate write struct\n");
		code = 1;
		goto finalise;
	}

	//Initialize info structure
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		fprintf(stderr, "Could not allocate info struct\n");
		code = 1;
		goto finalise;
	}

	// Setup Exception handling
	if (setjmp(png_jmpbuf(png_ptr))) {
		fprintf(stderr, "Error during png creation\n");
		code = 1;
		goto finalise;
	}

	png_init_io(png_ptr, fp);

	// Write header (8 bit colour depth)
	png_set_IHDR(png_ptr, info_ptr, width, height,
		8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	// Set title
	if (title != NULL) {
		png_text title_text;
		title_text.compression = PNG_TEXT_COMPRESSION_NONE;
		title_text.key = "Title";
		title_text.text = title;
		png_set_text(png_ptr, info_ptr, &title_text, 1);
	}

	png_write_info(png_ptr, info_ptr);

	// Allocate memory for one row (3 bytes per pixel - RGB)
	row = (png_bytep)malloc(3 * width * sizeof(png_byte));

	// Write image data
	int x, y;
	for (y = 0; y<height; y++) {
		for (x = 0; x<width; x++) {
			setRGB(&(row[x * 3]), buffer[y*width + x]);
		}
		png_write_row(png_ptr, row);
	}

	// End write
	png_write_end(png_ptr, NULL);

finalise:
	if (fp != NULL) fclose(fp);
	if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
	if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
	if (row != NULL) free(row);

	return code;
}
/*-----------------------------------*/

void savePNG(char* filename, imageData input) {
	writePNG(filename, input.width, input.height, input.data, filename);
}