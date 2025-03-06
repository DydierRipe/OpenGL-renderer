#pragma once
#include <memory>
#include "../Coords/Transform.h"
#include "Actor.h"

class Map;

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using json = nlohmann::json;

class Camera : public Actor
{
public:
	Camera();
	~Camera();


	virtual void configure(const json& data, const std::string& vname, std::shared_ptr<Map> map, unsigned int& index) override;

	glm::mat4 getProjection();
	glm::mat4 getView();
	inline glm::vec3 getFront() const { return camFront; }

	void setOrtho(bool ortho) { bOrtho = ortho; renovateCamStats(); }
	void setFOV(float fov) { FOV = fov; renovateCamStats(); }
	void setRenderDistance(float rD) { renderDistance = rD; renovateCamStats(); }

	void setPosition(Position Pos) { getTransform().setPosition(Pos.x, Pos.y, Pos.z); renovateCamStats(); }
	void setRotation(Rotation Rot) { getTransform().setRotation(Rot.roll, Rot.pitch, Rot.yaw); renovateCamStats(); }
	void setResolution(int w, int h);

	void renovateCamStats();
	void onUpdate(float deltaTime);
	inline void lockViewToObject(std::weak_ptr<Actor> target) { viewing = target; };

	virtual void mouseCamRotation(double posx, double posy);

private:

	glm::mat4 getOrthoProj();
	glm::mat4 getPerspectiveProj();

	glm::mat4 getOrthoView();
	glm::mat4 getPerspectiveView();

	glm::mat4 proj = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);

	bool bNeedsToUpdateStats = false;
	bool bFirstMouse = true;

	float sensibility = 0.1;

	std::weak_ptr<Actor> viewing;
	bool bOrtho;
	float FOV;

	float speed;

	float lastX;
	float lastY;

	float renderDistance;

	float resolutionH;
	float resolutionW;

	glm::vec3 camPos = glm::vec3(0.0f);
	glm::vec3 camFront = glm::vec3(0.0f);
	glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);

};
