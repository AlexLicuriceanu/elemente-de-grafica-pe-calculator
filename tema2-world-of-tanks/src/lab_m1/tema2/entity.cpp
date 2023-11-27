#include "./entity.h"
#include "utils/glm_utils.h"

Entity::Entity() {
	this->scaleX = 1;
	this->scaleY = 1;
	this->scaleZ = 1;
	this->x = 0;
	this->y = 0;
	this->z = 0;
	this->health = 1;
	this->bodyRotation = 0;
	this->turretRotation = 0;
	this->speed = 1;
	this->timeLeft = -1;
}

Entity::~Entity() {

}

void Entity::moveForward(float deltaTime) {

	// Calculate forward delta.
	float forwardDelta = this->getSpeed() * deltaTime;

	// Compute new coordinates.
	this->setX(this->getX() + forwardDelta * sin(glm::radians(this->getBodyRotation())));
	this->setZ(this->getZ() - forwardDelta * cos(glm::radians(this->getBodyRotation())));

	// Compute new coordinates for the bounding box.
	auto bbCoordinates = this->getBbCoordinates() +
		glm::vec3(forwardDelta * sin(glm::radians(this->getBodyRotation())), 0,
			-forwardDelta * cos(glm::radians(this->getBodyRotation())));

	// Set the bounding box's coordinates.
	this->setBbCoordinates(bbCoordinates);
}

void Entity::moveBackward(float deltaTime) {

	// Calculate backward delta.
	float backwardDelta = this->getSpeed() * deltaTime;

	// Compute new coordinates.
	this->setX(this->getX() - backwardDelta * sin(glm::radians(this->getBodyRotation())));
	this->setZ(this->getZ() + backwardDelta * cos(glm::radians(this->getBodyRotation())));

	// Compute new coordinates for the bounding box.
	auto bbCoordinates = this->getBbCoordinates() +
		glm::vec3(-backwardDelta * sin(glm::radians(this->getBodyRotation())), 0,
			backwardDelta * cos(glm::radians(this->getBodyRotation())));

	// Set the bounding box's coordinates.
	this->setBbCoordinates(bbCoordinates);
}

void Entity::rotateLeft(float deltaTime, float rotationSpeed) {

	// Rotate the tank to the left.
	this->setBodyRotation(this->getBodyRotation() - rotationSpeed * deltaTime);
	this->setTurretRotation(this->getTurretRotation() - rotationSpeed * deltaTime);
}

void Entity::rotateRight(float deltaTime, float rotationSpeed) {

	// Rotate the tank to the right.
	this->setBodyRotation(this->getBodyRotation() + rotationSpeed * deltaTime);
	this->setTurretRotation(this->getTurretRotation() + rotationSpeed * deltaTime);
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

void Entity::setTimeLeft(float timeLeft) {
	this->timeLeft = timeLeft;
}

float Entity::getTimeLeft() {
	return this->timeLeft;
}

float Entity::getBbWidth() {
	return bbWidth;
}

float Entity::getBbHeight() {
	return bbHeight;
}

float Entity::getBbLength() {
	return bbLength;
}

void Entity::setBbWidth(float width) {
	bbWidth = width;
}

void Entity::setBbHeight(float height) {
	bbHeight = height;
}

void Entity::setBbLength(float length) {
	bbLength = length;
}

void Entity::setBbCoordinates(float x, float y, float z) {
	bbCoordinates.x = x;
	bbCoordinates.y = y;
	bbCoordinates.z = z;
}

glm::vec3 Entity::getBbCoordinates() {
	return bbCoordinates;
}

void Entity::setBbCoordinates(glm::vec3 bbCoordinates) {
	this->bbCoordinates.x = bbCoordinates.x;
	this->bbCoordinates.y = bbCoordinates.y;
	this->bbCoordinates.z = bbCoordinates.z;
}

void Entity::setPosition(glm::vec3 coordinates) {
	this->x = coordinates.x;
	this->y = coordinates.y;
	this->z = coordinates.z;
}

std::string Entity::getState() {
	return this->state;
}

void Entity::setState(std::string state) {
	this->state = state;
}

void Entity::setCurrentCannonCooldown(float time) {
	this->currentCannonCooldown = time;
}

void Entity::setMinCannonCooldown(float time) {
	this->minCannonCooldown = time;
}

float Entity::getCurrentCannonCooldown() {
	return this->currentCannonCooldown;
}

float Entity::getMinCannonCooldown() {
	return this->minCannonCooldown;
}