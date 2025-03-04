#pragma once

#include "Vec2.h"
#include "Colour.h"
#include "Maths.h"
#include <vector>

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
	Colour mColour = Colour::GREEN;
	float mElasticity = 0.5f;

	// Can the object move 
	bool bIsKinematic = true;

	// Rotation axis
	Vec2 mLocalX;
	Vec2 mLocalY;

	// How the object is rotated in radians
	float mOrientation = 0;
	// When an object rotates from one orientation to another. (Degrees per second)
	float mAngularVelocity = 0;
	// Resistance against angular acc around the pivot
	float mInertia = 0;

	// Force
	Vec2 mForceAccumulator;
	float mAngularDrag = 0.3f;
	float mLinearDrag = 0.7f;

	bool bIsTrigger = false;
	bool bCanCollide = true;

	std::vector <PhysicsObject*> mOverlappingObjects;
	std::vector <PhysicsObject*> mCollidingObjects;

	bool bIgnoreCollisionOfSameType = false;

public:
	PhysicsObject();
	PhysicsObject(Vec2 pos, float mass);
	PhysicsObject(Vec2 pos, float mass, float elasticity);
	virtual ~PhysicsObject() {}

	void Update(float delta);
	virtual void Draw(LineRenderer* lines);

	Vec2& GetPos() { return mPos; }
	void SetPos(Vec2 pos) { mPos = pos; }

	Vec2& GetVelocity() { return mVel; }
	void SetVelocity(Vec2 vel) { mVel = vel; }

	Vec2& GetAcceleration() { return mAcc; }
	void SetAcceleration(Vec2 acc) { mAcc = acc; }

	void SetIsKinematic(bool value);
	bool GetKinematic() const { return bIsKinematic; }

	float GetMass() const { return mMass; }
	float GetInverseMass() const;
	float GetAngularVelocity() const { return mAngularVelocity; }
	float GetElasticity() const { return mElasticity; }
	float GetInertia() const { return mInertia; }
	float GetInverseInertia() const;
	virtual int GetType() const = 0;

	float GetOrientation() const { return mOrientation; }
	void SetOrientation(float orientation) { mOrientation = DegToRad(orientation); }

	void SetColour(Colour colour) { mColour = colour; }
	void SetElasticity(float elasticity);

	Vec2 GetLocalY() const{ return mLocalY; }
	Vec2 GetLocalX() const{ return mLocalX; }

	void ApplyForce(Vec2 force);
	void ApplyImpulse(Vec2 impulse);
	void ApplyImpulse(Vec2 impulse, Vec2 contactPoint);

	void OnBeginOverlap(PhysicsObject* other);
	void OnEndOverlap(PhysicsObject* other);

	void OnCollisionEnter(PhysicsObject* other);
	void OnCollisionExit(PhysicsObject* other);

	void SetCollision(bool value) { bCanCollide = value; }
	bool CanCollide() const { return bCanCollide; }

	void SetAsTrigger(bool value);
	bool IsTrigger() const { return bIsTrigger; }

	void SetIgnoreCollisionOfSameType(bool value) { bIgnoreCollisionOfSameType = value; }
	bool GetIgnoreCollisionOfSameType() const { return bIgnoreCollisionOfSameType; }
};

