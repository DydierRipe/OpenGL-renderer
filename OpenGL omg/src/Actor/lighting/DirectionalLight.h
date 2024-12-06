#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight();
	~DirectionalLight();

	inline Position getDirection() const { return direction; }

	virtual void configure(const json& data, const std::string& vname, std::shared_ptr<Map> map, unsigned int& index) override;
	virtual void onUpdate(float deltaTime) override;

private:
	Position direction;
};
