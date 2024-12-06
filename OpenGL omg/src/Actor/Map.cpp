#include "Map.h"
#include "Factory.h"
#include "Camera.h"
#include "Actor.h"
#include "../renderer/Renderer.h"
#include "../Coords/ModelProcessor.h"

#include <fstream>

// I wont understand this shit in two months, so I praise god chatgpt is still free for that time
Map::Map(const std::string& path)
{
	std::ifstream f("res/maps/firstMap.json");
	json data = json::parse(f);

	vector<string> modelPath;

	for (const auto& item : data.items()) {
		string key = item.key();
		json value = item.value();

		if (!value.contains("type"))
		{
			std::cerr << "WARNING: this actor (" << key << ") doesn't contain the value 'type', so it will not be considerated as a valid actor in the scene" << std::endl;
			continue;
		}
		else if (!value.contains("config")) {
			std::cerr << "WARNING: this actor (" << key << ") doesn't contain the value 'config', so it will not be considerated as a valid actor in the scene" << std::endl;
			continue;
		}
		else if (actors.find(key) != actors.end())
		{
			std::cerr << "WARNING: this actor (" << key << ") is already registered / its name has already been taken, so it will not be considerated as a valid actor in the scene" << std::endl;
			continue;
		}
		else {
			string type = value["type"];
			shared_ptr<Actor> actor = factory.create(type, key, value["config"], shared_from_this(), actorCount);

			if (!actor)
			{
				std::cerr << "WARNING: the actor: " << key << "\ntype: " << type << " could not be created, so it will not be considerated as a valid actor in the scene" << std::endl;
				continue;
			}
			else {
				if (value["config"].contains("model"))
				{
					string modelDir = value["config"]["model"];
					if (ModelProcessor::binarySearch(modelsDirections, 0, modelsDirections.size() - 1, modelDir) == -1)
					{
						ModelProcessor::insertSorted(modelsDirections, modelDir);
					}
				}

				if (value["config"].contains("texture"))
				{
					string texDir = value["config"]["texture"];
					int binSearchResult = ModelProcessor::binarySearch(texturesDirections, 0, texturesDirections.size() - 1, texDir);
					if (binSearchResult == -1)
					{
						binSearchResult = ModelProcessor::insertSorted(texturesDirections, texDir);
					}
					
					value["config"]["texID"] = binSearchResult;
				}

				actors.insert(std::make_pair(key, actor));
				if (shared_ptr<Camera> cam = std::dynamic_pointer_cast<Camera>(actor)) {
					cameras.insert(std::make_pair(key, cam));
				}
				actorCount++;
			}
		}
	}

	if (cameras.empty()) {
		string Data {
			R"({"resW": 700,
				"resH" : 700,
				"position" : [0.0, 0.0, 0.0] ,
				"rotation" : [0.0, 0.0, 0.0] ,
				"ortho" : true,
				"FOV" : 0.0,
				"maxRender" : 100.0})"
		};

		json config = json::parse(Data);

		shared_ptr<Actor> defCam = factory.create("Camera", "mainCam", config, shared_from_this(), actorCount);
		
		actors.insert(std::make_pair("mainCam", defCam));
		if (shared_ptr<Camera> cam = std::dynamic_pointer_cast<Camera>(defCam)) {
			cameras.insert(std::make_pair("mainCam", cam));
		}

		std::cerr << "WARNING, THERE ARE NO CAMERAS IN THIS SCENE, A DEFAULT ORTHOGRAFIC CAMERA WILL BE PROVIDED" << std::endl;
	}
}

Map::~Map()
{
	for (auto i = actors.begin(); i != actors.end(); i++)
	{
		i->second->~Actor();
	}

	for (auto i = cameras.begin(); i != cameras.end(); i++)
	{
		i->second->~Camera();
	}
}

void Map::onUpdate(float deltaTime)
{
	for (auto i = actors.begin(); i != actors.end(); i++)
	{
		i->second->onUpdate(deltaTime);
	}

	for (auto i = cameras.begin(); i != cameras.end(); i++)
	{
		i->second->onUpdate(deltaTime);
	}
}

std::shared_ptr<Map> Map::loadMap(const std::string& path, std::shared_ptr<Map> mapToDelete)
{
	shared_ptr<Renderer> rend = mapToDelete->getRenderer();

	if (mapToDelete)
	{
		Map::unloadMap(mapToDelete);
	}
	else {
		std::cerr << "WARNING, THIS MAP COULDN'T BE UNLOADED BECAUSE IT RETURNED NULLPTR" << std::endl;
	}

	shared_ptr<Map> map = std::make_shared<Map>(path);
	map->setRenderer(rend);

	return map;
}

// dont know if I did this right, I supose I will find that out in the future
shared_ptr<Map> loadMap(const string& path, shared_ptr<Renderer> rend) {
	shared_ptr<Map> map = std::make_shared<Map>(path);
	map->setRenderer(rend);

	return map;
}

// suicide function omg this should be illegal in Korea
void Map::unloadMap(shared_ptr<Map> prevMap)
{
	// this is suposed to clear any value that has been passed to the Renderer, and then delete itself, the function loadMap will call this
	// automatically for the previous map of the renderer :>
	prevMap->~Map();
}

shared_ptr<Camera> Map::getScreenCam()
{
	shared_ptr<Camera> ret = getCamera(screenCam);
	if (ret) {
		return ret;
	}

	std::cerr << "WARNING, " << std::endl;
}

shared_ptr<Actor> Map::getActor(const string &name)
{
	if (actors.find(name) != actors.end()) return actors[name];

	return nullptr;
}

shared_ptr<Camera> Map::getCamera(const string& name)
{
	if (cameras.find(name) != cameras.end()) return cameras[name];

	return nullptr;
}
