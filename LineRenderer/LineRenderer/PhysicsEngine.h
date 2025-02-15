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

	// DELETE AFTER DEBUGGING
	float projectionAngle = 0.0f;
	int normalIndex = 0;
	int edgeIndex = 0;
};