#pragma once
#include <memory>
#include <unordered_map>
#include <string>

class Model;

class ModelManager
{
public:
	ModelManager();
	~ModelManager();

	std::weak_ptr<Model> addModel(std::string path);
	std::weak_ptr<Model> getModel(std::string path);

	inline void unload(const std::string& path) { modelPool.erase(path); }
	inline void unloadAll() { modelPool.clear(); }

private:
	std::unordered_map<std::string, std::shared_ptr<Model>> modelPool;
};