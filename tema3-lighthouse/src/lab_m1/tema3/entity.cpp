#include "./entity.h"
#include "utils/glm_utils.h"

Entity::Entity() {
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
	this->scaleX = 1;
	this->scaleY = 1;
	this->scaleZ = 1;
	this->direction = 1;
	this->speed = 1;
}

Entity::~Entity() {
}

void Entity::setScale(float scale) {
	this->scaleX = scale;
	this->scaleY = scale;
	this->scaleZ = scale;
}

void Entity::setX(float x) {
	this->x = x;
}

void Entity::setY(float y) {
	this->y = y;
}

void Entity::setZ(float z) {
	this->z = z;
}

float Entity::getX() {
	return this->x;
}

float Entity::getY() {
	return this->y;
}

float Entity::getZ() {
	return this->z;
}

void Entity::setScaleX(float scaleX) {
	this->scaleX = scaleX;
}

void Entity::setScaleY(float scaleY) {
	this->scaleY = scaleY;
}

void Entity::setScaleZ(float scaleZ) {
	this->scaleZ = scaleZ;
}

float Entity::getScaleX() {
	return this->scaleX;
}

float Entity::getScaleY() {
	return this->scaleY;
}

float Entity::getScaleZ() {
	return this->scaleZ;
}

void Entity::setSpeed(float speed) {
	this->speed = speed;
}

float Entity::getSpeed() {
	return this->speed;
}

void Entity::setPosition(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

glm::vec3 Entity::getPosition() {
	return glm::vec3(this->x, this->y, this->z);
}

void Entity::setPosition(glm::vec3 coordinates) {
	this->x = coordinates.x;
	this->y = coordinates.y;
	this->z = coordinates.z;
}

int Entity::getDirection() {
	return this->direction;
}

void Entity::setDirection(int direction) {
	this->direction = direction;
}

float Entity::getHeading() {
	return this->heading;
}

void Entity::setHeading(float heading) {
	this->heading = heading;
}