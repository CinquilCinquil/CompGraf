#include "camera.h"

namespace rt3 {

Camera::Camera(Point2i size, point3 frame_pos, vec3 look_from, vec3 look_at) : frame_pos{frame_pos} {

    // Defining left, right, top and bottom bounds
    float screen_proportion = size[0]/size[1];
    l = -screen_proportion; r = +screen_proportion;
    t = +1.0; b = -1.0;
    nx = size[0]; ny = size[1];

    // Defining the camera's frame
    vec3 gaze = look_at - look_from;
    vec3 w = normalize(gaze);
    vec3 u = normalize(cross(vup, w));
    vec3 v = normalize(cross(w, u));

    frame_vecs = {u, v, w};
}

std::array<float, 2> Camera::xy_to_uv(int x, int y) {

    float u = l + (r - l)*(x + 0.5)*(1.0F/nx);
    float v = b + (t - b)*(y + 0.5)*(1.0F/ny);

    return {u, v};
}

Ray PerspectiveCamera::generate_ray(int x, int y) {

    std::array<float, 2> uv = xy_to_uv(x, y);
    float u = uv[0];
    float v = uv[1];

    return Ray(frame_pos, u*frame_vecs[0] + v*frame_vecs[1] + 1.0F*frame_vecs[2]);
}

Ray OrthographicCamera::generate_ray(int x, int y) {

    std::array<float, 2> uv = xy_to_uv(x, y);
    float u = uv[0];
    float v = uv[1];

    return Ray(frame_pos + u*frame_vecs[0] + v*frame_vecs[1], frame_vecs[2]);
}

}