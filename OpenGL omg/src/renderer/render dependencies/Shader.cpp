#include "shader.h"
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "glm/gtc/type_ptr.hpp"

#include "../Actor/lighting/DirectionalLight.h"
#include "../Actor/lighting/PointLight.h"
#include "../Actor/lighting/SpotLight.h"

Shader::Shader(const string& vertex, const string& fragment) : pathV(vertex), pathF(fragment), rendererID(0)
{
    shaderSource src = parseShader(pathV, pathF);
    rendererID = createShader(src.vertexSrc, src.fragmentSrc);

}

Shader::~Shader() { glDeleteProgram(rendererID); }

shaderSource Shader::parseShader(const string& vertex, const string& fragment) {
    std::ifstream streamV(vertex);
    std::ifstream streamF(fragment);

    string line;
    std::stringstream ss[2];

    while (getline(streamV, line)) {
        ss[0] << line << "\n";
    }

    while (getline(streamF, line)) {
        ss[1] << line << "\n";
    }

    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::compileShader(const string& source, unsigned int type) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* mes = (char*)_malloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, mes);

        std::cout << "failed to compile shader: " << (type == GL_VERTEX_SHADER ? "vertex " : "fragment") << std::endl;
        std::cout << mes << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int Shader::createShader(const string& vertexShader, const string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(vertexShader, GL_VERTEX_SHADER);
    unsigned int fs = compileShader(fragmentShader, GL_FRAGMENT_SHADER);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void Shader::bind() const
{
    glUseProgram(rendererID);
}

void Shader::unbind() const
{
    glUseProgram(0);
}

void Shader::setUniform1b(const string& name, bool v0)
{
    glUniform1i(getUniformLocation(name), v0);
}

void Shader::setUniform1i(const string& name, int v0)
{
    glUniform1i(getUniformLocation(name), v0);
}

void Shader::setUniform1f(const string& name, float v0)
{
    glUniform1f(getUniformLocation(name), v0);
}

void Shader::setUniform3f(const string& name, float v0, float v1, float f2)
{
    glUniform3f(getUniformLocation(name), v0, v1, f2);
}

void Shader::setUniform4f(const string& name, float v0, float v1, float f2, float f3)
{
    glUniform4f(getUniformLocation(name), v0, v1, f2, f3);
}

void Shader::setUniformMat4f(const string& name, const glm::mat4& matrix)
{
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::setUniformMat4fa(const string& name, std::vector<glm::mat4> matrices)
{
    glUniformMatrix4fv(getUniformLocation(name), matrices.size(), GL_FALSE, glm::value_ptr(matrices[0]));
}

void Shader::setUniform1iv(const string& name, int size, int samplers[])
{
    glUniform1iv(getUniformLocation(name), size, samplers);
}

void Shader::setUniformDirectional(const string& name, DirectionalLight &light)
{
    setUniformLightProperties(name, light);
    glUniform3f(getUniformLocation(name + ".direction"), light.getDirection().x, light.getDirection().y, light.getDirection().z);
}

void Shader::setUniformPoint(const string& name, PointLight &light)
{
    setUniformLightProperties(name, light);
    Position lightPos = light.getTransform().getPosition();

    glUniform3f(getUniformLocation(name + ".position"), lightPos.x, lightPos.y, lightPos.z);
    glUniform1f(getUniformLocation(name + ".constant"), light.getConstant());
    glUniform1f(getUniformLocation(name + ".linear"), light.getLinear());
    glUniform1f(getUniformLocation(name + ".quadratic"), light.getQuadratic());
}

void Shader::setUniformSpot(const string& name, SpotLight &light)
{
    setUniformLightProperties(name, light);
    setUniformPoint(name, light);

    glUniform3f(getUniformLocation(name + ".direction"), light.getDirection().x, light.getDirection().y, light.getDirection().z);
    glUniform1f(getUniformLocation(name + ".cutOff"), glm::cos(glm::radians(light.getCutOff())));
    glUniform1f(getUniformLocation(name + ".innerCutOff"), glm::cos(glm::radians(light.getInnerCutOff())));
}

void Shader::setUniformPointa(const string& name, const std::vector<std::shared_ptr<PointLight>>& lights)
{
    for (size_t i = 0; i < lights.size(); i++) setUniformPoint(name + "[" + std::to_string(i) + "]", *lights[i]);
}

void Shader::setUniformSpota(const string& name, const std::vector<std::shared_ptr<SpotLight>>& lights)
{
    for (size_t i = 0; i < lights.size(); i++) setUniformSpot(name + "[" + std::to_string(i) + "]", *lights[i]);
}

void Shader::setUniformLightProperties(const string& name, Light light)
{
    glUniform3f(getUniformLocation(name + ".ambient"), light.getAmbient().red, light.getAmbient().green, light.getAmbient().blue);
    glUniform3f(getUniformLocation(name + ".diffuse"), light.getDiffuse().red, light.getDiffuse().green, light.getDiffuse().blue);
    glUniform3f(getUniformLocation(name + ".specular"), light.getSpecular().red, light.getSpecular().green, light.getSpecular().blue);
}

int Shader::getUniformLocation(const string& name)
{
    if (uLocationCache.find(name) != uLocationCache.end()) return uLocationCache[name];

    int location = glGetUniformLocation(rendererID, name.c_str());
    if (location == -1) std::cout << name << " doesn't exist." << std::endl;
    
    uLocationCache[name] = location;
    return location;
}


