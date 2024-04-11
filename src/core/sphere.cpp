
#include "sphere.h"
#include <math.h>

namespace rt3 {
real_type scalarProd(const glm::highp_vec3 &x, const glm::highp_vec3 &y)
{
    return glm::dot<3, float, glm::qualifier::highp>(x, y);
}

Sphere::Sphere(real_type radius, point3 position) : radius{radius}, position{position} 
{
    this->material = new Material();
}


bool Sphere::intersect( const Ray& r, Surfel *sf ) const
{
    //negligible situation
    if(r.direction == point3(0, 0, 0) || radius == 0 ) { return false; }

    vec3 direct = this->position - r.origin;

    real_type direct_distance_sqr = scalarProd(direct, direct);
    real_type direct_ray_prod = scalarProd(direct, r.direction);
    point3 proj_point = r.direction*(direct_ray_prod/direct_distance_sqr) - direct;

    real_type proj_dist_sqr = scalarProd(proj_point, proj_point);

    if(proj_dist_sqr > radius*radius) { return false; }

    //const real_type k = radius*radius*proj_dist_sqr/direct_distance_sqr;
    real_type diffin = sqrt(radius*radius*proj_dist_sqr/direct_distance_sqr);

    sf->p = r.direction*((direct_ray_prod/direct_distance_sqr) - diffin);
    sf->n = (sf->p - r.origin) * (1/radius);
    sf->wo = -(r.direction);

    return true;
}

bool Sphere::intersect_p( const Ray& r) const
{   
    //negligible situation
    if(r.direction == point3(0, 0, 0) || radius == 0 ) { return false; }

    vec3 direct = this->position - r.origin;

    real_type direct_distance_sqr = scalarProd(direct, direct);
    real_type direct_ray_prod = scalarProd(direct, r.direction);
    point3 proj_point = r.direction*(direct_ray_prod/direct_distance_sqr) - direct;

    real_type proj_dist_sqr = scalarProd(proj_point, proj_point);

    //ray's projection touches sphere?
    return proj_dist_sqr <= radius*radius;
}

};
