#pragma once
#include "Actor.h"
#include "model/Model.h"
#include <string>

class Visible : public Actor
{
public:
	Visible();
	~Visible();

	// draws
	void draw(Shader& shader);
	void changeModel(const string& path);
	void getModel(std::vector<float>& bufferArray); // LEGACY
	void getModelIArray(std::vector<unsigned int>& indexArray, size_t& prevArrNum); // LEGACY
	Color proccessColor(const json& data, const string& name, const string& vname); // IN REVISION FOR LEGACIFICATION (is that a word?)

	virtual void configure(const json& data, const string& vmapName, const string& vname, std::shared_ptr<Scene> vscene);
	virtual void onUpdate(float deltaTime) override;

	inline Model& getModel() const { return *model.lock(); }
	inline std::string getAssignedShaderName() const { return shaderName; }
	// "Enabled" is a value that is checked before rendering. If true, the object will be considered for rendering.
	inline bool getEnabled() const { return enabled; }
	// "Enabled" is a value that is checked before rendering. If true, the object will be considered for rendering.
	inline void setEnabled(bool val) { enabled = val; }

private:
	bool enabled = true;
	std::string shaderName;
	std::weak_ptr<Model> model;
};
