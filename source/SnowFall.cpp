#include "SnowFall.hpp"
#include "Shader.hpp"
#include <atlas/utils/GUI.hpp>

SnowFall::SnowFall()
{        
    mSnowMapResolution = 4096;
    glGenTextures(1, &mSnowDepthTexture);
    glBindTexture(GL_TEXTURE_2D, mSnowDepthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mSnowMapResolution,
        mSnowMapResolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    const float kShadowBorderDepth[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, kShadowBorderDepth);
    glBindTexture(GL_TEXTURE_2D, 0);

    // GLint currentFBO;
    // glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFBO);

    glGenFramebuffers(1, &mSnowFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, mSnowFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mSnowDepthTexture, 0);
    // glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mSnowDepthTexture, 0);    
    // glDrawBuffer(GL_NONE); // No color buffer is drawn to.    
    // glReadBuffer(GL_NONE); // No color buffer is drawn to.  
    glClear(GL_DEPTH_BUFFER_BIT);    

    GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
        fprintf(stderr, "glCheckFramebufferStatus: %x\n", fboStatus);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenVertexArrays(1, &mVao);
	glGenBuffers(1, &mPositionBuffer);
    glGenBuffers(1, &mColorBuffer);
    glGenBuffers(1, &mIndexBuffer);   
    
    glBindVertexArray(mVao);
    
    glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, 0, mVertexPositions.data(), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    
    glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, 0, mVertexColors.data(), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    glBindVertexArray(0);    
    
    std::vector<atlas::gl::ShaderUnit> shaderUnitsSnow
    {
        atlas::gl::ShaderUnit(generated::Shader::getShaderDirectory() + "/SnowSurface.vert", GL_VERTEX_SHADER),
        atlas::gl::ShaderUnit(generated::Shader::getShaderDirectory() + "/SnowSurface.frag", GL_FRAGMENT_SHADER)
    };
        
    std::vector<atlas::gl::ShaderUnit> shaderUnits
    {
        atlas::gl::ShaderUnit(generated::Shader::getShaderDirectory() + "/SnowFalling.vert", GL_VERTEX_SHADER),
        atlas::gl::ShaderUnit(generated::Shader::getShaderDirectory() + "/SnowFalling.frag", GL_FRAGMENT_SHADER)
    };

    mShaders.push_back(atlas::gl::Shader(shaderUnitsSnow));        
    mShaders.push_back(atlas::gl::Shader(shaderUnits));
    
    mShaders[0].compileShaders();
    mShaders[0].linkShaders();    

    mShaders[1].compileShaders();
    mShaders[1].linkShaders();

    mSnowflakeSizeDistribution = std::normal_distribution<float>(0.05f, 0.01f);        
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
    mVertexIndices.clear();

    int i = 0;
    for(auto &snowFlake : mSnowFlakes)
    {
        snowFlake->updateGeometry(t);

        float snowflakeSize = mSnowflakeSizeDistribution(mGenerator);

        mVertexPositions.push_back(snowFlake->getPosition());
        mVertexPositions.push_back(snowFlake->getPosition() + glm::vec3(glm::vec4(snowflakeSize, 0.0, 0.0, 0.0)*snowFlake->getRotation()));
        mVertexPositions.push_back(snowFlake->getPosition() + glm::vec3(glm::vec4(0.0, snowflakeSize, 0.0, 0.0)*snowFlake->getRotation()));
        mVertexPositions.push_back(snowFlake->getPosition() + glm::vec3(glm::vec4(snowflakeSize, snowflakeSize, 0.0, 0.0)*snowFlake->getRotation()));

        mVertexColors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
        mVertexColors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
        mVertexColors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
        mVertexColors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
        
        mVertexIndices.push_back(4*i + 0);
        mVertexIndices.push_back(4*i + 1);
        mVertexIndices.push_back(4*i + 2);

        mVertexIndices.push_back(4*i + 1);
        mVertexIndices.push_back(4*i + 2);
        mVertexIndices.push_back(4*i + 3);

        
        ++i;
    }

    glBindVertexArray(mVao);
    
    glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, 3*mVertexPositions.size()*sizeof(GLfloat), mVertexPositions.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, 3*mVertexColors.size()*sizeof(GLfloat), mVertexColors.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mVertexIndices.size()*sizeof(GLint), mVertexIndices.data(), GL_DYNAMIC_DRAW);
        
    glBindVertexArray(0);

    //Removes snowflakes that are too low in the scene.
    //Prevents an infinite number of snowflakes from being stored
    for (auto it = mSnowFlakes.begin(); it != mSnowFlakes.end(); )
    {
        if((*it)->getPosition().y < 0)
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

    // Surface Snow Shader
    {        
        GLint m_viewport[4];
        glGetIntegerv(GL_VIEWPORT, m_viewport);

        mShaders[0].enableShaders();
        
        glBindFramebuffer(GL_FRAMEBUFFER, mSnowFBO);
        glClearDepth(1.0f);
        // glClear(GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, mSnowMapResolution, mSnowMapResolution);
        // glEnable(GL_DEPTH_TEST);    

        // glPolygonOffset(mShadowSlopeScaleBias, mShadowDepthBias);        
        // glEnable(GL_POLYGON_OFFSET_FILL);

        glm::mat4 skyView = glm::lookAt(glm::vec3(0.0f, 15.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 skyProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.01f, 100.0f);            
        glm::mat4 modelViewProjection = skyProjection * skyView * mModel;    

        const GLint MODEL_VIEW_PROJECTION_UNIFORM_LOCATION = glGetUniformLocation(mShaders[0].getShaderProgram(), "ModelViewProjection");
        glUniformMatrix4fv(MODEL_VIEW_PROJECTION_UNIFORM_LOCATION, 1, GL_FALSE, &modelViewProjection[0][0]);

        const GLint MODEL_UNIFORM_LOCATION = glGetUniformLocation(mShaders[0].getShaderProgram(), "Model");
        glUniformMatrix4fv(MODEL_UNIFORM_LOCATION, 1, GL_FALSE, &mModel[0][0]);

        glBindVertexArray(mVao);  
        glDrawElements(GL_TRIANGLES, mVertexIndices.size(), GL_UNSIGNED_INT, (void *) 0);
        glBindVertexArray(0);        
                
        mShaders[0].disableShaders();
        
        // glPolygonOffset(0.0f, 0.0f);        
        // glDisable(GL_POLYGON_OFFSET_FILL);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);     
        
        glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);        
    }
    
    
    //Falling Snow Shader
    {        
        mShaders[1].enableShaders();
        
        glm::mat4 modelViewProjection = projection * view * mModel;
        const GLint MODEL_VIEW_PROJECTION_UNIFORM_LOCATION = glGetUniformLocation(mShaders[1].getShaderProgram(), "ModelViewProjection");
        glUniformMatrix4fv(MODEL_VIEW_PROJECTION_UNIFORM_LOCATION, 1, GL_FALSE, &modelViewProjection[0][0]);      
    
        glBindVertexArray(mVao);        
        glDrawElements(GL_TRIANGLES, mVertexIndices.size(), GL_UNSIGNED_INT, (void *) 0);
        glBindVertexArray(0); 
    
        mShaders[1].disableShaders();
    }

}

GLuint SnowFall::getSnowDepthTexture() const
{
    return mSnowDepthTexture;
}