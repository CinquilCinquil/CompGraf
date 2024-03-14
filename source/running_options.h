#ifndef __MAIN_H__
#define __MAIN_H__

#include <iostream>
#include <string>
#include <cstring>
#include <vector>

using namespace std;

namespace CG
{

struct RunningOptions {
	
	vector<int> crop_window = {0, 1, 0, 1};
	string scenefile;
	string outfile;
	bool quick;
	
	void run(int argc, char *argv[]);
	
};

}

#endif