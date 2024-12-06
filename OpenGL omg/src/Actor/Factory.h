#pragma once
#include <memory>
#include <string>
#include <iostream>
#include <json/json.hpp>

using json = nlohmann::json;

class Actor;
class Camera;
class Visible;
class Map;

/*
	A este punto es mejor especificar el tipo con un string y no joder tanto, la otra es que en el mismo .h se implementen las funciones, 
	pero no creo que sea una opcion de todos modos.

	simplemente utiliza la unica funcion create y que cada una exija lo que debe en sus propios funciones de config.
*/

class Factory
{
public:
	using createFunction = std::function<std::shared_ptr<Actor>()>;

	Factory();

	std::shared_ptr<Actor> create(const std::string &type, const std::string &name, const json data, std::shared_ptr<Map> map, unsigned int &index);

private:
	std::unordered_map<std::string, createFunction> funcMap;

	inline void registerType(const std::string& type, createFunction fnc) {
		funcMap[type] = fnc;
	}
};
