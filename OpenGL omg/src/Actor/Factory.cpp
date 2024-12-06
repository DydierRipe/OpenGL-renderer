#include "Factory.h"
#include "Map.h"
#include "Actor.h"
#include "Camera.h"
#include "Visible.h"

Factory::Factory()
{
	registerType("Camera", []() { return std::make_shared<Camera>(); });
	registerType("Visible", []() {return std::make_shared<Visible>(); });
}

std::shared_ptr<Actor> Factory::create(const std::string &type, const std::string &name, const json data, std::shared_ptr<Map> map, unsigned int& index)
{
	auto it = funcMap.find(type);

	if (it != funcMap.end())
	{
		it->second()->configure(data, name, map, index);
		return it->second();
	}

	return nullptr;
}

