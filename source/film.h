#ifndef __FIML_H__
#define __FIML_H__
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "../ext/lodepng/lodepng.h"

using namespace std;

namespace CG
{
	struct RGB
	{
		float r;
		float g;
		float b;
	};

    class Film {
	private:
		string filename;
		vector<RGB> pixels;
		int w, h;
		void encodeOneStep(std::vector<unsigned char>& filename, const unsigned char* image, unsigned width, unsigned height);
	
	public:
		Film(string filename, int w, int h);
		void setPixels(vector<RGB> pixels);
		void createFile(string file_type);
	}
}
#endif