#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include <string>

#include <glm/glm.hpp>
#include "glad/glad.h"
#include "Array2D.h"
#include "Texture.h"

#include "Shader.h"
#include "Camera.h"

class Terrain
{
public:
	Terrain(std::string vertex_path, std::string fragmetn_path, int width, int height);

	void Init();

	void Render(Camera& camera);

	float GetHeight(int x, int z) const { return height_map.Get(x, z); }

	int GetSize() const { return terrain_size_; }

	float GetHeightInterpolated(float x, float z) const;

	void SetTexture(Texture* pTexture) { textures[0] = pTexture; }
protected:
	struct Vertex
	{
		glm::vec3 pos;
		glm::vec2 texture_pos;

		void InitVertex(Terrain* terrain, glm::vec3 vertecies);
	};

	void CreateGLstate();
	void PopulateBuffer();

	std::vector<Vertex> vertecies;
	Shader* shader;

	int width_;
	int height_;

	unsigned int vao, vbo, ebo;

	float min_height_, max_height_;

	int terrain_size_ = 0;

	Array2D<float> height_map;

	Texture* textures[4] = { 0 };

};

#endif