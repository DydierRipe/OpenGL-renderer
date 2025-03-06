#include "Light.h"
#include <Windows.h>

Light::Light() : Visible(), ambient(1), diffuse(1), specular(1)
{
}

Light::~Light()
{
}

void Light::onUpdate(float deltaTime)
{
	Visible::onUpdate(deltaTime);

	if (auto center = attach.lock())
	{
		getTransform().setPosition(center->getTransform().getPosition().x + cos(rTraslation) * 10,
			center->getTransform().getPosition().y + sin(rTraslation) * 10, center->getTransform().getPosition().z + cos(rTraslation) * 10);

		rTraslation += 2 * deltaTime;

		if (rTraslation > 359) rTraslation = 0;
	}

	/*
	if (GetKeyState(VK_LEFT) & 0x8000) getTransform().setPositionx(getTransform().getPosition().x - 10 * deltaTime);
	if (GetKeyState(VK_RIGHT) & 0x8000) getTransform().setPositionx(getTransform().getPosition().x + 10 * deltaTime);
	if (GetKeyState(VK_UP) & 0x8000) getTransform().setPositionz(getTransform().getPosition().z - 10 * deltaTime);
	if (GetKeyState(VK_DOWN) & 0x8000) getTransform().setPositionz(getTransform().getPosition().z + 10 * deltaTime);
	if (GetKeyState('U') & 0x8000) getTransform().setPositiony(getTransform().getPosition().y + 10 * deltaTime);
	if (GetKeyState('J') & 0x8000) getTransform().setPositiony(getTransform().getPosition().y - 10 * deltaTime);
	*/
}

void Light::configure(const json& data, const std::string& vname, std::shared_ptr<Map> map, unsigned int& index)
{
	json modelData = data;
	//modelData.push_back(json::object_t::value_type("model","res/3D models/Rubics_cube/cube.obj"));

	Visible::configure(modelData, vname, map, index);

	if (data.contains("illumination")) {
		ambient = proccessColor(data["illumination"], "ambient", vname);
		diffuse = proccessColor(data["illumination"], "diffuse", vname);
		specular = proccessColor(data["illumination"], "specular", vname);
	}
}
