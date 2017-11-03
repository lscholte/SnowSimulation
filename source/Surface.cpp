#include "Surface.hpp"
#include "Shader.hpp"
#include "SnowScene.hpp"
#include <atlas/utils/Application.hpp>

constexpr GLfloat Surface::vertexPositions[][3] = {
    {-10.0f, 0.0f, -10.0f},
    {10.0f, 0.0f, -10.0f},
    {-10.0f, 0.0f, 10.0f},
    {10.0f, 0.0f, 10.0f},
};

constexpr GLfloat Surface::vertexNormals[][3] = {
    {0.0f, 1.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
};

constexpr GLfloat Surface::vertexColors[][3] = {
    {1.0f, 0.6f, 0.6f},
    {1.0f, 0.6f, 0.6f},
    {1.0f, 0.6f, 0.6f},
    {1.0f, 0.6f, 0.6f},
};

constexpr GLfloat Surface::textureCoordinates[][2] = {
    {0.0f, 0.0f},
    {1.0f, 0.0f},
    {0.0f, 1.0f},
    {1.0f, 1.0f},
};

Surface::Surface()
{
    glGenVertexArrays(1, &mVao);
    glGenBuffers(1, &mPositionBuffer);
    glGenBuffers(1, &mNormalBuffer);
    glGenBuffers(1, &mColorBuffer);
    glGenBuffers(1, &mTextureCoordBuffer);

    glBindVertexArray(mVao);            

    glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, 12*sizeof(GLfloat), Surface::vertexPositions, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    
    glBindBuffer(GL_ARRAY_BUFFER, mNormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, 12*sizeof(GLfloat), Surface::vertexNormals, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	
	glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, 12*sizeof(GLfloat), Surface::vertexColors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    glBindBuffer(GL_ARRAY_BUFFER, mTextureCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, 8*sizeof(GLfloat), Surface::textureCoordinates, GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	        
    glBindVertexArray(0);        
    
    std::vector<atlas::gl::ShaderUnit> shaderUnits
    {
        atlas::gl::ShaderUnit(generated::Shader::getShaderDirectory() + "/scene.vert", GL_VERTEX_SHADER),
        atlas::gl::ShaderUnit(generated::Shader::getShaderDirectory() + "/scene.frag", GL_FRAGMENT_SHADER)
    };
    
    mShaders.push_back(atlas::gl::Shader(shaderUnits));
    
    mShaders[0].compileShaders();
    mShaders[0].linkShaders(); 
}


void Surface::renderGeometry(atlas::math::Matrix4 const &projection, atlas::math::Matrix4 const &view)
{
    //TODO: In separate shader,
    //Take the SnowMap and anything that is in 'shadow' of the snow,
    //render a white colour with a bit of transparency. Store the result in
    //a texture. Then pass this texture to the Scene.vert/frag shader to apply
    //overtop of the surface




	mShaders[0].enableShaders();
    
    glm::mat4 modelViewProjection = projection * view * mModel;
	const GLint MODEL_VIEW_PROJECTION_UNIFORM_LOCATION = glGetUniformLocation(mShaders[0].getShaderProgram(), "ModelViewProjection");
    glUniformMatrix4fv(MODEL_VIEW_PROJECTION_UNIFORM_LOCATION, 1, GL_FALSE, &modelViewProjection[0][0]);


    glm::mat4 skyView = glm::lookAt(glm::vec3(0.0f, 15.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 skyProjection = glm::perspective(glm::radians(70.0f), 1.0f, 0.01f, 100.0f);
    glm::mat4 offsetMatrix = glm::mat4(
        0.5f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f);
    glm::mat4 skyMatrix = offsetMatrix * skyProjection * skyView * mModel;
    const GLint SKY_MATRIX_UNIFORM_LOCATION = glGetUniformLocation(mShaders[0].getShaderProgram(), "SkyMatrix");
    glUniformMatrix4fv(SKY_MATRIX_UNIFORM_LOCATION, 1, GL_FALSE, &skyMatrix[0][0]);
    
    glActiveTexture(GL_TEXTURE1);
    GLint SCENE_SNOW_MAP_UNIFORM_LOCATION = glGetUniformLocation(mShaders[0].getShaderProgram(), "SnowMap");
    GLint SCENE_USE_SNOW_MAP_UNIFORM_LOCATION = glGetUniformLocation(mShaders[0].getShaderProgram(), "UseSnowMap");    
    glUniform1i(SCENE_SNOW_MAP_UNIFORM_LOCATION, 1);
    glUniform1i(SCENE_USE_SNOW_MAP_UNIFORM_LOCATION, true);    
    glBindTexture(GL_TEXTURE_2D, ((SnowScene *) atlas::utils::Application::getInstance().getCurrentScene())->getSnowFall().getSnowDepthTexture());

    glBindVertexArray(mVao);        
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);

    mShaders[0].disableShaders();
}