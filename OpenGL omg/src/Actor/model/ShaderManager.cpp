#include "ShaderManager.h"
#include "../../renderer/render dependencies/Shader.h"

ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
}

Shader& ShaderManager::addShader(string pathV, string pathF)
{
	string nameV = Shader::extractName(pathV);
	string nameF = Shader::extractName(pathF);

	if (auto shad = shaderList.find(nameV + "//" + nameF); shad != shaderList.end())
		return shad->second;

	auto shad = std::make_shared<Shader>(pathV, pathF);
	shaderList[nameV + "//" + nameF] = shad;
	return *shad.get();
}

std::weak_ptr<Shader> ShaderManager::getShader(string name)
{
	if (auto shad = shaderList.find(name); shad != shaderList.end()) 
		return shad->second;

	return {};
}
