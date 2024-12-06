#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

#include "glm/glm.hpp"

class Light;
class DirectionalLight;
class PointLight;
class SpotLight;

using std::string;

struct shaderSource {
	string vertexSrc;
	string fragmentSrc;
};

class Shader
{
private:
	string pathV;
	string pathF;
	unsigned int rendererID;
	std::unordered_map<string, int> uLocationCache;

public:
	Shader(const string& vertex, const string& fragment);
	~Shader();

	void bind() const;
	void unbind() const;

	void setUniform1b(const string& name, bool v0);
	void setUniform1i(const string& name, int v0);
	void setUniform1f(const string& name, float v0);
	void setUniform4f(const string& name, float v0, float v1, float f2, float f3);
	void setUniform3f(const string& name, float v0, float v1, float f2);
	void setUniformMat4f(const string& name, const glm::mat4& matrix);
	void setUniformMat4fa(const string& name, std::vector<glm::mat4> matrices);
	void setUniform1iv(const string& name, int size, int samplers[]);

	void setUniformDirectional(const string& name, DirectionalLight &light);
	void setUniformPoint(const string& name, PointLight& light);
	void setUniformSpot(const string& name, SpotLight& light);

	void setUniformPointa(const string& name, const std::vector<std::shared_ptr<PointLight>>& lights);
	void setUniformSpota(const string& name, const std::vector<std::shared_ptr<SpotLight>>& lights);

private:
	
	void setUniformLightProperties(const string& name, Light light);
	shaderSource parseShader(const string& vertex, const string& fragment);
	unsigned int compileShader(const string& source, unsigned int type);
	unsigned int createShader(const string& vertexShader, const string& fragmentShader);

	int getUniformLocation(const string& name);
};
