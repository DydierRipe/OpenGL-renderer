#include "Visible.h"
#include "Scene.h"
#include <Windows.h>
#include "../renderer/render dependencies/Shader.h"

/*
	POR IMPLEMENTAR: 

		implementa instanciacion y ssbo

		crea una clase y un archivo de mapa, estas tienen que contener y cargar los objetos a la vez que guardarlos y manejar los punteros a cada uno

*/

Visible::Visible()
	: Actor()
{
}

Visible::~Visible()
{
}

void Visible::draw(Shader& shader)
{
	shader.setUniformMat4f("u_model", getTransform().getTransformMatrix());
	shader.setUniform1f("u_material.shininess", 32.0f);
	if (auto mod = model.lock()) mod->draw(shader);
}

void Visible::changeModel(const string& path)
{
	model = scene->getModelManager().getModel(path);
}

//LEGACY
void Visible::getModel(std::vector<float>& bufferArray)
{
	//for (int i = 0; i < model.coords.size(); i++) {
		//bufferArray.push_back(model.coords[i]);
	//}
}
//LEGACY
void Visible::getModelIArray(std::vector<unsigned int>& indexArray, size_t &prevArrNum)
{
	//for (int i = 0; i < model.index.size(); i++) indexArray.push_back(model.index[i] + prevArrNum * getActorIndex());
	//prevArrNum = model.coords.size() / 10;
}

void Visible::configure(const json& data, const string& vmapName, const string& vname, std::shared_ptr<Scene> vscene)
{
	Actor::configure(data, vmapName, vname, vscene);

	if (data.contains("model")) {
		Color col = DEFAULT_COLOR;
		
		model = scene->getModelManager().addModel(data["model"]);
	}

	if (data.contains("vertex_shader") && data.contains("fragment_shader")) {

		std::weak_ptr<Shader> shad = scene->getShaderManager().addShader(data["vertex_shader"], data["fragment_shader"]);
		if (auto shader = shad.lock())
			shaderName = shader->getName();
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