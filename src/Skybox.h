#ifndef SKYBOX_CLASS_H
#define SKYBOX_CLASS_H

#include<vector>
#include <stb/stb_image.h>
#include"Texture.h"
#include"Camera.h"

class Skybox
{
public:

	static unsigned int loadCubemap(const std::vector<std::string>& faces);
	static void setupSkybox();
	static void drawSkybox(Shader skyboxShader, Camera camera);

	Skybox() = delete;
};

#endif
