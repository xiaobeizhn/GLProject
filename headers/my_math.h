//
// Created by zhn on 25-11-27.
//

#ifndef MY_MATH_H
#define MY_MATH_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <iostream>
using namespace glm;
using namespace std;

#define Debug2(var) std::cout<<#var<<" "<<(var).x<<" "<<(var).y<<std::endl;
#define Debug3(var) std::cout<<#var<<" "<<(var).x<<" "<<(var).y<<" "<<(var).z<<std::endl;

namespace my_math
{
	const float epsf = 1e-6f;
}

bool RayAABBIntersection(vec2 Origin,vec2 Dir, vec2 AABBMin, vec2 AABBMax);
bool RayAABBIntersection(vec3 Origin,vec3 Dir, vec3 AABBMin, vec3 AABBMax);

#endif //MY_MATH_H
