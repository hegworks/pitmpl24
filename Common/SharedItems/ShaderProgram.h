#pragma once

#ifndef SHADER_H
#define SHADER_H

#ifdef WINDOWS_BUILD
#include "glad/glad.h"
#endif

#ifdef Raspberry_BUILD
#include <GLES3/gl3.h>
#endif

#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include "ShaderType.h"
#include <string>

namespace Uknitty
{

// this class is not directly copied from, but based on the learnopengl.com
/// <summary>
/// this class compiles a ShaderProgram from a given Fragment and Vertex Shader file path.
/// </summary>
class ShaderProgram
{
public:
	// the program ID
	unsigned int ID;

	// constructor reads and builds the shader
	ShaderProgram(std::string vertexPath, std::string fragmentPath, ShaderType shaderType);

	// use/activate the shader
	void Use() const
	{
		glUseProgram(ID);
	}

	void UnUse() const
	{
		glUseProgram(0);
	}

	void Delete()
	{
		glDeleteProgram(ID);
	}

	ShaderType GetShaderType() const { return m_shaderType; }

	// utility uniform functions
	GLint GetUniformLocation(const std::string& name) const
	{
		return glGetUniformLocation(ID, name.c_str());
	}
	void SetBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void SetInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void SetFloat(const std::string& name, float value) const
	{
		glUniform1f(GetUniformLocation(name), value);
	}
	void SetMat4(const std::string& name, glm::mat4 value) const
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
	}
	void SetVec3(const std::string& name, glm::vec3 value) const
	{
		glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(value));
	}
	void SetVec4(const std::string& name, glm::vec4 value) const
	{
		glUniform4fv(GetUniformLocation(name), 1, glm::value_ptr(value));
	}

private:
	ShaderType m_shaderType = ShaderType::LIT;
};

inline ShaderProgram::ShaderProgram(std::string vertexPath, std::string fragmentPath, ShaderType shaderType)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch(std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ " << vertexPath << " " << fragmentPath << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// 2. compile shaders
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	// vertex ShaderProgram
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// similar for Fragment ShaderProgram
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	// print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	// print linking errors if any
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	m_shaderType = shaderType;
}

}

#endif
