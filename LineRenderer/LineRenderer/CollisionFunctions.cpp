#include "CollisionFunctions.h"
#include "Maths.h"

CollisionInfo CircleToCircleCollision(Circle* circleA, Circle* circleB)
{
	Vec2 displacementVec = circleB->GetPos() - circleA->GetPos();
	float distance = displacementVec.GetMagnitude();

	CollisionInfo info;

	info.overlapAmount = -(distance - circleB->GetRadius() - circleA->GetRadius());
	info.bIsOverlapping = info.overlapAmount > 0 ? true : false;
	// Normalise the difference in position to get the collision normal of two circles
	info.collisionNormal = displacementVec / distance;
	info.objA = circleA;
	info.objB = circleB;

	return info;
}

CollisionInfo PlaneToCircleCollision(Plane* planeA, Circle* circleB)
{
	float cDotN = Dot(circleB->GetPos(), planeA->GetNormal());
	float distance = cDotN - planeA->GetDistanceFromOrgin();
	float overlap = distance - circleB->GetRadius();

	float total = Dot(circleB->GetPos(), planeA->GetNormal()) - planeA->GetDistanceFromOrgin() - circleB->GetRadius();

	CollisionInfo info;

	info.objA = planeA;
	info.objB = circleB;
	info.overlapAmount = -overlap;
	info.bIsOverlapping = info.overlapAmount > 0 ? true : false;
	info.collisionNormal = planeA->GetNormal();

	return info;
}

CollisionInfo CircleToBoxCollision(Circle* circleA, Box* boxB)
{
	Vec2 circleCentre = circleA->GetPos();
	float circleRadius = circleA->GetRadius();
	float xMax = boxB->mXMax.x;
	float xMin = boxB->mXMin.x;
	float yMin = boxB->mYMin.y;
	float yMax = boxB->mYMax.y;

	Vec2 closestPoint = Vec2(Clamp(circleCentre.x, xMin, xMax), Clamp(circleCentre.y, yMin, yMax));
	float distance = (circleCentre - closestPoint).GetMagnitude();

	CollisionInfo info;

	info.objA = circleA;
	info.objB = boxB;
	info.overlapAmount = -(distance - circleRadius);
	info.bIsOverlapping = info.overlapAmount > 0 ? true : false;
	info.collisionNormal = -(circleCentre - closestPoint);
	info.closestPoint = closestPoint;

	return info;
}

CollisionInfo PlaneToBoxCollision(Plane* planeA, Box* boxB)
{
	float

	CollisionInfo info;

	info.objA = planeA;
	info.objB = boxB;
	info.collisionNormal = planeA->GetNormal();
	return info;
}

CollisionInfo BoxToBoxCollision(Box* boxA, Box* boxB)
{
	float overlapDepths[4];
	Vec2 overlapNormals[4];

	overlapDepths[0] = boxB->mXMax.x - boxA->mXMin.x;
	overlapNormals[0] = Vec2(-1, 0);

	overlapDepths[1] = boxA->mXMax.x - boxB->mXMin.x;
	overlapNormals[1] = Vec2(1, 0);

	overlapDepths[2] = boxB->mYMax.y - boxA->mYMin.y;
	overlapNormals[2] = Vec2(0, -1);

	overlapDepths[3] = boxA->mYMax.y - boxB->mYMin.y;
	overlapNormals[3] = Vec2(0, 1);

	int overlapIndex = 0;
	for (int i = 0; i < 4 ; i++)
	{
		if (overlapDepths[i] < overlapDepths[overlapIndex])
		{
			overlapIndex = i;
		}
	}

	CollisionInfo info;

	info.collisionNormal = overlapNormals[overlapIndex];
	info.overlapAmount = overlapDepths[overlapIndex];
	info.bIsOverlapping = info.overlapAmount > 0 ? true : false;
	info.objA = boxA;
	info.objB = boxB;

	return info;
}
