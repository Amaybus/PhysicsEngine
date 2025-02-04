#pragma once
#include "PhysicsObject.h"

class PhysicsObject;

class Plane : public PhysicsObject
{
	const Vec2 mNormal;
	const float mDistanceToOrigin;
	const float mLength;
	Vec2 mCentre;

public:
	Plane(Vec2 unitNormal, float distanceToOrigin, float length);
	~Plane();

	void Update(float delta);
	void Draw(LineRenderer* lines);

	Vec2 GetNormal() const { return mNormal; }
	float GetDistanceFromOrgin() const { return mDistanceToOrigin; }
};






