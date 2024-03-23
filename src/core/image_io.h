#ifndef IMAGE_H
# define IMAGE_H

# include <string>

#include "rt3.h"

namespace rt3 {
/// Routines to write images to a file.
bool save_ppm6(unsigned char*, size_t, size_t, size_t = 1, const std::string& = "image.ppm");

/// Saves an image as a **ascii** PPM file.
bool save_ppm3(unsigned char*, size_t, size_t, size_t = 1, const std::string& = "image.ppm");
void save_ppm3(std::vector<Spectrum>, size_t, size_t, const std::string& = "image.ppm");

/// Saves an image as a PNG file.
bool save_png(unsigned char*, size_t, size_t, size_t = 1, const std::string& = "image.png");
void save_png(std::vector<Spectrum>, size_t, size_t, const std::string& = "image.png");

}  // namespace rt3

#endif

//================================[ imagem.h ]================================//
