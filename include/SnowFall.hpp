#ifndef SnowFall_hpp
#define SnowFall_hpp

#include "SnowFlake.hpp"
#include <atlas/utils/Geometry.hpp>
#include <vector>

class SnowFall : public atlas::utils::Geometry
{
    public:

        SnowFall();
        ~SnowFall();        

        void updateGeometry(atlas::core::Time<> const &t) override;        
        void renderGeometry(atlas::math::Matrix4 const &projection, atlas::math::Matrix4 const &view) override;    
        
        void addSnowFlake(std::unique_ptr<SnowFlake> snowflake); 
        GLuint getSnowDepthTexture() const;       
                
    private:

        int mSnowMapResolution;
        GLuint mSnowFBO;
        GLuint mSnowDepthTexture;
        
        GLuint mVao;
        GLuint mPositionBuffer, mColorBuffer, mIndexBuffer;        
        
        std::vector<glm::vec3> mVertexPositions, mVertexColors;
        std::vector<GLint> mVertexIndices;

        std::vector<std::unique_ptr<SnowFlake>> mSnowFlakes;                 
        
};

#endif