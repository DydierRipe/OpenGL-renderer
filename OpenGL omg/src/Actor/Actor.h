#pragma once
#include <string>
#include <vector>
#include <memory>
#include <json/json.hpp>

#include "../Observer/Observable.h"
#include "../Observer/Observer.h"
#include "../Coords/CoordAttributes.h"
#include "../Coords/Transform.h"

class Scene;

using std::string;
using json = nlohmann::json;

class Actor : public Observable
{
public:
	Actor();

	virtual ~Actor() = default;

	virtual void configure(const json& data, const string& vmapName, const string &vname, std::shared_ptr<Scene> vscene);
	virtual void onUpdate(float deltaTime) = 0;

	inline bool isCam() const { return bIsCam; }
	inline Transform& getTransform() { return transform; }
	inline string getName() { return name; }
	inline string getType() { return type; }

	// THIS FUNCTION CAN ONLY BE USED ONCE AND WILL BE USED BEFORE YOU COULD, IF YOU USE IT, IT WILL NOT WORK
	void setType(string vtype);
	// renames the function
	void setName(string newName);
	// DO NOT USE THIS FUNCTION, if it is ever used it may (and will) create problems with legalization. 
	inline void setMapName(string newName) { mapName = newName; }
protected:
	inline void setIsCam(bool isCam) { bIsCam = isCam; }
	Position proccessPosition(const json& data, const std::string vname, const std::string name);

	std::shared_ptr<Scene> scene;
private:

	Transform transform;
	bool bIsCam = false;
	bool bIsTypeSet = false;
	string type = "", mapName = "", name = "";
};



