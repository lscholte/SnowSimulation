#include "SnowFall.hpp"
#include "Shader.hpp"

SnowFall::SnowFall()
{
    glGenVertexArrays(1, &mVao);
	glGenBuffers(1, &mPositionBuffer);
	glGenBuffers(1, &mColorBuffer);
	
        
    std::vector<atlas::gl::ShaderUnit> shaderUnits
    {
        atlas::gl::ShaderUnit(generated::Shader::getShaderDirectory() + "/scene.vert", GL_VERTEX_SHADER),
        atlas::gl::ShaderUnit(generated::Shader::getShaderDirectory() + "/scene.frag", GL_FRAGMENT_SHADER)
    };
    
    mShaders.push_back(atlas::gl::Shader(shaderUnits));
    
    mShaders[0].compileShaders();
    mShaders[0].linkShaders();    
}

SnowFall::~SnowFall()
{
}

void SnowFall::addSnowFlake(std::unique_ptr<SnowFlake> snowflake)
{
	mSnowFlakes.push_back(std::move(snowflake));
}

void SnowFall::updateGeometry(atlas::core::Time<> const &t)
{
    mVertexPositions.clear();
    mVertexColors.clear();
    for(auto &snowFlake : mSnowFlakes)
    {
        snowFlake->updateGeometry(t);
        mVertexPositions.push_back(snowFlake->getPosition());
        mVertexColors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
    }

    //Removes snowflakes that are too low in the scene.
    //Prevents an infinite number of snowflakes from being stored
    for (auto it = mSnowFlakes.begin(); it != mSnowFlakes.end(); )
    {
        if((*it)->getPosition().y < -10)
        {
            it = mSnowFlakes.erase(it);				
        }
        else
        {
            ++it;
        }
    }
}

void SnowFall::renderGeometry(atlas::math::Matrix4 const &projection, atlas::math::Matrix4 const &view)
{
	mShaders[0].enableShaders();
	
	glm::mat4 modelViewProjection = projection * view * mModel;
	const GLint MODEL_VIEW_PROJECTION_UNIFORM_LOCATION = glGetUniformLocation(mShaders[0].getShaderProgram(), "ModelViewProjection");
    glUniformMatrix4fv(MODEL_VIEW_PROJECTION_UNIFORM_LOCATION, 1, GL_FALSE, &modelViewProjection[0][0]);

    glPointSize(10.0f); 

    glBindVertexArray(mVao);
	
	glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, 3*mVertexPositions.size()*sizeof(GLfloat), &mVertexPositions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	
	glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, 3*mVertexColors.size()*sizeof(GLfloat), &mVertexColors[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    
    glDrawArrays(GL_POINTS, 0, mSnowFlakes.size());    
    
    glBindVertexArray(0);

    mShaders[0].disableShaders();
}