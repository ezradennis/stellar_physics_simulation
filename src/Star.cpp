#include "Star.h"
#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <vector>

const float G = 39.47841760435743f; // gravitational constant in (AU^3) / (solar mass * year^2)

void Star::setTemp(float temperature) {
	this->temperature = temperature;
}

void Star::smoothSetTemp(float temperature, float deltaTime) {
	float nextTemp = temperature + (this->temperature - temperature) * exp(-deltaTime);
	nextTemp = glm::clamp(nextTemp, 0.0f, 40000.0f);
	this->temperature = roundf(nextTemp);
	
}

void Star::updateAllStarsPhysics(std::vector<Star*>& stars, float deltaTime) {
	std::vector<glm::vec3> accelerations(stars.size(), glm::vec3(0.0f));

	// calculate accelerations due to gravity
	for (size_t i = 0; i < stars.size(); ++i) {
		for (size_t j = 0; j < stars.size(); ++j) {
			if (i == j) continue; // skip self
			glm::vec3 direction = stars[j]->position - stars[i]->position;
			float distance = glm::length(direction) + 0.01f; // avoid division by zero
			direction = glm::normalize(direction);
			float forceMagnitude = G * stars[i]->mass * stars[j]->mass / (distance * distance);
			glm::vec3 force = forceMagnitude * direction;
			accelerations[i] += force / stars[i]->mass;
		}
	}

	// update velocities and positions
	for (size_t i = 0; i < stars.size(); ++i) {
		stars[i]->velocity += accelerations[i] * deltaTime;
		stars[i]->position += stars[i]->velocity * deltaTime;
	}
}
	