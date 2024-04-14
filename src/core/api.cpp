#include "api.h"

#include <chrono>
#include <memory>
#include "material.h"
#include "sphere.h"

#define PI 3.14159265

namespace rt3 {

void render(std::unique_ptr<Camera> & camera, std::unique_ptr<Scene> & scene) {

  int wi = camera->film->m_initial_points[0];
	int hi = camera->film->m_initial_points[1];
  
	int wf = camera->film->m_final_points[0];
	int hf = camera->film->m_final_points[1];

  int w = camera->film->m_full_resolution[0];
	int h = camera->film->m_full_resolution[1];

/*
  vector<std::shared_ptr<Primitive>> obj_list = {
      std::shared_ptr<Primitive>(new Sphere(1, point3{0, 0, 0}))
  };
  // left is +x
  // top is +z
  // up is +y

 
  std::shared_ptr<Sphere>(new Sphere(0.4, point3{-1, 0.5, 5})),
      std::shared_ptr<Sphere>(new Sphere(0.4, point3{1, -0.5, 8})),
      std::shared_ptr<Sphere>(new Sphere(0.4, point3{-1, -1.5 , 3.5}))

  vector<std::shared_ptr<Sphere>> obj_list;
  for (int i = 0;i < 5; i++) 
  {
    for (int j = 0;j < 5; j++) 
    {
      obj_list.push_back(std::shared_ptr<Sphere>(new Sphere(10, point3{+ j * 50,-i * 50,- j * 50})));
    }
  }
  */

  for (int i = hi;i < hf; i++) 
  {
    for (int j = wi;j < wf; j++) 
    {
      Spectrum color;
      bool intersects = false;
      int i_ = (((float) i) / ((float) h)) * camera->getNx();
      int j_ = (((float) j) / ((float) w)) * camera->getNy();
      Ray ray = camera->generate_ray( i, j );

      // Checking if ray hit an object
      for (std::shared_ptr<Primitive> p : scene->obj_list) {
        
        Surfel* sf = new Surfel(p.get());
        if (p->intersect(ray, sf)) {
          color = p->get_material()->sampleUV(sf->uv);
          intersects = true;
        }

      }

      // If the ray didnt hit any object, then sample from background
      if (!intersects) {
        float u = ((float) j) / w;
        float v = ((float) i) / h;
        color = scene->background->sampleUV({u, v});
      }

      /*
      if (i % 100 == 0 && j % 100 == 0) {
        std::cout << color << '\n';
      }
      */
      

      camera->film->pixels.push_back(color);

    }
  }
	
	camera->film->write_image();
  //delete sph;
}

//=== API's static members declaration and initialization.
API::APIState API::curr_state = APIState::Uninitialized;
RunningOptions API::curr_run_opt;
std::unique_ptr<RenderOptions> API::render_opt;
std::unique_ptr<Scene> API::curr_scene;
std::shared_ptr<Material> API::curr_material;
// GraphicsState API::curr_GS;

// THESE FUNCTIONS ARE NEEDED ONLY IN THIS SOURCE FILE (NO HEADER NECESSARY)
// ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ

Camera* API::make_camera(const std::string& name, const ParamSet& ps) {
  std::cout << ">>> Inside API::make_camera()\n";
  Camera* cam{ nullptr };
  cam = create_camera(ps);

  return cam;
}


Film* API::make_film(const std::string& name, const ParamSet& ps) {
  std::cout << ">>> Inside API::make_film()\n";
  Film* film{ nullptr };
  film = create_film(ps);

  // Return the newly created film.
  return film;
}

BackgroundColor* API::make_background(const std::string& name, const ParamSet& ps) {
  std::cout << ">>> Inside API::background()\n";
  BackgroundColor* bkg{ nullptr };
  bkg = create_color_background(ps);

  // Return the newly created background.
  return bkg;
}

// ˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆ
// END OF THE AUXILIARY FUNCTIONS
// =========================================================================

//=== API's public methods implementation
void API::init_engine(const RunningOptions& opt) {
  // Save running option sent from the main().
  curr_run_opt = opt;
  // Check current machine state.
  if (curr_state != APIState::Uninitialized) {
    RT3_ERROR("API::init_engine() has already been called! ");
  }
  // Set proper machine state
  curr_state = APIState::SetupBlock;
  // Preprare render infrastructure for a new scene.
  render_opt = std::make_unique<RenderOptions>();
  // Create a new initial GS
  // curr_GS = GraphicsState();
  RT3_MESSAGE("[1] Rendering engine initiated.\n");
}

void API::clean_up() {
  // Check for correct machine state
  if (curr_state == APIState::Uninitialized) {
    RT3_ERROR("API::clean_up() called before engine initialization.");
  } else if (curr_state == APIState::WorldBlock) {
    RT3_ERROR("API::clean_up() called inside world definition section.");
  }
  curr_state = APIState::Uninitialized;

  RT3_MESSAGE("[4] Rendering engine clean up concluded. Shutting down...\n");
}

void API::run() {
  // Try to load and parse the scene from a file.
  RT3_MESSAGE("[2] Beginning scene file parsing...\n");
  // Recall that the file name comes from the running option struct.
  parse(curr_run_opt.filename.c_str());
}

void API::world_begin() {
  VERIFY_SETUP_BLOCK("API::world_begin");  // check for correct machine state.
  curr_state = APIState::WorldBlock;       // correct machine state.
  curr_scene = std::make_unique<Scene>();
}

void API::world_end() {
	
  VERIFY_WORLD_BLOCK("API::world_end");
  // The scene has been properly set up and the scene has
  // already been parsed. It's time to render the scene.

  // At this point, we have the background as a solitary pointer here.
  // In the future, the background will be parte of the scene object.
  curr_scene->background = make_background(render_opt->bkg_type, render_opt->bkg_ps) ;
  // Same with the film, that later on will belong to a camera object.
  std::unique_ptr<Camera> the_camera{ make_camera (render_opt->camera_type, render_opt->camera_ps) };
  the_camera->film = make_film(render_opt->film_type, render_opt->film_ps) ;

  // Run only if we got film and background.
  if (the_camera and curr_scene->background) {
    RT3_MESSAGE("    Parsing scene successfuly done!\n");
    RT3_MESSAGE("[2] Starting ray tracing progress.\n");

    // Structure biding, c++17.
    auto res = the_camera->film->get_resolution();
    size_t w = res[0];
    size_t h = res[1];
    RT3_MESSAGE("    Image dimensions in pixels (W x H): " + std::to_string(w) + " x "
                + std::to_string(h) + ".\n");
    RT3_MESSAGE("    Ray tracing is usually a slow process, please be patient: \n");

    //================================================================================
    auto start = std::chrono::steady_clock::now();
	
    render(the_camera, curr_scene);  // TODO: This is the ray tracer's  main loop.
	
    auto end = std::chrono::steady_clock::now();
    //================================================================================
    auto diff = end - start;  // Store the time difference between start and end
    // Seconds
    auto diff_sec = std::chrono::duration_cast<std::chrono::seconds>(diff);
    RT3_MESSAGE("    Time elapsed: " + std::to_string(diff_sec.count()) + " seconds ("
                + std::to_string(std::chrono::duration<double, std::milli>(diff).count())
                + " ms) \n");
  }
  // [4] Basic clean up
  curr_state = APIState::SetupBlock;  // correct machine state.
  reset_engine();
}

/// This api function is called when we need to re-render the *same* scene (i.e.
/// objects, lights, materials, etc) , maybe with different integrator, and
/// camera setup. Hard reset on the engine. User needs to setup all entities,
/// such as camera, integrator, accelerator, etc.
void API::reset_engine() {
  // curr_GS = GraphicsState();
  // This will delete all information on integrator, cameras, filters,
  // acceleration structures, etc., that has been set previously.
  render_opt = std::make_unique<RenderOptions>();
}

void API::background(const ParamSet& ps) {
  std::cout << ">>> Inside API::background()\n";
  VERIFY_WORLD_BLOCK("API::background");

  // retrieve type from ps.
  std::string type = retrieve(ps, "type", string{ "unknown" });
  render_opt->bkg_type = type;
  // Store current background object.
  render_opt->bkg_ps = ps;
}

void API::film(const ParamSet& ps) {
  std::cout << ">>> Inside API::film()\n";
  VERIFY_SETUP_BLOCK("API::film");

  // retrieve type from ps.
  std::string type = retrieve(ps, "type", string{ "unknown" });
  render_opt->film_type = type;
  render_opt->film_ps = ps;
}

void API::camera(const ParamSet& ps) {
  std::cout << ">>> Inside API::camera()\n";
  VERIFY_SETUP_BLOCK("API::camera");

  // retrieve type from ps.
  std::string type = retrieve(ps, "type", string{ "unknown" });
  render_opt->camera_type = type;
  render_opt->camera_ps = ps;
}

void API::material(const ParamSet& ps) {
  std::cout << ">>> Inside API::material()\n";
  VERIFY_WORLD_BLOCK("API::material");

  curr_material = shared_ptr<Material>(create_material(ps));
}

void API::object(const ParamSet& ps) 
{
  std::cout << ">>> Inside API::object()\n";
  VERIFY_WORLD_BLOCK("API::object");
  std::string type = retrieve(ps, "type", string{ "unknown" });

  std::cout<<"\n\n Api \n\n";
  if(type == "sphere")
  {
    Sphere* s = new Sphere(retrieve<real_type>(ps, "radius", 0), retrieve<Point3f>(ps, "position", Point3f{0,0,0}));
    s->material = curr_material.get();
    curr_scene->obj_list.push_back(shared_ptr<Sphere>(s));

    std::cout<<"\n\n sphere \n\n";
  }
  
}

}  // namespace rt3
