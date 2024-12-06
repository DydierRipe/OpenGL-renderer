#include "SpotLight.h"

SpotLight::SpotLight() : PointLight(), direction(0), cutOff(45), innerCutOff(44)
{
}

SpotLight::~SpotLight()
{
}

void SpotLight::configure(const json& data, const string& vname, std::shared_ptr<Map> map, unsigned int& index)
{
	PointLight::configure(data, vname, map, index);

	if (data["illumination"].contains("direction")) direction = proccessPosition(data["illumination"], vname, "direction");
	if (data["illumination"].contains("cutOff")) cutOff = data["illumination"]["cutOff"];
	if (data["illumination"].contains("innerCutOff")) innerCutOff = data["illumination"]["innerCutOff"];
}

void SpotLight::onUpdate(float deltaTime)
{
	if (auto cam = attach.lock()) {
		glm::vec3 camFront = glm::vec3(0);
		Rotation camRot = cam->getTransform().getRotation();
		camFront.x = cos(glm::radians(camRot.yaw)) * cos(glm::radians(camRot.pitch));
		camFront.y = sin(glm::radians(camRot.pitch));
		camFront.z = sin(glm::radians(camRot.yaw)) * cos(glm::radians(camRot.pitch));
		direction = Position(glm::normalize(camFront));
		getTransform().setRotationyp(-camRot.yaw, camRot.pitch);
		getTransform().setPosition(cam->getTransform().getPosition());
	}
}
