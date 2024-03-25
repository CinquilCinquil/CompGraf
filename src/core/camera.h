#ifndef CAMERA_H
#define CAMERA_H

#include "rt3-base.h"

namespace rt3 {

class Camera{
private:
	point3 frame_pos = {0, 0, 0};
	std::array<vec3, 3> frame_vecs = {vec3(), vec3(), vec3()};
	vec3 vup = {0, 1, 0};

public:
	Camera(point3 frame_pos, vec3 look_from, vec3 look_at);

    virtual Ray generate_ray(int x, int y) = 0;
};
class PerspectiveCamera : public Camera {
	Ray generate_ray(int x, int y);
};
class OrthographicCamera : public Camera {
	Ray generate_ray(int x, int y);
};


}

#endif