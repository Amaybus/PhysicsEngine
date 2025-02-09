#include "PhysicsObject.h"


PhysicsObject::PhysicsObject()
{
}

PhysicsObject::PhysicsObject(Vec2 pos, float mass) : mPos(pos), mMass(mass)
{
}

void PhysicsObject::Update(float delta)
{
	mAcc = mGravity + mForceAccumulator * GetInverseMass();
	mPos += mVel * delta;
	mVel += mAcc * delta;

	mForceAccumulator = Vec2();
}

// Applied over a duration
void PhysicsObject::ApplyForce(Vec2 force)
{
	mForceAccumulator += force;
}

// Instantaneous application
void PhysicsObject::ApplyImpulse(Vec2 impulse)
{
	mVel += impulse * GetInverseMass();
}