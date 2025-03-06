#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>
#include <cmath>
#include <algorithm>
#include <vector>
#include <memory>
#include <json/json.hpp>

#include "Actor/Camera.h"
#include "Actor/Visible.h"
#include "Actor/lighting/DirectionalLight.h"
#include "Actor/lighting/SpotLight.h"
#include "Actor/lighting/PointLight.h"
#include "renderer/Renderer.h"
#include "tests/TestClearColor.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

/*
	in order to implement other things before getting into those advanced topics such as instancing, it is better for you to adapt the code to
	before the map class was implemented. code such as config may be useful, but just go simple and gp back to the old map implementation model
	(implementing everything in main) and then you can make it better with time. you still have to implement lots of better things.
*/

struct WindowData {
	Camera* mainCam;
	Renderer* renderer;
};

class Map {
public:
	Map() : a(0) {

	}

	int a;
};

/*
GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "OpenGL Error: " << error << std::endl;
	}
*/

using std::string;

static void GLAPIENTRY MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	std::cout << stderr << " GL CALLBACK: " << id << " type = " << type << ", severity = " << severity << ", message = " << message <<"\n" <<
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "");
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;


	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(700, 700, "Yellow World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "Omggg an error";

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glClearColor(0, 0.7, 1, 1);

	//std::vector<string> textures = { "res/images/default.png", "res/images/container2.png", "res/images/container2_specular.png"};
	std::vector<std::shared_ptr<Visible>> actors;
	std::vector<std::shared_ptr<Camera>> cameras;
	std::vector<std::shared_ptr<SpotLight>> slighting;
	std::vector<std::shared_ptr<PointLight>> plighting;

	unsigned int actorCount = 0, camCount = 0, litCount = 0;

	auto cam1 = std::make_shared<Camera>();
	auto cam2 = std::make_shared<Camera>();

	cam1->configure(json::parse(R"({
		"resW": 700,
			"resH" : 700,
			"position" : [0.0, 0.0, 30.0] ,
			"rotation" : [-90.0, 0.0, 0.0] ,
			"ortho" : false,
			"FOV" : 60.0,
			"maxRender" : 1000.0,
			"speed": 50
	})"), "mainCam", std::make_shared<Map>(), camCount);

	cam2->configure(json::parse(R"({
		"resW": 700,
			"resH" : 700,
			"position" : [0.0, 0.0, 30.0] ,
			"rotation" : [-90.0, 0.0, 0.0] ,
			"ortho" : false,
			"FOV" : 60.0,
			"maxRender" : 1000.0,
			"speed": 50
	})"), "secCam", std::make_shared<Map>(), camCount);

	cameras.push_back(cam1);
	cameras.push_back(cam2);

	/*
		solo se puede generar mas actores una vez se tenga una clase de mapa, ya que para generar los indices se requiere de saber el numero de vertices 
		del actor anterior. la unica forma de hacer mas actores es que la geometria sea igual (mismos modelos), y aun asi se requiere de diferentes
		matrices de modelo por cada uno.

	*/

	auto actor1 = std::make_shared<Visible>();
	auto actor2 = std::make_shared<Visible>();
	auto actor3 = std::make_shared<Visible>();
	auto actor4 = std::make_shared<Visible>();

	actor1->configure(json::parse(R"({
	  "model": "res/3D models/human/FinalBaseMesh.obj",
	  "position": [ 0.0, 55.0, 40.0 ],
	  "rotation": [ 0.0 ],
	  "size": [ 0.5 ],
	  "material": {
			"shininess": 5
	  }
	})"), "actor1", std::make_shared<Map>(), actorCount);

	actor2->configure(json::parse(R"({
	  "model": "res/3D models/basic_shapes/cube/cube.obj",
	  "position": [ 0.0, -4, 0.0 ],
	  "rotation": [ 0.0],
	  "size": [ 2000, 1, 2000 ],
	  "material": {
			"shininess": 5
	  }
	})"), "actor2", std::make_shared<Map>(), actorCount);

	actor3->configure(json::parse(R"({
	  "model": "res/3D models/woodTower/wooden watch tower2.obj",
	  "position": [ -1.0, -10, 40.0 ],
	  "rotation": [ 0.0, 0.0, 0.0 ],
	  "size": [ 10, 10, 10 ],
	  "material": {
			"shininess": 5
	  }
	})"), "actor3", std::make_shared<Map>(), actorCount);

	actor4->configure(json::parse(R"({
	  "model": "res/3D models/floppa/flopp.obj",
	  "shader": "default",
	  "position": [ 0.0 ],
	  "rotation": [ 0.0, 0.0, 0.0 ],
	  "size": [ 1 ],
	  "material": {
			"shininess": 5
	  }
	})"), "actor3", std::make_shared<Map>(), actorCount);

	auto l1 = std::make_shared<DirectionalLight>();
	auto l2 = std::make_shared<PointLight>();
	auto l3 = std::make_shared<SpotLight>();
	auto l4 = std::make_shared<SpotLight>();

	l1->configure(json::parse(R"({
	  "position": [ 10.0, 0.0, 0.0 ],
	  "rotation": [ 0.0],
	  "size": [ 1.0 ],
	  "illumination": {
			"ambient": [0.3],
			"diffuse": [0.7],
			"specular": [0.1],
			"direction": [0.3, -1, 0.4]
	  }

	})"), "ambient", std::make_shared<Map>(), litCount);

	l2->configure(json::parse(R"({
	  "position": [ 10.0, 5.0, 10.0 ],
	  "rotation": [ 0.0],
	  "size": [ 1.0 ],
	  "illumination": {
			"ambient": [0.3],
			"diffuse": [1],
			"specular": [1],
			"constant": 1.0,
			"linear": 0.045,
			"quadratic": 0.0075
	  }
	})"), "point", std::make_shared<Map>(), litCount);

	l3->configure(json::parse(R"({
	  "position": [ 0.0, 10.0, -5.0 ],
	  "rotation": [ 0.0],
	  "size": [ 1.0 ],
	  "illumination": {
			"ambient": [0.5],
			"diffuse": [1],
			"specular": [0.8],
			"constant": 1.0,
			"linear": 0.0045,
			"quadratic": 0.00075,
			"direction": [0, 1, 0],
			"cutOff": 20.0,
			"innerCutOff": 15.0
	  }, "softenNormal": true
	})"), "spot", std::make_shared<Map>(), litCount);

	l4->configure(json::parse(R"({
	  "position": [ 10.0, 5.0, 10.0 ],
	  "rotation": [ 0.0],
	  "size": [ 1.0 ],
	  "illumination": {
			"ambient": [0.5],
			"diffuse": [1],
			"specular": [0.8],
			"constant": 1.0,
			"linear": 0.045,
			"quadratic": 0.0075,
			"direction": [0, -1, 0],
			"cutOff": 30.0,
			"innerCutOff": 25.0
	  }, "softenNormal": true
	})"), "spot", std::make_shared<Map>(), litCount);

	plighting.push_back(l2);
	slighting.push_back(l3);
	//slighting.push_back(l4);

	cam2->lockViewToObject(actor1);
	l3->setAttachment(cam1);

	actors.push_back(actor1);
	actors.push_back(actor2);
	actors.push_back(actor3);
	//actors.push_back(actor4);

	std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>(window,/* textures,*/ actors, cameras, l1, plighting, slighting);

	std::vector<std::shared_ptr<Observable>> observableActors;
	for (auto& actor : actors) {
		observableActors.push_back(std::static_pointer_cast<Observable>(actor));
	}

	renderer->initObservers(observableActors);

	//Gui mainGui(window, 2, true, false, ImVec4(0.45f, 0.55f, 0.60f, 1.00f), &cam1.rot, &cam1.pos);

	//test::TestClearColor test;

	WindowData wData = { cameras[0].get(), renderer.get() };

	glfwSetWindowUserPointer(window, &wData);

	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double posx, double posy) {
		WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
		
		if (data && data->mainCam) {
			data->mainCam->mouseCamRotation(posx, posy);
		}
	});

	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
		WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
		
		if (data && data->renderer)
		{
			data->renderer->updateScreenCam(width, height);

			glViewport(0, 0, width, height);
		}
	});

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);

	float deltaTime = 0.0f, lastFrame = 0.0f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		float currentFrame = glfwGetTime();

		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		renderer->draw();
		renderer->onUpdate(deltaTime);

		for (int i = 0; i < actors.size(); i++)
		{
			actors.at(i)->onUpdate(deltaTime);
		}

		for (int i = 0; i < plighting.size(); i++)
		{
			plighting.at(i)->onUpdate(deltaTime);
		}

		for (int i = 0; i < slighting.size(); i++)
		{
			slighting.at(i)->onUpdate(deltaTime);
		}

		//mainGui.createFrame();
		//mainGui.guiCreate();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	//shader.unbind();

	glfwTerminate();
	return 0;
}
