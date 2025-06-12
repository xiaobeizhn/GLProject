#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// #include "mesh.h"
#include "model.h"
#include "shader.h"
#include "camera.h"
#include "inputsystem.h"
#include "object.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace InputSystem;
using namespace glm;

Camera camera;
int shadercase;
GLuint backFbo,backTexture;

float crosshairVertices[]={
	-0.02f, 0.0f,   // 水平线左端点
	 0.02f, 0.0f,   // 水平线右端点
	 0.0f, -0.02f,  // 垂直线下端点
	 0.0f,  0.02f   // 垂直线上端点
};

GLuint crosshairVAO, crosshairVBO;

const char* modelPath[] = {"assets/models/Cube.obj", "assets/models/Monkey.obj", "assets/models/res.obj"};

vector<std::unique_ptr<Object>> objPool;

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

inline void Render3DScene(){
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	if(shadercase == 0){
		Shader shader("simple_3d.vert", "simple_3d.frag");
		for (const auto& obj : objPool){
			obj->Draw_simply(shader,camera);
		}
	}else if(shadercase == 1){
		Shader shader("simple_3d.vert", "smooth_3d.frag");
		for (const auto& obj : objPool){
			obj->Draw_simply(shader,camera);
		}
	}else if(shadercase == 2){
		Shader shader("simple_3d.vert", "cartoon_3d.frag");
		for (const auto& obj : objPool){
			obj->Draw_simply(shader,camera);
		}
	}else if(shadercase == 3){
		Shader shader1("texture_3d.vert", "texture_3d.frag");
		objPool[0]->Draw_simply(shader1,camera,true);
		Shader shader2("simple_3d.vert", "cartoon_3d.frag");
		objPool[1]->Draw_simply(shader2,camera);
		objPool[2]->Draw_simply(shader2,camera);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Shader Shader2D("simple_2d.vert","simple_2d.frag");

void DrawCrosshair(){
	Shader2D.Use();
	Shader2D.setVec3("Color", 1.0f, 1.0f, 1.0f); // 白色准心
	glBindVertexArray(crosshairVAO);
	glDrawArrays(GL_LINES, 0, 4);
	glBindVertexArray(0);
}

inline void Render2DScene(){
	DrawCrosshair();
}

inline void RenderFramebufferTexture(){

}

inline void RenderFrame(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Render3DScene();
	glClear(GL_COLOR_BUFFER_BIT );
	RenderFramebufferTexture();
	glDisable(GL_DEPTH_TEST);
	Render2DScene();
	glEnable(GL_DEPTH_TEST);

}

double deltaTime = 0.0f;
double lastFrame = 0.0f;

void ProcessReset(){
	double currentTime = glfwGetTime();
	deltaTime = currentTime - lastFrame;
	lastFrame = currentTime;
	CursorState::deltaX = 0;
	CursorState::deltaY = 0;
	MouseState::isLeftClick = false;
	MouseState::isRightClick = false;
}

int gameState;

void ProcessInput(GLFWwindow* window){
	camera.ProcessMouseMovement(static_cast<float>(CursorState::deltaX),
	                            static_cast<float>(CursorState::deltaY));
	camera.ProcessMouseScroll(static_cast<float>(CursorState::deltaY));
	camera.ProcessKeyboard(KeyboardState::moveX, KeyboardState::moveY, KeyboardState::moveU,
	                       static_cast<float>(deltaTime));
	if (MouseState::isLeftClick){
		for (const auto& obj : objPool){
			cout<<obj->name<<":"<<endl;
			if (obj->tag == "Enemy" && obj->IsShooted(camera)){
				cout<<"Shooted"<<endl;
				obj->RandomMove();
			}
		}
	}
	if(MouseState::isRightClick){
		shadercase=(shadercase + 1)%4;
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		exit(0);
	}
	// if (gameState == 0){
	// 	if (MouseState::isLeftClick){
	// 		gameState = 1;
	// 		cout << "Game Start" << endl;
	// 	} else if (gameState == 1){
	// 		cout << "Game Playing" << endl;
	// 	}
	// }
}


int main(){
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	auto* window = glfwCreateWindow(WindowState::WINDOW_WIDTH,WindowState::WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == nullptr){
		cout << "Fail to create window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetMouseButtonCallback(window, MouseState::MouseButtonCallback);
	glfwSetCursorPosCallback(window, CursorState::CursorPositionCallback);
	glfwSetScrollCallback(window, ScrollState::ScrollCallback);
	glfwSetKeyCallback(window, KeyboardState::KeyCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(window, WindowState::FrameBufferSizeCallback);
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))){
		cout << "Fail to initialize GLAD" << endl;
		return -1;
	}
	//初始化帧缓冲
	glGenFramebuffers(1,&backFbo);
	glGenTextures(1,&backTexture);
	glBindTexture(GL_TEXTURE_2D, backTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,  WindowState::WINDOW_WIDTH, WindowState::WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_INT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindFramebuffer(GL_FRAMEBUFFER, backFbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, backTexture, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "Framebuffer not complete!" << std::endl;
	}

	//初始化准心
	glGenVertexArrays(1, &crosshairVAO);
	glGenBuffers(1, &crosshairVBO);
	glBindVertexArray(crosshairVAO);
	glBindBuffer(GL_ARRAY_BUFFER, crosshairVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(crosshairVertices), crosshairVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	glViewport(0, 0,WindowState::WINDOW_WIDTH,WindowState::WINDOW_HEIGHT);
	objPool.emplace_back(new Object("Cube", "Friend",vec3(-2.0f, 0.0f, -8.0f), vec3(1.0f, 0.2f, 0.2f), modelPath[0],
	                                  1.0f));
	objPool.emplace_back(new Object("Monkey","Enemy",vec3(2.0f, 0.0f, -6.0f), vec3(0.5f, 0.0f, 0.5f), modelPath[1],
	                               1.0f));
	objPool.emplace_back(new Object("Res","Friend",vec3(0.0f, 2.0f, -10.0f), vec3(0.0f, 0.3f, 0.3f), modelPath[2],
								   1.0f));
	// cout<<"OBJPOOL SIZE"<<objPool.size()<<endl;
	while (!glfwWindowShouldClose(window)){
		// 1. 重置帧相关状态（如delta值）
		ProcessReset();
		// 2. 处理所有待处理事件（触发回调更新状态）
		glfwPollEvents();
		// 3. 基于当前状态处理游戏逻辑
		ProcessInput(window);
		// 4. 渲染等其他操作
		RenderFrame();
		// 5. 交换缓冲
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}
