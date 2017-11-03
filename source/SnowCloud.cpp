#include "SnowCloud.hpp"
#include "SnowFlake.hpp"
#include "SnowScene.hpp"
#include "Shader.hpp"
#include <iostream>
#include <atlas/utils/Application.hpp>



SnowCloud::SnowCloud() :
    mSnowFlakeRate(10.0f)
{    
    mUniformDistributionVector = std::uniform_real_distribution<float>(0.0f, 1.0f);
    mUniformDistributionAngle = std::uniform_real_distribution<float>(0.0f, (float) (2.0*M_PI));
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
        
        float vectorTheta = acos(2.0*mUniformDistributionVector(mGenerator) - 1);
        float vectorPhi = 2.0 * M_PI * mUniformDistributionVector(mGenerator);  
        
        glm::vec3 rotationVector = glm::vec3(sin(vectorTheta)*cos(vectorPhi), sin(vectorTheta)*sin(vectorPhi), cos(vectorTheta));
    
        float angleTheta = mUniformDistributionAngle(mGenerator);
    
        snowflake->setRotation(glm::rotate(glm::mat4(1.0f), angleTheta, rotationVector));
        
        ((SnowScene *) atlas::utils::Application::getInstance().getCurrentScene())->addSnowFlake(std::move(snowflake));        
    }
}