#include "TestClearColor.h"
#include <GL/glew.h>
#include "imgui/imgui.h"

namespace test {
	TestClearColor::TestClearColor() : clearColor { 0.2f, 0.3f, 0.8f, 1.0f }
	{
	}

	TestClearColor::~TestClearColor()
	{
	}

	void TestClearColor::onUpdate(float deltaTime)
	{
	}

	void TestClearColor::onRender()
	{
		glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void TestClearColor::onGuiRender()
	{
		ImGui::ColorEdit4("color", clearColor);
	}
}