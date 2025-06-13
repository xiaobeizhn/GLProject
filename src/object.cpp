//
// Created by zhn on 25-6-11.
//
#include "object.h"

Object::Object(string _name, string _tag, vec3 _pos, vec3 _color, const char* _model,float _radius):
	name(move(_name)),
	tag(move(_tag)),
	pos(_pos),
	color(_color),
	model(_model),
	shootedObject(_pos,_radius) {
};

void Object::Draw_simply(Shader& shader, Camera& camera, bool op){
	shader.Use();
	mat4 modelMatrix = mat4(1.0f);
	modelMatrix = translate(modelMatrix, pos);
	auto viewMatrix = camera.GetViewMatrix();
	mat4 projectionMatrix = perspective(
		radians(45.0f),
		static_cast<float>(WindowState::WINDOW_WIDTH) / static_cast<float>(WindowState::WINDOW_HEIGHT),
		0.1f,
		100.0f
	);
	// 传递变换矩阵
	shader.setMat4("projection", projectionMatrix);
	shader.setMat4("vie", viewMatrix);
	shader.setMat4("model", modelMatrix);

	// 传递光照参数
	shader.setVec3("lightPos", 5.0f, 5.0f, -3.0f);
	shader.setVec3("viewPos", camera.Position);
	shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	if(op){
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		// 为当前绑定的纹理对象设置环绕、过滤方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// 加载并生成纹理
		int width, height, nrChannels;
		unsigned char *data = stbi_load("assets/textures/wall.jpg", &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		} else {
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
	}else{
		shader.setVec3("objectColor", this->color);
	}
	model.Draw(shader);
}

bool Object::IsShooted(Camera& camera)const{
	vec4 viewSpacePos = camera.GetViewMatrix() *  vec4(pos, 1.0f);
	vec3 clipSpacePos = static_cast<vec3>(viewSpacePos) / viewSpacePos.w;
	cout<<clipSpacePos.x<<" "<<clipSpacePos.y<<" "<<clipSpacePos.z<<endl;
	return (clipSpacePos.x * clipSpacePos.x + clipSpacePos.y * clipSpacePos.y <= shootedObject.radius * shootedObject.radius);
}

void Object::RandomMove(){
	float x = rand() % 100 / 100.0f * (maxX - minX) + minX;
	float y = rand() % 100 / 100.0f * (maxY - minY) + minY;
	float z = rand() % 100 / 100.0f * (maxZ - minZ) + minZ;
	MoveTo(x, y, z);
};
void Object::MoveTo(float x, float y, float z){
	pos.x = x;
	pos.y = y;
	pos.z = z;
	shootedObject.center = pos;
}
