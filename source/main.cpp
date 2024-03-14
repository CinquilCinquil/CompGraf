#include <iostream>
#include "api.h"

using namespace std;
using namespace CG;

int w = 400;
int h = 200;

int main(int argc, char *argv[]) {
	
	// Reading command-line arguments
	RunningOptions runningOptions;
	runningOptions.run(argc, argv);
	
	Api::initialize(runningOptions.outfile, w, h, {RGB(0, 0, 51), RGB(0, 255, 0), RGB(255, 255, 0), RGB(255, 0, 0)});
	
	Api::render();
	
	
	return 0;
}