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
Renderer::Renderer(GLFWwindow* vWindow, std::vector<string> texs, 
	std::vector<std::shared_ptr<Visible>> act, 
	std::vector<std::shared_ptr<Camera>> cams, 
	std::shared_ptr<DirectionalLight> dirLight,
	std::vector<std::shared_ptr<PointLight>> point,
	std::vector<std::shared_ptr<SpotLight>> spot)
{
	//map = Map::loadMap(mapDir, shared_from_this()); NEW
	window = vWindow;

	std::vector<unsigned int> index;
	std::vector<unsigned int> lIndex;

	//for (std::string tex : map->getTextureDirections()) textures.push_back({tex, loadTexture(tex)}); NEW
	for (std::string tex : texs) textures.push_back({ tex, loadTexture(tex) }); // LEGACY

	// LEGACY
	for (unsigned int i = 0; i < cams.size(); i++) cameras.push_back(cams.at(i));
	// LEGACY
	unsigned int prevMod = 0;

	for (unsigned int i = 0; i < act.size(); i++) {
		actors.push_back(act.at(i));
		act.at(i)->getModel(positions);
		act.at(i)->getModelIArray(index, prevMod);
	}

	prevMod = 0;
	if (dir = dirLight) { 
		lighting.push_back(dir);
		dirLight->getModel(lPositions);
		dirLight->getModelIArray(lIndex, prevMod);
	}

	for (unsigned int i = 0; i < point.size(); i++) {
		lighting.push_back(point.at(i));
		poi.push_back(point.at(i));
		point.at(i)->getModel(lPositions);
		point.at(i)->getModelIArray(lIndex, prevMod);
	}

	for (unsigned int i = 0; i < spot.size(); i++) {
		lighting.push_back(spot.at(i));
		sp.push_back(spot.at(i));
		spot.at(i)->getModel(lPositions);
		spot.at(i)->getModelIArray(lIndex, prevMod);
	}

	std::cout << std::endl;
	for (size_t i = 0; i < lIndex.size() / 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			std::cout << lIndex[i * 3 + j] << "\t";
		}
		std::cout << std::endl;
	}

	vb = std::make_unique<VertexBuffer>(positions.data(), 1000 * VERTEX_SIZE);
	lvb = std::make_unique<VertexBuffer>(lPositions.data(), 1000 * VERTEX_SIZE);

	layout.push<float>(3);
	layout.push<float>(3);
	layout.push<float>(2);
	layout.push<float>(1);
	layout.push<float>(1);

	va.addBuffer(*vb, layout);
	lva.addBuffer(*lvb, layout);

	ib = std::make_unique <IndexBuffer>(index.data(), index.size());
	lib = std::make_unique <IndexBuffer>(lIndex.data(), lIndex.size());

	lightShader = std::make_unique<Shader>("res/shaders/vertex/basic.vertex.shader", "res/shaders/fragment/lightSource.fragment.shader");
	shader = std::make_unique<Shader>("res/shaders/vertex/basic.vertex.shader", "res/shaders/fragment/light.fragment.shader");
	lightShader->bind();
	shader->bind();

	glm::vec3 modelCoords(0.0f, 0.0f, 0.0f);
	for (size_t i = 0; i < actors.size(); i++) model.push_back(glm::translate(glm::mat4(1.0f), modelCoords));
	for (size_t i = 0; i < prevMod; i++) lightModel.push_back(glm::translate(glm::mat4(1.0f), modelCoords));

	Position camPos = cameras[currentCam]->getTransform().getPosition();

	diff = loadTexture("res/images/container2.png");
	spec = loadTexture("res/images/container2_specular.png");

	shader->setUniform1i("u_material.diffuse", 1);
	shader->setUniform1i("u_material.specular", 2);
	shader->setUniform1f("u_material.shininess", actors[0]->getMaterial().shininess);

	shader->setUniformDirectional("u_dirLight", *dirLight);	// LEGACY
	shader->setUniformPointa("u_pointLights", poi); // LEGACY
	shader->setUniformSpota("u_spotLights", sp); //LEGACY

	shader->setUniform3f("u_camPos", camPos.x, camPos.y, camPos.z);
	lightShader->setUniform1i("u_texSize", 16);
	shader->setUniformMat4fa("u_model", model);
	lightShader->setUniformMat4fa("u_model", lightModel);
	shader->setUniformMat4f("u_view", cameras.at(currentCam)->getView());
	lightShader->setUniformMat4f("u_view", cameras.at(currentCam)->getView());
	shader->setUniformMat4f("u_proj", cameras.at(currentCam)->getProjection());
	lightShader->setUniformMat4f("u_proj", cameras.at(currentCam)->getProjection());

	onActorUpdate();
	onLightUpdate();

	va.unbind();
	lva.unbind();
	shader->unbind();
	lightShader->unbind();
	vb->unbind();
	ib->unbind();
	lvb->unbind();
	lib->unbind();

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

	cameras.at(currentCam)->onUpdate(deltaTime);
	//}
}

void Renderer::updateScreenCam(int w, int h)
{
	cameras[currentCam]->setResolution(w, h);
}

void Renderer::draw()
{
	//if (bIsReseting)
	//{
	shader->bind();

	for (size_t i = 0; i < actors.size(); i++) model.at(i) = actors[i]->getTransform().getTransformMatrix();
	shader->setUniformMat4fa("u_model", model);
	shader->setUniformMat4f("u_view", cameras.at(currentCam)->getView());
	shader->setUniformMat4f("u_proj", cameras.at(currentCam)->getProjection());
	shader->setUniformSpota("u_spotLights", sp); //LEGACY
	
	std::vector<int> samplers;

	for (int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures.at(i).ID);
		samplers.push_back(i);
	}

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, diff);
	// bind specular map
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, spec);

	Position camPos = cameras[currentCam]->getTransform().getPosition();

		shader->setUniform3f("u_camPos", camPos.x, camPos.y, camPos.z);
		shader->setUniform1iv("u_texColor", samplers.size(), samplers.data());

		va.bind();
		ib->bind();

		glDrawElements(GL_TRIANGLES, ib->getCount(), GL_UNSIGNED_INT, nullptr);

		va.unbind();
		ib->unbind();
		shader->unbind();
		
		lightShader->bind();

		for (size_t i = 0; i < lighting.size(); i++) lightModel.at(i) = lighting[i]->getTransform().getTransformMatrix();

		lightShader->setUniformMat4fa("u_model", lightModel);
		lightShader->setUniformMat4f("u_view", cameras.at(currentCam)->getView());
		lightShader->setUniformMat4f("u_proj", cameras.at(currentCam)->getProjection());

		lightShader->setUniform1iv("u_texColor", samplers.size(), samplers.data());

		lva.bind();
		lib->bind();

		glDrawElements(GL_TRIANGLES, lib->getCount(), GL_UNSIGNED_INT, nullptr);

		lva.unbind();
		lib->unbind();
		lightShader->unbind();

	//}
}

void Renderer::initObservers(const std::vector<std::shared_ptr<Observable>>& actor) {
	for (const auto& act : actor) {
		act->addObserver(shared_from_this());
	}
}

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

void Renderer::onActorUpdate()
{
	std::vector<float> newPositions;

	for (auto& actor : actors) {
		actor->getModel(newPositions);
	}

	vb->bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, newPositions.size() * sizeof(float), newPositions.data());
}

void Renderer::onLightUpdate()
{
	std::vector<float> newPositions;

	for (auto& lit : lighting) {
		lit->getModel(newPositions);
	}

	lvb->bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, newPositions.size() * sizeof(float), newPositions.data());
}
