#ifndef SPHERE_H
#define SPHERE_H
#include "rt3-base.h"
#include "primitivel.h"

namespace rt3 {

class Sphere : public Primitive {
 private:
   real_type radius;


 public:
    Sphere(real_type radius, point3 position);
    //~Sphere() = default;

    bool intersect( const Ray& r, Surfel *sf ) const;
    bool intersect_p( const Ray& r ) const;
    real_type getRadius() {return radius;}
    point3 getPosition() {return position;}

};

}
#endif