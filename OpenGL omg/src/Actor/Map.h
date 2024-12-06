#pragma once
#include <json/json.hpp>
#include <unordered_map>
#include <string>
#include <memory>

// Forward declarations
class Actor;
class Camera;
class Renderer;

#include "Factory.h"
#include "../Coords/ModelProcessor.h"
#include "../Coords/Transform.h"

using std::shared_ptr;
using std::string;
using std::vector;
using std::unordered_map;
using json = nlohmann::json;

class Map : public std::enable_shared_from_this<Map>
{
public:

	Map(const std::string& path);
	~Map();

	void onUpdate(float deltaTime);

	static shared_ptr<Map> loadMap(const string &path, shared_ptr<Map> mapToDelete);
	static shared_ptr<Map> loadMap(const string& path, shared_ptr<Renderer> rend);
	static void unloadMap(shared_ptr<Map> map);

	inline Factory& getFactory() { return factory; }
	inline unordered_map<string, shared_ptr<Actor>>& getActorMap() { return actors; }
	inline unordered_map<string, shared_ptr<Camera>>& getCameraMap() { return cameras; }
	inline vector<string>& getTextureDirections() { return texturesDirections; }
	inline vector<string>& getModelDirections() { return modelsDirections; }
	shared_ptr<Camera> getScreenCam();

	shared_ptr<Actor> getActor(const string &name);
	shared_ptr<Camera> getCamera(const string& name);

	inline void setScreenCam(string sC) { screenCam = sC; }
	inline void setRenderer(shared_ptr<Renderer> rend) { currentRenderer = rend; }
	inline shared_ptr<Renderer> getRenderer() { return currentRenderer; }

private:

	Factory factory;
	shared_ptr<Renderer> currentRenderer;
	string screenCam = "mainCam";

	unsigned int actorCount = 0;

	vector<string> modelsDirections;
	vector<string> texturesDirections;
	unordered_map<std::string, shared_ptr<Actor>> actors;
	unordered_map<std::string, shared_ptr<Camera>> cameras;

};
