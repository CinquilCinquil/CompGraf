#ifndef MATERIAL_H
#define MATERIAL_H

#include "rt3.h"
#include "paramset.h"
#include <functional>

namespace rt3 {

class Material {

public:

    std::function <Spectrum(const Point2f& pixel_ndc)> samplingMethod;

    [[nodiscard]] virtual Spectrum sampleUV(const Point2f& pixel_ndc) const;

};

Material* create_material(const ParamSet& ps);

}

#endif