#include "camera.h"

namespace rt3 {

Camera::Camera(Point2i& vpdim, point3& frame_pos, vec3& look_from, vec3& look_at, vec3& vup,
 vector<real_type> screen_window, real_type frame_aspectratio) : frame_pos{look_from}, vup{vup}  {

    real_type screen_proportion = vpdim[0]/vpdim[1];

    // frame_aspectratio was specified in file
    if (frame_aspectratio != -1) {
      screen_proportion = frame_aspectratio;
    }

    // Defining left, right, top and bottom bounds
    l = -screen_proportion; r = +screen_proportion;
    b = -1.0; t = +1.0;
    nx = vpdim[0]; ny = vpdim[1];

    // screen_window was specified in file
    if (screen_window[0] != -1) {
      l = screen_window[0]; r = screen_window[1];
      b = screen_window[2]; t = screen_window[3];
      get_res_from_film = true;
    }

    // Defining the camera's frame
    vec3 gaze = look_at - look_from;
    vec3 w = normalize(gaze);
    vec3 u = normalize(cross(vup, w)); //vup, w
    vec3 v = normalize(cross(w, u)); //w, u

    // (vertical, forward, horizontal)

    frame_vecs = {u, v, w};

    std::cout << Spectrum{w[0], w[1], w[2]} << " " <<
     Spectrum{u[0], u[1], u[2]} << " " << Spectrum{v[0], v[1], v[2]} << " bbbbbbbbbb\n";
}

void Camera::setResFromFilm() {
  setNx(film->m_full_resolution[0]);
  setNy(film->m_full_resolution[1]);
}

std::array<real_type, 2> Camera::xy_to_uv(int x, int y) {

    real_type u = l + (r - l)*(x + 0.5)*(1.0F/((float)nx));
    real_type v = b + (t - b)*(y + 0.5)*(1.0F/((float)ny));

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
  vec3 lat = retrieve(ps, "look_at", vec3{ 0, 0, 10 });
  vec3 vup = retrieve(ps, "up", vec3{ 0, 1, 0 });
  point3 pos = point3{ 0, 0, 0 };

  // camera proportions

  Point2i vpdim = retrieve(ps, "vpdim", Point2i{800, 600});
  vector<real_type> scr_window = retrieve(ps, "screen_window", vector<real_type>{-1, -1, -1, -1});
  real_type frame_aspectratio = retrieve(ps, "frame_aspectratio", real_type{-1});
  real_type fovy = retrieve(ps, "fovy", real_type{-1}); // NOT BEING USED

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