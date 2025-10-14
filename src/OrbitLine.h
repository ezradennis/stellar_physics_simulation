#ifndef ORBITLINE_CLASS_H
#define ORBITLINE_CLASS_H

#include <glm/glm.hpp>

class OrbitLine
{
public:
	float lineWidth;
	float maxLineLength;
	float lineLength;
	glm::vec3 color = glm::vec3(1.0f);

	OrbitLine(float lineWidth, float maxLineLength, glm::vec3 color)
		: lineWidth(lineWidth), maxLineLength(maxLineLength), lineLength(0.0f), color(color)
	{
	}

};

#endif
