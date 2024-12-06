#pragma once

#include <assert.h>
#include <iostream>
#include <array>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#ifndef ATTRIB_DEFINES
#define ATTIB_DEFINES

#define DEFAULT_COLOR {1.0f, 1.0f, 1.0f, 1.0f}
#endif

constexpr unsigned int DEFAULT_TEXTURE = 0;

struct Size2D {
	float x, y;

	Size2D(float u) : x(u), y(u) {}
	Size2D(float tx, float ty) : x(tx), y(ty) {}

	std::array<float, 2> data() const {
		return { x, y};
	}
};

struct Size {
	float x, y, z;

	Size(float u) : x(u), y(u), z(u) {}
	Size(float tx, float ty, float tz) : x(tx), y(ty), z(tz) {}

	std::array<float, 3> data() const {
		return { x, y, z };
	}
};

struct Position2D {
	float x, y;

	Position2D(float u) : x(u), y(u) {}

	std::array<float, 2> data() const {
		return { x, y };
	}
};

struct Position {
	float x, y, z;

	Position(glm::vec3 v) : x(v.x), y(v.y), z(v.z) {}
	Position(float u) : x(u), y(u), z(u) {}
	Position(float tx, float ty, float tz) : x(tx), y(ty), z(tz) {}

	std::array<float, 3> data() const {
		return { x, y, z };
	}
};

struct Rotation2D {
	float yaw, pitch;

	Rotation2D(float u) : yaw(u), pitch(u) {}
	Rotation2D(float y, float p) : yaw(y), pitch(p) {}

	std::array<float, 2> data() const {
		return { yaw , pitch};
	}
};

struct Rotation {
	float yaw, pitch, roll;
	// YOU WERE CHANGING ALL THE VALUES TO YAW PITCH ROLL ORDER SINCE THEY ARE XYZ IN THAT ORDER, 
	// MAKE SURE TO ALSO MODIFY TRANSFORM, ACTOR CONFIG AND EVERYTHING THAT USES THIS
	Rotation(float u) : yaw(u), pitch(u), roll(u) {}
	Rotation(float y, float p, float r) : yaw(y), pitch(p), roll(r) {}

	std::array<float, 3> data() const {
		return { yaw, pitch, roll };
	}
};

struct Color {
	float red, green, blue, alpha;

	Color(float u) : red(u), green(u), blue(u), alpha(u) {}
	Color(float c, float a) : red(c), green(c), blue(c), alpha(a) {}
	Color(float r, float g, float b) : red(r), green(g), blue(b), alpha(1) {}
	Color(float r, float g, float b, float a) : red(r), green(g), blue(b), alpha(a) {}

	std::array<float, 4> data() const {
		return { red, green, blue, alpha };
	}
};

struct TextureID {
	std::string path;
	unsigned int ID;

	TextureID(const std::string& str, unsigned int texID) : path(str), ID(texID) {}
};


struct Material {
	unsigned int diffuse;
	unsigned int specular;

	float shininess;
};
