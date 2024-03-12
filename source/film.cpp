#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "../ext/lodepng/lodepng.cpp"

using namespace std;

class Film {
	private:
		string filename;
		vector<RGB> pixels;
		int w, h;
		
		// method from 'lodepng' for saving an image as png in one-step
		void encodeOneStep(const char* filename, std::vector<byte>& image, unsigned width, unsigned height) {
		  //Encode the image
		  unsigned error = lodepng::encode(filename, image, width, height);

		  //if there's an error, display it
		  if(error) std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
		}
	
	public:
	
	
		Film(string filename, int w, int h) {
			this->filename = filename;
			this->w = w;
			this->h = h;
		}
		
		void setPixels(vector<RGB> pixels) {
			this->pixels = pixels;
		}
		
		void createFile(string file_type) {
			
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
				const char* filename_ = (char*) (filename_with_png).c_str();
	
				vector<byte> bytes;
				
				for (int i{0};i < pixels.size();i ++) {
					bytes.push_back(pixels[i].r);
					bytes.push_back(pixels[i].g);
					bytes.push_back(pixels[i].b);
					bytes.push_back(255);
				}

				encodeOneStep(filename_, bytes, w, h);
				
			}
			
		}
	
};