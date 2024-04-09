
#include "sphere.h"
#include <math.h>

namespace rt3 {
real_type scalarProd(const glm::highp_vec3 &x, const glm::highp_vec3 &y)
{
    return glm::dot<3, float, glm::qualifier::highp>(x, y);
}

bool Sphere::intersect( const Ray& r, Surfel *sf ) const
{
    if(r.direction == point3(0, 0, 0)) { return false; }
    vec3 direct = this->position - r.origin;
    real_type d = scalarProd(direct, direct);
    real_type t = scalarProd(direct, r.direction);
    

    point3 point_in = r.direction*(t/d) - direct;

    real_type point_in_sqrd = scalarProd(point_in, point_in);

    if(point_in_sqrd > radius*radius) { return false; }

    //const real_type k = radius*radius*point_in_sqrd/d;
    real_type diffin = sqrt(radius*radius*point_in_sqrd/d);

    sf->p = r.direction*(t/d) - r.direction*diffin;
    //point3 norm = sf->p - r.origin;
    sf->n = sf->p - r.origin;
    sf->wo = -(r.direction);

    return true;
}

bool Sphere::intersect_p( const Ray& r) const
{
    return true;
}

};
