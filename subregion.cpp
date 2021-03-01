#include <iostream>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <string>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb/stb_image.h"
#include "stb/stb_image_resize.h"
#include "stb/stb_image_write.h"

using namespace std;

// 
// Image structure
//
struct MyImg {
	unsigned char* data;
	int width;
	int channels;
	int height;
	char* filename;
};

void prn_img(MyImg* img) {
	cout << "width = " << img->width << endl;
	cout << "height = " << img->height << endl;
	cout << "channels = " << img->channels << endl;
}

void rm_img(MyImg** img) {
	// deletes img and sets it to null
	stbi_image_free((*img)->data);
	delete *img;
	img = 0;
	cout << "Image successfully removed." << endl;
}

MyImg* load_img(char* filename) {
	// Returns a MyImg pointer containing image data.
	// If the file loading is unsuccessful, it returns a 0
	
	int width, height, channels;
	unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);
	if(data == 0) {
		cout << "Error loading image file." << endl;
		return 0;
	}
	
	MyImg* img = new MyImg;
	img->width = width;
	img->height = height;
	img->channels = channels;
	img->filename = filename;
	img->data = data;

	cout << "Image successfully loaded." << endl;

	return img;

}

void save_img(MyImg* img) {
	// Saves to a jpg file
	stbi_write_jpg("sub.jpg", img->width, img->height, img->channels, img->data, 100);
	cout << "Image successfully saved." << endl;
}

//
// Setters
//

void set_pixel_red(MyImg* img, int r, int c, uint8_t val) {
	int width = img->width;
	int height = img->height;
	int channels = img->channels;
	unsigned char* p = img->data + r*width*channels + c*channels;
	*p = val;
}

void set_pixel_green(MyImg* img, int r, int c, uint8_t val) {
	int width = img->width;
	int height = img->height;
	int channels = img->channels;
	unsigned char* p = img->data + r*width*channels + c*channels + 1;
	*p = val;
}

void set_pixel_blue(MyImg* img, int r, int c, uint8_t val) {
	int width = img->width;
	int height = img->height;
	int channels = img->channels;
	unsigned char* p = img->data + r*width*channels + c*channels + 2;
	*p = val;
}

//
// Getters
//

uint8_t get_pixel_red(MyImg* img, int r, int c) {
	int width = img->width;
	int height = img->height;
	int channels = img->channels;
	unsigned char* p = img->data + r*width*channels + c*channels;
	return *p;
}

uint8_t get_pixel_green(MyImg* img, int r, int c) {
	int width = img->width;
	int height = img->height;
	int channels = img->channels;
	unsigned char* p = img->data + r*width*channels + c*channels + 1;
	return *p;
}

uint8_t get_pixel_blue(MyImg* img, int r, int c) {
	int width = img->width;
	int height = img->height;
	int channels = img->channels;
	unsigned char* p = img->data + r*width*channels + c*channels + 2;
	return *p;
}

//
// Subregion generator
// 

MyImg* get_sub(MyImg* src, int top, int left, int bottom, int right) {
	MyImg* sub = new MyImg;
	sub->width = right - left;
	sub->height = bottom - top;
	sub->channels = src->channels;
	sub->data = (unsigned char*) malloc(sub->width * sub->height * sub->channels);

	for(int i = top; i < bottom; i ++) {
		for(int j = left; j < right; j ++) {
			set_pixel_red(sub, i - top, j - left, get_pixel_red(src, i, j));
			set_pixel_green(sub, i - top, j - left, get_pixel_green(src, i, j));
			set_pixel_blue(sub, i - top, j - left, get_pixel_blue(src, i, j));
		}
	}

	cout << "Subregion successfully generated." << endl;

	return sub;
}

// 
// The main function
//

int main(int argc, char** argv) {

	if(argc != 6) {
		cerr << "Usage: " << argv[0] << "[JPEG filename] [y-start] [x-start] [y-end] [x-end]" << endl;
		return -1;
	}

	if(atoi(argv[3]) > atoi(argv[5]) || atoi(argv[2]) > atoi(argv[4])) {
		cerr << "Improper bounds. First two must be less than latter two." << endl;
		return -2;
	}

	MyImg* img = load_img(argv[1]);

	if(img->width < atoi(argv[5]) || img->height < atoi(argv[4]) || atoi(argv[3]) < 0 || atoi(argv[2]) < 0) {
		cerr << "Dimensions out of bounds of input JPEG file." << endl;
		cerr << "X must not exceed " << img->width << endl;
		cerr << "Y must not exceed " << img->height << endl;
		return -3;
	}

	// Generate the subregion
	
	MyImg* img2 = get_sub(img, atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));

	save_img(img2);

	rm_img(&img);
	rm_img(&img2);
	
	return 0;
}
