#pragma once
#include "PhysicsObject.h"

class PhysicsObject;

class Plane : public PhysicsObject
{
	const Vec2 mNormal;
	const float mDistanceToOrigin;
	const float mLength = 50;
	Vec2 mCentre;

public:
	Plane(Vec2 unitNormal, float distanceToOrigin);
	void Draw(LineRenderer* lines);

	Vec2 GetNormal() const { return mNormal; }
	Vec2 GetCentre() const { return mCentre; }
	float GetDistanceFromOrigin() const { return mDistanceToOrigin; }
	int GetType() const override { return 1; }
};






