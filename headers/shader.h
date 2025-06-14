#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "file_system.h"

using namespace std;
using namespace glm;
using namespace FileSystem;

class Shader {
	GLuint ID;
public:
	int GetID() const;
	void Use() const;
	Shader(const char* vertexSource, const char* fragmentSource);

	// Uniform 设置方法
	void SetFloat(const string& name, float value) const;
	void SetInt(const string& name, int value) const;
	void SetVec2(const string& name, const vec2& value) const;
	void SetVec2(const string& name, float x, float y) const;
	void SetVec3(const string& name, const vec3& value) const;
	void SetVec3(const string& name, float x, float y, float z) const;
	void SetVec4(const string& name, const vec4& value) const;
	void SetVec4(const string& name, float x, float y, float z, float w) const;
	void SetMat2(const string& name, const mat2& mat) const;
	void SetMat3(const string& name, const mat3& mat) const;
	void SetMat4(const string& name, const mat4& mat) const;
};

#endif // SHADER_H