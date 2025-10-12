#include "Star.h"
#include <glm/glm.hpp>
#include <glm/common.hpp>

void Star::setTemp(float temperature) {
	this->temperature = temperature;
}

void Star::smoothSetTemp(float temperature, float deltaTime) {
	float nextTemp = temperature + (this->temperature - temperature) * exp(-deltaTime);
	nextTemp = glm::clamp(nextTemp, 0.0f, 40000.0f);
	this->temperature = roundf(nextTemp);
	
}
