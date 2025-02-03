#pragma once
#include <vector>

#include "Application.h"
#include "Circle.h"

class PhysicsEngine : public Application
{
private:
	//Any data that should persist for the duration of your program,
	//declare it here.

	std::vector<Circle*> mCircles;

public:
	PhysicsEngine();

	void Initialise() override;
	void Update(float delta) override;
	void OnLeftClick() override;
};