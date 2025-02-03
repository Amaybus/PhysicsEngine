#include "CollisionFunctions.h"

#include <stdexcept>

CollisionInfo CircleToCircleCollision(Circle* a, Circle* b)
{
	CollisionInfo info;
	Vec2 distanceVector = b->GetPos() - a->GetPos();
	float distance = distanceVector.GetMagnitude();

	info.overlapAmount = -(distance - b->GetRadius() - a->GetRadius());
	info.bIsOverlapping = info.overlapAmount > 0 ? true : false;

	// Normalise the difference in position to get the collision normal of two circles
	info.collisionNormal = distanceVector / distance;
	info.objA = a;
	info.objB = b;

	return info;
}
