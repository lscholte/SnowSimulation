#include "SnowFlake.hpp"
#include "Shader.hpp"
#include "SnowScene.hpp"
#include <atlas/utils/Application.hpp>

int SnowFlake::snowFlakeCount = 0;

SnowFlake::SnowFlake() :
    mMass(0.01)
{
    ++(SnowFlake::snowFlakeCount);
    
    mNormalDistribution = std::normal_distribution<float>(0.0f, 2.0f);    
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

	if(true) //use RK4
	{
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
	}
	else
	{
        glm::vec3 a_t = computeAcceleration(0, mVelocity);
        newPosition = mPosition + mVelocity*deltaTime + 0.5f*a_t*deltaTime*deltaTime;
        newVelocity = mVelocity + 0.5f*(a_t + computeAcceleration(deltaTime, computeVelocity(deltaTime)))*deltaTime;
	}

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

    const float radius = 0.0025f; //quarter of a centimeter
    const float kv = 6.0 * 3.1415926 * radius;
    const float n = 1.81;
    glm::vec3 forceViscosity = -kv * n * mVelocity;

    glm::vec3 forceWind = mMass * ((SnowScene *)atlas::utils::Application::getInstance().getCurrentScene())->getWindForce();

    //Adds the flake flutter effect
    float offsetRadius = std::fabs(mNormalDistribution(mGenerator));
    float theta = mUniformDistribution(mGenerator);
    glm::vec3 offset = mMass * glm::vec3(offsetRadius*cos(theta), 0.0f, offsetRadius*sin(theta));
    

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