#include "camera.h"

namespace rt3 {

Ray PerspectiveCamera::generate_ray(int x, int y) {
    return Ray();
}

Ray OrthographicCamera::generate_ray(int x, int y) {
    return Ray();//Ray(Point3f{(float)x, (float)y, 0}, );
}

}