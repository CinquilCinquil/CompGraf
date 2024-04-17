#include "api.h"

namespace rt3 {

Integrator::Integrator(std::shared_ptr<Camera> cam) : camera{cam} {}

Spectrum FlatIntegrator::Li(const Ray& ray, Scene * scene) const {

    // Find closest ray intersection or return background radiance.
    Surfel isect; // Intersection information.
    if (!(scene->intersect(ray, &isect))) {
        return Spectrum{-1, -1, -1}; // empty object.
    }
    // Some form of determining the incoming radiance at the ray's origin.
    // Polymorphism in action.
    const Material *m = isect.primitive->get_material();

    // Assign diffuse color to L.
    return m->sampleUV(isect.uv);

}

void FlatIntegrator::render(Scene * scene) {

    int wi = camera->film->m_initial_points[0];
	int hi = camera->film->m_initial_points[1];
  
	int wf = camera->film->m_final_points[0];
	int hf = camera->film->m_final_points[1];

    int w = camera->film->m_full_resolution[0];
	int h = camera->film->m_full_resolution[1];

    for (int i = hi;i < hf; i++) 
    {
    for (int j = wi;j < wf; j++) 
    {
        int dw = camera->getNx() - w;
        int dh = camera->getNy() - h;

        Spectrum color;
        Ray ray = camera->generate_ray( j + dw/2, i + dh/2 );

        color = Li(ray, scene);

        // If the ray didnt hit any object, then sample from background
        if (color[0] < 0) {
            float u = ((float) j) / w;
            float v = ((float) i) / h;
            color = scene->background->sampleUV({u, v});
        }

        camera->film->pixels.push_back(color);
    }
    }
	
	camera->film->write_image();

}

void FlatIntegrator::preprocess( const Scene& scene ) {

}

Integrator * create_integrator(const ParamSet& ps) {

    std::cout << ">>> Inside create_integrator()\n";
        
    std::string type = retrieve<std::string>(ps, "type", string{ "" });

    if (type == "flat") {
        return new FlatIntegrator();
    }
    else {
        return new FlatIntegrator();
    }

}

}