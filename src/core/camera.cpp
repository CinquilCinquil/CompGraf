#include "camera.h"

namespace rt3 {

Camera::Camera(Point2i& size, point3& frame_pos, vec3& look_from, vec3& look_at, vec3& vup) : frame_pos{look_from}, vup{vup} {

    // Defining left, right, top and bottom bounds
    float screen_proportion = size[0]/size[1];
    l = -screen_proportion; r = +screen_proportion;
    t = +1.0; b = -1.0;
    nx = size[0]; ny = size[1];

    // Defining the camera's frame
    vec3 gaze = look_at - look_from;
    vec3 w = normalize(gaze);
    vec3 u = normalize(cross(w, vup)); //vup, w
    vec3 v = normalize(cross(u, w)); //w, u

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

Camera* create_camera(const ParamSet& ps) {
  std::cout << ">>> Inside create_camera()\n";
  
  vec3 lfr = retrieve(ps, "look_from", vec3{ 0, 0, 0 }); /// look down
  vec3 lat = retrieve(ps, "look_at", vec3{ 0, 10, 0 });
  vec3 vup = retrieve(ps, "up", vec3{ 1, 0, 0 });
  point3 pos = point3{ 0, 0, 0 };
  Point2i size = retrieve(ps, "vpdim", Point2i{16, 9});
  std::string cam_type = retrieve<std::string>(ps, "type", string{ "" });

  if(cam_type == "orthographic")
  {
    return new OrthographicCamera(size, pos, lfr, lat, vup);
  }
  else
  {
    return new PerspectiveCamera(size, pos, lfr, lat, vup);
  }
  
}

}