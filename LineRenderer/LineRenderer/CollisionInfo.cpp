#include "CollisionInfo.h"
#include <iostream>

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
		//objB->SetVelocity(Vec2());
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

	Vec2 perp = Vec2(collisionNormal.y, -collisionNormal.x);
	float r1 = Dot(contactPoint - objA->GetPos(), -perp);

	std::cout << r1 << '\n';
	
	objA->ApplyImpulse(-force,contactPoint);
	objB->ApplyImpulse(force, contactPoint);
}
