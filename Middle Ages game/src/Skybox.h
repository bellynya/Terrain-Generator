#ifndef SKYBOX_H
#define SKYBOX_H

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "Shader.h"
#include "Camera.h"

class Skybox
{
public:
	Skybox(std::string vertex, std::string fragment);
	void Init(std::vector<std::string> faces);
	void Render(Camera& camera);
private:

	void CreateGLstate();

	void PopulateBuffer();

	unsigned int LoadCubemap(std::vector<std::string> faces);

	Shader* shader;

	unsigned int vao, vbo, cubemap_texture;

	std::vector<float> vertecies;

};

#endif