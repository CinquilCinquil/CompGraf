#include <vector>
#include "structures.h"

namespace CG
{
    class Background {
        private:

            RGB corners[4] = {RGB(), RGB(), RGB(), RGB()};

            enum corners_e {bl = 0, tl, tr, br};
            float lerp(float a, float b, float t) const;
            RGB lerp(const RGB &A, const RGB &B, float t) const;

        public:
            Background(const std::vector<RGB>& colors);
            ~Background();
            RGB sampleUV(float u, float v) const;
    };
}

