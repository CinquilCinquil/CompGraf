#include "background.h"

namespace rt3 {
/*!
 * Samples a color base on spherical interpolation of an image ou colored
 * background.
 *
 * @param pixel_ndc Pixel position in NDC space,  in \f$[0:1]\f$.
 * \return The interpolated color.
 */
 

Spectrum BackgroundColor::lerp(const Spectrum &A, const Spectrum &B, float t) const {
	return {rt3::Lerp(A[0], B[0], t), rt3::Lerp(A[1], B[1], t), rt3::Lerp(A[2], B[2], t)};
}
 
Spectrum BackgroundColor::sampleUV(const Point2f& pixel_ndc) const {
	
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
