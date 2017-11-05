#include "Surface.hpp"
#include "Shader.hpp"

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
	mShaders[0].enableShaders();
    
    glm::mat4 modelViewProjection = projection * view * mModel;
	const GLint MODEL_VIEW_PROJECTION_UNIFORM_LOCATION = glGetUniformLocation(mShaders[0].getShaderProgram(), "ModelViewProjection");
    glUniformMatrix4fv(MODEL_VIEW_PROJECTION_UNIFORM_LOCATION, 1, GL_FALSE, &modelViewProjection[0][0]);

    glBindVertexArray(mVao);    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);

    mShaders[0].disableShaders();
}