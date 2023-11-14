#include <vector>
#include <string>

using namespace std;

class Entity {

public:
	Entity();
	Entity(std::string modelName, std::string colorName);
	Entity(std::string modelName, std::string colorName, int x, int y);
	~Entity();

	void setModelName(std::string modelName);
	void setColorName(std::string colorName);
	std::string getModelName();
	std::string getColorName();
	void setX(float x);
	void setY(float y);
	float getX();
	float getY();
	void setHealth(int health);
	int getHealth();
	void setScale(float scale);
	float getScale();
	void setRotation(float rotation);
	float getRotation();


private:
	std::string modelName;
	std::string colorName;
	float x, y;
	int health;
	float scale;
	float rotation;
};