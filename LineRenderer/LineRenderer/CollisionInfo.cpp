#include "CollisionInfo.h"
#include <iostream>

#include "dirent.h"

void CollisionInfo::Resolve()
{
	if (!bIsOverlapping) return;

	// If objA is a plane
	if (objA->GetMass() <= 0)
	{
		float totalInverseMass = objB->GetInverseMass();
		//Depen Step
		objB->GetPos() += collisionNormal * overlapAmount;

		//Apply Force
		float impulseMag = Dot(-2 * objB->GetVelocity(), collisionNormal) / totalInverseMass;
		Vec2 force = collisionNormal * impulseMag;

		objB->ApplyImpulse(force, contactPoint);
		return;
	}

	float totalInverseMass = objA->GetInverseMass() + objB->GetInverseMass();

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

void CollisionInfo::ResolveRotation()
{
	float totalInverseMass = objA->GetInverseMass() + objB->GetInverseMass();

	// Depen
	objA->GetPos() -= collisionNormal * overlapAmount * objA->GetInverseMass() / totalInverseMass;
	objB->GetPos() += collisionNormal * overlapAmount * objB->GetInverseMass() / totalInverseMass;

	// Find perpendicular angle to collision point
	Vec2 perp = Vec2(collisionNormal.y, -collisionNormal.x);

	// Level arm
	float ra = Dot(contactPoint - objA->GetPos(), -perp);
	float rb = Dot(contactPoint - objB->GetPos(), perp);

	// Relative velocity
	float va = Dot(objA->GetVelocity(), collisionNormal) - ra * objA->GetAngularVelocity();
	float vb = Dot(objB->GetVelocity(), collisionNormal) - rb * objB->GetAngularVelocity();

	//float vaCrossvb = PseudoCross(collisionNormal, objA->GetVelocity().GetNormalised());

	if (va > vb)
	{
		float massA = 1.0f / (objA->GetInverseMass() + (ra * ra) * objA->GetInverseInertia());
		float massB = 1.0f / (objB->GetInverseMass() + (rb * rb) * objB->GetInverseInertia());

		Vec2 force = 2 * massA * massB / (massA + massB) * (va - vb) * collisionNormal;
		objA->ApplyImpulse(-force, contactPoint);
		objB->ApplyImpulse(force, contactPoint);
	}
}
