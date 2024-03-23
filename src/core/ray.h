#ifndef RAY_H
#define RAY_H

namespace rt3 {

/*
class Ray {
    public:
        Ray (const Point3& o, const Vector3& d,
            real_type start=0, real_type end=INFINITY) : o{o}, d{d},
            t_min{start}, t_max{end} {/*empty*/ //}
    /*
        Ray() : t_min{0.f}, t_max{INFINITY}{/*empty*/ //}
    /*
    private:
        Point3 o; //!< origin
        Vector3 d: //!< direction
        mutable real_type t_min, t_max //!< parameters
        // ... the methods goes here
        Point3 operator()(real_type t) const { return o + d * t; }
    */
//};

/*
Ray r{ Point3{0,0,0}, Vector3{3,2,-4} }; // creating an infinity ray.
float t{2.3};                            // a parameter for the ray.
Point3 p = r( t );                       // overloading operator()(), so that it returns o + t*d.

*/

}

#endif