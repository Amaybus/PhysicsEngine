#pragma once
#include <vector>

#include "Application.h"
#include "Plane.h"

class PhysicsEngine : public Application
{
	// List of all phyiscs objects in the scene
	std::vector<PhysicsObject*> mPhysicsObjects;

public:
	PhysicsEngine();
	~PhysicsEngine() override;

	void Initialise() override;
	void Update(float delta) override;
};