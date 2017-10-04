#include "SnowFlake.hpp"
#include "Shader.hpp"
#include <iostream>
#include <atlas/utils/Application.hpp>

constexpr GLfloat SnowFlake::POSITIONS[][3] = {
    {0.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},    
};

constexpr GLfloat SnowFlake::COLORS[][3] = {
    {1.0, 1.0, 1.0},
    {1.0, 1.0, 1.0},    
};

constexpr GLint SnowFlake::INDICES[][1] = {
    {1},
    {2},
};

SnowFlake::SnowFlake() :
    mMass(0.01)
{
    glGenVertexArrays(1, &mVao);
	glGenBuffers(1, &mPositionBuffer);
	glGenBuffers(1, &mColorBuffer);
	glGenBuffers(1, &mIndexBuffer);
	
	glBindVertexArray(mVao);
	
	glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SnowFlake::POSITIONS), SnowFlake::POSITIONS, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	
	glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SnowFlake::COLORS), SnowFlake::COLORS, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    
    glBindVertexArray(0);
        
    std::vector<atlas::gl::ShaderUnit> shaderUnits
    {
        atlas::gl::ShaderUnit(generated::Shader::getShaderDirectory() + "/scene.vert", GL_VERTEX_SHADER),
        atlas::gl::ShaderUnit(generated::Shader::getShaderDirectory() + "/scene.frag", GL_FRAGMENT_SHADER)
    };
    
    mShaders.push_back(atlas::gl::Shader(shaderUnits));
    
    mShaders[0].compileShaders();
    mShaders[0].linkShaders();
    
    mNormalDistribution = std::normal_distribution<float>(5.0f, 1.0f);    
    mUniformDistribution = std::uniform_real_distribution<float>(0.0f, 3.1415926f);   
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
	if(true) //use RK4
	{
		//Compute the new position by Runge-Kutta Order 4.
		//based on https://www.intmath.com/differential-equations/12-runge-kutta-rk4-des.php 
        glm::vec3 v1 = t.deltaTime * computeVelocity(0);
		glm::vec3 v2 = t.deltaTime * computeVelocity(0.5*t.deltaTime);
		glm::vec3 v3 = t.deltaTime * computeVelocity(0.5*t.deltaTime);
		glm::vec3 v4 = t.deltaTime * computeVelocity(t.deltaTime);
		newPosition = mPosition + (1.0f / 6.0f) * (v1 + 2.0f*v2 + 2.0f*v3 + v4);

		//Compute the new velocity by Runge-Kutta Order 4
		//based on https://www.intmath.com/differential-equations/12-runge-kutta-rk4-des.php
		glm::vec3 a1 = t.deltaTime * computeAcceleration(0, mVelocity);
		glm::vec3 a2 = t.deltaTime * computeAcceleration(0.5*t.deltaTime, mVelocity + 0.5f*a1);
		glm::vec3 a3 = t.deltaTime * computeAcceleration(0.5*t.deltaTime, mVelocity + 0.5f*a2);
		glm::vec3 a4 = t.deltaTime * computeAcceleration(t.deltaTime, mVelocity + a3);
		newVelocity = mVelocity + (1.0f / 6.0f) * (a1 + 2.0f*a2 + 2.0f*a3 + a4);

		//Compute the new velocity by Runge-Kutta Order 4
		//based on https://www.intmath.com/differential-equations/12-runge-kutta-rk4-des.php
		//Update acceleration so that we use it to compute velocity
		//next the next time we execute Runge-Kutta
		mAcceleration = (newVelocity - mVelocity) / t.deltaTime;
	}
	else
	{
        glm::vec3 a_t = computeAcceleration(0, mVelocity);
        newPosition = mPosition + mVelocity*t.deltaTime + 0.5f*a_t*t.deltaTime*t.deltaTime;
        newVelocity = mVelocity + 0.5f*(a_t + computeAcceleration(t.deltaTime, computeVelocity(t.deltaTime)))*t.deltaTime;
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

    const float radius = 0.0025f; //quarter a centimeter
    const float kv = 6.0 * 3.1415926 * radius;
    const float n = 1.81;
    glm::vec3 forceViscosity = -kv * n * mVelocity;

    //Adds the flake flutter effect
    float offsetRadius = std::max(0.0f, mNormalDistribution(mGenerator));
    float theta = mUniformDistribution(mGenerator);
    glm::vec3 offset = mMass * glm::vec3(offsetRadius*cos(theta), 0.0f, offsetRadius*sin(theta));

    // std::cerr << offset.x << " " << offset.y << " " << offset.z << "\n";

    glm::vec3 netForce = forceGravity + forceViscosity + offset;

    return netForce / mMass;    
}

void SnowFlake::setModel(glm::mat4 const &model)
{
	mModel = model;
}

void SnowFlake::renderGeometry(atlas::math::Matrix4 const &projection, atlas::math::Matrix4 const &view)
{
	mShaders[0].enableShaders();
	
	glm::mat4 modelViewProjection = projection * view * mModel;
	const GLint MODEL_VIEW_PROJECTION_UNIFORM_LOCATION = glGetUniformLocation(mShaders[0].getShaderProgram(), "ModelViewProjection");
	glUniformMatrix4fv(MODEL_VIEW_PROJECTION_UNIFORM_LOCATION, 1, GL_FALSE, &modelViewProjection[0][0]);

    // glLineWidth(10.0f);   
    glPointSize(10.0f); 
	
	glBindVertexArray(mVao);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(SnowFlake::INDICES), SnowFlake::INDICES, GL_STATIC_DRAW);
	glDrawElements(GL_POINTS, 2, GL_UNSIGNED_INT, (void *) 0);

	glBindVertexArray(0);

    mShaders[0].disableShaders();
}