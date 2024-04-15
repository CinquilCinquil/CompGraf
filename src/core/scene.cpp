#include "scene.h"
#include "sphere.h"

namespace rt3 {

    Scene::Scene()
    {
        Point3f origin{0,0,0};
        real_type rsqr = 0;
        Point3f last_p = origin;
        //stub
        for(std::shared_ptr<Primitive> p : this->obj_list) {
            origin += p->position;
        }
        origin = origin * (1/(float)obj_list.size());
        this->aggregate = std::shared_ptr<Sphere>(new Sphere(1, origin)); //1 is stub a magic number;
    }

    /// Determines the intersection info; return true if there is an intersection.
    bool Scene::intersect( const Ray& r, Surfel *isect ) const { return this->aggregate->intersect(r, isect); }
    /*! A faster version that only determines whether there is an intersection or not;
        * it doesn't calculate the intersection info.
        */
    bool Scene::intersect_p( const Ray& r ) const { return this->aggregate->intersect_p(r); }

}