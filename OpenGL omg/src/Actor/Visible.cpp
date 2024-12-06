#include "Visible.h"
#include <Windows.h>

/*
	POR IMPLEMENTAR: 

		implementa instanciacion y ssbo

		crea una clase y un archivo de mapa, estas tienen que contener y cargar los objetos a la vez que guardarlos y manejar los punteros a cada uno

*/

Visible::Visible()
	: Actor(), model({}), material({ 1, 1, 1 }), texture("-1", 0)
{
}

Visible::~Visible()
{
}

void Visible::getModel(std::vector<float>& bufferArray)
{
	for (int i = 0; i < model.coords.size(); i++) {
		bufferArray.push_back(model.coords[i]);
	}
}

void Visible::getModelIArray(std::vector<unsigned int>& indexArray, size_t &prevArrNum)
{
	for (int i = 0; i < model.index.size(); i++) indexArray.push_back(model.index[i] + prevArrNum * getActorIndex());
	prevArrNum = model.coords.size() / 10;
}

void Visible::configure(const json& data, const std::string& vname, std::shared_ptr<Map> map, unsigned int& index)
{
	Actor::configure(data, vname, map, index);

	if (data.contains("model")) {
		unsigned int texID = DEFAULT_TEXTURE;
		Color col = DEFAULT_COLOR;

		bool softenNormal = false;
		if (data.contains("softenNormal")) softenNormal = data["softenNormal"];

		if (data.contains("texID")) texID = data["texID"];
		if (data.contains("material")) {
			material.diffuse = 1;
			material.specular = 1;

			if (data["material"].contains("shininess")) material.shininess = data["material"]["shininess"];
		}

		texture = { "-1", texID };
		model = ModelProcessor::arrayData(data["model"], texID, getActorIndex(), softenNormal);
	}
}

Color Visible::proccessColor(const json& data, const string& name, const string& vname)
{
	Color color(1);
	if (data.contains(name))
	{
		switch (data[name].size())
		{
		case 0:
			std::cout << "ATTENTION: THE OBJECT " << vname << " DOESN'T CONTAIN VALUES ON MATERIAL, VALUES WILL STAY AS 0." << std::endl;
			break;
		case 1:
			color = Color(data[name][0], 1);
			break;
		case 2:
			color = Color(data[name][0], data[name][1]);
			break;
		case 3:
			color = Color(data[name][0], data[name][1], data[name][2]);
			break;
		case 4:
			color = Color(data[name][0], data[name][1], data[name][2], data[name][3]);
			break;
		default:
			std::cout << "ATTENTION: THE OBJECT " << vname << " DOESN'T CONTAIN A VALID NUMBER OF ELEMENTS ON MATERIAL."<< name <<" ARRAY, FIRST ELEMENT WILL BE TAKEN FOR EVERY VALUE INSTEAD." << std::endl;
			color = Color(data[name][0]);
			break;
		}
	}

	return color;
}

void Visible::onUpdate(float deltaTime)
{
}