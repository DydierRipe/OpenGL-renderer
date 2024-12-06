#include "Camera.h"
#include <Windows.h>
//#include <GLFW/glfw3.h>

Camera::Camera() : Actor(), resolutionH(0), resolutionW(0),
	bOrtho(true), FOV(60), renderDistance(100), speed(0)
{
	setIsCam(true);
	renovateCamStats();
}

Camera::~Camera()
{
}

void Camera::configure(const json& data, const std::string& vname, std::shared_ptr<Map> map, unsigned int& index)
{
	Actor::configure(data, vname, map, index);
	bool bContainsOblig = true;

	if (data.contains("resW")) resolutionW = data["resW"];
	else bContainsOblig = false;

	if (data.contains("resH")) resolutionH = data["resH"];
	else bContainsOblig = false;

	if (data.contains("sensibility")) sensibility = data["sensibility"];

	if (data.contains("speed")) speed = data["speed"];

	lastY = resolutionH / 2;
	lastX = resolutionW / 2;

	if (data.contains("ortho")) bOrtho = data["ortho"];
	if (data.contains("FOV")) FOV = data["FOV"];
	if (data.contains("maxRender")) renderDistance = data["maxRender"];

	if (!bContainsOblig) std::cout << data["name"] << " json of camera doesn't contains either resH or resW. this will cause problems";

	renovateCamStats();
}

glm::mat4 Camera::getProjection()
{
	return proj;
}

glm::mat4 Camera::getView()
{
	return view;
}

void Camera::onUpdate(float deltaTime)
{
	float adjustedSpeed = speed * deltaTime;

	if (GetKeyState('W') & 0x8000) { 
		glm::vec3 newPos = getTransform().getVectorPosition() + camFront * adjustedSpeed;
		getTransform().setPosition(newPos.x, newPos.y, newPos.z);
		bNeedsToUpdateStats = true; 
	}
	if (GetKeyState('S') & 0x8000) { 
		glm::vec3 newPos = getTransform().getVectorPosition() - camFront * adjustedSpeed;
		getTransform().setPosition(newPos.x, newPos.y, newPos.z);
		bNeedsToUpdateStats = true;
	}
	if (GetKeyState('A') & 0x8000) {
		glm::vec3 newPos = getTransform().getVectorPosition() - glm::normalize(glm::cross(camFront, camUp)) * adjustedSpeed;
		getTransform().setPosition(newPos.x, newPos.y, newPos.z);
		bNeedsToUpdateStats = true;
	}
	if (GetKeyState('D') & 0x8000) {
		glm::vec3 newPos = getTransform().getVectorPosition() + glm::normalize(glm::cross(camFront, camUp)) * adjustedSpeed;
		getTransform().setPosition(newPos.x, newPos.y, newPos.z);
		bNeedsToUpdateStats = true;
	}
	if (GetKeyState(VK_SPACE) & 0x8000) {
		getTransform().setPositiony(getTransform().getPosition().y + adjustedSpeed);
		bNeedsToUpdateStats = true;
	}
	if (GetKeyState('C') & 0x8000) {
		getTransform().setPositiony(getTransform().getPosition().y - adjustedSpeed);
		bNeedsToUpdateStats = true;
	}


	if (viewing.lock() || bNeedsToUpdateStats) {
		renovateCamStats();
		bNeedsToUpdateStats = false;
	}
}

void Camera::mouseCamRotation(double posx, double posy)
{
	if (bFirstMouse)
	{
		lastX = posx;
		lastY = posy;
		bFirstMouse = false;
	}

	float offsetx = posx - lastX;
	float offsety = lastY - posy;

	lastX = posx;
	lastY = posy;

	offsetx *= sensibility;
	offsety *= sensibility;

	Rotation currRot = getTransform().getRotation();

	float pitchCandidate = currRot.pitch + offsety;

	if (pitchCandidate > 89.0f) pitchCandidate = 89.0f;
	if (pitchCandidate < -89.0f) pitchCandidate = -89.0f;

	getTransform().setRotationyp(currRot.yaw + offsetx, pitchCandidate);

	bNeedsToUpdateStats = true;
}

glm::mat4 Camera::getOrthoProj()
{
	return glm::ortho(-resolutionW / 2, resolutionW / 2, -resolutionH / 2, resolutionH / 2, -renderDistance, renderDistance);
}

glm::mat4 Camera::getPerspectiveProj()
{
	return glm::perspective(glm::radians(FOV), resolutionW / resolutionH, 0.1f, renderDistance);
}

glm::mat4 Camera::getOrthoView()
{
	return glm::translate(glm::mat4(1.0f), glm::vec3(getTransform().getPosition().x, getTransform().getPosition().y, getTransform().getPosition().z));
}

glm::mat4 Camera::getPerspectiveView()
{
	camPos = glm::vec3(getTransform().getPosition().x, getTransform().getPosition().y, getTransform().getPosition().z);

	if (auto target = viewing.lock()) {
		Position targetPos = target->getTransform().getPosition();
		Position thisPos = getTransform().getPosition();
		camFront = glm::normalize(glm::vec3(targetPos.x - thisPos.x, targetPos.y - thisPos.y, targetPos.z - thisPos.z));
	}
	else {
		camFront = glm::vec3(1);

		camFront.x = cos(glm::radians(getTransform().getRotation().yaw)) * cos(glm::radians(getTransform().getRotation().pitch));
		camFront.y = sin(glm::radians(getTransform().getRotation().pitch));
		camFront.z = sin(glm::radians(getTransform().getRotation().yaw)) * cos(glm::radians(getTransform().getRotation().pitch));
		camFront = glm::normalize(camFront);
	}

	camUp = glm::vec3(0.0f, 1.0f, 0.0f);

	return glm::lookAt(camPos, camPos + camFront, camUp);
}

void Camera::setResolution(int w, int h)
{
	resolutionW = static_cast<float>(w);
	resolutionH = static_cast<float>(h);

	renovateCamStats();
}

void Camera::renovateCamStats()
{
	if (bOrtho)
	{
		proj = getOrthoProj();
		view = getOrthoView();
	}
	else {
		proj = getPerspectiveProj();
		view = getPerspectiveView();
	}
}
