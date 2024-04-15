#include "background.h"

namespace rt3 {
/*!
 * Samples a color base on spherical interpolation of an image ou colored
 * background.
 *
 * @param pixel_ndc Pixel position in NDC space,  in \f$[0:1]\f$.
 * \return The interpolated color.
 */
 
BackgroundColor::BackgroundColor(const vector<Spectrum>& colors) {
	// clock-wise starting at bottom_left
	for (int i = 0;i < 4;i ++) {corners[i] = colors[i];}
}

BackgroundColor::BackgroundColor(const Spectrum& color) {
	// clock-wise starting at bottom_left
	for (int i = 0;i < 4;i ++) {corners[i] = color;}
}

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

	// if a 'color' param is detected
	if (ps.find("color") != ps.end()) {
		Spectrum color = retrieve(ps, "color", Spectrum{0, 0, 0});
		return new BackgroundColor({color, color, color, color});
	}
	else {
		Spectrum tl, tr, bl, br;
		tl = retrieve(ps, "tl", Spectrum{0, 0, 0});
		tr = retrieve(ps, "tr", Spectrum{0, 0, 0});
		bl = retrieve(ps, "bl", Spectrum{0, 0, 0});
		br = retrieve(ps, "br", Spectrum{0, 0, 0});

		std::cout << tl << " " << tr << " " << bl << " " << br;

		return new BackgroundColor({bl, tl, tr, br});
	}
}


}  // namespace rt3
