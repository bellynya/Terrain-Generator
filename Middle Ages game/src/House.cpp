#include "House.h"

House::House(std::string vertex, std::string fragment, float sheet_width, float sheet_height, float sprite_width)
{
	_shader = new Shader(vertex, fragment);

    _sheet_width = sheet_width;
    _sheet_height = sheet_height;
    _sprite_width = sprite_width;
}

void House::Init(std::string texture_path)
{
    CreateGLstate();

    PopulateBuffer();
    _texture_id = LoadTextures(texture_path);

    _shader->use();
    _shader->setInt("texture1", 0);

}

void House::Render(Camera& camera)
{
    _shader->use();
    view = camera.GetViewMatrix();
    projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 300.0f);
    _shader->setMat4("view", view);
    _shader->setMat4("projection", projection);

    glBindVertexArray(_vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture_id);
    _shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 54);
}

void House::Translate(float x, float y, float z)
{
    model = glm::translate(model, glm::vec3(x, y, z));
}

void House::Rotate(float x, float y, float z, float angle)
{
    model = glm::rotate(model, glm::radians(angle), glm::vec3(x, y, z));
}

void House::CreateGLstate()
{

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);


    int pos_loc = 0;
    int tex_loc = 1;
    glEnableVertexAttribArray(pos_loc);
    glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(tex_loc);
    glVertexAttribPointer(tex_loc, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

void House::PopulateBuffer()
{
    vertecies.resize(3 * 2 * 9);

    float t = RandomFloatGenerator();

    vertecies = {
        // координаты        // текстурные координаты
       -0.5f, -0.5f, -0.5f, (((_x + t) + 1 ) * _sprite_width) / _sheet_width, ((_y + 3) * _sprite_width) / _sheet_height,
        0.5f, -0.5f, -0.5f, ((_x + t) * _sprite_width) / _sheet_width, ((_y + 3) * _sprite_width) / _sheet_height,
        0.5f,  0.5f, -0.5f, ((_x + t) * _sprite_width) / _sheet_width, ((_y + 2) * _sprite_width) / _sheet_height,
        0.5f,  0.5f, -0.5f, ((_x + t) * _sprite_width) / _sheet_width, ((_y + 2)* _sprite_width) / _sheet_height,                       // far back plate
       -0.5f,  0.5f, -0.5f, (((_x + t) + 1 )* _sprite_width) / _sheet_width, ((_y + 2) * _sprite_width) / _sheet_height,
       -0.5f, -0.5f, -0.5f, (((_x + t) + 1 )* _sprite_width) / _sheet_width, ((_y + 3) * _sprite_width) / _sheet_height,

       -0.5f, -0.5f,  0.5f,  ((_x + 1) * _sprite_width) / _sheet_width, ((_y + 1) * _sprite_width) / _sheet_height,
        0.5f, -0.5f,  0.5f,  (_x * _sprite_width) / _sheet_width, ((_y + 1) * _sprite_width) / _sheet_height,
        0.5f,  0.5f,  0.5f,  (_x * _sprite_width) / _sheet_width, (_y * _sprite_width) / _sheet_height,
        0.5f,  0.5f,  0.5f,  (_x * _sprite_width) / _sheet_width, (_y * _sprite_width) / _sheet_height,             // near front plate
       -0.5f,  0.5f,  0.5f,  ((_x + 1) * _sprite_width) / _sheet_width, (_y * _sprite_width) / _sheet_height,
       -0.5f, -0.5f,  0.5f,  ((_x + 1) * _sprite_width) / _sheet_width, ((_y + 1) * _sprite_width) / _sheet_height,

       -0.5f, -0.5f, -0.5f,  (((_x + 2) + 1) * _sprite_width) / _sheet_width, ((_y + 1) * _sprite_width) / _sheet_height,
       -0.5f, -0.5f,  0.5f,  ((_x + 2) * _sprite_width) / _sheet_width, ((_y + 1) * _sprite_width) / _sheet_height,
       -0.5f,  0.5f,  0.5f,  ((_x + 2) * _sprite_width) / _sheet_width, (_y * _sprite_width) / _sheet_height,
       -0.5f,  0.5f,  0.5f,  ((_x + 2) * _sprite_width) / _sheet_width, (_y  * _sprite_width) / _sheet_height,             // left plate
       -0.5f,  0.5f, -0.5f,  (((_x + 2) + 1) * _sprite_width) / _sheet_width, (_y * _sprite_width) / _sheet_height,
       -0.5f, -0.5f, -0.5f,  (((_x + 2) + 1) * _sprite_width) / _sheet_width, ((_y + 1) * _sprite_width) / _sheet_height,

        0.5f, -0.5f, -0.5f,  (((_x + 1) + 1) * _sprite_width) / _sheet_width, ((_y + 1) * _sprite_width) / _sheet_height,
        0.5f, -0.5f,  0.5f,  ((_x + 1) * _sprite_width) / _sheet_width, ((_y + 1) * _sprite_width) / _sheet_height,
        0.5f,  0.5f,  0.5f,  ((_x + 1) * _sprite_width) / _sheet_width, (_y * _sprite_width) / _sheet_height,
        0.5f,  0.5f,  0.5f,  ((_x + 1) * _sprite_width) / _sheet_width, (_y * _sprite_width) / _sheet_height,             // right plate
        0.5f,  0.5f, -0.5f,  (((_x + 1) + 1) * _sprite_width) / _sheet_width, (_y * _sprite_width) / _sheet_height,
        0.5f, -0.5f, -0.5f,  (((_x + 1) + 1) * _sprite_width) / _sheet_width, ((_y + 1) * _sprite_width) / _sheet_height,

       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // botom plate
       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top plate
       -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

       -0.65f, 0.4f, -0.6f,  (((_x + 3) + 1) * _sprite_width) / _sheet_width, (((_y + 2) + 1) * _sprite_width) / _sheet_height,
       -0.65f, 0.4f,  0.6f,  ((_x + 3) * _sprite_width) / _sheet_width, (((_y + 2) + 1) * _sprite_width) / _sheet_height,
       -0.0f,  0.8f,  0.6f,  ((_x + 3) * _sprite_width) / _sheet_width, ((_y + 2) * _sprite_width) / _sheet_height, // left roof side
       -0.0f,  0.8f,  0.6f,  ((_x + 3) * _sprite_width) / _sheet_width, ((_y + 2) * _sprite_width) / _sheet_height,
       -0.0f,  0.8f, -0.6f,  (((_x + 3) + 1) * _sprite_width) / _sheet_width, ((_y + 2) * _sprite_width) / _sheet_height,
       -0.65f, 0.4f, -0.6f,  (((_x + 3) + 1) * _sprite_width) / _sheet_width, (((_y + 2) + 1) * _sprite_width) / _sheet_height,

        0.65f, 0.4f, -0.6f,  (((_x + 3) + 1) * _sprite_width) / _sheet_width, (((_y + 2) + 1) * _sprite_width) / _sheet_height,
        0.65f, 0.4f,  0.6f,  ((_x + 3) * _sprite_width) / _sheet_width, (((_y + 2) + 1) * _sprite_width) / _sheet_height,
        0.0f,  0.8f,  0.6f,  ((_x + 3) * _sprite_width) / _sheet_width, ((_y + 2) * _sprite_width) / _sheet_height, // right roof side
        0.0f,  0.8f,  0.6f,  ((_x + 3) * _sprite_width) / _sheet_width, ((_y + 2) * _sprite_width) / _sheet_height,
        0.0f,  0.8f, -0.6f,  (((_x + 3) + 1) * _sprite_width) / _sheet_width, ((_y + 2) * _sprite_width) / _sheet_height,
        0.65f, 0.4f, -0.6f,  (((_x + 3) + 1) * _sprite_width) / _sheet_width, (((_y + 2) + 1) * _sprite_width) / _sheet_height,

        -0.5f, 0.5f,  0.5f,  ((_x + 3) * _sprite_width) / _sheet_width, ((_y + 1) * _sprite_width) / _sheet_height,
        -0.0f, 0.8f,  0.5f,  ((_x + 3.5f) * _sprite_width) / _sheet_width, ((_y + 2) * _sprite_width) / _sheet_height,
         0.5f, 0.5f,  0.5f,  ((_x + 4) * _sprite_width) / _sheet_width, ((_y + 1) * _sprite_width) / _sheet_height,

        -0.5f, 0.5f, -0.5f,  ((_x + 3) * _sprite_width) / _sheet_width, ((_y + 1) * _sprite_width) / _sheet_height,
        -0.0f, 0.8f, -0.5f,  ((_x + 3.5f) * _sprite_width) / _sheet_width, ((_y + 2) * _sprite_width) / _sheet_height,
         0.5f, 0.5f, -0.5f,  ((_x + 4) * _sprite_width) / _sheet_width, ((_y + 1) * _sprite_width) / _sheet_height,
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertecies[0]) * vertecies.size(), &vertecies[0], GL_STATIC_DRAW);
}

float House::RandomFloatGenerator()
{
    return (float)(rand() % 2);
}

unsigned int House::LoadTextures(std::string& texture_path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    stbi_set_flip_vertically_on_load(0);
    int width, height, nrComponents;
    unsigned char* data = stbi_load(texture_path.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << texture_path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
