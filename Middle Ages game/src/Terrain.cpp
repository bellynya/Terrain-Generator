#include "Terrain.h"

Terrain::Terrain(std::string vertex_path, std::string fragmetn_path, int width, int height)
{
	shader = new Shader(vertex_path, fragmetn_path);
	width_ = width;
	height_ = height;
}

void Terrain::Init()
{
	CreateGLstate();

	PopulateBuffer();
}

void Terrain::Render(Camera& camera)
{
	shader->use();
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 300.0f);
	shader->setMat4("model", model);
	shader->setMat4("view", view);
	shader->setMat4("projection", projection);

	for (size_t i = 0; i < 4; i++)
	{
		if (textures[i])
		{
			textures[i]->Bind(GL_TEXTURE0 + i);
		}
	}
	
	glBindVertexArray(vao);

	glDrawElements(GL_TRIANGLES, (width_ - 1) * (height_ - 1) * 6, GL_UNSIGNED_INT, NULL);

	glBindVertexArray(0);

}

void Terrain::CreateGLstate()
{
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	int pos_loc = 0;
	int tex_loc = 1;

	unsigned int offset_cnt = 0;

	glEnableVertexAttribArray(pos_loc);

	glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	offset_cnt += 3;

	glEnableVertexAttribArray(tex_loc);

	glVertexAttribPointer(tex_loc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offset_cnt * sizeof(float)));
	offset_cnt += 2;

}

void Terrain::PopulateBuffer()
{
	vertecies.resize(height_ * width_);

	int index = 0;
	for (size_t x = 0; x < width_; x++)
	{
		for (size_t z = 0; z < height_; z++)
		{
			vertecies[index].InitVertex(this, glm::vec3(x, 0.0f, z));
			index++;
		}
	}
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertecies[0]) * vertecies.size(), &vertecies[0], GL_STATIC_DRAW);

	std::vector<unsigned int> indices;

	int num_quads = (width_ - 1) * (height_ - 1);
	indices.resize(num_quads * 6);

	index = 0;

	for (size_t x = 0; x < width_ - 1; x++)
	{
		for (size_t z = 0; z < height_ - 1; z++)
		{
			unsigned int index_bottom_left = z * width_ + x;
			unsigned int index_top_left = (z + 1) * width_ + x;
			unsigned int index_top_right = (z + 1) * width_ + x + 1;
			unsigned int index_bottom_right = z * width_ + x + 1;

			//top left triangle
			indices[index++] = index_bottom_left;
			indices[index++] = index_top_left;
			indices[index++] = index_top_right;

			//bottom right triangle
			indices[index++] = index_bottom_left;
			indices[index++] = index_top_right;
			indices[index++] = index_bottom_right;
		}
	}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);
}

void Terrain::Vertex::InitVertex(Terrain* terrain, glm::vec3 vertecies)
{
	float y = terrain->GetHeight((int)vertecies.x, (int)vertecies.z);
	pos = glm::vec3(vertecies.x, y, vertecies.z);

	float size = (float)terrain->terrain_size_;

	texture_pos = glm::vec2((float)vertecies.x / size, (float)vertecies.z / size);
}

float Terrain::GetHeightInterpolated(float x, float z) const
{
	float BaseHeight = GetHeight((int)x, (int)z);

	if (((int)x + 1 >= terrain_size_) || ((int)z + 1 >= terrain_size_)) {
		return BaseHeight;
	}

	float NextXHeight = GetHeight((int)x + 1, (int)z);

	float RatioX = x - floorf(x);

	float InterpolatedHeightX = (float)(NextXHeight - BaseHeight) * RatioX + (float)BaseHeight;

	float NextZHeight = GetHeight((int)x, (int)z + 1);

	float RatioZ = z - floorf(z);

	float InterpolatedHeightZ = (float)(NextZHeight - BaseHeight) * RatioZ + (float)BaseHeight;

	float FinalHeight = (InterpolatedHeightX + InterpolatedHeightZ) / 2.0f;

	return FinalHeight;
}