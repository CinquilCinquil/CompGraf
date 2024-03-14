#include "running_options.h"

using namespace std;

namespace CG
{

void RunningOptions::run(int argc, char *argv[]) {
	
	if (argc < 2) {
		cout << "Scene file not specified!";
		return;
	}
		
	scenefile = argv[1];
	
	// reading flags
	for (int i{2};i < argc;i ++) {
		
		if (strcmp(argv[i], "--help") == 0) {
			
			cout << "Usage: rt3 [<options>] <input_scene_file>\n";
			cout << "Rendering simulation options:\n";
			cout << "--help                     Print this help text.\n";
			cout << "--cropwindow <x0,x1,y0,y1> Specify an image crop window.\n";
			cout << "--quick                    Reduces quality parameters to render image quickly.\n";
			cout << "--outfile <filename>       Write the rendered image to <filename>.\n";
		}
		else if (strcmp(argv[i], "--cropwindow") == 0) {
			
			for (int j{1};j <= 4;j ++) {
				crop_window[j - 1] = atoi(argv[i + j]);
			}
			i += 4;
		}
		else if (strcmp(argv[i], "--quick") == 0) {
			
			quick = true;
		}
		else if (strcmp(argv[i], "--outfile") == 0) {
			
			outfile = argv[i + 1];
		}
	}
}

}