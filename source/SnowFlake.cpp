#include "SnowFlake.hpp"
#include "Shader.hpp"
#include "SnowScene.hpp"
#include <atlas/utils/Application.hpp>

int SnowFlake::snowFlakeCount = 0;

SnowFlake::SnowFlake() :
    mMass(0.0001)
{
    ++(SnowFlake::snowFlakeCount);
    
    mNormalDistribution = std::normal_distribution<float>(0.0f, mMass*20.0f);    
    mUniformDistribution = std::uniform_real_distribution<float>(0.0f, (float) (2.0*M_PI));
}

SnowFlake::~SnowFlake()
{
    ((SnowScene *) atlas::utils::Application::getInstance().getCurrentScene())->getSnowOverlay().updateVertexNearestTo(mPosition);            
    --(SnowFlake::snowFlakeCount);    
}

void SnowFlake::setMass(float mass)
{
    mMass = mass;
}

float SnowFlake::getMass() const
{
    return mMass;
}

void SnowFlake::setPosition(glm::vec3 const &position)
{
    mPosition = position;
}

glm::vec3 SnowFlake::getPosition() const
{
    return mPosition;
}

void SnowFlake::setVelocity(glm::vec3 const &velocity)
{
    mVelocity = velocity;
}

glm::vec3 SnowFlake::getVelocity() const
{
    return mVelocity;
}

void SnowFlake::setAcceleration(glm::vec3 const &acceleration)
{
    mAcceleration = acceleration;
}

glm::vec3 SnowFlake::getAcceleration() const
{
    return mAcceleration;
}

void SnowFlake::updateGeometry(atlas::core::Time<> const &t)
{
    glm::vec3 newPosition;
	glm::vec3 newVelocity;

    float deltaTime = t.deltaTime;

    //Compute the new position by Runge-Kutta Order 4.
    //based on https://www.intmath.com/differential-equations/12-runge-kutta-rk4-des.php 
    glm::vec3 v1 = deltaTime * computeVelocity(0);
    glm::vec3 v2 = deltaTime * computeVelocity(0.5*deltaTime);
    glm::vec3 v3 = deltaTime * computeVelocity(0.5*deltaTime);
    glm::vec3 v4 = deltaTime * computeVelocity(deltaTime);
    newPosition = mPosition + (1.0f / 6.0f) * (v1 + 2.0f*v2 + 2.0f*v3 + v4);

    //Compute the new velocity by Runge-Kutta Order 4
    //based on https://www.intmath.com/differential-equations/12-runge-kutta-rk4-des.php
    glm::vec3 a1 = deltaTime * computeAcceleration(0, mVelocity);
    glm::vec3 a2 = deltaTime * computeAcceleration(0.5*deltaTime, mVelocity + 0.5f*a1);
    glm::vec3 a3 = deltaTime * computeAcceleration(0.5*deltaTime, mVelocity + 0.5f*a2);
    glm::vec3 a4 = deltaTime * computeAcceleration(deltaTime, mVelocity + a3);
    newVelocity = mVelocity + (1.0f / 6.0f) * (a1 + 2.0f*a2 + 2.0f*a3 + a4);

    //Compute the new velocity by Runge-Kutta Order 4
    //based on https://www.intmath.com/differential-equations/12-runge-kutta-rk4-des.php
    //Update acceleration so that we use it to compute velocity
    //next the next time we execute Runge-Kutta
    mAcceleration = (newVelocity - mVelocity) / deltaTime;

    mPosition = newPosition;
    mVelocity = newVelocity;
    
	applyTransformations();
}

void SnowFlake::applyTransformations()
{
    setModel(glm::mat4(1.0f));
    transformGeometry(glm::translate(glm::mat4(1.0f), mPosition)); 
}

glm::vec3 SnowFlake::computeVelocity(float deltaTime, glm::vec3 const &velocity)
{
    return velocity + mAcceleration*deltaTime;    
}

glm::vec3 SnowFlake::computeVelocity(float deltaTime)
{
    return computeVelocity(deltaTime, mVelocity);  
}

glm::vec3 SnowFlake::computeAcceleration(float deltaTime, glm::vec3 const &velocity)		
{
	glm::vec3 position = mPosition + velocity*deltaTime;
	return computeAcceleration(position, velocity);
}

glm::vec3 SnowFlake::computeAcceleration(glm::vec3 const &position, glm::vec3 const &velocity)
{
    const float g = 9.81;
    glm::vec3 forceGravity(0.0f, -g*mMass, 0.0f);

    //Essentially some magical constant that seems to look good
    //at preventing the snowflakes from falling too quickly
    const float viscosityConstant = 7.5f;
    glm::vec3 forceViscosity = -viscosityConstant * mMass * mVelocity;

    glm::vec3 forceWind =  mMass * ((SnowScene *)atlas::utils::Application::getInstance().getCurrentScene())->getWindForce();

    //Adds the flake flutter effect
    float offsetRadius = std::fabs(mNormalDistribution(mGenerator));
    float theta = mUniformDistribution(mGenerator);
    glm::vec3 offset = offsetRadius * glm::vec3(cos(theta), 0.0f, sin(theta));
    
    glm::vec3 netForce = forceGravity + forceViscosity + forceWind + offset;

    return netForce / mMass;    
}

void SnowFlake::setModel(glm::mat4 const &model)
{
	mModel = model;
}

glm::mat4 SnowFlake::getRotation() const
{
    return mRotationMatrix;
}

void SnowFlake::setRotation(glm::mat4 const &rotation)
{
    mRotationMatrix = rotation;
}

int SnowFlake::getSnowFlakeCount()
{
    return SnowFlake::snowFlakeCount;
}