#include "./entity.h"

Entity::Entity() {
	this->colorName = "";
	this->modelName = "";
	this->x = 0;
	this->y = 0;
	this->health = 1;
	this->scale = 1.0;
	this->rotation = 0;
}

Entity::Entity(std::string modelName, std::string colorName) {
	this->colorName = colorName;
	this->modelName = modelName;
	this->x = 0.0;
	this->y = 0.0;
	this->health = 1;
	this->scale = 1.0;
	this->rotation = 0;
}

Entity::Entity(std::string modelName, std::string colorName, int x, int y) {
	this->colorName = colorName;
	this->modelName = modelName;
	this->x = x;
	this->y = y;
	this->health = 1;
	this->scale = 1.0;
	this->rotation = 0;
}

Entity::~Entity() {

}

void Entity::setModelName(std::string modelName) {
	this->modelName = modelName;
}

void Entity::setColorName(std::string colorName) {
	this->colorName = colorName;
}

std::string Entity::getModelName() {
	return this->modelName;
}

std::string Entity::getColorName() {
	return this->colorName;
}

void Entity::setX(float x) {
	this->x = x;
}
void Entity::setY(float y) {
	this->y = y;
}

float Entity::getX() {
	return this->x;
}

float Entity::getY() {
	return this->y;
}

void Entity::setHealth(int health) {
	this->health = health;
}

int Entity::getHealth() {
	return this->health;
}

void Entity::setScale(float scale) {
	this->scale = scale;
}

float Entity::getScale() {
	return this->scale;
}

void Entity::setRotation(float rotation) {
	this->rotation = rotation;
}

float Entity::getRotation() {
	return this->rotation;
}