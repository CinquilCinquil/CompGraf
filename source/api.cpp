#include "api.h"

using namespace std;

namespace CG
{
	string Api::filename;
	int Api::w, Api::h;
	Background * Api::bkg;
	Film * Api::film;

	void Api::initialize(string filename, int w, int h, vector<RGB> colors) {
		filename = filename;
		w = w;
		h = h;
		film = new Film(filename, w, h);
		bkg = new Background(colors);
	}

	void Api::render(){
		vector<RGB> pixels;

		for (int i = 0;i < h;i ++) {
			for (int j = 0;j < w;j ++) {
				
				float u = ((float) j) / w;
				float v = ((float) i) / h;
				
				pixels.push_back(bkg->sampleUV(u, v));
			}
		}
		
		film->setPixels(pixels);
		
		cout << "Generating " << filename + ".ppm ...\n";
		film->createFile("ppm");
		cout << "Done!\n";
		
		cout << "Generating " << filename + ".png ...\n";
		film->createFile("png");
		cout << "Done!\n";
	}
}

// initializing variables
/*
int Api::w = 0, Api::h = 0;
string Api::filename = "";
Film * Api::film = NULL;
Background * Api::bkg = NULL;
*/