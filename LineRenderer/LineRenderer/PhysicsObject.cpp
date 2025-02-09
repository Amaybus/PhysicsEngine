#include "PhysicsObject.h"

#include "PhysicsEngine.h"


PhysicsObject::PhysicsObject()
{
}

PhysicsObject::PhysicsObject(Vec2 pos, float mass) : mPos(pos), mMass(mass)
{
}

void PhysicsObject::Update(float delta)
{
	mAcc = mGravity + mForceAccumulator * GetInverseMass();
	mVel += mAcc * delta;
	mPos += mVel * delta;

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