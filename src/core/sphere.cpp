
#include "sphere.h"
#include <math.h>

#define PI 3.14159265

namespace rt3 {
real_type scalarProd(const glm::highp_vec3 &x, const glm::highp_vec3 &y)
{
    return glm::dot<3, float, glm::qualifier::highp>(x, y);
}

Sphere::Sphere(real_type radius, point3 position) : radius{radius}, position{position} 
{
    this->material = new MatSphereUV();
}


bool Sphere::intersect( const Ray& r, Surfel *sf ) const
{
    //negligible situation
    if(r.direction == point3(0, 0, 0) || radius == 0 ) { return false; }

    vec3 direct = this->position - r.origin;

    real_type direct_distance_sqr = scalarProd(direct, direct);
    real_type direct_ray_prod = scalarProd(direct, r.direction);
    point3 proj_point_vec = r.direction*(direct_ray_prod/direct_distance_sqr) - direct;

    real_type proj_dist_sqr = scalarProd(proj_point_vec, proj_point_vec);

    if(proj_dist_sqr > radius*radius) { return false; }

    real_type diffin = sqrt(radius*radius*proj_dist_sqr/direct_distance_sqr);

    sf->p = this->position + proj_point_vec - (r.direction*diffin);
    sf->n = (sf->p - r.origin) * (1/radius);
    sf->wo = -(r.direction);
    sf->uv = {  asin((sf->p[0]-this->position[0])/radius)/(4*PI) + 0.5F, 
                asin((sf->p[1]-this->position[1])/radius)/(2*PI)
            };
    
    return true;
}

bool Sphere::intersect_p( const Ray& r) const
{   
    //negligible situation
    if(r.direction == point3(0, 0, 0) || radius == 0 ) { return false; }

    vec3 direct = this->position - r.origin;

    real_type direct_distance_sqr = scalarProd(direct, direct);
    real_type direct_ray_prod = scalarProd(direct, r.direction);
    point3 proj_point_vec = r.direction*(direct_ray_prod/direct_distance_sqr) - direct;

    real_type proj_dist_sqr = scalarProd(proj_point_vec, proj_point_vec);

    //ray's projection touches sphere?
    return proj_dist_sqr <= radius*radius;
}

};
