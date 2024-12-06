#pragma once
#include "Light.h"

class PointLight : public Light
{
public:
	PointLight();
	~PointLight();

	virtual void configure(const json& data, const string& vname, std::shared_ptr<Map> map, unsigned int& index) override;
	virtual void onUpdate(float deltaTime);

	inline float getConstant() const { return constant; }
	inline float getLinear() const { return linear; }
	inline float getQuadratic() const { return quadratic; }

private:
	float constant;
	float linear;
	float quadratic;
};
