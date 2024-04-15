#ifndef SCENE_H
#define SCENE_H

//#include "rt3-base.h"
#include "primitivel.h"
#include "background.h"
#include "ray.h"

#include <vector>

namespace rt3 {

class Scene {
    //=== Public data
    public:
        //std::vector<shared_ptr<Light>> lights; // list of lights
        BackgroundColor* background; // The background object.
		point3 position;
		vector<std::shared_ptr<Primitive>> obj_list;
        std::shared_ptr<Primitive> aggregate; // The scene graph of objects, acceleration structure.

    //=== Public interface
    public:
		Scene();
        /// Determines the intersection info; return true if there is an intersection.
        bool intersect( const Ray& r, Surfel *isect ) const;
        /*! A faster version that only determines whether there is an intersection or not;
         * it doesn't calculate the intersection info.
         */
        bool intersect_p( const Ray& r ) const;
};

}
#endif