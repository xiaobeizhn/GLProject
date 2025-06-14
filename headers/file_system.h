//
// Created by zhn on 25-6-12.
//

#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include <fstream>
#include <filesystem>
#include <string>
#include <iostream>
using namespace std;

namespace FileSystem
{
	inline constexpr const char* modelPath[] = {"assets/models/Cube.obj", "assets/models/Monkey.obj", "assets/models/res.obj"};
	inline const string& VERT_SHADER_DIR("shaders/vertex/");
	inline const string& FRAG_SHADER_DIR("shaders/fragment/");

	inline string VertShader(const char* path){
		return VERT_SHADER_DIR + string(path);
	}
	inline string FragShader(const char* path){
		return FRAG_SHADER_DIR + string(path);
	}

	inline string readFileToString(const string& filePath) {
		ifstream file;
		string ret;
		file.exceptions (ifstream::failbit | ifstream::badbit);
		try{
			file.open(filePath);
			stringstream buffer;
			buffer << file.rdbuf();
			file.close();
			ret = buffer.str();
		}catch(ifstream::failure &e){
			cout << "READ " << filePath << " ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ : " <<e.what()<< endl;
		}
		return ret;
	}
}

#endif //FILESYSTEM_H
