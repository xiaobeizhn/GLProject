#ifndef MESH_H
#define MESH_H
#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

#include <string>
#include <vector>

using namespace std;

struct Vertex{
	glm::vec3 Position;
	glm::vec3 Normal;
	void Print(){
		cout << "Position: " << Position.x << " " << Position.y << " " << Position.z << endl;
		cout << "Normal: " << Normal.x << " " << Normal.y << " " << Normal.z << endl;
		cout<<endl;
	}
	glm::vec2 TexCoord;
};

struct Texture{
	GLuint id;
	std::string type;
};

class Mesh{
public:
	std::vector<Vertex>vertices;
	std::vector<unsigned int>indices;
    // std::vector<Texture>textures;
	unsigned int VAO, VBO, EBO;
	// vec3 aabbMin,aabbMax;

	// Mesh(std::vector<Vertex>_vertices,std::vector<unsigned int>_indices,std::vector<Texture>_textures){
	// 	this->vertices = _vertices;
	// 	this->indices = _indices;
	// 	this->textures = _textures;
	// 	setupMesh();
	// }

	Mesh(std::vector<Vertex>_vertices,std::vector<unsigned int>_indices){
		this->vertices = _vertices;
		this->indices = _indices;
		setupMesh();
	}
	Mesh(Mesh&& o) noexcept
	   :vertices(std::move(o.vertices)),
		indices(std::move(o.indices)),
		// aabbMax(std::move(o.aabbMax)),
		// aabbMin(std::move(o.aabbMin)),
		// textures(std::move(o.textures)),
		VAO(o.VAO),VBO(o.VBO),EBO(o.EBO){ };

	// void Draw(Shader &shader);
	void Draw_simply(Shader &shader,bool op = false);
	// void Draw();
	void setupMesh();
};
inline void Mesh::setupMesh(){
	glGenVertexArrays(1,&VAO);
	glGenBuffers(1,&VBO);
	glGenBuffers(1,&EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,vertices.size() * sizeof(Vertex), &vertices[0],GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(GLuint),&indices[0],GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),nullptr);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),reinterpret_cast<const void*>(offsetof(Vertex, Normal)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),reinterpret_cast<const void*>(offsetof(Vertex, TexCoord)));

	glBindVertexArray(0);
}

inline void Mesh::Draw_simply(Shader &shader,bool op){
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT,nullptr);
	glBindVertexArray(0);
}

// void Mesh::Draw(Shader& shader){
// 	GLuint diffuseNr = 1;
// 	GLuint specularNr = 1;
// 	for(int i=0;i<textures.size();i++){
// 		glActiveTexture(GL_TEXTURE0 + i);
// 		std::string number;
// 		std::string name = textures[i].type;
// 		if(name == "texture_diffuse")
// 			number = std::to_string(diffuseNr++);
// 		else if(name == "texture_specular"){
// 			number = std::to_string(specularNr++);
// 		}
//
// 		shader.SetInt(("material." + name + number).c_str(),i);
// 		glBindTexture(GL_TEXTURE_2D,textures[i].id);
// 	}
//
// 	glActiveTexture(GL_TEXTURE0);
// 	glBindVertexArray(VAO);
// 	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT,nullptr);
// 	glBindVertexArray(0);
// }

#endif //MESH_H
