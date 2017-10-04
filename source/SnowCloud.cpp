#include "SnowCloud.hpp"
#include "SnowFlake.hpp"
#include "SnowScene.hpp"
#include "Shader.hpp"
#include <iostream>
#include <atlas/utils/Application.hpp>



SnowCloud::SnowCloud() :
    mSnowFlakeRate(2.0f)
{    
}



void SnowCloud::setBoundingBox(glm::vec3 const &a, glm::vec3 const &b)
{
    mBoundingBoxA = a;
    mBoundingBoxB = b;

    mUniformDistributionX = a.x <= b.x ? std::uniform_real_distribution<float>(a.x, b.x) : std::uniform_real_distribution<float>(b.x, a.x);
    mUniformDistributionY = a.y <= b.y ? std::uniform_real_distribution<float>(a.y, b.y) : std::uniform_real_distribution<float>(b.y, a.y);       
    mUniformDistributionZ = a.z <= b.z ? std::uniform_real_distribution<float>(a.z, b.z) : std::uniform_real_distribution<float>(b.z, a.z);         
}

void SnowCloud::updateGeometry(atlas::core::Time<> const &t)
{
    for(int i = 0; i < mSnowFlakeRate; ++i)
    {
        std::unique_ptr<SnowFlake> snowflake = std::make_unique<SnowFlake>();
        snowflake->setPosition(glm::vec3(mUniformDistributionX(mGenerator), mUniformDistributionY(mGenerator), mUniformDistributionZ(mGenerator)));
        snowflake->setVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
        
        ((SnowScene *) atlas::utils::Application::getInstance().getCurrentScene())->addSnowFlake(std::move(snowflake));        
    }

}