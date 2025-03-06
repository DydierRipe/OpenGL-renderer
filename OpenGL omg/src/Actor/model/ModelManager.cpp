#include "ModelManager.h"
#include "Model.h"

ModelManager::ModelManager()
{
}

ModelManager::~ModelManager()
{
}
// NOMBRAR SEGUN PATH NO ES UNA FORMA IDEAL DE HACER ESTO, BUSCA OTRA FORMA DE GENERAR OTRA ENTRADA MEJOR
std::weak_ptr<Model> ModelManager::addModel(std::string path)
{
	if (auto model = modelPool.find(path); model != modelPool.end()) 
		return model->second;

	auto model = std::make_shared<Model>(path);
	modelPool[path] = model;
	return model;
}

std::weak_ptr<Model> ModelManager::getModel(std::string path)
{
	if (auto model = modelPool.find(path); model != modelPool.end()) 
		return model->second;

	return {};
}
