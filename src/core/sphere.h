#include "rt3-base.h"

namespace rt3 {

class Sphere : public Primitive {

    bool intersect( const Ray& r, Surfel *sf ) const;

};

}