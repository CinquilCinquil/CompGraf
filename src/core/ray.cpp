#include "ray.h"

namespace rt3 {

    Ray::Ray (const point3& o, const vec3& d, real_type start, real_type end) : o{o}, d{d}, t_min{start}, t_max{end} 
    {}

    Ray::Ray() : t_min{0.f}, t_max{INFINITY}{/*empty*/}
    point3 Ray::operator()(real_type t) const {
        return this->o + this->d * t;
    }
}