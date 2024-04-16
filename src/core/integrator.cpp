#include "integrator.h"

namespace rt3 {

SamplerIntegrator::SamplerIntegrator( std::shared_ptr<const Camera> cam ) : camera{cam} {};

Spectrum FlatIntegrator::Li( const Ray& ray, const Scene& scene ) const {

    // Find closest ray intersection or return background radiance.
    Surfel isect; // Intersection information.
    if (!scene.intersect(ray, &isect)) {
        return {}; // empty object.
    }
    // Some form of determining the incoming radiance at the ray's origin.
    // Polymorphism in action.
    Material *m = dynamic_cast< FlatMaterial *>( iscet.primitive->get_material() );

    // Assign diffuse color to L.
    return m->color;

}

void FlatIntegrator::render( const Scene& scene ) {
  
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
        bool intersects = false;
        Ray ray = camera->generate_ray( j + dw/2, i + dh/2 );
        

        // TODO: chamar o LI, e dentro do LI fazer o loop com os objs

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