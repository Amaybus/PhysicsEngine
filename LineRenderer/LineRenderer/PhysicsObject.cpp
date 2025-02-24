#include "PhysicsObject.h"
#include "PhysicsEngine.h"
#include "LineRenderer.h"

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

void PhysicsObject::Draw(LineRenderer* lines)
{
	lines->DrawLineWithArrow(mPos, mPos + mLocalY, Colour::GREEN, 0.1);
	lines->DrawLineWithArrow(mPos, mPos + mLocalX, Colour::RED, 0.1);
}

float PhysicsObject::GetInverseMass() const
{
	if (mMass == 0) { return 0; }
	else return 1.0f / mMass;
}

float PhysicsObject::GetInverseInertia() const
{
	if (mInertia == 0) { return 0; }
	else return 1.0f / mInertia;
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
	mAngularVelocity += (impulse.y * objSpace.x - impulse.x * objSpace.y) * GetInverseInertia();
}



