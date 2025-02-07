#pragma once
#include <vector>

#include "Application.h"
#include "Circle.h"
#include "Plane.h"
#include "Box.h"
#include "Polygon.h"

class PhysicsEngine : public Application
{
	std::vector<PhysicsObject*> mPhysicsObjects;

public:
	PhysicsEngine();

	void Initialise() override;
	void Update(float delta) override;
	void OnLeftClick() override;
};