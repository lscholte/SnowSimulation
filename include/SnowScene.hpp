#ifndef SnowScene_hpp
#define SnowScene_hpp

#include "SnowFlake.hpp"
#include "SnowCloud.hpp"
#include "SnowFall.hpp"
#include "SnowOverlay.hpp"
#include <atlas/utils/Scene.hpp>

class SnowScene : public atlas::utils::Scene
{
	public:
		SnowScene();
		~SnowScene();
	
		void renderScene() override;
		void updateScene(double time) override;

		void mousePressEvent(int button, int action, int modifiers, double xPos, double yPos) override;		
		void mouseMoveEvent(double xPos, double yPos) override;
        void mouseScrollEvent(double xOffset, double yOffset) override;
		void keyPressEvent(int key, int scancode, int action, int mods) override;
		void screenResizeEvent(int width, int height) override;		
		void onSceneEnter() override;
		void onSceneExit() override;
		
		void addSnowFlake(std::unique_ptr<SnowFlake> snowflake);
		SnowFall const& getSnowFall() const;
		SnowOverlay & getSnowOverlay();
		glm::vec3 getWindForce();		

		
	private:
		glm::mat4 mProjection;
		float mWidth, mHeight;

		bool mPaused;

		SnowFall mSnowFall;
		SnowOverlay mSnowOverlay;

		float mTheta, mRow;

		glm::vec3 mWindForce;
};

#endif
