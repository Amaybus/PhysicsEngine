#pragma once
#include <vector>

#include "Application.h"
#include "Plane.h"

class PhysicsEngine : public Application
{
	// List of all phyiscs objects in the scene
	std::vector<PhysicsObject*> mPhysicsObjects;
	Vec2 mGravity = Vec2(0, -9.81f);

public:
	PhysicsEngine();
	~PhysicsEngine() override;

	void Initialise() override;
	void Update(float delta) override;
};