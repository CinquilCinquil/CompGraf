#include "film.h"
#include <cmath>

#include "api.h"
#include "image_io.h"
#include "paramset.h"

namespace rt3 {

//=== Film Method Definitions
Film::Film(const Point2i& resolution, const std::string& filename, image_type_e imgt)
    : m_full_resolution{ resolution }, m_filename{ filename }, m_image_type{ imgt } {
  // TODO
}

Film::~Film() = default;

/// Add the color to image.
void Film::add_sample(const Point2f& pixel_coord, const Color24& pixel_color) {
  // TODO: add color to the proper location.
}

/// Convert image to RGB, compute final pixel values, write image.
void Film::write_image() const {
  // TODO: call the proper writing function, either PPM or PNG.
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

  // TODO
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
