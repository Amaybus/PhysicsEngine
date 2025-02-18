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
	mAcc = /*mGravity +*/ mForceAccumulator * GetInverseMass();
	mVel += mAcc * delta;
	mPos += mVel * delta;

	// Rotation
	float cs = cosf(mOrientation);
	float sn = sinf(mOrientation);
	mLocalX = Vec2(cs, sn).Normalise();
	mLocalY = Vec2(-sn, cs).Normalise();

	// Update Rotation
	mOrientation += mAngularVelocity * delta;
	mForceAccumulator = Vec2();
}

float PhysicsObject::GetInverseMass() const
{
	if (mMass == 0) { return 0; }
	else return 1.0f / mMass;
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

void PhysicsObject::ApplyImpulse(Vec2 impulse, Vec2 contactPoint)
{
	Vec2 objSpace = contactPoint - mPos;
	mVel += impulse * GetInverseMass();
	mAngularVelocity += (impulse.y * objSpace.x - impulse.x * objSpace.y) / GetInertia();
}



