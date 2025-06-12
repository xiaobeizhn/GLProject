//
// Created by zhn on 25-6-12.
//
#include "model.h"

Model::Model(const char* path){
	loadModel(path);
}
void Model::Draw(Shader& shader){
	for(int i=0;i<meshes.size();i++){
		meshes[i].Draw_simply(shader);
	}
}
void Model::loadModel(string path){
	Assimp::Importer importer;
	auto *scene = importer.ReadFile(path,aiProcess_Triangulate | aiProcess_FlipUVs);

	if(scene == nullptr || scene->mRootNode == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE){
		cout<<"ERROR::ASSIMP::"<<importer.GetErrorString()<<endl;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}
void Model::processNode(aiNode *node, const aiScene *scene){
	for(int i=0;i<node->mNumMeshes;i++){
		auto *mesh = scene->mMeshes[i];
		meshes.emplace_back(processMesh(mesh, scene));
	}
	for(int i=0;i<node->mNumChildren;i++){
		processNode(node->mChildren[i],scene);
	}
}
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene){
	vector<Vertex>vertices;
	vector<unsigned int>indices;
    // vector<Texture>textures;
	for(GLuint i=0;i<mesh->mNumVertices;i++){
		glm::vec3 _Position;
		_Position.x = mesh->mVertices[i].x;
		_Position.y = mesh->mVertices[i].y;
		_Position.z = mesh->mVertices[i].z;

		glm::vec3 _Normal;
		_Normal.x = mesh->mNormals[i].x;
		_Normal.y = mesh->mNormals[i].y;
		_Normal.z = mesh->mNormals[i].z;

		glm::vec2 _TexCoord(0.0f,0.0f);
		if(mesh->mTextureCoords[0]){
			_TexCoord.x = mesh->mTextureCoords[0][i].x;
			_TexCoord.y = mesh->mTextureCoords[0][i].y;
		}
		vertices.emplace_back(_Position,_Normal,_TexCoord);
		// vertices.emplace_back(_Position,_Normal);
	}
	for(GLuint i=0;i<mesh->mNumFaces;i++){
		auto face = mesh->mFaces[i];
		for(GLuint j=0;j<face.mNumIndices;j++){
			indices.emplace_back(face.mIndices[j]);
		}
	}
/*
	if(mesh->mMaterialIndex >= 0){
		auto *material = scene->mMaterials[mesh->mMaterialIndex];
		vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(),diffuseMaps.end());
		vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(),specularMaps.end());
	} */
	return Mesh(vertices,indices);
}

// vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName){
// 	vector<Texture> textures;
// 	for(GLuint i=0;i<mat->GetTextureCount(type);i++){
// 		aiString str;
// 		mat->GetTexture(type,i,&str);
// 		auto it = textures_loaded.find(str.C_Str());
// 		if(it == textures_loaded.end()){
// 			Texture texture;
// 			texture.id = TextureFromFile(str.C_Str(),directory);
// 			texture.type = typeName;
// 			textures_loaded[str.C_Str()] = texture;
// 			textures.push_back(texture);
// 		}else{
// 			textures.push_back(it->second);
// 		}
// 	}
// 	return textures;
// }

GLuint TextureFromFile(const char *path, const string &directory, bool gamma) {
	string filename = string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data) {
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	} else {
		cout << "Texture failed to load at path: " << path << endl;
		stbi_image_free(data);
	}

	return textureID;
}
