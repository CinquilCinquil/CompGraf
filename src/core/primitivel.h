#ifndef PRIMITIVE_H
#define PRIMITIVE_H


#include "rt3.h"
#include "ray.h"
#include "material.h"

namespace rt3 {

class Surfel;

class Primitive {
	public:
		virtual ~Primitive() {};
		
		virtual bool intersect( const Ray& r, Surfel *sf ) const = 0;

		// Simpler & faster version of intersection that only return true/false.
		// It does not compute the hit point information.
		virtual bool intersect_p( const Ray& r ) const = 0;

		const Material * get_material(void) const { return material; }

		Material * material;
		Point3f position = {0,0,0};
};

class Surfel {
	public:
		Surfel( const point3&p,const vec3&n, const vec3&wo, float time,
			const Point2f& uv, const Primitive *pri )
			: p{p}, n{n}, wo{wo}, time{time}, uv{uv}, primitive{pri}
            {};

		Surfel(const Primitive *pri ) : primitive{pri} {};

		point3 p;     //!< Contact point.
		vec3 n;       //!< The surface normal.
		vec3 wo;      //!< Outgoing direction of light, which is -ray.
		Point2f uv;   //!< Parametric coordinate (u,v) of the hit surface.
        float time;
		const Primitive *primitive=nullptr; //!< Pointer to the primitive.
};

}

#endif