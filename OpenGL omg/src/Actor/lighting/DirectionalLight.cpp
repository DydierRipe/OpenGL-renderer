#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : direction(0)
{
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::configure(const json& data, const std::string& vname, std::shared_ptr<Map> map, unsigned int& index)
{
	Light::configure(data, vname, map, index);

	if (data["illumination"].contains("direction")) direction = proccessPosition(data["illumination"], vname, "direction");
}

void DirectionalLight::onUpdate(float deltaTime)
{
}
