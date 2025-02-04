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

		// This shit is fakes rn
		objB->SetPos(Vec2(objA->GetPos().x, objA->GetPos().y+2));
		return;
	}

	float totalInverseMass = objA->GetInverseMass() + objB->GetInverseMass();

	objA->GetPos() -= collisionNormal * overlapAmount * objA->GetInverseMass() / totalInverseMass;
	objB->GetPos() += collisionNormal * overlapAmount * objB->GetInverseMass() / totalInverseMass;
}
