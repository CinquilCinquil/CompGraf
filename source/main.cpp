#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "background.cpp"
#include "../ext/lodepng/lodepng.cpp"

using namespace std;
using byte = unsigned char;

int w = 400;
int h = 200;

string header() {
	
	string str = "";
	str += "P3\n";
	str += to_string(w) + " " + to_string(h) + "\n";
	str += "255\n";
	
	return str;
}

void write_pixels(ofstream &file,vector<RGB> pixels) {
	for (RGB rgb : pixels) {
		file << (int) rgb.r << " ";
		file << (int) rgb.g << " ";
		file << (int) rgb.b << " ";
		file << '\n';
	}
}

void encodeOneStep(const char* filename, std::vector<byte>& image, unsigned width, unsigned height) {
  //Encode the image
  unsigned error = lodepng::encode(filename, image, width, height);

  //if there's an error, display it
  if(error) std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
}

int main() {
	
	ofstream myfile("a4.ppm");
	
	vector<RGB> colors = {RGB(0, 0, 51), RGB(0, 255, 0), RGB(255, 255, 0), RGB(255, 0, 0)};
	BackgroundColor bkg(colors);
	
	vector<RGB> pixels;
	
	for (int i = 0;i < h;i ++) {
		for (int j = 0;j < w;j ++) {
			
			float u = ((float) j) / w;
			float v = ((float) i) / h;
			
			pixels.push_back(bkg.sampleUV(u, v));
		}
	}
	
	myfile << header();
	write_pixels(myfile, pixels);
	
	myfile.close();
	
	
	//NOTE: this sample will overwrite the file or test.png without warning!
	const char* filename = "a4.png";

	/*
	//generate some image
	unsigned width = 512, height = 512;
	std::vector<unsigned char> image;
	image.resize(width * height * 4);

	for(unsigned y = 0; y < height; y++)
		for(unsigned x = 0; x < width; x++) {
			image[4 * width * y + 4 * x + 0] = 255 * !(x & y);
			image[4 * width * y + 4 * x + 1] = x ^ y;
			image[4 * width * y + 4 * x + 2] = x | y;
			image[4 * width * y + 4 * x + 3] = 255;
	}
	*/
	
	vector<byte> bytes;
	
	for (int i{0};i < pixels.size();i ++) {
		bytes.push_back(pixels[i].r);
		bytes.push_back(pixels[i].g);
		bytes.push_back(pixels[i].b);
		bytes.push_back(255);
	}

	encodeOneStep(filename, bytes, w, h);
	
	
	return 0;
}