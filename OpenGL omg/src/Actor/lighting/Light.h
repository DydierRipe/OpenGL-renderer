#pragma once

#include "../Visible.h"

class Light : public Visible
{
public:
	Light();
	~Light();

	virtual void onUpdate(float deltaTime) override;
	virtual void configure(const json& data, const std::string& vname, std::shared_ptr<Map> map, unsigned int& index) override;

	inline void setAttachment(std::weak_ptr<Actor> att) { attach = att; }
	inline Color getAmbient() { return ambient; }
	inline Color getDiffuse() { return diffuse; }
	inline Color getSpecular() { return specular; }

protected:
	std::weak_ptr<Actor> attach;
private:

	float rTraslation = 0;

	Color ambient;
	Color diffuse;
	Color specular;
};
