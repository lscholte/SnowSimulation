#include "SnowOverlay.hpp"
#include "Shader.hpp"
#include "SnowScene.hpp"
#include <atlas/utils/Application.hpp>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/Filtered_kernel.h>
#include <CGAL/Cartesian.h>
#include "LinearSubdivision.hpp"
#include "Asset.hpp"
#include <fstream>
#include <iostream>

using Real = float;
using Kernel0 = CGAL::Cartesian<Real>;
using Kernel = CGAL::Filtered_kernel<Kernel0>;
using Polyhedron = CGAL::Polyhedron_3<Kernel>;
using Point = Polyhedron::Point_3;
using Halfedge = Polyhedron::Halfedge;
using Face = Polyhedron::Facet;

SnowOverlay::SnowOverlay()
{

    Polyhedron mesh;

    std::ifstream quadFile((generated::Asset::getAssetDirectory() + "/quad.off").c_str());
    if(quadFile)
    {
        std::stringstream buffer;
        buffer << quadFile.rdbuf();
        quadFile.close();

        if(!(buffer >> mesh))
        {
            std::cerr << "cannot read mesh from file\n";
            exit(1);
        }
    }

    for (auto it = mesh.vertices_begin(); it != mesh.vertices_end(); ++it)
    {
        Point &p = it->point();
        p = Point(
            p[0]*20.0f - 10.0f,
            0.001f, //keeps overlay slightly above surface
            p[1]*20.0f - 10.0f            
        );
    }    
    LinearSubdivision::subdivide(mesh, 5);    

    mNumVertices = 3*mesh.size_of_facets();
    
    GLfloat positions[mesh.size_of_facets()*9];
    GLfloat colors[mesh.size_of_facets()*12];

    int i = 0;
    int j = 0;
    for (auto it = mesh.facets_begin(); it != mesh.facets_end(); ++it, i += 9, j += 12)
    {
        auto halfedgeCirc = it->facet_begin();
        const Point &p1 = halfedgeCirc->vertex()->point();
        ++halfedgeCirc;
        const Point &p2 = halfedgeCirc->vertex()->point();
        ++halfedgeCirc;
		const Point &p3 = halfedgeCirc->vertex()->point();
                
        positions[i] = p1.x();
        positions[i + 1] = p1.y();
        positions[i + 2] = p1.z();

        colors[j] = 0.0f;
        colors[j + 1] = 0.0f;
        colors[j + 2] = 0.0f;
        colors[j + 3] = 0.0f;        

        positions[i + 3] = p2.x();
        positions[i + 4] = p2.y();
        positions[i + 5] = p2.z();

        colors[j + 4] = 0.0f;
        colors[j + 5] = 0.0f;
        colors[j + 6] = 0.0f;
        colors[j + 7] = 0.0f;

        positions[i + 6] = p3.x();
        positions[i + 7] = p3.y();
        positions[i + 8] = p3.z();

        colors[j + 8] = 0.0f;
        colors[j + 9] = 0.0f;
        colors[j + 10] = 0.0f;
        colors[j + 11] = 0.0f;
    }

    glGenVertexArrays(1, &mVao);
    glGenBuffers(1, &mPositionBuffer);
    glGenBuffers(1, &mColorBuffer);

    glBindVertexArray(mVao);            

    glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, mesh.size_of_facets()*9*sizeof(GLfloat), positions, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	
	glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, mesh.size_of_facets()*12*sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
       
    glBindVertexArray(0);        
    
    std::vector<atlas::gl::ShaderUnit> shaderUnits
    {
        atlas::gl::ShaderUnit(generated::Shader::getShaderDirectory() + "/SnowOverlay.vert", GL_VERTEX_SHADER),
        atlas::gl::ShaderUnit(generated::Shader::getShaderDirectory() + "/SnowOverlay.frag", GL_FRAGMENT_SHADER)
    };
    
    mShaders.push_back(atlas::gl::Shader(shaderUnits));
    
    mShaders[0].compileShaders();
    mShaders[0].linkShaders(); 
}


void SnowOverlay::renderGeometry(atlas::math::Matrix4 const &projection, atlas::math::Matrix4 const &view)
{

	mShaders[0].enableShaders();
    
    glm::mat4 modelViewProjection = projection * view * mModel;
	const GLint MODEL_VIEW_PROJECTION_UNIFORM_LOCATION = glGetUniformLocation(mShaders[0].getShaderProgram(), "ModelViewProjection");
    glUniformMatrix4fv(MODEL_VIEW_PROJECTION_UNIFORM_LOCATION, 1, GL_FALSE, &modelViewProjection[0][0]);


    glm::mat4 skyView = glm::lookAt(glm::vec3(0.0f, 15.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    // glm::mat4 skyProjection = glm::perspective(glm::radians(70.0f), 1.0f, 0.01f, 100.0f);
    glm::mat4 skyProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.01f, 100.0f);            
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
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Draw wireframe    
    glDrawArrays(GL_TRIANGLES, 0, mNumVertices);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    
    glBindVertexArray(0);

    mShaders[0].disableShaders();
}