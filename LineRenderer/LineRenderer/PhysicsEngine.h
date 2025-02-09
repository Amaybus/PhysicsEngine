#pragma once
#include <vector>

#include "Application.h"
#include "Plane.h"

class PhysicsEngine : public Application
{
	std::vector<PhysicsObject*> mPhysicsObjects;

public:
	PhysicsEngine();

	void Initialise() override;
	void Update(float delta) override;
	void OnLeftClick() override;
};