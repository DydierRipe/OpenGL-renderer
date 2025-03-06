#pragma once
#include <json/json.hpp>
#include <unordered_map>
#include <map>
#include <string>
#include <memory>
#include <bitset>

// Forward declarations
class Actor;
class Camera;
class Visible;
class Light;

constexpr size_t MAX_ACTIVE_ACTORS = 1024;
constexpr int MAX_BUCKETS = 10;

#include "Factory.h"
#include "../Coords/Transform.h"
#include "model/ModelManager.h"
#include "model/ShaderManager.h"

using std::shared_ptr;
using std::string;
using std::vector;
using std::unordered_map;
using json = nlohmann::json;

enum ActorType
{
	NONE = 0,
	VISIBLE = 1,
	CAMERA = 2,
	LIGHT = 3
};

class Scene : public std::enable_shared_from_this<Scene>
{
public:

	Scene(const string& path);
	~Scene();

	void onUpdate(float deltaTime);
	void addActiveActor(string name, shared_ptr<Actor> actor, unsigned int framerate = 1);

	// Loads a scene from a json file
	void loadScene(const string& path);

	// Unloads a scene from a json file and deletes every instance of any object, an scene can only be deleted if it has already been loaded
	void unloadScene(const string& path, bool bDeleteModelIfRef0 = false);
	
	// Unloads a scene, but exceptions can be specified, if a new scene is not specified, the instance wil be sent to the orphanage
	void unloadScene(const string& path, vector<string> but, string newScene = "orphanage");
	
	void unloadAllScenes();
	void unloadAllScenes(vector<string> but);

	void changeActorScene(string actorName, string newScene = "orphanage");

	inline Factory& getFactory() { return factory; }
	inline ModelManager& getModelManager() { return modelPool; }
	inline ShaderManager& getShaderManager() { return shaderPool; }

	void draw();
	void renderVisibles();
	void renderPositionMarkers();
	void renderCollisionBoxes();

	string legalizeNameChange(string originalName, string newName, string type, string mapName);
	// returns a weak pointer to an actor. Name corresponds 
	std::weak_ptr<Actor> getActor(string name, ActorType = VISIBLE);

private:
	Factory factory;
	ModelManager modelPool;
	ShaderManager shaderPool;

	unordered_map<string, shared_ptr<Actor>> actors;
	std::map<string, shared_ptr<Visible>> visibles;
	unordered_map<string, shared_ptr<Camera>> cameras;
	unordered_map<string, shared_ptr<Light>> lighting;

	std::bitset<MAX_ACTIVE_ACTORS> activeActors;
	unordered_map<int, vector<shared_ptr<Actor>>> updateBuckets;

	string currentShowingCam;
	vector<string> sceneList;

	// verifies if the name has already been taken, if not, returns a valid name
	string verifyName(string name, string type, string mapName);

	bool renderPosMark = false, renderCollBox = false;
};
