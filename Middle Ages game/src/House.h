#ifndef HOUSE_H
#define HOUSE_H
#include <string>
#include <vector>

#include "stb_image.h"
#include "glm/glm.hpp"

#include "Shader.h"
#include "Camera.h"


class House
{
public:
	House(std::string vertex, std::string fragment, float sheet_width, float sheet_height, float sprite_width);

	void Init(std::string texture_path);

	void Render(Camera& camera);

	void Translate(float x, float y, float z);

	void Rotate(float x, float y, float z, float angle);

private:

	void CreateGLstate();

	void PopulateBuffer();

	float RandomFloatGenerator();

	unsigned int LoadTextures(std::string& texture_path);

	Shader* _shader;

	unsigned int _vao, _vbo, _texture_id;

	std::vector<float> vertecies;

	float _sheet_width, _sheet_height, _sprite_width;

	int _x = 0, _y = 0; // left top of the texture atlas

	glm::mat4 model = glm::mat4(1.0f), view, projection;
};

#endif
