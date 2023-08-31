#include "Shader.h"

Shader::Shader(const std::string& vertex_path, const std::string& fragment_path)
{
	std::ifstream vertex(vertex_path.c_str());
	std::ifstream fragment(fragment_path.c_str());

	std::string vertex_shader_source, vertex_line;
	std::string fragment_shader_source, fragment_line;

	if (vertex.is_open() && fragment.is_open())
	{
		while (vertex || fragment)
		{
			std::getline(vertex, vertex_line);
			vertex_shader_source.append(vertex_line + "\n");
			std::getline(fragment, fragment_line);
			fragment_shader_source.append(fragment_line + "\n");

			vertex_line.clear();
			fragment_line.clear();
		}
	}
	else std::cout << "Error with shader reading." << std::endl;

	std::cout << vertex_shader_source << std::endl;
	std::cout << fragment_shader_source << std::endl;

	unsigned int vertex_shader, fragment_shader, shader_program;

	const char* v_src = vertex_shader_source.c_str();
	const char* f_src = fragment_shader_source.c_str();

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &v_src, NULL);
	glCompileShader(vertex_shader);

	int  vertex_success;
	char vertex_infoLog[512];
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &vertex_success);
	if (!vertex_success)
	{
		glGetShaderInfoLog(vertex_shader, 512, NULL, vertex_infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << vertex_infoLog << std::endl;
	}


	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &f_src, NULL);
	glCompileShader(fragment_shader);

	int  fragment_success;
	char fragment_infoLog[512];
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &fragment_success);
	if (!fragment_success)
	{
		glGetShaderInfoLog(fragment_shader, 512, NULL, fragment_infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fragment_infoLog << std::endl;
	}

	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	int programm_success;
	char programm_infoLog[512];
	glGetProgramiv(shader_program, GL_LINK_STATUS, &programm_success);
	if (!programm_success) {
		glGetProgramInfoLog(shader_program, 512, NULL, programm_infoLog);
		std::cout << "ERROR::SHADER::PROGRAMM::LINKING_FAILED\n" << programm_infoLog << std::endl;
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	ID = shader_program;

}

Shader::~Shader()
{
	glDeleteProgram(ID);
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat)
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setFloat3(const std::string& name, float f1, float f2, float f3)
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), f1, f2, f3);
}

void Shader::setVec3(const std::string& name, const glm::vec3& v)
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &v[0]);
}
