#pragma once
#include <string>
#include <vector>
#include <memory>
#include <json/json.hpp>

#include "../Observer/Observable.h"
#include "../Observer/Observer.h"
#include "../Coords/CoordAttributes.h"
#include "../Coords/Transform.h"

class Map;

using std::string;
using json = nlohmann::json;

class Actor : public Observable
{
public:
	Actor();

	virtual ~Actor() = default;

	virtual void configure(const json& data, const std::string &vname, std::shared_ptr<Map> map, unsigned int &index);
	virtual void onUpdate(float deltaTime) = 0;

	inline bool isCam() const { return bIsCam; }
	inline unsigned int getActorIndex() { return actorIndex; }
	inline Transform& getTransform() { return transform; }
	inline std::string getName() { return name; }

protected:
	inline void setIsCam(bool isCam) { bIsCam = isCam; }
	inline Position proccessPosition(const json& data, const std::string vname, const std::string name);
private:

	Transform transform;
	bool bIsCam = false;
	std::string name;
	std::shared_ptr<Map> currentMap;
	unsigned int actorIndex;
};



