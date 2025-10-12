#ifndef STAR_CLASS_H
#define STAR_CLASS_H

#include "Sphere.h"

class Star : public Sphere
{
public:
	// fields
	float mass;          // mass in solar masses
	float temperature;   // temp in kelvin
	float luminosity;    // luminosity in solar luminosities

	// constructors
	Star(float radius, unsigned int sectorCount, unsigned int stackCount, float mass,
		float temperature, glm::vec3 pos = glm::vec3(0.0f))
		: Sphere(radius, sectorCount, stackCount, pos), mass(mass),
		temperature(temperature), luminosity(luminosity)
	{}

	// setters
	void setMass();
	void setTemp(float temperature);
	void smoothSetTemp(float temperature, float deltaTime);
	void updatePhysics(float deltaTime);    // placeholder for future physics updates
};

#endif
