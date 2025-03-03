#pragma once
#include "Application.h"
#include "PhysicsObject.h"

class Game : public Application
{
	// List of all phyiscs objects in the scene
	std::vector<PhysicsObject*> mPhysicsObjects;
	std::vector<PhysicsObject*> mStaticObjects;
	std::vector<PhysicsObject*> mTargets;
	Vec2 mGravity = Vec2(0, -9.81f);

public:
	Game();
	~Game() override;

	void Initialise() override;
	void Update(float delta) override;

	void SetStaticObjects();
	void SetKinematicObjects();
	void SetTargets();

	void CheckForCollisions();
	void Draw(LineRenderer* lines);
	void OnLeftRelease() override;
};

