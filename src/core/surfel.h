#include "rt3-base.h"

namespace rt3 {

class Surfel{
	public:
		Surfel( const point3&p,const vec3&n, const vec3&wo, float time,
			const Point2f& uv, const Primitive *pri )
			: p{p}, n{n}, wo{wo}, time{time}, uv{uv}, primitive{pri}
            {/* empty */};

		point3 p;     //!< Contact point.
		vec3 n;       //!< The surface normal.
		vec3 wo;      //!< Outgoing direction of light, which is -ray.
		Point2f uv;   //!< Parametric coordinate (u,v) of the hit surface.
        float time;
		const Primitive *primitive=nullptr; //!< Pointer to the primitive.
};

}