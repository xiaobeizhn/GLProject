#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include <filesystem>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

namespace InputSystem
{
	inline double sensitivity = 0.05;
	namespace WindowState
	{
		inline int WINDOW_WIDTH = 800;
		inline int WINDOW_HEIGHT = 600;
		inline void FrameBufferSizeCallback(GLFWwindow* window, int width, int height){
			glViewport(0, 0, width, height);
		}
	}
	namespace MouseState
	{
		inline bool isLeftPressed = false;
		inline bool isRightPressed = false;
		inline bool isLeftClick = false;
		inline bool isRightClick = false;
		inline void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods){
			if (button == GLFW_MOUSE_BUTTON_LEFT) {
				if (action == GLFW_PRESS){
					if(!isLeftPressed){
						isLeftClick = true;
					}
					isLeftPressed = true;
					cout << "LEFT PRESS" << endl;
				}else if(action == GLFW_REPEAT){
					isLeftClick = false;
				} else if (action == GLFW_RELEASE) {
					isLeftPressed = false;
					isLeftClick = false;
					cout << "LEFT RELEASE" << endl;
				}
			}
			if (button == GLFW_MOUSE_BUTTON_RIGHT) {
				if (action == GLFW_PRESS){
					if(!isRightPressed){
						isRightClick = true;
					}
					isRightPressed = true;
					cout << "RIGHT PRESS" << endl;
				}else if(action == GLFW_REPEAT){
					isRightClick = false;
				} else if (action == GLFW_RELEASE) {
					isRightPressed = false;
					isRightClick = false;
					cout << "RIGHT RELEASE" << endl;
				}
			}
		}
	}

	namespace CursorState
	{
		inline double x = WindowState::WINDOW_WIDTH / 2.0,y = WindowState::WINDOW_HEIGHT / 2.0;
		inline double deltaX = 0,deltaY = 0;
		inline bool firstMouse = true;
		inline void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos){
			if(firstMouse){
				firstMouse = false;
				x = xpos;
				y = ypos;
			}
			deltaX = xpos - x;
			deltaY = y - ypos;
			x = xpos;
			y = ypos;
		}
	};

	namespace ScrollState
	{
		inline double x = 0,y = 0;
		inline double deltaX = 0,deltaY = 0;
		inline void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
			y -= yoffset;
			x += xoffset;
			deltaY = yoffset;
			deltaX = xoffset;
		}
	}

	namespace KeyboardState
	{
		inline int moveX = 0,moveY = 0,moveU = 0;
		inline void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
			if(key==GLFW_KEY_A && action==GLFW_PRESS){
				-- moveX;
			}
			if(key==GLFW_KEY_A && action==GLFW_RELEASE){
				++ moveX;
			}
			if(key==GLFW_KEY_D && action==GLFW_PRESS){
				++ moveX;
			}
			if(key==GLFW_KEY_D && action==GLFW_RELEASE){
				-- moveX;
			}
			if(key==GLFW_KEY_W && action==GLFW_PRESS){
				++ moveY;
			}
			if(key==GLFW_KEY_W && action==GLFW_RELEASE){
				-- moveY;
			}
			if(key==GLFW_KEY_S && action==GLFW_PRESS){
				-- moveY;
			}
			if(key==GLFW_KEY_S && action==GLFW_RELEASE){
				++ moveY;
			}
			if(key==GLFW_KEY_SPACE && action==GLFW_PRESS){
				++ moveU;
			}
			if(key==GLFW_KEY_SPACE && action==GLFW_RELEASE){
				-- moveU;
			}
			if(key==GLFW_KEY_LEFT_SHIFT && action==GLFW_PRESS){
				-- moveU;
			}
			if(key==GLFW_KEY_LEFT_SHIFT && action==GLFW_RELEASE){
				++ moveU;
			}
		}

	}
}

#endif //INPUT_SYSTEM_H
