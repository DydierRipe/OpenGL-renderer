#pragma once

#include "../renderer/render dependencies/Shader.h"
#include "../renderer/render dependencies/VertexArray.h"
#include "../renderer/render dependencies/VertexBuffer.h"
#include "../renderer/render dependencies/IndexBuffer.h"
#include "../renderer/render dependencies/VertexBufferLayout.h"
#include "Vertex.h"
#include <vector>

using std::vector;

class Mesh {
public:
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	Mesh(vector<Vertex>, vector<unsigned int>, vector<Texture>);
	void draw(Shader &shader);

private:
	unsigned int diffuseTC, specularTC, emissionTC;
	unsigned int VAO, VBO, EBO;

	bool bNeedToChangeTex = true;

	//VertexArray VAO;
	//VertexBuffer VBO;
	//IndexBuffer EBO;
	//VertexBufferLayout layout;

	void createMesh();
	void setupTexture(Shader &shader);
};
