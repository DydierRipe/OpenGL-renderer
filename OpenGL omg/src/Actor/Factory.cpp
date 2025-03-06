#include "Factory.h"
#include "FactoryIncludes.h"

Factory::Factory()
{
	registerType("Camera", []() { return std::make_shared<Camera>(); });
	registerType("Visible", []() {return std::make_shared<Visible>(); });
	registerType("Light", []() {return std::make_shared<Light>(); });
	registerType("DirectionalLight", []() {return std::make_shared<DirectionalLight>(); });
	registerType("PointLight", []() {return std::make_shared<PointLight>(); });
	registerType("SpotLight", []() {return std::make_shared<SpotLight>(); });
}

std::shared_ptr<Actor> Factory::create(const std::string &type, const string& mapName, const std::string &name, const json data, std::shared_ptr<Scene> scene)
{
	auto it = funcMap.find(type);

	if (it != funcMap.end())
	{
		it->second()->configure(data, mapName, name, scene);
		return it->second();
	}

	return nullptr;
}

