#ifndef RAY_H
#define RAY_H

namespace rt3 {

class Ray {
    public:
        Ray (const Point3f& o, const Vector3f& d,
            real_type start=0, real_type end=INFINITY) : o{o}, d{d}, t_min{start}, t_max{end} {/*empty*/}

        Ray() : t_min{0.f}, t_max{INFINITY}{/*empty*/}
    private:
        Point3f o; //!< origin
        Vector3f d; //!< direction
        mutable real_type t_min, t_max; //!< parameters

        // ... the methods goes here

        // TODO!!!
        //Point3f operator()(real_type t) const { return Point3f();/*return o + d * t*/ } // uses scalar product
};

}

#endif