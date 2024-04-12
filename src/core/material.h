#include "rt3-base.h"

namespace rt3 {



class Material {

public:
    Spectrum color = {255, 0, 0};

    [[nodiscard]] virtual Spectrum sampleUV(const Point2f& pixel_ndc) const;

};

class MatSphereUV : public Material
{
    [[nodiscard]] Spectrum sampleUV(const Point2f& pixel_ndc) const override;
};

}