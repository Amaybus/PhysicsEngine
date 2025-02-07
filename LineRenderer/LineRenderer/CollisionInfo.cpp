#include "CollisionInfo.h"
#include <iostream>

void CollisionInfo::Resolve()
{
	if (!bIsOverlapping) return;

	// If objA is a plane
	if (objA->GetMass() <= 0)
	{
		float totalInverseMass = objB->GetInverseMass();
		std::cout << closestPoint.x << std::endl;
		objB->GetPos() += collisionNormal * overlapAmount; 

		float impulseMag = Dot(-2 * objB->GetVelocity(), collisionNormal) / totalInverseMass;
		Vec2 force = collisionNormal * impulseMag;

		objB->ApplyImpulse(force);
		return;
	}

	float totalInverseMass = objA->GetInverseMass() + objB->GetInverseMass();

	// Depen step
	objA->GetPos() -= collisionNormal * overlapAmount * objA->GetInverseMass() / totalInverseMass;
	objB->GetPos() += collisionNormal * overlapAmount * objB->GetInverseMass() / totalInverseMass;

	// Apply Force
	Vec2 relVe = objB->GetVelocity() - objA->GetVelocity();
	float impulseMag = Dot(-2 * relVe, collisionNormal) / totalInverseMass;
	Vec2 force = collisionNormal * impulseMag;

	objA->ApplyImpulse(-force);
	objB->ApplyImpulse(force);
}
