#include "CollisionFunctions.h"
#include "Maths.h"
#include "Box.h"
#include "Plane.h"
#include "Polygon.h"
#include "Circle.h"

#include <functional>



CollisionInfo CheckCollision(PhysicsObject* objA, PhysicsObject* objB)
{
	std::function<CollisionInfo(PhysicsObject*, PhysicsObject*)> collisionFunctions[(int)ObjectType::Count][(int)ObjectType::Count];
	collisionFunctions[(int)ObjectType::Circle][(int)ObjectType::Circle] = CircleToCircleCollision;
	collisionFunctions[(int)ObjectType::Circle][(int)ObjectType::Plane] = CircleToPlaneCollision;
	collisionFunctions[(int)ObjectType::Circle][(int)ObjectType::Box] = CircleToBoxCollision;
	collisionFunctions[(int)ObjectType::Circle][(int)ObjectType::Polygon] = CircleToPolygonCollision;
	collisionFunctions[(int)ObjectType::Plane][(int)ObjectType::Circle] = PlaneToCircleCollision;
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
	info.closestPoint = closestPoint;

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
	info.bIsOverlapping = info.overlapAmount > 0 ? true : false;
	info.collisionNormal = planeA->GetNormal();
	info.closestPoint = Vec2(circleB->GetPos().x + circleB->GetRadius(), circleB->GetPos().y);

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

	float distance = Dot(planeA->GetNormal(), closestPoint) - planeA->GetDistanceFromOrgin();
	//float distanceToCentre = Dot(boxB->GetPos(), planeA->GetNormal()) - planeA->GetDistanceFromOrgin();
	//float distance = distanceToCentre - pointDistance[distanceIndex];

	CollisionInfo info;
	info.objA = planeA;
	info.objB = boxB;
	info.overlapAmount = -distance;
	info.bIsOverlapping = info.overlapAmount > 0 ? true : false;
	info.collisionNormal = planeA->GetNormal();
	info.closestPoint = closestPoint;
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
	info.bIsOverlapping = info.overlapAmount > 0 ? true : false;
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
	return CircleToPolygonCollision(CircleB,polygonA);
}
CollisionInfo PolygonToPlaneCollision(PhysicsObject* polygonA, PhysicsObject* planeB)
{
	return PlaneToPolygonCollision(planeB,polygonA);
}
CollisionInfo PolygonToBoxCollision(PhysicsObject* polygonA, PhysicsObject* boxB)
{
	return BoxToPolygonCollision(boxB,polygonA);
}
CollisionInfo PolygonToPolygonCollision(PhysicsObject* polyA, PhysicsObject* polyB)
{
	Polygon* polygonA = (Polygon*)polyA;
	Polygon* polygonB = (Polygon*)polyB;

	return CollisionInfo();
}
