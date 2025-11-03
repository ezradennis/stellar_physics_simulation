#include"Skybox.h"
#include"VAO.h"
#include"VBO.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"Camera.h"

#include <cmath>

GLfloat skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

std::vector<std::string> faces =
{
	"assets/right.png",
	"assets/left.png",
	"assets/top.png",
	"assets/bottom.png",
	"assets/front.png",
	"assets/back.png"
};

unsigned int cubemapTexture;

VAO* skyboxVAO = nullptr;
VBO* skyboxVBO = nullptr;

// learnopengl.com skybox 
unsigned int Skybox::loadCubemap(const std::vector<std::string>& faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	stbi_set_flip_vertically_on_load(false);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (!data)
		{
			std::cerr << "Failed to load cubemap face: " << faces[i] << std::endl;
			continue;
		}

		GLenum format = GL_RGB;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		std::cout << "Loaded: " << faces[i] << " (" << width << "x" << height
			<< ", " << nrChannels << " channels)" << std::endl;

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

void Skybox::setupSkybox() {
	
	cubemapTexture = loadCubemap(faces);

	
	if (!skyboxVAO) skyboxVAO = new VAO();
	if (!skyboxVBO) skyboxVBO = new VBO(skyboxVertices, sizeof(skyboxVertices));

	skyboxVAO->Bind();
	skyboxVBO->Bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	skyboxVAO->Unbind();
	skyboxVBO->Unbind();
}

void Skybox::drawSkybox(Shader skyboxShader, Camera camera) {
	// SKYBOX RENDERING
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);

	skyboxShader.Activate();
	glUniform1i(glGetUniformLocation(skyboxShader.ID, "skybox"), 0);

	// remove translation part of view matrix
	glm::mat4 view = glm::mat4(glm::mat3(camera.getViewMatrix()));
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)camera.width / camera.height, 0.1f, 1000.0f);
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	// skybox cube
	glBindVertexArray(skyboxVAO->ID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	// SKYBOX DONE
}