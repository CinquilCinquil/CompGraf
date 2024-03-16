#ifndef API_H
#define API_H 1

#include <string>

#include "paramset.h"
#include "rt3.h"

//=== API Macro definitions

/// Check whether the current state has been intialized.
#define VERIFY_INITIALIZED(func_name)                                          \
  if (curr_state == APIState::Uninitialized) {                                 \
    RT3_ERROR(std::string{"API::init() must be called before "} +              \
              std::string{func_name} + std::string{"(). Ignoring."});          \
  }

/// Check whether the current state corresponds to setup section.
#define VERIFY_SETUP_BLOCK(func_name)                                          \
  VERIFY_INITIALIZED(func_name)                                                \
  if (curr_state == APIState::WorldBlock) {                                    \
    RT3_ERROR(std::string{"Rendering setup cannot happen inside "} +           \
              std::string{"World definition block; "} +                        \
              std::string{func_name} +                                         \
              std::string{"() not allowed. Ignoring"});                        \
  }

/// Check whether the current state corresponds to the world section.
#define VERIFY_WORLD_BLOCK(func_name)                                          \
  VERIFY_INITIALIZED(func_name)                                                \
  if (curr_state == APIState::SetupBlock) {                                    \
    RT3_ERROR(std::string{"Scene description must happen inside "} +           \
              std::string{"World Definition block; "} +                        \
              std::string{func_name} +                                         \
              std::string{"() not allowed. Ignoring"});                        \
  }

namespace rt3 {
/// Collection of objects and diretives that control rendering, such as camera,
/// lights, prims.
struct RenderOptions {
  // the Film
  std::string film_type{"image"}; // The only type available.
  ParamSet film_ps;
  /// the Camera
  string camera_type{"perspective"};
  ParamSet camera_ps;
  /// the Bakcground
  string bkg_type{"solid"}; // "image", "interpolated"
  ParamSet bkg_ps;
};

/// Collection of data related to a Graphics state, such as current material,
/// lib of material, etc.
struct GraphicsState {
  // Not necessary in Project 01 through Project 07.
};

/// Static class that manages the render process
class API {
public:
  /// Defines the current state the API may be at a given time
  enum class APIState {
    Uninitialized, //!< Initial state, before parsing.
    SetupBlock,    //!< Parsing the render setup section.
    WorldBlock     //!< Parsing the world's information section.
  };

  /// Stores the running options collect in main().
  static RunningOptions curr_run_opt;

private:
  /// Current API state
  static APIState curr_state;
  /*
   * The unique pointer below is useful to support various calls to
   * init()-run()-clean-up(), in case we want to process several input files in
   * a single run of the system.
   */
  /// Unique infrastructure to render a scene (camera, integrator, etc.).
  static std::unique_ptr<RenderOptions> render_opt;
  // [NO NECESSARY IN THIS PROJECT]
  // /// The current GraphicsState
  // static GraphicsState curr_GS;
  // [NOT NECESSARY IN THIS PROJECT]
  // /// Pointer to the scene. We keep it as parte of the API because it may be
  // reused later [1] Create the integrator. static unique_ptr< Scene >
  // the_scene;

  // === Helper functions.
  ///
  static Film *make_film(const string &name, const ParamSet &ps);
  static Background *make_background(const string &name, const ParamSet &ps);
  static Camera *make_camera(const string &name, const ParamSet &ps);

public:
  //=== API function begins here.
  static void init_engine(const RunningOptions &);
  static void run();
  static void clean_up();
  static void reset_engine();

  static void film(const ParamSet &ps);
  static void camera(const ParamSet &ps);
  static void background(const ParamSet &ps);
  static void world_begin();
  static void world_end();
};
} // namespace rt3

#endif // API_H
