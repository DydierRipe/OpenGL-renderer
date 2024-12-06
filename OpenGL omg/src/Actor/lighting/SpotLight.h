#pragma once
#include "PointLight.h"

class SpotLight : public PointLight
{
public:
	SpotLight();
	~SpotLight();

	inline Position getDirection() const { return direction; }
	inline float getCutOff() const { return cutOff; }
	inline float getInnerCutOff() const { return innerCutOff; }

	virtual void configure(const json& data, const string& vname, std::shared_ptr<Map> map, unsigned int& index) override;
	virtual void onUpdate(float deltaTime) override;

private:

	Position direction;
	float cutOff;
	float innerCutOff;
};
