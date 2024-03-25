#include "camera.h"

namespace rt3 {

Camera::Camera(point3 frame_pos, vec3 look_from, vec3 look_at) : frame_pos{frame_pos} {
    vec3 gaze = look_at - look_from;
    vec3 w = normalize(gaze);
    vec3 u = normalize(cross(vup, w));
    vec3 v = normalize(cross(w, u));

    frame_vecs = {u, v, w};
}

Ray PerspectiveCamera::generate_ray(int x, int y) {
    return Ray();
}

Ray OrthographicCamera::generate_ray(int x, int y) {
    return Ray();//Ray(Point3f{(float)x, (float)y, 0}, );
}

}