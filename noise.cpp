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

void gen_rand_loc(int width, int height, int &r, int &c) {
	r = rand() % height;
	c = rand() % width;
}

// 
// Image structure
//
struct MyImg {
	unsigned char* data;
	int width;
	int channels;
	int height;
	std::string filename;
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

void save_img(const string& filename, MyImg* img) {
	// Saves to a jpg file
	stbi_write_jpg("noisy.jpg", img->width, img->height, img->channels, img->data, 100);
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
// The main function
//

int main(int argc, char** argv) {

	if(argc != 2) {
		cerr << "Usage: " << argv[0] << " [JPEG filename]" << endl;
		return -1;
	}

	MyImg* img = load_img(argv[1]);

	// Set 20% of pixels to black.
	
	srand(time(NULL));
	
	int r,c;
	for(int i = 0; i < floor(img->width * img->height * img->channels / 5); i++) {
		gen_rand_loc(img->width, img->height, r, c);
		set_pixel_red(img, r, c, 0);
		set_pixel_green(img, r, c, 0);
		set_pixel_blue(img, r, c, 0);
	}

	save_img("noisy.jpg", img);

	rm_img(&img);
	
	return 0;
}
