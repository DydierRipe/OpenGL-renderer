#pragma once
#include "Actor.h"
#include "../Coords/ModelProcessor.h"
#include <string>

class Visible : public Actor
{
public:
	Visible();
	~Visible();

	void getModel(std::vector<float>& bufferArray);
	void getModelIArray(std::vector<unsigned int>& indexArray, size_t& prevArrNum);
	Color proccessColor(const json& data, const string& name, const string& vname);

	virtual void configure(const json& data, const std::string& vname, std::shared_ptr<Map> map, unsigned int& index) override;
	virtual void onUpdate(float deltaTime) override;

	inline Material getMaterial() const { return material; }
	inline TextureID getTexture() const { return texture; }

private:
	ModelProcessor::TBVertexValues model;

	TextureID texture;
	Material material;
};
