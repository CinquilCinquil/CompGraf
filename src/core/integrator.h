#include "rt3-base.h"

namespace rt3 {

class Integrator {
	public:
		virtual ~Integrator() = default;
		Integrator() = default;
		Integrator(std::shared_ptr<Camera> cam );

		virtual void render(Scene * scene) = 0;

		std::shared_ptr<Camera> camera;
};

class SamplerIntegrator : public Integrator {
	//=== Public interface
	public:
		virtual ~SamplerIntegrator() = default;
		SamplerIntegrator() = default;

		virtual Spectrum Li( const Ray& ray, Scene * scene ) const = 0;
		virtual void render(Scene * scene) = 0;
		virtual void preprocess( const Scene& scene ) = 0;
};

class FlatIntegrator : public SamplerIntegrator {

    public:
        FlatIntegrator() {};

        Spectrum Li( const Ray& ray, Scene * scene ) const;
		void render(Scene * scene);
		void preprocess( const Scene& scene );

};

//Integrator * create_integrator(const ParamSet& ps);

Integrator * create_integrator(const ParamSet& ps);

}