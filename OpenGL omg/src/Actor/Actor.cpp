#include "Actor.h"
#include <memory>
#include "Scene.h"


Actor::Actor() : transform(Position(0), Rotation(0)), scene(nullptr) {}


void Actor::configure(const json& data, const string& vmapName, const std::string& vname, std::shared_ptr<Scene> vscene)
{
	mapName = vmapName;
	name = vname;
	bool bNeedsSize = false;
	Position pos(0);
	Rotation rot(0);
	Size siz(0);

	
	if (data.contains("position")) pos = proccessPosition(data, vname, "position");

	if (data.contains("rotation")) {

		switch (data["rotation"].size())
		{
		case 0:
			std::cout << "ATTENTION: THE OBJECT " << vname << " DOESN'T CONTAIN VALUES ON ROTATION, VALUES WILL STAY AS 0." << std::endl;
			break;
		case 1:
			rot = Rotation(data["rotation"][0]);
			break;
		case 3:
			rot = Rotation(data["rotation"][0], data["rotation"][1], data["rotation"][2]);
			break;
		default:
			std::cout << "ATTENTION: THE OBJECT " << vname << " DOESN'T CONTAIN A VALID NUMBER OF ELEMENTS ON ROTATION ARRAY, FIRST ELEMENT WILL BE TAKEN FOR EVERY VALUE INSTEAD." << std::endl;
			rot = Rotation(data["rotation"][0]);
			break;
		}
	}

	if (data.contains("size")) {
		bNeedsSize = true;

		switch (data["size"].size())
		{
		case 0:
			std::cout << "ATTENTION: THE OBJECT " << vname << " DOESN'T CONTAIN VALUES ON SIZE, VALUES WILL STAY AS 0." << std::endl;
			break;
		case 1:
			siz = Size(data["size"][0]);
			break;
		case 3:
			siz = Size(data["size"][0], data["size"][1], data["size"][2]);
			break;
		default:
			std::cout << "ATTENTION: THE OBJECT " << vname << " DOESN'T CONTAIN A VALID NUMBER OF ELEMENT ON SIZE ARRAY, FIRST ELEMENT WILL BE TAKEN FOR EVERY VALUE INSTEAD." << std::endl;
			siz = Size(data["size"][0]);
			break;
		}
	}

	if (vscene == nullptr) std::cout << "WARNING: this actor does not have a pointer to its scene, this may cause some issues accesing to other objects/actors" << std::endl;
	else scene = vscene;

	if (bNeedsSize) transform = Transform(pos, rot, siz);
	else transform = Transform(pos, rot);
}

void Actor::setType(string vtype)
{
	if (!bIsTypeSet)
	{
		type = vtype;
		bIsTypeSet = true;
	}
}

void Actor::setName(string newName)
{
	string nn = scene->legalizeNameChange(name, newName, type, mapName);
	if (nn != "-1")
		name = nn;
	else std::cout << "Name of " << name << " could not be changed." << std::endl;
}

Position Actor::proccessPosition(const json& data, const std::string vname, const std::string name)
{
	Position pos(0);
	switch (data[name].size())
	{
	case 0:
		std::cout << "ATTENTION: THE OBJECT " << vname << " DOESN'T CONTAIN VALUES ON " << name << ", VALUES WILL STAY AS 0." << std::endl;
		break;
	case 1:
		pos = Position(data[name][0]);
		break;
	case 3:
		pos = Position(data[name][0], data[name][1], data[name][2]);
		break;
	default:
		std::cout << "ATTENTION: THE OBJECT " << vname << " DOESN'T CONTAIN A VALID NUMBER OF ELEMENTS ON " << name << " ARRAY, FIRST ELEMENT WILL BE TAKEN FOR EVERY VALUE INSTEAD." << std::endl;
		pos = Position(data[name][0]);
		break;
	}

	return pos;
}
