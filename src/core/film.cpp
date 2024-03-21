#include "film.h"

namespace rt3 {
	
// from lode
void encodeOneStep(const char* filename, std::vector<byte>& image, unsigned width, unsigned height) {
	//Encode the image
	unsigned error = lodepng::encode(filename, image, width, height);

	//if there's an error, display it
	if(error) std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
}	


//=== Film Method Definitions
Film::Film(const Point2i& resolution, const std::string& filename, image_type_e imgt)
    : m_full_resolution{ resolution }, m_filename{ filename }, m_image_type{ imgt } {
 
	//TODO
}

Film::~Film() = default;

/// Add the color to image.
void Film::add_sample(const Point2f& pixel_coord, const Spectrum& pixel_color) {
	size_t idx = pixel_coord[0]*m_full_resolution[0] + pixel_coord[1]; 
	
	pixels[idx][0] = (float)pixel_color[0];
	pixels[idx][1] = (float)pixel_color[1];
	pixels[idx][2] = (float)pixel_color[2];
}

/// Convert image to RGB, compute final pixel values, write image.
void Film::write_image() const {
	
  
	int w = m_full_resolution[0];
	int h = m_full_resolution[1];
	
	std::cout << "Info: " << w << " " << h << " " << pixels.size() << "\n";
  
	if (m_image_type == Film::image_type_e::PPM3)
	{
		/*
			Saving in ppm3
		*/
		
		std::ofstream file(m_filename + ".ppm");

		// Header
		file << "P3\n";
		file << std::to_string(w) + " " + std::to_string(h) + "\n";
		file << "255\n";
		
		// Stream of pixels
		for (Spectrum rgb : pixels) {
			file << (int) rgb[0] << " ";
			file << (int) rgb[1] << " ";
			file << (int) rgb[2] << " ";
			file << '\n';
		}
		
		file.close();
		return;
	}
	else 
	if(m_image_type == Film::image_type_e::PNG)
	{
		/*
			Saving in png using the library lodepng
		*/
		
		std::string filename_with_png = m_filename + ".png";

		const char* filename_ = (char*) (filename_with_png).c_str();
		std::cout << filename_ << "\n";

		std::vector<byte>* bytes = new std::vector<byte>();
		for (int i{0};i < pixels.size();i ++) {
			bytes->push_back((byte)pixels[i][0]);
			bytes->push_back((byte)pixels[i][1]);
			bytes->push_back((byte)pixels[i][2]);
			bytes->push_back((byte)255);
		}
		encodeOneStep(filename_, *bytes, w, h);			
	} 
}

// Factory function pattern.
// This is the function that retrieves from the ParamSet object
// all the information we need to create a Film object.
Film* create_film(const ParamSet& ps) {
  std::cout << ">>> Inside create_film()\n";
  std::string filename;
  // Let us check whether user has provided an output file name via
  // command line arguments in main().
  if (not API::curr_run_opt.outfile.empty()) {
    // Retrieve filename defined via CLI.
    filename = API::curr_run_opt.outfile;
    // Retrieve another filename, now from the ParamSet object.
    std::string filename_from_file = retrieve(ps, "filename", std::string{ "" });
    if (not filename_from_file.empty()) {  // We also get a filename from scene file...
      RT3_WARNING(string{ "Output filename supplied on command line, \"" }
                  + API::curr_run_opt.outfile
                  + string{ "\" is overriding filename provided in scene "
                            "description file, \"" }
                  + filename_from_file);
    }
  } else {
    // Try yo retrieve filename from scene file.
    filename = retrieve(ps, "filename", std::string{ "image.png" });
  }

  // Read resolution.
  // Aux function that retrieves info from the ParamSet.
  int xres = retrieve(ps, "x_res", int(1280));
  // Aux function that retrieves info from the ParamSet.
  int yres = retrieve(ps, "y_res", int(720));
  // Quick render?
  if (API::curr_run_opt.quick_render) {
    // decrease resolution.
    xres = std::max(1, xres / 4);
    yres = std::max(1, yres / 4);
  }

  // 
  // Read crop window information.
  std::vector<real_type> cw = retrieve(ps, "crop_window", std::vector<real_type>{ 0, 1, 0, 1 });
  std::cout << "Crop window ";
  for (const auto& e : cw) {
    std::cout << e << " ";
  }
  std::cout << '\n';
  
  // Note that the image type is fixed here. Must be read from ParamSet, though.
  return new Film(Point2i{ xres, yres }, filename, Film::image_type_e::PNG);
}
}  // namespace rt3
