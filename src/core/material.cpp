#include "material.h"

namespace rt3 {

    Material* create_material(const ParamSet& ps) 
    {
        std::cout << ">>> Inside create_material()\n";
        
        std::string mat_type = retrieve<std::string>(ps, "type", string{ "" });
        Spectrum color = retrieve(ps, "color", Spectrum{ 255, 255, 255 });
        
        std::cout<< "\n\n entrada \n\n";
        std::cout<< "\n\n " << mat_type << " \n\n";
        std::cout<< "\n\n " << color << " \n\n";
          
          
        // material options

        Material* m = new Material();

        if(mat_type == "flat")
        {
            m->samplingMethod = [color](const Point2f& pixel_ndc) -> Spectrum {
                return {color[0]*255, color[1]*255, color[2]*255};
            };
        }
        else if(mat_type == "uv_sphere")
        {
            m->samplingMethod = [](const Point2f& pixel_ndc) -> Spectrum {
                return {255*pixel_ndc[0], 255*pixel_ndc[1], 255*(1-pixel_ndc[1])};
            };
        }
        else
        {
            std::clog << ">>> Invalid Material Type!!!\n";
            m->samplingMethod = [](const Point2f& pixel_ndc) -> Spectrum {
                return {0, 0, 0};
            };
        }
        std::cout<< "\n\n sample dentro do create" << m->sampleUV({0, 0}) << " \n\n";
        return m;
    
    }

    [[nodiscard]] Spectrum Material::sampleUV(const Point2f& pixel_ndc) const {
        return samplingMethod(pixel_ndc);
    }
    
};
