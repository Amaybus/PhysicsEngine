#include "CollisionInfo.h"


void CollisionInfo::Resolve()
{
	if (!bIsOverlapping) return;

	float totalInverseMass = objA->GetInverseMass() + objB->GetInverseMass();

	// If objA is a plane
	if (objA->GetInverseMass() == 0)
	{
		totalInverseMass = objB->GetInverseMass();
		//Depen Step
		objB->GetPos() += collisionNormal * overlapAmount;
	}

	// Depen step
	objA->GetPos() -= collisionNormal * overlapAmount * objA->GetInverseMass() / totalInverseMass;
	objB->GetPos() += collisionNormal * overlapAmount * objB->GetInverseMass() / totalInverseMass;

	// Apply Force
	Vec2 relVe = objB->GetVelocity() - objA->GetVelocity();
	float impulseMag = Dot(-2 * relVe, collisionNormal) / totalInverseMass;

	if (impulseMag < 0) return;
	Vec2 force = collisionNormal * impulseMag;

	objA->ApplyImpulse(-force);
	objB->ApplyImpulse(force);
}

void CollisionInfo::ResolveWithRotation()
{
	if (objA->IsTrigger())
	{
		objA->OnBeginOverlap(objB);
		return;
	}

	if (objB->IsTrigger())
	{
		objB->OnBeginOverlap(objA);
		return;
	}

	// Call collision for each object
	objA->OnCollisionEnter(objB);
	objB->OnCollisionEnter(objA);

	float totalInverseMass = objA->GetInverseMass() + objB->GetInverseMass();
	if (totalInverseMass == 0) { return; }

	// Depen
	objA->GetPos() -= collisionNormal * overlapAmount * objA->GetInverseMass() / totalInverseMass;
	objB->GetPos() += collisionNormal * overlapAmount * objB->GetInverseMass() / totalInverseMass;

	// Find perpendicular angle to collision point
	Vec2 perp = Vec2(collisionNormal.y, -collisionNormal.x);

	// Lever arm
	float ra = Dot(aContactPoints[0] - objA->GetPos(), -perp);
	float rb = Dot(bContactPoints[0] - objB->GetPos(), perp);

	// Relative velocity
	float va = Dot(objA->GetVelocity(), collisionNormal) - ra * objA->GetAngularVelocity();
	float vb = Dot(objB->GetVelocity(), collisionNormal) + rb * objB->GetAngularVelocity();

	// Find elasticity
	float elasticity = (objA->GetElasticity() + objB->GetElasticity()) * 0.5f;

	if (va > vb)
	{
		float massA = 1.0f / (objA->GetInverseMass() + (ra * ra) * objA->GetInverseInertia());
		float massB = 1.0f / (objB->GetInverseMass() + (rb * rb) * objB->GetInverseInertia());

		Vec2 force = (1+elasticity) * massA * massB / (massA + massB) * (va - vb) * collisionNormal;

		// For collision with planes or static objects
		if(objA->GetInverseMass() == 0)
		{
			Vec2 velB = GetVelocityAtPoint(objB, bContactPoints[0] - objB->GetPos());
			force = (-(1 + elasticity) * Dot(velB, collisionNormal) * massB) * collisionNormal;
		}

		// For collision with static objects 
		if (objB->GetInverseMass() == 0)
		{
			// Inverse the force
			Vec2 velA = GetVelocityAtPoint(objA, aContactPoints[0] - objA->GetPos());
			force = -(-(1 + elasticity) * Dot(velA, collisionNormal) * massA) * collisionNormal;
		}

		// Apply equal amount of force over each contact point
		for(Vec2& cp : aContactPoints)
		{
			objA->ApplyImpulse(-force / (float)aContactPoints.size(), cp);
		}

		for (Vec2& cp : bContactPoints)
		{
			objB->ApplyImpulse(force / (float)bContactPoints.size(), cp);
		}
	}
}

Vec2 CollisionInfo::GetVelocityAtPoint(PhysicsObject* object, Vec2 contactPoint)
{
	return object->GetVelocity() + object->GetAngularVelocity() * Vec2(-contactPoint.y, contactPoint.x);
}
