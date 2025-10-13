#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <cmath>

#include"shaderclass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Texture.h"
#include"Camera.h"
#include"Sphere.h"
#include"Star.h"


const unsigned int width = 1920;
const unsigned int height = 1080;
float textMult = 1.0f;

GLfloat vertices[] =
{//      COORDINATES      /        COLORS          /   texCoord   //
	-0.5f,  0.0f,  0.5f,     0.83f, 0.70f, 0.44f,    0.0f, 0.0f,  // front left
	-0.5f,  0.0f, -0.5f,     0.83f, 0.70f, 0.44f,    5.0f, 0.0f,  // back left
	 0.5f,  0.0f, -0.5f,     0.83f, 0.70f, 0.44f,    0.0f, 0.0f,  // back right
	 0.5f,  0.0f,  0.5f,     0.83f, 0.70f, 0.44f,    5.0f, 0.0f,  // front right
	 0.0f,  0.8f,  0.0f,     0.92f, 0.86f, 0.76f,    2.5f, 5.0f,   // top
};

GLuint indices[] =
{
	0, 1, 2,
	0, 3, 2,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
};

int main()
{
	glfwInit();

	// tell glfw the major and minor version of OpenGL, and which profile we are using (core)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// creates window with 800 width 800 height, title, and null parameters for monitor and share
	// then checks if window actually exists and if it doesnt it terminates GLFW and does a print statement
	GLFWwindow* window = glfwCreateWindow(width, height, "stellar physics", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// introduce the window into current context (whatever that means)
	glfwMakeContextCurrent(window);

	// load Glad so it can configure OpenGL
	gladLoadGL();


	// specify the viewport of OpenGL in the Window
	// in this case the viewport goes from x = 0, y = 0, to x = width, y = height  (down is positive)
	glViewport(0, 0, width, height);

	// generate Shader object using shaders (vertex, fragment)
	Shader shaderProgram("shaders/default.vert", "shaders/star.frag");

	

	//     radius  mass  temperature       position
	Star sun(1.0f, 1.0f, 6000.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	Star coldDwarf(0.25f, 0.2f, 3000.0f, glm::vec3(5.0f, 0.0f, 0.0f));
	// Star hotGiant(5.0f, 10.0f, 20000.0f, glm::vec3(-8.0f, -3.0f, 0.0f));
	// Star coldSupermassive(50.0f, 5.0f, 3000.0f, glm::vec3(70.0f, 20.0f, 0.0f));

	float r = glm::length(coldDwarf.position - sun.position);
	float v = sqrt(1.0 * sun.mass / r);
	coldDwarf.velocity = glm::vec3(0.0f, v, 0.0f);

	std::vector<Star*> stars = { &sun, &coldDwarf };
	// texture

	Texture muffin("assets/muffin.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	muffin.texUnit(shaderProgram, "tex0", 0);


	// enable depth testing so OpenGL knows what triangles to draw in front
	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	// delta time (framerate independance)
	float deltaTime = 0.0f;	// time between current frame and last frame
	float lastFrame = 0.0f;	// time of last frame

	// main while loop
	while (!glfwWindowShouldClose(window))
	{
		// calculate delta time
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// specify background color
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		
		// clear the color and depth in the back buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// tell OpenGL which Shader Program to use
		shaderProgram.Activate();

		camera.Inputs(window, deltaTime);
		// updates and exports camera matrix to the Vertex Shader
		camera.Matrix(45.0f, 0.1f, 1000.0f, shaderProgram, "camMatrix");

		// TEMP INPUTS
		bool hKeyPressed = glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS;
		bool gKeyPressed = glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS;

		if (hKeyPressed) {
			sun.smoothSetTemp(sun.temperature + 2000.0f, deltaTime);
			printf("Temperature: %f K\n", sun.temperature);
		}
		else if (gKeyPressed) {
			sun.smoothSetTemp(sun.temperature - 2000.0f, deltaTime);
			printf("Temperature: %f K\n", sun.temperature);
		}

		// END TEMP INPUTS - REMOVE LATER

		//glUniform1f(glGetUniformLocation(shaderProgram.ID, "temperature"), sun.temperature);
		//sun.Draw(shaderProgram);

		
		for (Star* star : stars) {
			glUniform1f(glGetUniformLocation(shaderProgram.ID, "temperature"), star->temperature);
			star->Draw(shaderProgram);
		}
		

		Star::updateAllStarsPhysics(stars, deltaTime);

		glfwSwapBuffers(window);

		// takes care of all GLFW events
		glfwPollEvents();
	}

	// delete all objects we created
	muffin.Delete();
	shaderProgram.Delete();

	// delete the window before ending program
	glfwDestroyWindow(window);
	// terminate GLFW before ending program
	glfwTerminate();
	return 0;
}