#ifndef RAY_H
#define RAY_H

#include "rt3-base.h"

namespace rt3 {

class Ray {
    public:
        Ray (const point3& o, const vec3& d,
            real_type start=0, real_type end=INFINITY) : o{o}, d{d}, t_min{start}, t_max{end} {/*empty*/}

        Ray() : t_min{0.f}, t_max{INFINITY}{/*empty*/}
    private:
        point3 o; //!< origin
        vec3 d; //!< direction
        mutable real_type t_min, t_max; //!< parameters

        // ... the methods goes here

        // TODO!!!
        //point3 operator()(real_type t) const { return point3();/*return o + d * t*/ } // uses scalar product
};

}

#endif