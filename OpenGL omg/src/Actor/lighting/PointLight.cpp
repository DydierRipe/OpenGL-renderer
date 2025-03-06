#include "PointLight.h"
#include <Windows.h>

PointLight::PointLight() : Light(), constant(1.0f), linear(0.7f), quadratic(1.8f)
{
}

PointLight::~PointLight()
{
}

void PointLight::configure(const json& data, const string& vname, std::shared_ptr<Map> map, unsigned int& index)
{
	Light::configure(data, vname, map, index);

	if (data["illumination"].contains("constant")) constant = data["illumination"]["constant"];
	if (data["illumination"].contains("linear")) linear = data["illumination"]["linear"];
	if (data["illumination"].contains("quadratic")) quadratic = data["illumination"]["quadratic"];
}

void PointLight::onUpdate(float deltaTime)
{
	if (GetKeyState(VK_UP) & 0x8000) {
		Position newPos = getTransform().getPosition();
		getTransform().setPosition(newPos.x + 1, newPos.y, newPos.z);
	}
	if (GetKeyState(VK_DOWN) & 0x8000) {
		Position newPos = getTransform().getPosition();
		getTransform().setPosition(newPos.x - 1, newPos.y, newPos.z);
	}
	if (GetKeyState(VK_LEFT) & 0x8000) {
		Position newPos = getTransform().getPosition();
		getTransform().setPosition(newPos.x, newPos.y, newPos.z - 1);
	}
	if (GetKeyState(VK_RIGHT) & 0x8000) {
		Position newPos = getTransform().getPosition();
		getTransform().setPosition(newPos.x, newPos.y, newPos.z + 1);
	}
}
