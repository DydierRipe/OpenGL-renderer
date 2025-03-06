#pragma once
#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"

#include "Vertex.h"

using std::string;
using std::vector;
class Shader;

class Model {
public:
	Model() {}
	Model(const char* path) {
		loadModel(path);
	}
	void draw(Shader& shader);
	vector<Mesh> meshes;
private:
	vector<Texture> textures_loaded;
	string directory;

	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};
