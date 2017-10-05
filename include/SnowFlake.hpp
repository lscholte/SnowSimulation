#ifndef SnowFlake_hpp
#define SnowFlake_hpp

#include <atlas/utils/Geometry.hpp>
#include <set>
#include <vector>
#include <random>

class SnowFlake : public atlas::utils::Geometry
{
    public:

        SnowFlake();
        ~SnowFlake();        

        void computeGeometry(atlas::core::Time<> const &t);

        void updateGeometry(atlas::core::Time<> const &t) override;
        void renderGeometry(atlas::math::Matrix4 const &projection, atlas::math::Matrix4 const &view) override;       
        
        void applyTransformations();        
        
        void setMass(float mass);
        float getMass() const;

        void setPosition(glm::vec3 const &position);
        glm::vec3 getPosition() const;

        void setVelocity(glm::vec3 const &velocity);
		glm::vec3 getVelocity() const;

		void setAcceleration(glm::vec3 const &acceleration);
        glm::vec3 getAcceleration() const;
        
        void setModel(glm::mat4 const &model);
        
        static int getSnowFlakeCount();
        
    private:

        float mMass;
        glm::vec3 mPosition, mVelocity, mAcceleration;

        glm::vec3 computeVelocity(float deltaTime, glm::vec3 const &velocity);
        glm::vec3 computeVelocity(float deltaTime);        
		glm::vec3 computeAcceleration(float deltaTime, glm::vec3 const &velocity);		
        glm::vec3 computeAcceleration(glm::vec3 const &position, glm::vec3 const &velocity);
        
        GLuint mVao;
        GLuint mPositionBuffer, mColorBuffer;

        std::default_random_engine mGenerator;
        std::normal_distribution<float> mNormalDistribution;  
        std::uniform_real_distribution<float> mUniformDistribution;          
        
        static const GLfloat POSITIONS[][3];
		static const GLfloat COLORS[][3];
        static const GLint INDICES[][1];
        
        static int snowFlakeCount;
};

#endif