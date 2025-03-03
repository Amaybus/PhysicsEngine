#include "PhysicsObject.h"
#include "LineRenderer.h"

PhysicsObject::PhysicsObject()
{
}

PhysicsObject::PhysicsObject(Vec2 pos, float mass) : mPos(pos), mMass(mass)
{
}

PhysicsObject::PhysicsObject(Vec2 pos, float mass, float elasticity) : mPos(pos), mMass(mass)
{
	Clamp(elasticity, 0.0f, 1.0f);
	mElasticity = elasticity;
}

void PhysicsObject::Update(float delta)
{
	if (bIsTrigger) { return; }

	// Orientation of object
	float cs = cosf(mOrientation);
	float sn = sinf(mOrientation);
	mLocalX = Vec2(cs, sn).Normalise();
	mLocalY = Vec2(-sn, cs).Normalise();

	// Static Objects
	if(!bIsKinematic || bIsTrigger)
	{
		mVel = Vec2();
		mAngularVelocity = 0;
		mForceAccumulator = Vec2();
		return;
	}

	// Kinematic Objects
	mAcc = mForceAccumulator * GetInverseMass();
	mVel += mAcc * mLinearDrag * delta;
	if(mVel.GetMagnitude() < 0.1)
	{
		mVel = Vec2();
	}
	mPos += mVel * delta;

	// Update Rotation
	mAngularVelocity -= mAngularVelocity * mAngularDrag * delta;
	if(abs(mAngularVelocity) < 0.01)
	{
		mAngularVelocity = 0;
	}

	mOrientation += mAngularVelocity * delta;
	mForceAccumulator = Vec2();
}

void PhysicsObject::Draw(LineRenderer* lines)
{
	lines->DrawLineWithArrow(mPos, mPos + mLocalY, Colour::GREEN, 0.1);
	lines->DrawLineWithArrow(mPos, mPos + mLocalX, Colour::RED, 0.1);
}

void PhysicsObject::SetIsKinematic(bool value)
{
	bIsKinematic = value;
	mElasticity = 1;
	mInertia = 0;
	mColour = Colour::BLUE.Lighten();
}

float PhysicsObject::GetInverseMass() const
{
	if (mMass == 0 || bIsKinematic == false) { return 0; }
	else return 1.0f / mMass;
}

float PhysicsObject::GetInverseInertia() const
{
	if (mInertia == 0 || bIsKinematic == false) { return 0; }
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

void PhysicsObject::OnBeginOverlap(PhysicsObject* other)
{
	mColour = Colour::ORANGE.Darken();
	// Store overlapping element
	std::vector<PhysicsObject*>::iterator it = std::find(mOverlappingObjects.begin(), mOverlappingObjects.end(), other);
	if (it == mOverlappingObjects.end())
	{
		mOverlappingObjects.push_back(other);
	}
}

void PhysicsObject::OnEndOverlap(PhysicsObject* other)
{
	// Remove element that is no longer overlapping 
	std::vector<PhysicsObject*>::iterator it = std::find(mOverlappingObjects.begin(), mOverlappingObjects.end(), other);
	if (it != mOverlappingObjects.end())
	{
		mOverlappingObjects.erase(it);
	}

	if (mOverlappingObjects.size() <= 0)
	{
		mColour = Colour::ORANGE;
	}
}

void PhysicsObject::OnCollisionEnter(PhysicsObject* other)
{
	mColour = Colour::RED;
	// Store colliding element
	std::vector<PhysicsObject*>::iterator it = std::find(mCollidingObjects.begin(), mCollidingObjects.end(), other);
	if (it == mCollidingObjects.end())
	{
		mCollidingObjects.push_back(other);
	}
}

void PhysicsObject::OnCollisionExit(PhysicsObject* other)
{
	// Remove element that is no longer colliding
	std::vector<PhysicsObject*>::iterator it = std::find(mCollidingObjects.begin(), mCollidingObjects.end(), other);
	if (it != mCollidingObjects.end())
	{
		mCollidingObjects.erase(it);
	}

	if (mCollidingObjects.size() <= 0)
	{
		if(bIsKinematic) { mColour = Colour::GREEN; }
		else { mColour = Colour::BLUE.Lighten();}
	}
}

void PhysicsObject::SetAsTrigger(bool value)
{
	bIsTrigger = value; 
	bCanCollide = false;
	mColour = Colour::ORANGE;
}
