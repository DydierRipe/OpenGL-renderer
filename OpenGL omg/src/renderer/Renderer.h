#pragma once

#include <memory>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "stb_image/stb_image.h"


#include "../Coords/CoordAttributes.h"
#include "../Observer/Observer.h"
#include "render dependencies/VertexArray.h"
#include "render dependencies/VertexBuffer.h"
#include "render dependencies/IndexBuffer.h"
#include "render dependencies/Shader.h"

class Observable;
class Actor;
class Visible;
class Camera;
class Light;
class DirectionalLight;
class PointLight;
class SpotLight;

struct GLFWwindow;

class Renderer : public Observer, public std::enable_shared_from_this<Renderer>
{
public:
	// Renderer(GLFWwindow* window, const std::string &mapDir); NEW
	Renderer(GLFWwindow* vWindow, //std::vector<string> texs, 
		std::vector<std::shared_ptr<Visible>> act, 
		std::vector<std::shared_ptr<Camera>> cams, 
		std::shared_ptr<DirectionalLight> dirLight,
		std::vector<std::shared_ptr<PointLight>> point, 
		std::vector<std::shared_ptr<SpotLight>> spot); // LEGACY
	~Renderer();

	void updateScreenCam(int w, int h);
	void onUpdate(float deltaTime);

	void onNotify(std::string eventID) override;
	void draw();
	void onActorUpdate();
	void onLightUpdate();

	//void resetData();

	void initObservers(const std::vector<std::shared_ptr<Observable>>& actor);

	//inline std::shared_ptr<Map> getMap() { return map; }
	inline void setCurrentCam(unsigned int cam) { currentCam = cam; }

	static unsigned int loadTexture(const std::string &path);
private:

	GLFWwindow* window;

	bool bIsReseting = true;

	std::unique_ptr <Shader> shader;

	unsigned int currentCam = 0;
	std::vector<std::shared_ptr<Visible>> actors; // LEGACY
	std::vector<std::shared_ptr<Camera>> cameras; // LEGACY
	std::vector<std::shared_ptr<Light>> lighting; // LEGACY
	std::shared_ptr<DirectionalLight> dir; // LEGACY
	std::vector<std::shared_ptr<PointLight>> poi; // LEGACY
	std::vector<std::shared_ptr<SpotLight>> sp; // LEGACY
	
	//std::vector<TextureID> textures;
	//std::shared_ptr<Map> map = nullptr;

	std::vector<glm::mat4> model;
	std::vector<glm::mat4> lightModel;
};
