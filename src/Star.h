#ifndef STAR_CLASS_H
#define STAR_CLASS_H

#include "Sphere.h"
#include <vector>

class Star : public Sphere
{
public:
	// fields
	float mass;          // mass in solar masses
	float temperature;   // temp in kelvin
	float luminosity;    // luminosity in solar luminosities
	glm::vec3 velocity;  // velocity

	// constructors
	Star(float radius, float mass,
		float temperature, glm::vec3 pos = glm::vec3(0.0f))
		: Sphere(radius, 50, 50, pos), mass(mass),
		temperature(temperature), luminosity(1.0f)
	{}

	// setters
	void setMass();
	void setTemp(float temperature);
	void smoothSetTemp(float temperature, float deltaTime);
	static void updateAllStarsPhysics(std::vector<Star*>& stars, float deltaTime);
};

#endif
