#ifndef Surface_hpp
#define Surface_hpp

#include <atlas/utils/Geometry.hpp>

class Surface : public atlas::utils::Geometry
{
    public:

        Surface();

        void renderGeometry(atlas::math::Matrix4 const &projection, atlas::math::Matrix4 const &view) override;    
                        
    private:
        
        GLuint mVao;
        GLuint mPositionBuffer, mNormalBuffer, mColorBuffer, mTextureCoordBuffer;   
        
        // GLfloat *mVertexPositions, *mVertexNormals, *mVertexColors;
        static const GLfloat vertexPositions[][3], vertexNormals[][3], vertexColors[][3], textureCoordinates[][2];
};

#endif