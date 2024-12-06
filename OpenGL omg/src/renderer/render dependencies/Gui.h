#pragma once

#include "../Coords/CoordAttributes.h"
#include <GL/glew.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Gui
{
public:
	Gui(GLFWwindow* window, unsigned int styleColors, bool sDemoWindow, bool sAnotherWindow, ImVec4 clearColor,
		Rotation* pBright, Position* pModelCoords);
	~Gui();

	void createFrame();
	void guiCreate();

private:
	bool show_demo_window;
	bool show_another_window;
	ImVec4 clear_color;
	ImGuiIO* io;


	Rotation* bright;
	Position* modelCoords;

	void guiManager();
};