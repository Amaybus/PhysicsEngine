#pragma once

#include "Vec2.h"
#include "Colour.h"

class LineRenderer;


enum class ObjectType
{
	Circle,
	Plane,
	Box,
	Polygon,

	Count
};

class PhysicsObject
{
protected:
	// Location
	Vec2 mPos{ 0,0 };
	// How position changes over time
	Vec2 mVel{ 0,0 };
	// How the velocity changes over time
	Vec2 mAcc{ 0,0 };
	const float mMass = 0;
	Colour mColour;

	Vec2 mForceAccumulator;
	Vec2 mGravity = Vec2(0, -9.81f);

	// Rotation
	// Rotation axis
	Vec2 mLocalX;
	Vec2 mLocalY;

	// How the object is rotated in radians
	float mOrientation = 0;
	// When an object rotates from one orientation to another. degrees per second
	float mAngularVelocity = 0;
	// Resistance against angular acc around the pivot
	float mInertia = 0;

public:
	PhysicsObject();
	PhysicsObject(Vec2 pos, float mass);

	virtual void Update(float delta);
	virtual void Draw(LineRenderer* lines) = 0;


	Vec2& GetPos() { return mPos; }
	Vec2& GetVelocity() { return mVel; }
	Vec2& GetAcceleration() { return mAcc; }
	float GetAngularVelocity() const { return mAngularVelocity; }
	float GetMass() const { return mMass; }
	float GetInverseMass() const;
	float GetInertia() const { return mInertia; }
	float GetInverseInertia() const;
	virtual int GetType() const = 0;

	Vec2 GetLocalY() { return mLocalY; }
	Vec2 GetLocalX() { return mLocalX; }

	virtual void SetPos(Vec2 pos) { mPos = pos; }
	void SetVelocity(Vec2 vel) { mVel = vel; }
	void SetAcc(Vec2 acc) { mAcc = acc; }
	void SetColour(Colour colour) { mColour = colour; }

	void ApplyForce(Vec2 force);
	void ApplyImpulse(Vec2 impulse);
	void ApplyImpulse(Vec2 impulse, Vec2 contactPoint);
};

