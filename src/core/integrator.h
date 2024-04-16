#include "rt3-base.h"

namespace rt3 {

class Integrator {
	public:
		virtual ~Integrator() = default;
		virtual void render( const Scene& scene ) = 0;
};

class SamplerIntegrator : public Integrator {
	//=== Public interface
	public:
		virtual ~SamplerIntegrator() = default;
		SamplerIntegrator(std::shared_ptr<const Camera> cam );

		virtual Color24 Li( const Ray& ray, const Scene& scene ) const = 0;
		virtual void render( const Scene& scene ) = 0;
		virtual void preprocess( const Scene& scene ) = 0;

	protected:
		std::shared_ptr<const Camera> camera;
};

class FlatIntegrator : public SamplerIntegrator {

    public:
        FlatIntegrator() = default;

        Spectrum Li( const Ray& ray, const Scene& scene ) const;
		void render( const Scene& scene );
		void preprocess( const Scene& scene );

};

}