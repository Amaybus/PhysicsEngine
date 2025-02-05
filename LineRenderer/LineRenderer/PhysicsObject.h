#pragma once

#include "Vec2.h"
#include "Colour.h"

class LineRenderer;


class PhysicsObject
{
protected:
	Vec2 mPos{ 0,0 };
	Vec2 mVel{ 0,0 };
	Vec2 mAcc{ 0,0 };
	const float mMass = 0;
	Colour mColour;

public:
	PhysicsObject();
	PhysicsObject(Vec2 pos, float mass);

	virtual void Update(float delta) = 0;
	virtual void Draw(LineRenderer* lines) = 0;


	Vec2& GetPos() { return mPos; }
	Vec2& GetVelocity() { return mVel; }
	Vec2& GetAcceleration() { return mAcc; }
	float GetMass() const { return mMass; }
	float GetInverseMass() const { return 1.0f / mMass; }

	void SetPos(Vec2 pos) { mPos = pos; }
	void SetVelocity(Vec2 vel) { mVel = vel; }
	void SetAcc(Vec2 acc) { mAcc = acc; }
	void SetColour(Colour colour) { mColour = colour; }
};

