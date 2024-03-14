#ifndef __API_H__
#define __API_H__
#include <iostream>
#include <string>
#include <vector>
#include "structures.h"
#include "background.h"
#include "film.h"
#include "running_options.h"

namespace CG 
{
    class Api {
	
	public:
		static string filename;
		static int w, h;
		static Background * bkg;
		static Film * film;
		
		static void initialize(string filename, int w, int h, vector<RGB> colors);
		
		static void render();
    };
}
	

#endif