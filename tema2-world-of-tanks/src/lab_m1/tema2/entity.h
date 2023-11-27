#include <vector>
#include <string>
#include "utils/glm_utils.h"

using namespace std;

class Entity {

public:
	Entity();
	~Entity();

	void setX(float x);
	void setY(float y);
	void setZ(float z);
	float getX();
	float getY();
	float getZ();

	void setHealth(int health);
	int getHealth();
	
	void setScaleX(float scaleX);
	void setScaleY(float scaleY);
	void setScaleZ(float scaleZ);
	void setScale(float scale);
	float getScaleX();
	float getScaleY();
	float getScaleZ();

	void setBodyRotation(float rotation);
	void setTurretRotation(float rotation);
	float getBodyRotation();
	float getTurretRotation();

	void setSpeed(float speed);
	float getSpeed();

	void setPosition(float x, float y, float z);
	void setPosition(glm::vec3 coordinates);
	glm::vec3 getPosition();

	void setTimeLeft(float timeLeft);
	float getTimeLeft();

	float getBbWidth();
	float getBbHeight();
	float getBbLength();
	void setBbWidth(float width);
	void setBbHeight(float height);
	void setBbLength(float length);

	void setBbCoordinates(float x, float y, float z);
	void setBbCoordinates(glm::vec3 bbCoordinates);
	glm::vec3 getBbCoordinates();

	std::string getState();
	void setState(std::string state);
	
	void setCurrentCannonCooldown(float time);
	void setMinCannonCooldown(float time);
	float getCurrentCannonCooldown();
	float getMinCannonCooldown();

	void moveForward(float deltaTime);
	void moveBackward(float deltaTime);
	void rotateLeft(float deltaTime, float rotationSpeed);
	void rotateRight(float deltaTime, float rotationSpeed);
	

private:
	std::string type;
	float x, y, z;
	int health;
	float scaleX, scaleY, scaleZ;
	float bodyRotation, turretRotation;
	float speed;
	float timeLeft;

	glm::vec3 bbCoordinates;
	float bbWidth;
	float bbHeight;
	float bbLength;

	std::string state;

	float currentCannonCooldown;
	float minCannonCooldown;
};