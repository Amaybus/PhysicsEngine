#include "CollisionFunctions.h"
#include "Maths.h"
#include "Box.h"
#include "Plane.h"
#include "Polygon.h"
#include "Circle.h"

#include <iostream>



void CollisionFuncInit()
{
	collisionFunctions[(int)ObjectType::Circle][(int)ObjectType::Circle] = CircleToCircleCollision;
	collisionFunctions[(int)ObjectType::Circle][(int)ObjectType::Plane] = CircleToPlaneCollision;
	collisionFunctions[(int)ObjectType::Circle][(int)ObjectType::Box] = CircleToBoxCollision;
	collisionFunctions[(int)ObjectType::Circle][(int)ObjectType::Polygon] = CircleToPolygonCollision;
	collisionFunctions[(int)ObjectType::Plane][(int)ObjectType::Circle] = PlaneToCircleCollision;
	collisionFunctions[(int)ObjectType::Plane][(int)ObjectType::Plane] = PlaneToPlaneCollision;
	collisionFunctions[(int)ObjectType::Plane][(int)ObjectType::Box] = PlaneToBoxCollision;
	collisionFunctions[(int)ObjectType::Plane][(int)ObjectType::Polygon] = PlaneToPolygonCollision;
	collisionFunctions[(int)ObjectType::Box][(int)ObjectType::Circle] = BoxToCircleCollision;
	collisionFunctions[(int)ObjectType::Box][(int)ObjectType::Plane] = BoxToPlaneCollision;
	collisionFunctions[(int)ObjectType::Box][(int)ObjectType::Box] = BoxToBoxCollision;
	collisionFunctions[(int)ObjectType::Box][(int)ObjectType::Polygon] = BoxToPolygonCollision;
	collisionFunctions[(int)ObjectType::Polygon][(int)ObjectType::Circle] = PolygonToCircleCollision;
	collisionFunctions[(int)ObjectType::Polygon][(int)ObjectType::Plane] = PolygonToPlaneCollision;
	collisionFunctions[(int)ObjectType::Polygon][(int)ObjectType::Box] = PolygonToBoxCollision;
	collisionFunctions[(int)ObjectType::Polygon][(int)ObjectType::Polygon] = PolygonToPolygonCollision;
}

CollisionInfo CheckCollision(PhysicsObject* objA, PhysicsObject* objB)
{
	int objTypeA = (int)objA->GetType();
	int objTypeB = (int)objB->GetType();

	return collisionFunctions[objTypeA][objTypeB](objA, objB);
}


// ~~~~~~~ CIRCLE COLLISION  ~~~~~~~ //
CollisionInfo CircleToCircleCollision(PhysicsObject* circA, PhysicsObject* circB)
{
	Circle* circleA = (Circle*)circA;
	Circle* circleB = (Circle*)circB;

	Vec2 displacementVec = circleB->GetPos() - circleA->GetPos();
	float distance = displacementVec.GetMagnitude();

	CollisionInfo info;

	info.overlapAmount = -(distance - circleB->GetRadius() - circleA->GetRadius());
	info.bIsOverlapping = info.overlapAmount > 0 ? true : false;
	// Normalise the difference in position to get the collision normal of two circles
	info.collisionNormal = displacementVec / distance;
	info.objA = circleA;
	info.objB = circleB;
	info.contactPoint = (circleB->GetPos() + circleA->GetPos()) / 2;

	return info;
}
CollisionInfo CircleToPlaneCollision(PhysicsObject* circleA, PhysicsObject* planeB)
{
	return PlaneToCircleCollision(planeB, circleA);
}
CollisionInfo CircleToBoxCollision(PhysicsObject* circA, PhysicsObject* bB)
{
	Circle* circleA = (Circle*)circA;
	Box* boxB = (Box*)bB;

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
	info.collisionNormal = -(circleCentre - closestPoint).Normalise();
	info.contactPoint = closestPoint;

	return info;
}
CollisionInfo CircleToPolygonCollision(PhysicsObject* circleA, PhysicsObject* polygonB)
{
	return CollisionInfo();
} // TODO


// ~~~~~~~ PLANE COLLISION  ~~~~~~~ //
CollisionInfo PlaneToCircleCollision(PhysicsObject* plA, PhysicsObject* circB)
{
	Plane* planeA = (Plane*)plA;
	Circle* circleB = (Circle*)circB;

	float distance = Dot(circleB->GetPos(), planeA->GetNormal()) - planeA->GetDistanceFromOrgin();
	float overlap = distance - circleB->GetRadius();

	CollisionInfo info;

	info.objA = planeA;
	info.objB = circleB;
	info.overlapAmount = -overlap;
	info.bIsOverlapping = info.overlapAmount > 0;
	info.collisionNormal = planeA->GetNormal();
	info.contactPoint = circB->GetPos() + -planeA->GetNormal() * circleB->GetRadius();

	return info;
}
CollisionInfo PlaneToPlaneCollision(PhysicsObject* planeA, PhysicsObject* planeB)
{
	CollisionInfo info;
	info.bIsOverlapping = false;
	info.objA = planeA;
	info.objB = planeB;
	return info;
}
CollisionInfo PlaneToBoxCollision(PhysicsObject* plA, PhysicsObject* bB)
{
	Plane* planeA = (Plane*)plA;
	Box* boxB = (Box*)bB;

	float pointDistance[4];
	pointDistance[0] = Dot(boxB->mXMax, planeA->GetNormal()) - planeA->GetDistanceFromOrgin();
	pointDistance[1] = Dot(boxB->mXMin, planeA->GetNormal()) - planeA->GetDistanceFromOrgin();
	pointDistance[2] = Dot(boxB->mYMin, planeA->GetNormal()) - planeA->GetDistanceFromOrgin();
	pointDistance[3] = Dot(boxB->mYMax, planeA->GetNormal()) - planeA->GetDistanceFromOrgin();

	int distanceIndex = 0;
	for (int i = 0; i < 4; i++)
	{
		if (pointDistance[i] < pointDistance[distanceIndex])
		{
			distanceIndex = i;
		}
	}

	Vec2 closestPoint;
	if (distanceIndex == 0) { closestPoint = boxB->mXMax; }
	if (distanceIndex == 1) { closestPoint = boxB->mXMin; }
	if (distanceIndex == 2) { closestPoint = boxB->mYMin; }
	if (distanceIndex == 3) { closestPoint = boxB->mYMax; }

	float distance = Dot(closestPoint, planeA->GetNormal()) - planeA->GetDistanceFromOrgin();

	CollisionInfo info;
	info.objA = planeA;
	info.objB = boxB;
	info.overlapAmount = -distance;
	info.bIsOverlapping = info.overlapAmount >= 0;
	info.collisionNormal = planeA->GetNormal();

	// #### Might need more work, check back when rotation is added ####
	info.contactPoint = closestPoint;
	return info;
}
CollisionInfo PlaneToPolygonCollision(PhysicsObject* plA, PhysicsObject* polyB)
{
	Plane* planeA = (Plane*)plA;
	Polygon* polygonB = (Polygon*)polyB;

	return CollisionInfo();
}


// ~~~~~~~ BOX COLLISION  ~~~~~~~ //
CollisionInfo BoxToCircleCollision(PhysicsObject* boxA, PhysicsObject* circB)
{
	return CircleToBoxCollision(circB, boxA);
}
CollisionInfo BoxToPlaneCollision(PhysicsObject* boxA, PhysicsObject* planeB)
{
	return PlaneToBoxCollision(planeB, boxA);
}
CollisionInfo BoxToBoxCollision(PhysicsObject* bA, PhysicsObject* bB)
{
	Box* boxA = (Box*)bA;
	Box* boxB = (Box*)bB;

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
	for (int i = 0; i < 4; i++)
	{
		if (overlapDepths[i] < overlapDepths[overlapIndex])
		{
			overlapIndex = i;
		}
	}

	CollisionInfo info;

	info.collisionNormal = overlapNormals[overlapIndex];
	info.overlapAmount = overlapDepths[overlapIndex];
	info.bIsOverlapping = info.overlapAmount > 0;
	info.objA = boxA;
	info.objB = boxB;

	return info;
}
CollisionInfo BoxToPolygonCollision(PhysicsObject* bA, PhysicsObject* polyB)
{
	Box* boxA = (Box*)bA;
	Polygon* polygonB = (Polygon*)polyB;

	return CollisionInfo();
}


// ~~~~~~~ POLYGON COLLISION  ~~~~~~~ //
CollisionInfo PolygonToCircleCollision(PhysicsObject* polygonA, PhysicsObject* CircleB)
{
	return CircleToPolygonCollision(CircleB, polygonA);
}
CollisionInfo PolygonToPlaneCollision(PhysicsObject* polygonA, PhysicsObject* planeB)
{
	return PlaneToPolygonCollision(planeB, polygonA);
}
CollisionInfo PolygonToBoxCollision(PhysicsObject* polygonA, PhysicsObject* boxB)
{
	return BoxToPolygonCollision(boxB, polygonA);
}
CollisionInfo PolygonToPolygonCollision(PhysicsObject* polyA, PhysicsObject* polyB)
{
	Polygon* polygonA = (Polygon*)polyA;
	Polygon* polygonB = (Polygon*)polyB;

	CollisionInfo info;
	info.objA = polyA;
	info.objB = polyB;
	info.overlapAmount = 0;
	info.contactPoint = Vec2();


	std::vector<Vec2> collectiveNormals;
	for (Vec2& nA : polygonA->mNormals)
	{
		collectiveNormals.push_back(nA);
	}

	for (Vec2& nB : polygonB->mNormals)
	{
		collectiveNormals.push_back(nB);
	}

	std::vector<float> distanceResults;
	std::vector < float> polyAResults;
	std::vector < float> polyBResults;
	Vec2 aMax = Vec2(), aMin = Vec2(), bMax = Vec2(), bMin = Vec2();
	float smallest = FLT_MAX, largest = 0;

	float overlaps[4];
	int smallestOverlapIndex = 0;

	for (Vec2& cN : collectiveNormals)
	{
		for (int i = 0; i < polygonA->mVertices.size(); i++)
		{
			float result = Dot(polygonA->mVertices[i], cN);
			polyAResults.push_back(result);
			if (result < smallest)
			{
				smallest = result;
				aMin = polygonA->mVertices[i];
			}
			if (result > largest)
			{
				largest = result;
				aMax = polygonA->mVertices[i];
			}
		}
		smallest = FLT_MAX;
		largest = 0;

		for (int i = 0; i < polygonB->mVertices.size(); i++)
		{
			float result = Dot(polygonB->mVertices[i], cN);
			polyAResults.push_back(result);
			if (result < smallest)
			{
				smallest = result;
				bMin = polygonB->mVertices[i];
			}
			if (result > largest)
			{
				largest = result;
				bMax = polygonB->mVertices[i];
			}
		}

		overlaps[0] = aMax.x - bMin.x;
		overlaps[1] = bMax.x - aMin.x;
		overlaps[2] = aMax.y - bMin.y;
		overlaps[3] = bMax.y - aMin.y;

		for (int i = 0; i < 4; i++)
		{
			if (overlaps[i] < overlaps[smallestOverlapIndex])
			{
				smallestOverlapIndex = i;
			}
		}

		if (overlaps[smallestOverlapIndex] < info.overlapAmount)
		{
			info.overlapAmount = overlaps[smallestOverlapIndex];
			if (smallestOverlapIndex == 0) { info.contactPoint = bMin; }
			if (smallestOverlapIndex == 1) { info.contactPoint = bMax; }
			if (smallestOverlapIndex == 2) { info.contactPoint = bMin; }
			if (smallestOverlapIndex == 3) { info.contactPoint = bMax; }
		}
	}

	info.bIsOverlapping = info.overlapAmount < 0 ? true : false;
	return info;
}


