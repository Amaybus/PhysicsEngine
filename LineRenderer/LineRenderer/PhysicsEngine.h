#pragma once
#include <vector>

#include "Application.h"
#include "Circle.h"
#include "Plane.h"
#include "Box.h"
#include "Polygon.h"

class PhysicsEngine : public Application
{
	std::vector<Circle*> mCircles;
	std::vector<Plane*> mPlanes;
	std::vector<Box*> mBoxes;
	std::vector<Polygon*> mPolygons;

public:
	PhysicsEngine();

	void Initialise() override;
	void Update(float delta) override;
	void OnLeftClick() override;
};