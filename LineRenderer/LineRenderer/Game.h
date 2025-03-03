#pragma once
#include "Application.h"
#include "PhysicsObject.h"

class Game : public Application
{
	// List of all phyiscs objects in the scene
	std::vector<PhysicsObject*> mPhysicsObjects;
	Vec2 mGravity = Vec2(0, -9.81f);

public:
	Game();
	~Game() override;

	void Initialise() override;
	void Update(float delta) override;

	void CheckForCollisions();
	void Draw(LineRenderer* lines);
	void OnLeftRelease() override;
};

