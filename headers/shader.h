#ifndef SHADER_H
#define SHADER_H

#include<iostream>
#include<string>
#include<glad/glad.h>

#include "filesystem.h"
using namespace std;
using namespace glm;
using namespace FileSystem;


class Shader{
	GLuint ID;
public:
	int GetID()const {
		return ID;
	}
	void Use()const {
		glUseProgram(ID);
	}
	Shader(const char* vertexSource, const char* fragmentSource);
	void SetFloat(const string& name, float value) const{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	void SetInt(const string& name, int value) const{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setVec2(const string &name, const vec2 &value) const {
		glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}

	void setVec2(const string &name, float x, float y) const {
		glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
	}

	void setVec3(const string &name, const vec3 &value) const {
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}

	void setVec3(const string &name, float x, float y, float z) const {
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}

	void setVec4(const string &name, const vec4 &value) const {
		glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}

	void setVec4(const string &name, float x, float y, float z, float w) const {
		glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
	}

	void setMat2(const string &name, const mat2 &mat) const {
		glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void setMat3(const string &name, const mat3 &mat) const {
		glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void setMat4(const string &name, const mat4 &mat) const {
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
};

inline Shader::Shader(const char* vertexCodePath, const char* fragmentCodePath){
	string vertexCodeStr = readFileToString(VertShader(vertexCodePath));
	string fragmentCodeStr = readFileToString(FragShader(fragmentCodePath));
	const char* vertexCode = vertexCodeStr.c_str();
	const char* fragmentCode = fragmentCodeStr.c_str();
	// cout<<"C1\n"<<vertexCode<<endl<<"C2\n"<<fragmentCode<<endl;

	int success;
	char infoLog[512];

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexCode, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cerr<<"ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"<<infoLog<<endl;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cerr<<"ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"<<infoLog<<endl;
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cerr<<"ERROR::SHADER::PROGRAM::LINK_FAILED\n"<<infoLog<<endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}



#endif //SHADER_H
