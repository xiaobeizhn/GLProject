//
// Created by zhn on 25-6-11.
//

#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// #include "mesh.h"
#include "model.h"
#include "shader.h"
#include "camera.h"
#include "input_system.h"

#include <iostream>
// #include <vector>

using namespace std;
using namespace InputSystem;
using namespace glm;

struct BoundingSphere {
	vec3 center;
	float radius;
};


class Object{
public:
	virtual ~Object() = default;
	inline static int defaultCnt = 0;
	string name;
	string tag;
	vec3 pos;
	vec3 color;
	Model model;
	BoundingSphere shootedObject;

	static constexpr float minX = -8.0f;
	static constexpr float maxX = 8.0f;
	static constexpr float minY = -2.0f;
	static constexpr float maxY = 4.0f;
	static constexpr float minZ = -6.0f;
	static constexpr float maxZ = -8.0f;

	Object(string _name, string _tag, vec3 _pos, vec3 _color, const char* _model,float _radius);

	void MoveTo(float x, float y, float z);

	void Draw_simply(Shader& shader,Camera& camera,bool op = false);

	bool IsShooted(Camera &camera)const;

	virtual void RandomMove();
};

// class Light : public Object{
// };

// class Enemy : Object{
// 	inline static const float minX = -30.0f;
// 	inline static const float maxX = 30.0f;
// 	inline static const float minY = 0.0f;
// 	inline static const float maxY = 20.0f;
// 	inline static const float minZ = -5.0f;
// 	inline static const float maxZ = -20.0f;
//
// public:
// 	Enemy(vec3 _pos, vec3 _color, const char* _path, string _name):
// 		Object(_pos, _color, _path, _name, "Enemy"){
// 	};
// 	void RandomMove() override{
// 		float x = rand() % 100 / 100.0f * (maxX - minX) + minX;
// 		float y = rand() % 100 / 100.0f * (maxY - minY) + minY;
// 		float z = rand() % 100 / 100.0f * (maxZ - minZ) + minZ;
// 		MoveTo(x, y, z);
// 	}
// };

#endif //OBJECT_H
