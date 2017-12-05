#ifndef SnowCloud_hpp
#define SnowCloud_hpp

#include <atlas/utils/Geometry.hpp>
#include <random>

class SnowCloud : public atlas::utils::Geometry
{
    public:

        SnowCloud();

        void updateGeometry(atlas::core::Time<> const &t) override;
        void drawGui();
                
        void setBoundingBox(glm::vec3 const &a, glm::vec3 const &b);
        
    private:

        glm::vec3 mBoundingBoxA, mBoundingBoxB;

        std::default_random_engine mGenerator;
        std::uniform_real_distribution<float> mUniformDistributionX, mUniformDistributionY, mUniformDistributionZ;
        std::uniform_real_distribution<float> mUniformDistributionVector, mUniformDistributionAngle;        

        int mSnowFlakeRate;
        
        float mStockpiledSnowflakes;
              
};

#endif