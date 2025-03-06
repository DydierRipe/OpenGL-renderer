#pragma once

#include <memory>
#include <unordered_map>
#include <string>

class Shader;
using std::unordered_map;

class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();

	std::weak_ptr<Shader> addShader(string pathV, string pathF);
	Shader& getShader(string name);
	inline void deleteShader(string name) { shaderList.erase(name); }
private:
	unordered_map<string, std::shared_ptr<Shader>> shaderList;
};