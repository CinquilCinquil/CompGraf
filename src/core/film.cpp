#include "film.h"

namespace rt3 {

//=== Film Method Definitions
Film::Film(const Point2i& resolution, const std::string& filename, image_type_e imgt)
    : m_full_resolution{ resolution }, m_filename{ filename }, m_image_type{ imgt } {
 
	//TODO
}

//=== Film Method Definitions
Film::Film(const Point2i& resolution, const Point2i& p1, const Point2i& p2, const std::string& filename, image_type_e imgt)
    : m_full_resolution{resolution}, 
	m_initial_points{p1}, m_final_points{p2},
	m_filename{ filename }, m_image_type{ imgt } {
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
	
	size_t w = m_final_points[0] - m_initial_points[0];
	size_t h = m_final_points[1] - m_initial_points[1];

	if (m_image_type == Film::image_type_e::PPM3)
	{
		save_ppm3(pixels, w, h, m_filename);
	}
	else if(m_image_type == Film::image_type_e::PNG) {
		save_png(pixels, w, h, m_filename);
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
  std::vector<real_type> cw;
  if (API::curr_run_opt.croped_window) {
	cw = {API::curr_run_opt.crop_window[0][0], API::curr_run_opt.crop_window[0][1],
		API::curr_run_opt.crop_window[1][0], API::curr_run_opt.crop_window[1][1]};
  }
  else {
	cw = retrieve(ps, "crop_window", std::vector<real_type>{ 0, 1, 0, 1 });
  }

  std::cout << "Crop window ";
  for (const auto& e : cw) {
    std::cout << e << " ";
  }
  std::cout << '\n';

  // crop window logic

  Point2i p1 = {(int) (xres * cw[0]), (int) (yres * cw[2])};
  Point2i p2 = {(int) (xres * cw[1]), (int) (yres * cw[3])};
  
  // determing image type

  std::string img_type_str = retrieve(ps, "img_type", std::string{ "" });

  Film::image_type_e img_type;
  if (img_type_str == "ppm3") img_type = Film::image_type_e::PPM3;
  if (img_type_str == "ppm6") img_type = Film::image_type_e::PPM6;
  if (img_type_str == "png") img_type = Film::image_type_e::PNG;

  //

  return new Film(Point2i{xres, yres}, p1, p2, filename, img_type);
}
}  // namespace rt3
