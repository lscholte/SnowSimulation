#include "SnowScene.hpp"
#include "SnowFlake.hpp"
#include "SnowCloud.hpp"
#include "Shader.hpp"
#include <atlas/core/GLFW.hpp>
#include <atlas/utils/GUI.hpp>
#include <glm/gtc/type_ptr.hpp>

SnowScene::SnowScene() :
	mPaused(true)
{
	//An interesting observation is that due to the depth of the cloud,
	//some snowflakes appear to fall faster despite all snowflakes
	//having the same downward forces acting upon them, creating an interesting effect
	std::unique_ptr<SnowCloud> snowCloud = std::make_unique<SnowCloud>();
	snowCloud->setBoundingBox(glm::vec3(-3.0f, 3.0f, -2.0f), glm::vec3(3.0f, 3.0f, 2.0f));
	mGeometries.push_back(std::move(snowCloud));
}

SnowScene::~SnowScene()
{
}

void SnowScene::mousePressEvent(int button, int action, int modifiers, double xPos, double yPos)
{
	atlas::utils::Gui::getInstance().mousePressed(button, action, modifiers);
}

void SnowScene::mouseMoveEvent(double xPos, double yPos)
{
	atlas::utils::Gui::getInstance().mouseMoved(xPos, yPos);
}

void SnowScene::mouseScrollEvent(double xOffset, double yOffset)
{
	atlas::utils::Gui::getInstance().mouseScroll(xOffset, yOffset);
}

void SnowScene::keyPressEvent(int key, int scancode, int action, int mods)
{
	atlas::utils::Gui::getInstance().keyPress(key, scancode, action, mods);
}

void SnowScene::screenResizeEvent(int width, int height)
{
	glViewport(0, 0, width, height);

	float aspectRatio = (float) width / height;
	mProjection = glm::perspective(glm::radians(70.0f), aspectRatio, 0.01f, 100.0f);	

	atlas::utils::Gui::getInstance().screenResize(width, height);

	mWidth = (float) width;
	mHeight = (float) height;
}

void SnowScene::renderScene()
{
	atlas::utils::Gui::getInstance().newFrame();
	
	float aspectRatio = 1.0f;
	
	glm::vec3 eye(0.0, 0.0, 3.0);
	glm::vec3 look(0.0, 0.0, 0.0);
	glm::vec3 up(0.0, 1.0, 0.0);
	
	glm::mat4 view = glm::lookAt(eye, look, up);
	
	//Render black background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_FirstUseEver);
	
	ImGui::Begin("Scene Options");
	ImGui::Text(
		"Snowflake Count: %d",
		SnowFlake::getSnowFlakeCount()
	);
	ImGui::Text(
		"Application average %.3f ms/frame (%.1f FPS)",
		1000.0f / ImGui::GetIO().Framerate,
		ImGui::GetIO().Framerate
	);
	ImGui::Checkbox("Simulation Paused", &mPaused);
	ImGui::End();
			
	mSnowFall.renderGeometry(mProjection, view);
	for(auto &geometry : mGeometries)
	{
		geometry->drawGui();
		geometry->renderGeometry(mProjection, view);
	}

	ImGui::Render();				
}

void SnowScene::updateScene(double time)
{
	double delta = time - mTime.currentTime;

	mTime.deltaTime = delta;
	mTime.totalTime += mTime.deltaTime;
	mTime.currentTime += delta;

	atlas::utils::Gui::getInstance().update(mTime);
	
	if(!mPaused)
	{
		for(auto &geometry : mGeometries)
		{
			geometry->updateGeometry(mTime);
		}
		mSnowFall.updateGeometry(mTime);		
	}
}

void SnowScene::addSnowFlake(std::unique_ptr<SnowFlake> snowflake)
{
	mSnowFall.addSnowFlake(std::move(snowflake));
}

void SnowScene::onSceneEnter()
{
	// glDepthFunc(GL_LESS);
    // glEnable(GL_DEPTH_TEST);
	// glEnable(GL_CULL_FACE);
}

void SnowScene::onSceneExit()
{
    // glDisable(GL_DEPTH_TEST);
	// glDisable(GL_CULL_FACE);
}