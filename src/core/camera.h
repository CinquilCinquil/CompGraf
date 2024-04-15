#ifndef CAMERA_H
#define CAMERA_H

#include "rt3-base.h"
#include "ray.h"

namespace rt3 {

class Camera{
protected:
	point3 frame_pos = {0, 0, 0};
	std::array<vec3, 3> frame_vecs = {vec3(), vec3(), vec3()};
	vec3 vup = {0, 1, 0};
	float l, r, b, t;
	int nx, ny;

	std::array<float, 2> xy_to_uv(int x, int y); // converts pixel coords to screen-space coords

public:

	Film* film;
	bool get_res_from_film = false;

	void setResFromFilm();

	Camera(Point2i& vpdim, point3& frame_pos, vec3& look_from, vec3& look_at, vec3& vup,
	 vector<real_type> screen_window, real_type frame_aspectratio, real_type fovy);

    virtual Ray generate_ray(int x, int y) = 0;

	int getNx() {return nx;};
	int getNy() {return ny;};
	void setNx(int w) {nx = w;};
	void setNy(int h) {ny = h;};

};
class PerspectiveCamera : public Camera {
	using Camera::Camera;
	Ray generate_ray(int x, int y);
};
class OrthographicCamera : public Camera {
	using Camera::Camera;
	Ray generate_ray(int x, int y);
};

// Factory pattern. It's not part of these classes.
Camera* create_camera(const ParamSet& ps);

}

#endif