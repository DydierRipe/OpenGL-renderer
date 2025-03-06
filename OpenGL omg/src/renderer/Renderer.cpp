#include "Renderer.h"
// #include "../Actor/Map.h"

#include "../Actor/Camera.h"
#include "../Actor/Visible.h"
#include "../Actor/lighting/DirectionalLight.h"
#include "../Actor/lighting/SpotLight.h"
#include "../Actor/lighting/PointLight.h"
#include <GLFW/glfw3.h>
#include <Windows.h>
constexpr unsigned int VERTEX_SIZE = sizeof(float[10]);

/*const std::string& mapDir*/
Renderer::Renderer(GLFWwindow* vWindow, //std::vector<string> texs, 
	std::vector<std::shared_ptr<Visible>> act, 
	std::vector<std::shared_ptr<Camera>> cams, 
	std::shared_ptr<DirectionalLight> dirLight,
	std::vector<std::shared_ptr<PointLight>> point,
	std::vector<std::shared_ptr<SpotLight>> spot)
{
	//map = Map::loadMap(mapDir, shared_from_this()); NEW
	window = vWindow;

	//for (std::string tex : map->getTextureDirections()) textures.push_back({tex, loadTexture(tex)}); NEW

	// LEGACY
	cameras = cams;
	// LEGACY
	actors = act;

	if (dir = dirLight) { 
		lighting.push_back(dir);
	}

	for (unsigned int i = 0; i < point.size(); i++) {
		lighting.push_back(point.at(i));
		poi.push_back(point.at(i));
	}

	for (unsigned int i = 0; i < spot.size(); i++) {
		lighting.push_back(spot.at(i));
		sp.push_back(spot.at(i));
	}

	shader = std::make_unique<Shader>("res/shaders/vertex/basic.vertex.shader", "res/shaders/fragment/texture.fragment.shader");
	shader->bind();

	glm::vec3 modelCoords(0.0f, 0.0f, 0.0f);
	for (size_t i = 0; i < actors.size(); i++) model.push_back(glm::translate(glm::mat4(1.0f), modelCoords));

	Position camPos = cameras[currentCam]->getTransform().getPosition();

	shader->setUniformDirectional("u_dirLight", *dirLight);	// LEGACY
	//shader->setUniformPointa("u_pointLights", poi); // LEGACY
	//shader->setUniformSpota("u_spotLights", sp); //LEGACY

	shader->setUniform3f("u_camPos", camPos.x, camPos.y, camPos.z);
	shader->setUniformMat4fa("u_model", model);
	shader->setUniformMat4f("u_view", cameras.at(currentCam)->getView());
	shader->setUniformMat4f("u_proj", cameras.at(currentCam)->getProjection());

	//onActorUpdate();
	//onLightUpdate();

	std::cout << "AAAAAAAAAAAA" << std::endl;

	shader->unbind();
}

Renderer::~Renderer()
{
}

void Renderer::onNotify(std::string eventID)
{
	if (eventID == "ACTOR 1 MOVE") onActorUpdate();
}

void Renderer::onUpdate(float deltaTime)
{
	//if (bIsReseting)
	//{
	/*if (GetKeyState('0') & 0x8000) {
		currentCam = 0;

		if (window) {
			glfwSetWindowUserPointer(window, cameras[currentCam].get());
			glfwSetCursorPosCallback(window, [](GLFWwindow* window, double posx, double posy) {
				Camera* mainCam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
				mainCam->mouseCamRotation(posx, posy);
			});
		}
	}

	if (GetKeyState('1') & 0x8000) {
		currentCam = 1;
		if (window) {
			glfwSetWindowUserPointer(window, cameras[currentCam].get());
			glfwSetCursorPosCallback(window, [](GLFWwindow* window, double posx, double posy) {
				Camera* mainCam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
				mainCam->mouseCamRotation(posx, posy);
			});
		}
	}*/

	if (GetKeyState('0') & 0x8000) 
		currentCam = 0;

	if (GetKeyState('1') & 0x8000)
		currentCam = 1;

	if (GetKeyState('R') & 0x8000)
		cameras.at(currentCam)->getTransform().setPosition(0);

	cameras.at(currentCam)->onUpdate(deltaTime);
	//}
}

void Renderer::updateScreenCam(int w, int h)
{
	cameras[currentCam]->setResolution(w, h);
}

/*
	WERE GOING INSANE AAAAAAAAAAAAAAAAAA
	this is literally a mess because ur trying to make this work, so here are some ideas to make this be functional:
		* model probably should go in Visible
		* dont know what u will do since now the model thing is executed once per mesh, but just in case, make that just a single model and
		  then go on and dunno, whatever
		* good luck trying to clean this up and make the whole system go along with the json properties API
*/

void Renderer::draw()
{
	//if (bIsReseting)
	//{
	shader->bind();

	shader->setUniformMat4f("u_view", cameras.at(currentCam)->getView());
	shader->setUniformMat4f("u_proj", cameras.at(currentCam)->getProjection());
	//shader->setUniformSpota("u_spotLights", sp); //LEGACY
	//shader->setUniformPointa("u_pointLights", poi); //LEGACY

	Position camPos = cameras[currentCam]->getTransform().getPosition();
	shader->setUniform3f("u_camPos", camPos.x, camPos.y, camPos.z);
	
	for (auto& act : actors)
	{
		act->draw(*shader.get());
	}

	/*for (auto& lit : lighting)
	{
		lit->draw(*shader.get());
	}*/

	shader->unbind();

	//}
}

void Renderer::initObservers(const std::vector<std::shared_ptr<Observable>>& actor) {
	for (const auto& act : actor) {
		act->addObserver(shared_from_this());
	}
}

// LEGACY
unsigned int Renderer::loadTexture(const std::string& path)
{
	int w, h, bpp;

	stbi_set_flip_vertically_on_load(1);
	auto* pixels = stbi_load(path.c_str(), &w, &h, &bpp, STBI_rgb_alpha);

	GLuint texID = 0;

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	stbi_image_free(pixels);

	return texID;
}

// LEGACY
void Renderer::onActorUpdate()
{
	std::vector<float> newPositions;

	for (auto& actor : actors) {
		actor->getModel(newPositions);
	}

	//vb->bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, newPositions.size() * sizeof(float), newPositions.data());
}

// LEGACY
void Renderer::onLightUpdate()
{
	std::vector<float> newPositions;

	for (auto& lit : lighting) {
		lit->getModel(newPositions);
	}

	//lvb->bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, newPositions.size() * sizeof(float), newPositions.data());
}
