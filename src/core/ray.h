#ifndef __RAY_H__
#define __RAY_H__

#include "rt3.h"

namespace rt3 {
    class Ray {
        public:
            Ray (const point3& o, const vec3& d, real_type start=0, real_type end=INFINITY);
            Ray();
        
            point3 origin; //!< origin
            vec3 direction; //!< direction
            mutable real_type t_min, t_max; //!< parameters
        
            point3 operator()(real_type t) const;
    };
}

#endif