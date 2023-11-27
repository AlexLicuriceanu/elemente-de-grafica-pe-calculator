#include <vector>
#include <string>
#include "utils/glm_utils.h"

using namespace std;

class Entity {

public:
	Entity();
	~Entity();

	void setType(std::string type);
	std::string getType();

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
	glm::vec3 getPosition();
	

private:
	std::string type;
	float x, y, z;
	int health;
	float scaleX, scaleY, scaleZ;
	float bodyRotation, turretRotation;
	float speed;
};