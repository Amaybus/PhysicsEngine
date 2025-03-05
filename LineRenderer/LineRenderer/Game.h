#pragma once
#include "Application.h"
#include <vector>

class PhysicsObject; 

class Game : public Application
{
	std::vector<PhysicsObject*> mPhysicsObjects;
	std::vector<PhysicsObject*> mStaticObjects;
	std::vector<PhysicsObject*> mMarkers;
	std::vector<PhysicsObject*> mOverlappedMarkers;
	std::vector<PhysicsObject*> mProjectiles;
	int mProjectileIndex;
	int mMaxProjectiles = 3;
	int mCurrentProjectiles = 0;

	PhysicsObject* mPlayer;
	Vec2 mPlayerPos = Vec2();

	Vec2 mGravity = Vec2(0, -9.81f);
	Vec2 mCursorPosEnd;
	Vec2 mProjectionVelocity;
	bool bDrawProjectionLine = false;

	int mTotalMarkersCollected = 0;

public:
	Game();
	Game(const Game& other) = delete; //The 'game' class is uncopyable
	Game& operator=(const Game& other) = delete; //and unassignable, which is kind of the same thing
	~Game() override;

	void Initialise() override;
	void Update(float delta) override;

	void SetStaticObjects();
	void SpawnRandomKinematicObjects();
	void SetTargets();

	void CheckForCollisions();
	void Draw(LineRenderer* lines);
	void OnLeftClick() override;
	void OnLeftRelease() override;
};

