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
	stbi_write_jpg("flipped.jpg", img->width, img->height, img->channels, img->data, 100);
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
// Flip code
//

void flip(MyImg* img, int dir) {
	if(dir == 0) { // Flip image vertically
		MyImg* temp = load_img(img->filename);
		
		for(int i = 0; i <= img->height; i++) {
			for(int j = 0; j <= img->width; j++) {
				set_pixel_red(img, i, j, get_pixel_red(temp, img->height - i, j));
				set_pixel_green(img, i, j, get_pixel_green(temp, img->height - i, j));
				set_pixel_blue(img, i, j, get_pixel_blue(temp, img->height - i, j));
			}
		}
		rm_img(&temp);
		cout << "Image successfully flipped vertically." << endl;
	} else if(dir == 1) { // Flip image horizontally
		MyImg* temp = load_img(img->filename);
		
		for(int i = 0; i < img->height; i++) {
			for(int j = 0; j < img->width; j++) {
				set_pixel_red(img, i, j, get_pixel_red(temp, i, img->width - j));
				set_pixel_green(img, i, j, get_pixel_green(temp, i, img->width - j));
				set_pixel_blue(img, i, j, get_pixel_blue(temp, i, img->width - j));
			}
		}
		rm_img(&temp);
		cout << "Image successfully flipped horizontally." << endl;
	} else {
		cerr << "Invalid input for dir. Must be either 0 or 1," << endl;
	}
}

// 
// The main function
//

int main(int argc, char** argv) {

	if(argc != 3) {
		cerr << "Usage: " << argv[0] << "[JPEG filename] [flip direction]" << endl;
		cout << "JPEG filename: The .jpg file name/address." << endl;
		cout << "Flip direction: The direction which the image is flipped (0 for vertical, 1 for horizontal)." << endl;
		return -1;
	}

	MyImg* img = load_img(argv[1]);

	// Flip the image
	
	flip(img, atoi(argv[2]));

	save_img(img);

	rm_img(&img);
	
	return 0;
}
