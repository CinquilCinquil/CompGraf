#include "background.h"

namespace rt3 {
/*!
 * Samples a color base on spherical interpolation of an image ou colored
 * background.
 *
 * @param pixel_ndc Pixel position in NDC space,  in \f$[0:1]\f$.
 * \return The interpolated color.
 */
 
float lerp_float(float a, float b, float t) {	
	return (1 - t)*a + t*b;
}

Spectrum BackgroundColor::lerp(const Spectrum &A, const Spectrum &B, float t) const {
	return {lerp_float(A[0], B[0], t), lerp_float(A[1], B[1], t), lerp_float(A[2], B[2], t)};
}
 
Spectrum BackgroundColor::sampleXY(const Point2f& pixel_ndc) const {
	
	float u = pixel_ndc[0];
	float v = pixel_ndc[1];
	
	Spectrum a = lerp(corners[Corners_e::bl], corners[Corners_e::br], u);
	Spectrum b = lerp(corners[Corners_e::tl], corners[Corners_e::tr], u);
	
	return lerp(b, a, v);
}


BackgroundColor* create_color_background(const ParamSet& ps) {
  return new BackgroundColor({{0, 0, 51}, {0, 255, 0}, {255, 255, 0}, {255, 0, 0}});
}


}  // namespace rt3
