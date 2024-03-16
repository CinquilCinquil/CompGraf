#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "image_io.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

#include "../ext/lodepng.h"
#include "../ext/stb_image_write.h"

namespace rt3 {

/// Saves an image as a **binary** PPM file.
bool save_ppm6(unsigned char* data, size_t w, size_t h, size_t d, const std::string& file_name_) {
  std::ofstream ofs_file(file_name_, std::ios::out | std::ios::binary);
  if (not ofs_file.is_open()) {
    return false;
  }

  ofs_file << "P6\n"
           << w << " " << h << "\n"
           << "255\n";

  ofs_file.write((char*)data, d * w * h);

  // Did it not fail?
  auto result = not ofs_file.fail();

  ofs_file.close();

  return result;
}

/// Saves an image as a **ascii** PPM file.
bool save_ppm3(unsigned char* data, size_t w, size_t h, size_t d, const std::string& file_name_) {
  std::ofstream ofs_file(file_name_, std::ios::out);
  if (not ofs_file.is_open()) {
    return false;
  }

  ofs_file << "P3\n"
           << w << " " << h << "\n"
           << "255\n";

  size_t i{ 0 };
  while (i < (w * h * d)) {
    // depth traversal, usually 3.
    for (auto id{ 0u }; id < d; id++) {
      ofs_file << (int)*(data + i++) << " ";
    }
    ofs_file << '\n';
  }

  // Did it not fail?
  auto result = not ofs_file.fail();

  ofs_file.close();

  return result;
}

bool save_png(unsigned char* data, size_t w, size_t h, size_t d, const std::string& file_name_) {
#define LODEPNG
#ifdef LODEPNG
  std::cout << "depth = " << d << '\n';
  std::vector<unsigned char> img;  //( w * h * d );
  std::copy(data, data + (w * h * d), std::back_inserter(img));
  // Encode from raw pixels to disk with a single function call
  // The image argument has width * height RGBA pixels or width * height * 4
  // bytes Encode the image
  unsigned error = lodepng::encode(file_name_, img, w, h);

  // if there's an error, display it
  if (error != 0U) {
    std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << '\n';
  }

  return true;

#else
  // =============================================
  // INFO on the stb_image_write API
  // =============================================
  //
  // int stbi_write_png(char const *filename, int w, int h, int comp, const void
  // *data, int stride_in_bytes);
  //
  // The functions create an image file defined by the parameters. The image
  // is a rectangle of pixels stored from left-to-right, top-to-bottom.
  // Each pixel contains 'comp' channels of data stored interleaved with 8-bits
  // per channel, in the following order: 1=Y, 2=YA, 3=RGB, 4=RGBA. (Y is
  // monochrome color.) The rectangle is 'w' pixels wide and 'h' pixels tall.
  // The *data pointer points to the first byte of the top-left-most pixel.
  // For PNG, "stride_in_bytes" is the distance in bytes from the first byte of
  // a row of pixels to the first byte of the next row of pixels.
  // Each function returns 0 on failure and non-0 on success.
  //
  // PNG allows you to set the deflate compression level by setting the global
  // variable 'stbi_write_png_compression_level' (it defaults to 8).
  // int stbi_write_png_compression_level;    // defaults to 8; set to higher
  // for more compression
  //
  stbi_write_png_compression_level = 0;  // defaults to 8; set to higher for more compression
  return (0
          != stbi_write_png(file_name_.c_str(),  // file name
                            w,
                            h,        // image dimensions
                            d,        // # of channels per pixel
                            data,     // the pixels
                            w * d));  // length of a row (in bytes), se above.
#endif
}

}  // namespace rt3

//================================[ imagem_io.h
//]================================//
