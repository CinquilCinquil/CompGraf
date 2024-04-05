#include "rt3-base.h"
#include "primitivel.h"

namespace rt3 {

class Sphere : public Primitive {
 private:
    point3 position;
    real_type radius;

 public:
    Sphere(real_type radius, point3 position) : radius{radius}, position{position} {};

    //~Sphere() = default;

    bool intersect( const Ray& r, Surfel *sf ) const;
    bool intersect_p( const Ray& r ) const;
    real_type getRadius() {return radius;}
    point3 getPosition() {return position;}

};

}