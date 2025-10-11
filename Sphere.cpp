#include "Sphere.h"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sphere::Sphere(float radius, unsigned int sectorCount, unsigned int stackCount, glm::vec3 pos)
    : vbo(nullptr, 0), ebo(nullptr, 0), position(pos) // Temporary init, will re-init after generate
{
    generate(radius, sectorCount, stackCount);

    vao.Bind();
    vbo = VBO(vertices.data(), vertices.size() * sizeof(float));
    ebo = EBO(indices.data(), indices.size() * sizeof(unsigned int));

    // 3 position, 3 normal, 2 texcoord = 8 floats per vertex
    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    vao.LinkAttrib(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();
}

Sphere::~Sphere() {
    vao.Delete();
    vbo.Delete();
    ebo.Delete();
}

void Sphere::generate(float radius, unsigned int sectorCount, unsigned int stackCount)
{
    const float PI = 3.14159265359f;
    float x, y, z, xy;
    float nx, ny, nz, lengthInv = 1.0f / radius;
    float s, t;

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    for (unsigned int i = 0; i <= stackCount; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;
        xy = radius * cosf(stackAngle);
        z = radius * sinf(stackAngle);

        for (unsigned int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;

            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            vertices.push_back(nx);
            vertices.push_back(ny);
            vertices.push_back(nz);

            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            vertices.push_back(s);
            vertices.push_back(t);
        }
    }

    unsigned int k1, k2;
    for (unsigned int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);
        k2 = k1 + sectorCount + 1;

        for (unsigned int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            if (i != (stackCount - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}

void Sphere::Draw(Shader& shader) {
    shader.Activate();
    vao.Bind();

    // Create model matrix for translation
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);

    // Pass model matrix to shader
    GLuint modelLoc = glGetUniformLocation(shader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}