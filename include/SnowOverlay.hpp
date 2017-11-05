#ifndef SnowOverlay_hpp
#define SnowOverlay_hpp

#include <atlas/utils/Geometry.hpp>

class SnowOverlay : public atlas::utils::Geometry
{
    public:

        SnowOverlay();

        void renderGeometry(atlas::math::Matrix4 const &projection, atlas::math::Matrix4 const &view) override;    
                        
    private:
        
        GLuint mVao;
        GLuint mPositionBuffer, mColorBuffer;   
        
        int mNumVertices;
};

#endif