#ifndef SPHERE_CLASS_H
#define SPHERE_CLASS_H

#include <vector>
#include <glm/glm.hpp>

#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"shaderClass.h"

class Sphere
{
public:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    glm::vec3 position;

    VAO vao;
    VBO vbo;
    EBO ebo;

    Sphere(float radius, unsigned int sectorCount, unsigned int stackCount, glm::vec3 pos = glm::vec3(0.0f));
    void Draw(Shader& shader);
    ~Sphere();

private:
    void generate(float radius, unsigned int sectorCount, unsigned int stackCount);
};




#endif
