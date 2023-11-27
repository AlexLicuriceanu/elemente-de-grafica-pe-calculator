#include "./entity.h"
#include "utils/glm_utils.h"

Entity::Entity() {
	this->scaleX = 1;
	this->scaleY = 1;
	this->scaleZ = 1;
	this->type = "";
	this->x = 0;
	this->y = 0;
	this->z = 0;
	this->health = 1;
	this->bodyRotation = 0;
	this->turretRotation = 0;
	this->speed = 1;
}

Entity::~Entity() {

}

void Entity::setScale(float scale) {
	this->scaleX = scale;
	this->scaleY = scale;
	this->scaleZ = scale;
}

void Entity::setType(std::string type) {
	this->type = type;
}

std::string Entity::getType() {
	return this->type;
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

void Entity::setHealth(int health) {
	this->health = health;
}
int Entity::getHealth() {
	return this->health;
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

void Entity::setBodyRotation(float bodyRotation) {
	this->bodyRotation = bodyRotation;
}
void Entity::setTurretRotation(float turretRotation) {
	this->turretRotation = turretRotation;
}

float Entity::getBodyRotation() {
	return this->bodyRotation;
}
float Entity::getTurretRotation() {
	return this->turretRotation;
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