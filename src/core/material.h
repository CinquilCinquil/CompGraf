#include "rt3-base.h"

namespace rt3 {



class Material {

public:
    Spectrum color = {255, 0, 0};

    [[nodiscard]] virtual Spectrum sampleUV(const Point2f& pixel_ndc) const = 0;

};

class MatSphereUV : public Material {
    [[nodiscard]] Spectrum sampleUV(const Point2f& pixel_ndc) const override {return Spectrum{255, 0, 0};};
};

}