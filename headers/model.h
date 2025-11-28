#ifndef MODEL_H
#define MODEL_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <vector>
#include <string>
using namespace std;
using namespace glm;

#include <stb_image.h>

#include <mesh.h>
#include <shader.h>

GLuint TextureFromFile(const char *path, const string &directory, bool gamma = false);

class Model{
	// static unordered_map<string,Texture> textures_loaded;
	vector<Mesh>meshes;
	string directory;

	void loadModel(string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	// vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
public:
	vec3 aabbMin,aabbMax;
	Model(const char* path);
	void Draw(Shader& shader);
};


#endif //MODEL_H
