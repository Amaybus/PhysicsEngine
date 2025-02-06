#include "CollisionInfo.h"

void CollisionInfo::Resolve()
{
	if (!bIsOverlapping) return;
	
	// If objA is a plane
	if (objA->GetMass() <= 0)
	{
		// Only apply impulse to the circle
		// the reletive vel is the circles vel
		// collision normal is the planes normal
		float totalInverseMass = objB->GetInverseMass();
		objB->GetPos() += collisionNormal * overlapAmount * objB->GetInverseMass() / totalInverseMass;
		
		return;
	}

	float totalInverseMass = objA->GetInverseMass() + objB->GetInverseMass();

	objA->GetPos() -= collisionNormal * overlapAmount * objA->GetInverseMass() / totalInverseMass;
	objB->GetPos() += collisionNormal * overlapAmount * objB->GetInverseMass() / totalInverseMass;
}
