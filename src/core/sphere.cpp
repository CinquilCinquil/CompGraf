
#include "sphere.h"
#include <math.h>

namespace rt3 {
real_type scalarProd(const glm::highp_vec3 &x, const glm::highp_vec3 &y)
{
    return glm::dot<3, float, glm::qualifier::highp>(x, y);
}

Sphere::Sphere(real_type radius, point3 position) : radius{radius}
{
    this->position = position; 
}


bool Sphere::intersect( const Ray& r, Surfel *sf ) const
{
    //negligible situation
    if(r.direction == point3(0, 0, 0) || radius == 0 ) { return false; }

    vec3 direct = this->position - r.origin;

    // not pointing towards the same general direction
    if (scalarProd(direct, r.direction) < 0) {
        return false;
    }

    real_type direct_distance_sqr = scalarProd(direct, direct);
    real_type direct_ray_prod = scalarProd(direct, r.direction);
    point3 proj_point_vec = r.direction * (direct_ray_prod/scalarProd(r.direction, r.direction)) - direct;

    real_type proj_dist_sqr = scalarProd(proj_point_vec, proj_point_vec);

    if(proj_dist_sqr > radius*radius) { return false; }

    real_type diffin = sqrt(radius*radius*proj_dist_sqr/direct_distance_sqr);

    proj_point_vec -= r.direction*diffin;
    sf->p = this->position + proj_point_vec;
    sf->n = (sf->p - r.origin) * (1/radius);
    sf->wo = -(r.direction);

    auto a = proj_point_vec[0]/radius;
    auto b = proj_point_vec[1]/sqrt(proj_point_vec[1]*proj_point_vec[1] + proj_point_vec[2]*proj_point_vec[2]);
       
    sf->uv = {  a/2 + 1, (b+sign(proj_point_vec[2]))/4 + 1};

    //std::cout<<"UV: " << sf->uv[0] << ", " << sf->uv[1] << std::endl;
    
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
