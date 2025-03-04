#include "CollisionFunctions.h"
#include "Maths.h"
#include "Box.h"
#include "Plane.h"
#include "Polygon.h"
#include "Circle.h"


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
	info.bIsOverlapping = info.overlapAmount > 0;
	// Normalise the difference in position to get the collision normal of two circles
	info.collisionNormal = (circleB->GetPos() - circleA->GetPos()).Normalise();
	info.objA = circleA;
	info.objB = circleB;
	info.aContactPoints.push_back((circleB->GetPos() + circleA->GetPos()) / 2);
	info.bContactPoints.push_back((circleB->GetPos() + circleA->GetPos()) / 2);

	return info;
}
CollisionInfo CircleToPlaneCollision(PhysicsObject* circleA, PhysicsObject* planeB)
{
	return PlaneToCircleCollision(planeB, circleA);
}
CollisionInfo CircleToBoxCollision(PhysicsObject* circA, PhysicsObject* bB)
{
	Circle* circleA = (Circle*)circA;
	Box* box = (Box*)bB;

	Vec2 pos = box->GetPos();
	Vec2 localY = box->GetLocalY();
	Vec2 localX = box->GetLocalX();
	float width = box->GetWidth();
	float height = box->GetHeight();

	// Verts are converted to world space in circle to poly func
	box->mVertices[0] = (pos + localX * (0.5f * width) + localY * (0.5f * height) - pos);
	box->mVertices[2] = (pos - localX * (0.5f * width) - localY * (0.5f * height) - pos);
	box->mVertices[1] = (pos - localX * (0.5f * width) + localY * (0.5f * height) - pos);
	box->mVertices[3] = (pos + localX * (0.5f * width) - localY * (0.5f * height) - pos);

	Polygon* boxB = (Polygon*)bB;
	return CircleToPolygonCollision(circA, boxB);
}
CollisionInfo CircleToPolygonCollision(PhysicsObject* circA, PhysicsObject* polyB)
{
	CollisionInfo info;
	info.overlapAmount = FLT_MAX;
	Circle* circleA = (Circle*)circA;
	Polygon* polygonB = (Polygon*)polyB;

	Vec2 circleCentre = circleA->GetPos();
	float circleRadius = circleA->GetRadius();

	std::vector<Vec2> collectiveNormals;
	std::vector<std::vector<float>> collectiveBProjs;
	std::vector<std::vector<float>> collectiveAProjs;

	for (Vec2& n : polygonB->GetNormals())
	{
		collectiveNormals.push_back(n);
	}

	for (int i = 0; i < polygonB->mVertices.size(); i++)
	{
		collectiveNormals.push_back((circleCentre - (polygonB->mVertices[i] + polygonB->GetPos())).Normalise());
	}

	for (int i = 0; i < collectiveNormals.size(); i++)
	{
		// Find min for circle
		float aMin = Dot(circleCentre - (circleRadius * collectiveNormals[i]), collectiveNormals[i]);;
		float bMax = -FLT_MAX;

		// Find max for the polygon
		std::vector<float> polyBResults;
		for (Vec2& v : polygonB->mVertices)
		{
			polyBResults.push_back(Dot(v + polygonB->GetPos(), collectiveNormals[i]));
		}

		for (float f : polyBResults)
		{
			if (f > bMax) { bMax = f; }
		}

		float overlap = bMax - aMin;
		if (overlap < info.overlapAmount)
		{
			info.overlapAmount = overlap;
			info.collisionNormal = -collectiveNormals[i];
		}
	}

	info.objA = circA;
	info.objB = polyB;
	info.bIsOverlapping = info.overlapAmount > 0;
	info.aContactPoints.push_back(circleA->GetPos() + info.collisionNormal * (circleA->GetRadius() - info.overlapAmount));
	info.bContactPoints.push_back(circleA->GetPos() + info.collisionNormal * (circleA->GetRadius() - info.overlapAmount));

	return info;
}


// ~~~~~~~ PLANE COLLISION  ~~~~~~~ //
CollisionInfo PlaneToCircleCollision(PhysicsObject* plA, PhysicsObject* circB)
{
	Plane* planeA = (Plane*)plA;
	Circle* circleB = (Circle*)circB;

	float distance = Dot(circleB->GetPos(), planeA->GetNormal()) - planeA->GetDistanceFromOrigin();
	float overlap = distance - circleB->GetRadius();

	CollisionInfo info;

	info.objA = planeA;
	info.objB = circleB;
	info.overlapAmount = -overlap;
	info.bIsOverlapping = info.overlapAmount > 0;
	info.collisionNormal = planeA->GetNormal();
	info.bContactPoints.push_back(circB->GetPos() + -planeA->GetNormal() * circleB->GetRadius());
	info.aContactPoints.push_back(circB->GetPos() + -planeA->GetNormal() * circleB->GetRadius());

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

	Vec2 pos = boxB->GetPos();
	Vec2 localY = boxB->GetLocalY();
	Vec2 localX = boxB->GetLocalX();
	float width = boxB->GetWidth();
	float height = boxB->GetHeight();

	// Verts are converted to world space in circle to poly func
	boxB->mVertices[0] = (pos + localX * (0.5f * width) + localY * (0.5f * height) - pos);
	boxB->mVertices[2] = (pos - localX * (0.5f * width) - localY * (0.5f * height) - pos);
	boxB->mVertices[1] = (pos - localX * (0.5f * width) + localY * (0.5f * height) - pos);
	boxB->mVertices[3] = (pos + localX * (0.5f * width) - localY * (0.5f * height) - pos);

	Polygon* boxA = (Polygon*)bB;
	return PlaneToPolygonCollision(planeA, boxB);
}
CollisionInfo PlaneToPolygonCollision(PhysicsObject* plA, PhysicsObject* polyB)
{
	CollisionInfo info;
	Plane* planeA = (Plane*)plA;
	Polygon* polygonB = (Polygon*)polyB;
	std::vector<float> vertDistances;

	for (Vec2 vert : polygonB->mVertices)
	{
		vertDistances.push_back(Dot(vert + polygonB->GetPos(), planeA->GetNormal()) - planeA->GetDistanceFromOrigin());
	}

	int distanceIndex = 0;
	for (int i = 0; i < vertDistances.size(); i++)
	{
		if (vertDistances[i] < vertDistances[distanceIndex])
		{
			distanceIndex = i;
		}
		float threshold = 0.01f;
		if (vertDistances[i] < 0 && vertDistances[i] - vertDistances[distanceIndex] <= threshold && i != distanceIndex)
		{
			info.bContactPoints.push_back(polygonB->GetPos() + polygonB->mVertices[i]);
		}
	}

	info.overlapAmount = -vertDistances[distanceIndex];
	info.bIsOverlapping = info.overlapAmount >= 0;

	info.objA = planeA;
	info.objB = polygonB;
	info.collisionNormal = planeA->GetNormal();
	info.bContactPoints.push_back(polygonB->GetPos() + polygonB->mVertices[distanceIndex]);
	info.aContactPoints.push_back(Vec2());

	return info;
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

	// Set box A's vertices
	Vec2 pos = boxA->GetPos();
	Vec2 localY = boxA->GetLocalY();
	Vec2 localX = boxA->GetLocalX();
	float width = boxA->GetWidth();
	float height = boxA->GetHeight();

	// Verts are converted to world space in circle to poly func
	boxA->mVertices[0] = (pos + localX * (0.5f * width) + localY * (0.5f * height) - pos);
	boxA->mVertices[2] = (pos - localX * (0.5f * width) - localY * (0.5f * height) - pos);
	boxA->mVertices[1] = (pos - localX * (0.5f * width) + localY * (0.5f * height) - pos);
	boxA->mVertices[3] = (pos + localX * (0.5f * width) - localY * (0.5f * height) - pos);


	// Set box B's vertices
	pos = boxB->GetPos();
	localY = boxB->GetLocalY();
	localX = boxB->GetLocalX();
	width = boxB->GetWidth();
	height = boxB->GetHeight();

	// Verts are converted to world space in circle to poly func
	boxB->mVertices[0] = (pos + localX * (0.5f * width) + localY * (0.5f * height) - pos);
	boxB->mVertices[2] = (pos - localX * (0.5f * width) - localY * (0.5f * height) - pos);
	boxB->mVertices[1] = (pos - localX * (0.5f * width) + localY * (0.5f * height) - pos);
	boxB->mVertices[3] = (pos + localX * (0.5f * width) - localY * (0.5f * height) - pos);

	Polygon* polyA = (Polygon*)bA;
	Polygon* polyB = (Polygon*)bB;

	CollisionInfo info = PolygonToPolygonCollision(polyA, polyB);
	Vec2 displacement = info.objA->GetPos()- info.objB->GetPos();
	info.aContactPoints.push_back(info.objA->GetPos() - displacement);
	info.bContactPoints.push_back(info.objB->GetPos() + displacement);
	return info;
}
CollisionInfo BoxToPolygonCollision(PhysicsObject* bA, PhysicsObject* polyB)
{
	Box* box = (Box*)bA;

	Vec2 pos = box->GetPos();
	Vec2 localY = box->GetLocalY();
	Vec2 localX = box->GetLocalX();
	float width = box->GetWidth();
	float height = box->GetHeight();

	// Verts are converted to world space in circle to poly func
	box->mVertices[0] = (pos + localX * (0.5f * width) + localY * (0.5f * height) - pos);
	box->mVertices[2] = (pos - localX * (0.5f * width) - localY * (0.5f * height) - pos);
	box->mVertices[1] = (pos - localX * (0.5f * width) + localY * (0.5f * height) - pos);
	box->mVertices[3] = (pos + localX * (0.5f * width) - localY * (0.5f * height) - pos);

	Polygon* boxA = (Polygon*)bA;

	CollisionInfo info = PolygonToPolygonCollision(boxA, polyB);

	Polygon* poly = (Polygon*)polyB;
	float smallest = FLT_MAX;
	int index = 0;
	for(int i = 0; i < poly->mVertices.size(); i++)
	{
		float result = Dot(poly->mVertices[i], info.collisionNormal);
		if(result < smallest)
		{
			smallest = result;
			index = i;
		}
	}
	info.aContactPoints[0] = poly->GetPos() + poly->mVertices[index];
	info.bContactPoints[0] = poly->GetPos() + poly->mVertices[index];

	return info;
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
CollisionInfo PolygonToBoxCollision(PhysicsObject* polyA, PhysicsObject* bB) 
{
	return BoxToPolygonCollision(bB, polyA);
}
CollisionInfo PolygonToPolygonCollision(PhysicsObject* polyA, PhysicsObject* polyB)
{
	Polygon* polygonA = (Polygon*)polyA;
	Polygon* polygonB = (Polygon*)polyB;

	CollisionInfo info;
	info.objA = polyA;
	info.objB = polyB;
	info.overlapAmount = FLT_MAX;

	// Add all normals of each polygon to a list
	std::vector<Vec2> collectiveNormals;
	for (Vec2& nA : polygonA->GetNormals())
	{
		collectiveNormals.push_back(nA);
	}

	for (Vec2& nB : polygonB->GetNormals())
	{
		collectiveNormals.push_back(nB);
	}

	std::vector<float> polyAProjections;
	std::vector<float> polyBProjections;

	Vec2 displacement = polyA->GetPos() - polyB->GetPos();

	// Dot each vert in each shape against the collective normals
	for (int i = 0; i < collectiveNormals.size(); i++)
	{
		if (Dot(displacement.Normalise(), collectiveNormals[i]) > 0) continue;

		for (Vec2 v : polygonA->mVertices)
		{
			polyAProjections.push_back(Dot(v + polygonA->GetPos(), collectiveNormals[i]));
		}

		for (Vec2 v : polygonB->mVertices)
		{
			polyBProjections.push_back(Dot(v + polygonB->GetPos(), collectiveNormals[i]));
		}

		float aMax = -FLT_MAX;
		float aMin = FLT_MAX;
		float bMax = -FLT_MAX;
		float bMin = FLT_MAX;

		// Find the min and max points of each polygon
		for (float f : polyAProjections)
		{
			if (f < aMin) { aMin = f; }
			if (f > aMax) { aMax = f; }
		}
		polyAProjections.clear();


		for (float f : polyBProjections)
		{
			if (f < bMin) { bMin = f; }
			if (f > bMax) { bMax = f; }
		}
		polyBProjections.clear();

		// Check for overlaps 
		int smallestOverlapIndex = 0;
		float overlaps[2];
		overlaps[0] = aMax - bMin;
		overlaps[1] = bMax - aMin;

		for (int i = 0; i < 2; i++)
		{
			if (overlaps[i] < overlaps[smallestOverlapIndex])
			{
				smallestOverlapIndex = i;
			}
		}

		if (overlaps[smallestOverlapIndex] < info.overlapAmount)
		{
			info.overlapAmount = overlaps[smallestOverlapIndex];
			info.collisionNormal = collectiveNormals[i];
		}
	}

	info.bIsOverlapping = info.overlapAmount > 0;

	info.aContactPoints.push_back(info.objA->GetPos() - displacement);
	info.bContactPoints.push_back(info.objB->GetPos() + displacement);

	return info;
}



