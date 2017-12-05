#include "SnowScene.hpp"
#include "SnowFlake.hpp"
#include "SnowCloud.hpp"
#include "Surface.hpp"
#include "SnowOverlay.hpp"
#include "Shader.hpp"
#include <atlas/core/GLFW.hpp>
#include <atlas/utils/GUI.hpp>
#include <glm/gtc/type_ptr.hpp>

SnowScene::SnowScene() :
	mPaused(true),
	mRow(5.0),
	mTheta(0.0),
	mLightPosition(-25.0f, 15.0f, -25.0f)
{
	std::unique_ptr<SnowCloud> snowCloud = std::make_unique<SnowCloud>();
	// snowCloud->setBoundingBox(glm::vec3(-10.0f, 12.0f, -10.0f), glm::vec3(10.0f, 12.0f, 10.0f));
	snowCloud->setBoundingBox(glm::vec3(-10.5f, 12.0f, -10.5f), glm::vec3(10.5f, 12.0f, 10.5f));
	
	std::unique_ptr<Surface> surface = std::make_unique<Surface>();
	// std::unique_ptr<SnowOverlay> snowOverlay = std::make_unique<SnowOverlay>();	

	mGeometries.push_back(std::move(snowCloud));
	mGeometries.push_back(std::move(surface));
	// mGeometries.push_back(std::move(snowOverlay));	
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
	switch(key)
	{
		case GLFW_KEY_RIGHT:
			mTheta -= 2.0 * (M_PI/180.0f);
			break;
		case GLFW_KEY_LEFT:
			mTheta += 2.0 * (M_PI/180.0f);		
			break;
		case GLFW_KEY_UP:
			mRow += 0.1;
			break;
		case GLFW_KEY_DOWN:
			mRow -= 0.1;
			break;
	}
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
	
	// glm::vec3 eye(0.0, 5.0, -15.0);
	glm::vec3 eye(20.0f*cos(mTheta), mRow, 20.0f*sin(mTheta));
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
	ImGui::SliderFloat3("Wind Force", value_ptr(mWindForce), -50.0f, 50.0f);
	ImGui::SliderFloat3("Light Position", value_ptr(mLightPosition), -25.0f, 25.0f);		
	ImGui::End();
			
	mSnowFall.renderGeometry(mProjection, view);

	for(auto &geometry : mGeometries)
	{
		geometry->drawGui();
		geometry->renderGeometry(mProjection, view);
	}

	mSnowOverlay.renderGeometry(mProjection, view);
	mSnowOverlay.drawGui();

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
		mSnowOverlay.updateGeometry(mTime);		
	}
}

void SnowScene::addSnowFlake(std::unique_ptr<SnowFlake> snowflake)
{
	mSnowFall.addSnowFlake(std::move(snowflake));
}

SnowFall const& SnowScene::getSnowFall() const
{
	return mSnowFall;
}

SnowOverlay & SnowScene::getSnowOverlay()
{
	return mSnowOverlay;
}

glm::vec3 SnowScene::getWindForce() 
{
	return mWindForce;
}

glm::vec3 SnowScene::getCameraPosition() const
{
	return glm::vec3(20.0f*cos(mTheta), mRow, 20.0f*sin(mTheta));
}

glm::vec3 SnowScene::getLightPosition() const
{
	return mLightPosition;
}

void SnowScene::onSceneEnter()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
}

void SnowScene::onSceneExit()
{
    glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);	
}