
#include "film.h"

namespace CG
{
	void Film::encodeOneStep(std::vector<unsigned char>&  filename, const unsigned char* image, unsigned width, unsigned height) {
		//Encode the image
		unsigned error = lodepng::encode(filename, image, width, height);

		//if there's an error, display it
		if(error) std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
	}
	Film::Film(string filename, int w, int h) {
		this->filename = filename;
		this->w = w;
		this->h = h;
	}
	void Film::setPixels(vector<RGB> pixels) {
		this->pixels = pixels;
	}
	void Film::createFile(string file_type) {
		
		if (file_type == "ppm")
		{
			/*
				Saving in ppm3
			*/
			
			ofstream file(filename + ".ppm");

			// Header
			file << "P3\n";
			file << to_string(w) + " " + to_string(h) + "\n";
			file << "255\n";
			
			// Stream of pixels
			for (RGB rgb : pixels) {
				file << (int) rgb.r << " ";
				file << (int) rgb.g << " ";
				file << (int) rgb.b << " ";
				file << '\n';
			}
			
			file.close();
		}
		else 
		if(file_type == "png")
		{
			/*
				Saving in png using the library lodepng
			*/
			
			string filename_with_png = filename + ".png";
			std::vector<unsigned char>* filename_ = new std::vector<unsigned char>();
			filename_.emplace(filename_with_png.begin());
			std::vector<unsigned char>* bytes = new std::vector<unsigned char>();
			for (int i{0};i < pixels.size();i ++) {
				bytes->push_back((unsigned char)pixels[i].r);
				bytes->push_back((unsigned char)pixels[i].g);
				bytes->push_back((unsigned char)pixels[i].b);
				bytes->push_back((unsigned char)255);
			}

			encodeOneStep(filename_, bytes, w, h);
			
		}
		
	}
}
class Film {
	
		
		
		
		
		
	
};