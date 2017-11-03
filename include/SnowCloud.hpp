#ifndef SnowCloud_hpp
#define SnowCloud_hpp

#include <atlas/utils/Geometry.hpp>
#include <random>

class SnowCloud : public atlas::utils::Geometry
{
    public:

        SnowCloud();

        // void computeGeometry(atlas::core::Time<> const &t);

        void updateGeometry(atlas::core::Time<> const &t) override;
        // void renderGeometry(atlas::math::Matrix4 const &projection, atlas::math::Matrix4 const &view) override;       
                
        void setBoundingBox(glm::vec3 const &a, glm::vec3 const &b);
        
        
    private:

        glm::vec3 mBoundingBoxA, mBoundingBoxB;

        std::default_random_engine mGenerator;
        std::uniform_real_distribution<float> mUniformDistributionX, mUniformDistributionY, mUniformDistributionZ;
        std::uniform_real_distribution<float> mUniformDistributionVector, mUniformDistributionAngle;        

        float mSnowFlakeRate;
        
              
};

#endif