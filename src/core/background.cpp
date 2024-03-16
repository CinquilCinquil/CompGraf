#include "background.h"

namespace rt3 {
/*!
 * Samples a color base on spherical interpolation of an image ou colored
 * background.
 *
 * @param pixel_ndc Pixel position in NDC space,  in \f$[0:1]\f$.
 * \return The interpolated color.
 */
Spectrum Background::sampleXYZ(const Point2f& pixel_ndc) const {
  // TODO
  return Spectrum{ 0, 0, 0 };
}

BackgroundColor* create_color_background(const ParamSet& ps) {
  // TODO
  return new BackgroundColor();
}
}  // namespace rt3
