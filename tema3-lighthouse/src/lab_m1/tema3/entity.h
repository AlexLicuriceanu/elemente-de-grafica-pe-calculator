#include <vector>
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

	void setScaleX(float scaleX);
	void setScaleY(float scaleY);
	void setScaleZ(float scaleZ);
	void setScale(float scale);
	float getScaleX();
	float getScaleY();
	float getScaleZ();

	void setSpeed(float speed);
	float getSpeed();

	void setPosition(float x, float y, float z);
	void setPosition(glm::vec3 coordinates);
	glm::vec3 getPosition();

	void setDirection(int direction);
	int getDirection();

	void setHeading(float heading);
	float getHeading();

private:

	float x, y, z;
	float scaleX, scaleY, scaleZ;
	float heading;
	float speed;
	int direction;
};