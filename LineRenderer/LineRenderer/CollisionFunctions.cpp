#include "CollisionFunctions.h"

#include <stdexcept>

CollisionInfo CircleToCircleCollision(Circle* circleA, Circle* circleB)
{
	CollisionInfo info;
	Vec2 distanceVector = circleB->GetPos() - circleA->GetPos();
	float distance = distanceVector.GetMagnitude();

	info.overlapAmount = -(distance - circleB->GetRadius() - circleA->GetRadius());
	info.bIsOverlapping = info.overlapAmount > 0 ? true : false;

	// Normalise the difference in position to get the collision normal of two circles
	info.collisionNormal = distanceVector / distance;
	info.objA = circleA;
	info.objB = circleB;

	return info;
}

CollisionInfo PlaneToCircleCollision(Plane* planeA, Circle* circleB)
{
	CollisionInfo info;

	float cDotN = Dot(circleB->GetPos(), planeA->GetNormal());
	float distance = cDotN - planeA->GetDistanceFromOrgin();
	float overlap = distance - circleB->GetRadius();

	float total = Dot(circleB->GetPos(), planeA->GetNormal()) - planeA->GetDistanceFromOrgin() - circleB->GetRadius();

	info.objA = planeA;
	info.objB = circleB;
	info.overlapAmount = -overlap;
	info.bIsOverlapping = info.overlapAmount > 0 ? true : false;
	info.collisionNormal = planeA->GetNormal();

	return info;
}

CollisionInfo CircleToBox(Circle* circleA, Box* boxA)
{
	return CollisionInfo();
}

CollisionInfo PlaneToBox(Plane* planeA, Box* boxB)
{
	return CollisionInfo();
}

CollisionInfo BoxToBox(Box* boxA, Box* boxB)
{
	return CollisionInfo();
}
