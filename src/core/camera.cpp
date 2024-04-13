#include "camera.h"

namespace rt3 {

Camera::Camera(Point2i& vpdim, point3& frame_pos, vec3& look_from, vec3& look_at, vec3& vup,
 std::array<real_type, 4> screen_window, real_type frame_aspectratio) : frame_pos{look_from}, vup{vup}  {

    // Defining left, right, top and bottom bounds
    real_type screen_proportion = vpdim[0]/vpdim[1];
    l = -screen_proportion; r = +screen_proportion;
    b = -1.0; t = +1.0;
    nx = vpdim[0]; ny = vpdim[1];

    // screen_window was specified in file
    if (screen_window[0] != -1) {
      l = screen_window[0]; r = screen_window[1];
      b = screen_window[2]; t = screen_window[3];
    }

    // frame_aspectratio was specified in file
    if (frame_aspectratio != -1) {
      screen_proportion = frame_aspectratio;
    }

    // Defining the camera's frame
    vec3 gaze = look_at - look_from;
    vec3 w = normalize(gaze);
    vec3 u = normalize(cross(w, vup)); //vup, w
    vec3 v = normalize(cross(u, w)); //w, u

    // (vertical, forward, horizontal)

    frame_vecs = {u, v, w};

    std::cout << Spectrum{w[0], w[1], w[2]} << " " <<
     Spectrum{u[0], u[1], u[2]} << " " << Spectrum{v[0], v[1], v[2]} << " bbbbbbbbbb\n";
}

std::array<real_type, 2> Camera::xy_to_uv(int x, int y) {

    real_type u = l + (r - l)*(x + 0.5)*(1.0F/nx);
    real_type v = b + (t - b)*(y + 0.5)*(1.0F/ny);

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
  
  std::string cam_type = retrieve<std::string>(ps, "type", string{ "" });

  // camera angle and position

  vec3 lfr = retrieve(ps, "look_from", vec3{ 0, 0, 0 });
  vec3 lat = retrieve(ps, "look_at", vec3{ 0, 1, 0 });
  vec3 vup = retrieve(ps, "up", vec3{ 1, 0, 0 });
  point3 pos = point3{ 0, 0, 0 };

  // camera proportions

  Point2i vpdim = retrieve(ps, "vpdim", Point2i{1000, 1000});
  std::array<real_type, 4> scr_window = retrieve(ps, "screen_window", std::array<real_type, 4>{-1, -1, -1, -1});
  real_type frame_aspectratio = retrieve(ps, "frame_aspectratio", real_type{-1});

  std::cout << vpdim << "aaaaaaaaaaa\n";
  std::cout << scr_window << "aaaaaaaaaaa\n";
  std::cout << frame_aspectratio << "aaaaaaaaaaa\n";

  if(cam_type == "orthographic")
  {
    return new OrthographicCamera(vpdim, pos, lfr, lat, vup, scr_window, frame_aspectratio);
  }
  else
  {
    return new PerspectiveCamera(vpdim, pos, lfr, lat, vup, scr_window, frame_aspectratio);
  }
  
}

}