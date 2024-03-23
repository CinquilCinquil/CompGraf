#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"

namespace rt3 {

class Camera{
        //Ray generate_ray(int x, int y) = 0;
};
class PerspectiveCamera : public Camera {
	// ...
};
class OrthographicCamera : public Camera {
	// ...
};


}

#endif