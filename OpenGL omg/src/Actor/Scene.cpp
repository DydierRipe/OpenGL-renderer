#include "Scene.h"
#include "Factory.h"
#include "lighting/Light.h"
#include "Visible.h"
#include "camera/Camera.h"
#include "Actor.h"
#include "../renderer/Renderer.h"

#include <fstream>

// I wont understand this shit in two months, so I praise god chatgpt is still free for that time
Scene::Scene(const std::string& path)
{
	sceneList.push_back("orphanage");

	loadScene(path);

}

Scene::~Scene()
{
}

void Scene::onUpdate(float deltaTime)
{

}

void Scene::addActiveActor(string name, shared_ptr<Actor> actor, unsigned int framerate = 1)
{

}

void Scene::loadScene(const string& path)
{
	std::ifstream f(path);
	if (!f.is_open())
	{
		std::cerr << "ERROR: scene file could not be opened: " << path << std::endl;
		return;
	}

	json data, actorsList;
	try {
		data = json::parse(f);
	}
	catch (const json::parse_error& e) {
		std::cerr << "ERROR: scene file could not be parsed: " << e.what() << std::endl;
		return;
	}

	string mapName;
	if (!data.contains("name") || !data.contains("actors"))
	{
		std::cerr << "ERROR: scene has no name: " << path << std::endl;
		return;
	}

	mapName = data["name"];
	actorsList = data["actors"];

	for (const auto& item : actorsList.items()) {
		string key = item.key();
		json value = item.value();

		if (!value.contains("type") || !value.contains("config"))
		{
			std::cerr << "WARNING: this actor (" << key << ") doesn't contain the value 'type' or 'config', so it will not be considerated as a valid actor in the scene" << std::endl;
			continue;
		}
		
		string type = value["type"];
		shared_ptr<Actor> actor = factory.create(type, mapName, key, value["config"], shared_from_this());

		if (!actor)
		{
			std::cerr << "WARNING: the actor: " << key << "\ntype: " << type << " could not be created, so it will not be considerated as a valid actor in the scene" << std::endl;
			continue;
		}
		else {
			string group = "none::";

			if (std::dynamic_pointer_cast<Camera>(actor)) 
				group = "camera::";
			else if (std::dynamic_pointer_cast<Light>(actor)) 
				group = "lighting::";
			else if (std::dynamic_pointer_cast<Visible>(actor))
				group = "visible::";

			actor->setType(group);
			actor->setName(key);

			actors.insert({ actor->getName(), actor});
			if (group == "camera::") {
				cameras.insert({ actor->getName(), std::dynamic_pointer_cast<Camera>(actor) });
			}
			else if (group == "lighting::") {
				lighting.insert({ actor->getName(), std::dynamic_pointer_cast<Light>(actor) });
			}
			else if (group == "visible::") {
				auto vis = std::dynamic_pointer_cast<Visible>(actor);
				visibles.insert({ vis->getAssignedShaderName() + "::" + vis->getName(), vis});
			}
		}
	}

	if (cameras.empty()) {
		std::cerr << "WARNING, THERE ARE NO CAMERAS IN THIS SCENE, A DEFAULT ORTHOGRAFIC CAMERA WILL BE PROVIDED" << std::endl;

		string Data{
			R"({"resW": 700,
				"resH" : 700,
				"position" : [0.0, 0.0, 0.0] ,
				"rotation" : [0.0, 0.0, 0.0] ,
				"ortho" : true,
				"FOV" : 0.0,
				"maxRender" : 100.0})"
		};

		json config = json::parse(Data);

		shared_ptr<Actor> defCam = factory.create("Camera", "orphanage", "mainCam", config, shared_from_this());
		defCam->setType("camera::");

		if (shared_ptr<Camera> cam = std::dynamic_pointer_cast<Camera>(defCam)) {
			actors.insert({ "camera::orphanage::mainCam", cam });
			cameras.insert({ "camera::orphanage::mainCam", cam });
		}
		else std::cerr << "WARNING, DEFAULT CAMERA COULD NOT BE PROVIDED" << std::endl;
	}
}

string Scene::verifyName(string name, string type, string mapName)
{
	string orgKey = name, modKey = name, fullKey = type + mapName + "::" + mapName;
	int suffix = 1;
	while (actors.find(fullKey) != actors.end()) {
		modKey = orgKey + std::to_string(suffix);
		fullKey = type + mapName + "::" + modKey;
		suffix++;
	}

	return modKey;
}

void Scene::draw()
{
	renderVisibles();
	if (renderCollBox) renderCollisionBoxes();
	if (renderPosMark) renderPositionMarkers();
}

void Scene::renderVisibles()
{
	//  THIS IS A CODE THAT HAS TO BE CHANGED TO FIT RENDERING ORDERS PARAMETERS (SHADER -> MODEL -> TEXTURE)

	/*  MAKE THE VISIBLES AN ORDERED MAP WITH THE NAME OF EACH SHADER, THEN RENDER ACCORDING TO SHADER TYPE DOING A FOR,
		BUT CHANGING THE SHADER RIGHT AFTER THE NAME IS NOT DETECTED ANYMORE */

	for (auto& vis : visibles)
	{
		if (vis.second->getEnabled()) {
			auto &final = vis.second;

			final->draw(shaderPool.getShader(final->getAssignedShaderName()));
		}
	}
}

string Scene::legalizeNameChange(string originalName, string newName, string type, string mapName)
{
	string nn = verifyName(newName, type, mapName), key = type + mapName + "::" + originalName;

	auto handler = actors.extract(key);
	if (!handler.empty())
	{
		handler.key() = type + mapName + "::" + nn;
		actors.insert(std::move(handler));

		if (type == "camera::")
		{
			auto handler2 = cameras.extract(key);
			if (!handler2.empty())
			{
				handler2.key() = type + mapName + "::" + nn;
				cameras.insert(std::move(handler2));
			}
		}
		else if (type == "visible::")
		{
			auto handler2 = visibles.extract(key);
			if (!handler2.empty())
			{
				handler2.key() = type + mapName + "::" + nn;
				visibles.insert(std::move(handler2));
			}
		}
		else if (type == "lighting::")
		{
			auto handler2 = lighting.extract(key);
			if (!handler2.empty())
			{
				handler2.key() = type + mapName + "::" + nn;
				lighting.insert(std::move(handler2));
			}
		}
		return nn;
	}

	return "-1";
}

std::weak_ptr<Actor> Scene::getActor(string name, ActorType)
{
	return std::weak_ptr<Actor>();
}
