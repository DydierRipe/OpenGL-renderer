#pragma once

#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using std::string;

struct Vertex {
	glm::vec3 vertex;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Texture {
	unsigned int id;
	string type;
	string path;
};
