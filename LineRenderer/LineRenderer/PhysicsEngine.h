#pragma once
#include <vector>

#include "Application.h"
#include "Circle.h"
#include "Plane.h"
#include "Box.h"

class PhysicsEngine : public Application
{
private:
	std::vector<Circle*> mCircles;
	Plane* mPlanes[1];
	std::vector<Box*> mBoxes;

public:
	PhysicsEngine();

	void Initialise() override;
	void Update(float delta) override;
	void OnLeftClick() override;
};