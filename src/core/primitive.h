//#include <memory>

#include "rt3-base.h"

namespace rt3 {

class Primitive {
	public:
		virtual ~Primitive();
		virtual bool intersect( const Ray& r, Surfel *sf ) const = 0;
		// Simpler & faster version of intersection that only return true/false.
		// It does not compute the hit point information.
		virtual bool intersect_p( const Ray& r ) const = 0;
		//virtual const Material * get_material(void) const = { return material; }
	private:
		//std::shared_ptr<Material> material;
};

}