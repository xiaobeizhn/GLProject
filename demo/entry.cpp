#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// #include "mesh.h"
#include "model.h"
#include "shader.h"
#include "camera.h"
#include "input_system.h"
#include "object.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace InputSystem;
using namespace glm;

Camera camera;
int shadercase;
vector<std::unique_ptr<Object>> objPool;

namespace RenderObject
{
	constexpr float crosshairVertices[]={
		-0.03f, 0.0f,   // 水平线左端点
		 0.03f, 0.0f,   // 水平线右端点
		 0.0f, -0.04f,  // 垂直线下端点
		 0.0f,  0.04f   // 垂直线上端点
	};
	GLuint crosshairVAO, crosshairVBO;
	//初始化准心
	inline void CrosshairInit() {
		glGenVertexArrays(1, &crosshairVAO);
		glGenBuffers(1, &crosshairVBO);
		glBindVertexArray(crosshairVAO);
		glBindBuffer(GL_ARRAY_BUFFER, crosshairVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(crosshairVertices), crosshairVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);
	}

	constexpr float quadVertices[] = {
		// 只需要位置(x,y)
		-1.0f,  1.0f,  // 左上
		-1.0f, -1.0f,  // 左下
		 1.0f, -1.0f,  // 右下

		-1.0f,  1.0f,  // 左上
		 1.0f, -1.0f,  // 右下
		 1.0f,  1.0f   // 右上
	};
	GLuint quadVAO, quadVBO;
	//初始化全屏四边形
	inline void QuadInit() {
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
		glBindVertexArray(0);
	}

	//初始化帧缓冲
	GLuint backFBO,backRBO,backTexture;
	inline void BackFrameBufferInit(){
		//帧缓冲
		glGenFramebuffers(1,&backFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, backFBO);
		//颜色纹理
		glGenTextures(1,&backTexture);
		glBindTexture(GL_TEXTURE_2D, backTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,  WindowState::WINDOW_WIDTH, WindowState::WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_INT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, backTexture, 0);
		//渲染缓冲对象
		glGenRenderbuffers(1, &backRBO);
		glBindRenderbuffer(GL_RENDERBUFFER, backRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WindowState::WINDOW_WIDTH, WindowState::WINDOW_HEIGHT);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, backRBO);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cerr << "Framebuffer not complete!" << std::endl;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
inline void Render3DScene(){
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	if(shadercase == 0){
		Shader shader1("texture_3d.vert", "texture_3d.frag");
		objPool[0]->Draw_simply(shader1,camera,true);
		Shader shader2("simple_3d.vert", "simple_3d.frag");
		objPool[1]->Draw_simply(shader2,camera);
		objPool[2]->Draw_simply(shader2,camera);
	}else if(shadercase == 1){
		Shader shader1("texture_3d.vert", "texture_3d.frag");
		objPool[0]->Draw_simply(shader1,camera,true);
		Shader shader2("simple_3d.vert", "smooth_3d.frag");
		objPool[1]->Draw_simply(shader2,camera);
		objPool[2]->Draw_simply(shader2,camera);
	}else if(shadercase == 2){
		Shader shader1("texture_3d.vert", "texture_3d.frag");
		objPool[0]->Draw_simply(shader1,camera,true);
		Shader shader2("simple_3d.vert", "cartoon_3d.frag");
		objPool[1]->Draw_simply(shader2,camera);
		objPool[2]->Draw_simply(shader2,camera);
	}
}


void DrawCrosshair(){
	Shader Shader2D("simple_2d.vert","simple_2d.frag");
	Shader2D.Use();
	Shader2D.SetVec3("ObjectColor", 1.0f, 1.0f, 1.0f); // 白色准心
	glBindVertexArray(RenderObject::crosshairVAO);
	glDrawArrays(GL_LINES, 0, 4);
	glBindVertexArray(0);
}

inline void Render2DScene(){
	DrawCrosshair();
}

inline void Draw3DScene(){
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	Shader screenShader("simple_2d.vert", "texture_2d.frag");  // 新建屏幕着色器
	screenShader.Use();
	screenShader.SetInt("ourTexture", 0);
	glBindVertexArray(RenderObject::quadVAO);
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, RenderObject::backTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

inline void RenderFrame(){
	glBindFramebuffer(GL_FRAMEBUFFER, RenderObject::backFBO);
	Render3DScene();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Draw3DScene();
	// Draw2DScene();
	DrawCrosshair();
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
		shadercase=(shadercase + 1)%3;
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
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))){
		cout << "Fail to initialize GLAD" << endl;
		return -1;
	}
	glViewport(0, 0,WindowState::WINDOW_WIDTH,WindowState::WINDOW_HEIGHT);

	InputSystem::Init(window);
	RenderObject::CrosshairInit();
	RenderObject::QuadInit();
	RenderObject::BackFrameBufferInit();

	objPool.emplace_back(new Object("Cube", "Friend", vec3(0.0f, 0.0f, -4.0f),
	                                vec3(0.2f, 0.5f, 0.5f), FileSystem::modelPath[0],
	                                1.0f));
	objPool.emplace_back(new Object("Monkey", "Enemy", vec3(0.0f, 0.0f, -6.0f),
	                                vec3(0.5f, 0.0f, 0.5f), FileSystem::modelPath[1],
	                                1.0f));
	objPool.emplace_back(new Object("Res", "Friend", vec3(0.0f, -1.0f, -6.0f),
	                                vec3(0.0f, 0.3f, 0.3f), FileSystem::modelPath[2],
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
