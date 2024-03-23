#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "rt3-base.h"

namespace rt3 {
// TODO: Create a virtual class Background and derive BackgroundColor,
// BackgroundSphereImage, BackgroundSkyBoxImage.

/*!
 * A background is basically a rectangle, have a color associated to each
 * corner. A background might be sampled based on a normalized coordinate in
 * \f$[0,1]^2.\f$. We generate samples with bilinear interpolation of the corner
 * colors. We consider the corners in a circular clockwise order, starting at
 * the lower-left corner.
 *
 * Alternatively, a background might be a regular image.
 */
class Background {
public:
  /// Types of color mapping scheme
  enum class mapping_t {
    screen = 0,  //!< background is assigned to the image screen
    spherical    //!< background is assigne to a sphere surrounding the scene.
  };

  mapping_t mapping_type;  //!< sampling type we use to pick a color.

  /// Ctro receiving a single color for the entire background.
  Background(mapping_t mt = mapping_t::spherical) : mapping_type{ mt } { /* empty */ }

  virtual ~Background(){ /* empty */ };
};

class BackgroundColor : public Background {
private:
  /// Each corner has a color associated with.
  Spectrum corners[4] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
  /// Corner indices.
  enum Corners_e {
    bl = 0,  //!< Bottom left corner.
    tl,      //!< Top left corner.
    tr,      //!< Top right corner.
    br       //!< Bottom right corner.
  };

public:
  /// Ctro receives a list of four colors, for each corner.
  
  BackgroundColor(const vector<Spectrum>& colors) {
    // clock-wise starting at bottom_left
    for (int i = 0;i < 4;i ++) {corners[i] = colors[i];}
  }
  
  Spectrum lerp(const Spectrum &A, const Spectrum &B, float t) const;
	
  [[nodiscard]] Spectrum sampleUV(const Point2f& pixel_ndc) const;

  virtual ~BackgroundColor(){};
};

// factory pattern functions.
BackgroundColor* create_color_background(const ParamSet& ps);

}  // namespace rt3
#endif
