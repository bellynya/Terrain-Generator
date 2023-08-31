#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include "GLFW/glfw3.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "glm/glm.hpp"


class Shader
{
public:
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shader
    Shader(const std::string& vertex_path, const std::string& fragment_path);
    ~Shader();
    // use/activate the shader
    void use();
    // utility uniform functions
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const glm::mat4& mat);
    void setFloat3(const std::string& name, float f1, float f2, float f3);
    void setVec3(const std::string& name, const glm::vec3& v);
};

#endif

