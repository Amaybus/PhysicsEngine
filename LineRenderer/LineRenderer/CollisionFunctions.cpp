#include "CollisionFunctions.h"
#include "Maths.h"
#include "Box.h"
#include "Plane.h"
#include "Polygon.h"
#include "Circle.h"
#include "imgui.h"

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
	info.collisionNormal = (circleB->GetPos() - circleA->GetPos()).Normalise();
	info.objA = circleA;
	info.objB = circleB;
	info.contactPoint = (circleB->GetPos() + circleA->GetPos()) / 2;
	info.contactPoint2 = (circleB->GetPos() + circleA->GetPos()) / 2;

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

	Vec2 next;
	for (int i = 0; i < box->mVertices.size(); i++)
	{
		if (i == box->mVertices.size() - 1) { next = box->mVertices[0]; }
		else { next = box->mVertices[i + 1]; }

		box->mNormals[i] = ((next - box->mVertices[i]).GetRotatedBy270().GetNormalised());
	}


	Polygon* boxB = (Polygon*)bB;
	return CircleToPolygonCollision(circA, boxB);

	// Collision detection for non-rotating boxes
	//Circle* circleA = (Circle*)circA;
	//Box* boxB = (Box*)bB;

	//Vec2 circleCentre = circleA->GetPos();
	//float circleRadius = circleA->GetRadius();


	//Vec2 closestPoint = Vec2(Clamp(circleCentre.x, boxB->GetXMin(), boxB->GetXMax()), Clamp(circleCentre.y, boxB->GetYMin(), boxB->GetYMax()));
	//float distance = (circleCentre - closestPoint).GetMagnitude();

	//CollisionInfo info;

	//info.objA = circleA;
	//info.objB = boxB;
	//info.overlapAmount = -(distance - circleRadius);
	//info.bIsOverlapping = info.overlapAmount > 0 ? true : false;
	//info.collisionNormal = -(circleCentre - closestPoint).Normalise();
	//info.contactPoint = closestPoint;

	//return info;
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


	std::vector<Vec2> normals;
	for (Vec2& n : polygonB->mNormals)
	{
		collectiveNormals.push_back(n);
	}

	for (int i = 0; i < polygonB->mVertices.size(); i++)
	{
		collectiveNormals.push_back((circleCentre - (polygonB->mVertices[i] + polygonB->GetPos())).Normalise());
	}

	for (int i = 0; i < collectiveNormals.size(); i++)
	{
		float aMin = FLT_MAX;
		float bMax = -FLT_MAX;

		// Find min and max for cirlce
		aMin = Dot(circleCentre - (circleRadius * collectiveNormals[i]), collectiveNormals[i]);

		// Find min and max for the polygon
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
	info.contactPoint = circleA->GetPos() + info.collisionNormal * (circleA->GetRadius() - info.overlapAmount);
	info.contactPoint2 = circleA->GetPos() + info.collisionNormal * (circleA->GetRadius() - info.overlapAmount);

	return info;
}


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
	info.contactPoint2 = circB->GetPos() + -planeA->GetNormal() * circleB->GetRadius();

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

	Vec2 next;
	for (int i = 0; i < boxB->mVertices.size(); i++)
	{
		if (i == boxB->mVertices.size() - 1) { next = boxB->mVertices[0]; }
		else { next = boxB->mVertices[i + 1]; }

		boxB->mNormals[i] = ((next - boxB->mVertices[i]).GetRotatedBy270().GetNormalised());
	}


	Polygon* boxA = (Polygon*)bB;
	return PlaneToPolygonCollision(planeA, boxB);


	// NON ROTATION CALC
	//float pointDistance[4];

	//Vec2 topLeft = Vec2(boxB->GetXMax(), boxB->GetYMax());
	//Vec2 topRight = Vec2(boxB->GetXMin(), boxB->GetYMax());
	//Vec2 bottomLeft = Vec2(boxB->GetXMin(), boxB->GetYMin());
	//Vec2 bottonRight = Vec2(boxB->GetXMin(), boxB->GetYMax());

	//pointDistance[0] = Dot(topLeft, planeA->GetNormal()) - planeA->GetDistanceFromOrgin();
	//pointDistance[1] = Dot(topRight, planeA->GetNormal()) - planeA->GetDistanceFromOrgin();
	//pointDistance[2] = Dot(bottomLeft, planeA->GetNormal()) - planeA->GetDistanceFromOrgin();
	//pointDistance[3] = Dot(bottonRight, planeA->GetNormal()) - planeA->GetDistanceFromOrgin();

	//int distanceIndex = 0;
	//for (int i = 0; i < 4; i++)
	//{
	//	if (pointDistance[i] < pointDistance[distanceIndex])
	//	{
	//		distanceIndex = i;
	//	}
	//}

	//Vec2 closestPoint;
	//if (distanceIndex == 0) { closestPoint = topLeft; }
	//if (distanceIndex == 1) { closestPoint = topRight; }
	//if (distanceIndex == 2) { closestPoint = bottomLeft; }
	//if (distanceIndex == 3) { closestPoint = bottonRight; }

	//// Fix this, doing calc twice ??why??
	//float distance = Dot(closestPoint, planeA->GetNormal()) - planeA->GetDistanceFromOrgin();

	//CollisionInfo info;
	//info.objA = planeA;
	//info.objB = boxB;
	//info.overlapAmount = -distance;
	//info.bIsOverlapping = info.overlapAmount >= 0;
	//info.collisionNormal = planeA->GetNormal();

	//// #### Might need more work, check back when rotation is added ####
	//info.contactPoint = closestPoint;
	//return info;
}
CollisionInfo PlaneToPolygonCollision(PhysicsObject* plA, PhysicsObject* polyB)
{
	Plane* planeA = (Plane*)plA;
	Polygon* polygonB = (Polygon*)polyB;
	std::vector<float> vertDistances;

	for (Vec2 vert : polygonB->mVertices)
	{
		vertDistances.push_back(Dot(vert + polygonB->GetPos(), planeA->GetNormal()) - planeA->GetDistanceFromOrgin());
	}

	int distanceIndex = 0;
	for (int i = 0; i < vertDistances.size(); i++)
	{
		if (vertDistances[i] < vertDistances[distanceIndex])
		{
			distanceIndex = i;
		}
	}

	CollisionInfo info;
	info.overlapAmount = -vertDistances[distanceIndex];
	info.bIsOverlapping = info.overlapAmount >= 0;
	info.objA = planeA;
	info.objB = polygonB;
	info.collisionNormal = planeA->GetNormal();
	info.contactPoint = polygonB->GetPos() + polygonB->mVertices[distanceIndex];
	info.contactPoint2 = polygonB->GetPos() + polygonB->mVertices[distanceIndex];

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

	Vec2 next;
	for (int i = 0; i < boxA->mVertices.size(); i++)
	{
		if (i == boxA->mVertices.size() - 1) { next = boxA->mVertices[0]; }
		else { next = boxA->mVertices[i + 1]; }

		boxA->mNormals[i] = ((next - boxA->mVertices[i]).GetRotatedBy270().GetNormalised());
	}

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

	for (int i = 0; i < boxB->mVertices.size(); i++)
	{
		if (i == boxB->mVertices.size() - 1) { next = boxB->mVertices[0]; }
		else { next = boxB->mVertices[i + 1]; }

		boxB->mNormals[i] = ((next - boxB->mVertices[i]).GetRotatedBy270().GetNormalised());
	}


	Polygon* polyA = (Polygon*)bA;
	Polygon* polyB = (Polygon*)bB;
	return PolygonToPolygonCollision(polyA, polyB);

	// NON ROTATION 
	//float overlapDepths[4];
	//Vec2 overlapNormals[4];

	//overlapDepths[0] = boxB->GetXMax() - boxA->GetXMin();
	//overlapNormals[0] = Vec2(-1, 0);

	//overlapDepths[1] = boxA->GetXMax() - boxB->GetXMin();
	//overlapNormals[1] = Vec2(1, 0);

	//overlapDepths[2] = boxB->GetYMax() - boxA->GetYMin();
	//overlapNormals[2] = Vec2(0, -1);

	//overlapDepths[3] = boxA->GetYMax() - boxB->GetYMin();
	//overlapNormals[3] = Vec2(0, 1);

	//int overlapIndex = 0;
	//for (int i = 0; i < 4; i++)
	//{
	//	if (overlapDepths[i] < overlapDepths[overlapIndex])
	//	{
	//		overlapIndex = i;
	//	}
	//}

	//CollisionInfo info;

	//info.collisionNormal = overlapNormals[overlapIndex];
	//info.overlapAmount = overlapDepths[overlapIndex];
	//info.bIsOverlapping = info.overlapAmount > 0;
	//info.objA = boxA;
	//info.objB = boxB;

	//return info;
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

	Vec2 next;
	for (int i = 0; i < box->mVertices.size(); i++)
	{
		if (i == box->mVertices.size() - 1) { next = box->mVertices[0]; }
		else { next = box->mVertices[i + 1]; }

		box->mNormals[i] = ((next - box->mVertices[i]).GetRotatedBy270().GetNormalised());
	}


	Polygon* boxA = (Polygon*)bA;
	return PolygonToPolygonCollision(boxA, polyB);



	// Collision Detection for non-rotating box
	//Box* boxA = (Box*)bA;
	//Polygon* polygonB = (Polygon*)polyB;

	//CollisionInfo info;
	//info.objB = polygonB;
	//info.objA = boxA;
	//info.overlapAmount = FLT_MAX;
	//info.contactPoint = Vec2();

	//// Poly to Box collision
	//std::vector<Vec2> boxBnormals;
	//std::vector<Vec2> boxBVerts;

	//Vec2 topRight = Vec2(boxA->GetXMax(), boxA->GetYMax());
	//Vec2 topLeft = Vec2(boxA->GetXMin(), boxA->GetYMax());
	//Vec2 bottomLeft = Vec2(boxA->GetXMin(), boxA->GetYMin());
	//Vec2 bottomRight = Vec2(boxA->GetXMax(), boxA->GetYMin());

	//// Get normals of the box
	//boxBnormals.push_back(Vec2(-(topRight - topLeft).y, (topRight - topLeft).x).Normalise());
	//boxBnormals.push_back(Vec2(-(bottomLeft - topRight).y, (bottomLeft - topRight).x).Normalise());
	//boxBnormals.push_back(Vec2(-(bottomRight - bottomLeft).y, (bottomRight - bottomLeft).x).Normalise());
	//boxBnormals.push_back(Vec2(-(topLeft - bottomRight).y, (topLeft - bottomRight).x).Normalise());

	//boxBVerts.push_back(topLeft);
	//boxBVerts.push_back(topRight);
	//boxBVerts.push_back(bottomLeft);
	//boxBVerts.push_back(bottomRight);

	//// Add all normals of each polygon to a list
	//std::vector<Vec2> collectiveNormals;

	//for (Vec2& nA : polygonB->mNormals)
	//{
	//	collectiveNormals.push_back(nA);
	//}

	//for (Vec2& nB : boxBnormals)
	//{
	//	collectiveNormals.push_back(nB);
	//}

	//std::vector<float> polyAProjections;
	//std::vector<float> polyBProjections;

	//// Dot each vert in each shape against the collective normals
	//for (int i = 0; i < collectiveNormals.size(); i++)
	//{
	//	for (Vec2 v : polygonB->mVertices)
	//	{
	//		polyAProjections.push_back(Dot(v + polygonB->GetPos(), collectiveNormals[i]));
	//	}

	//	for (Vec2 v : boxBVerts)
	//	{
	//		polyBProjections.push_back(Dot(v, collectiveNormals[i]));
	//	}

	//	float aMax = -FLT_MAX;
	//	float aMin = FLT_MAX;
	//	float bMax = -FLT_MAX;
	//	float bMin = FLT_MAX;

	//	// Find the min and max points of each polygon
	//	for (float f : polyAProjections)
	//	{
	//		if (f < aMin) { aMin = f; }
	//		if (f > aMax) { aMax = f; }
	//	}
	//	polyAProjections.clear();


	//	for (float f : polyBProjections)
	//	{
	//		if (f < bMin) { bMin = f; }
	//		if (f > bMax) { bMax = f; }
	//	}
	//	polyBProjections.clear();

	//	// Check for overlaps 
	//	int smallestOverlapIndex = 0;
	//	float overlaps[2];
	//	overlaps[0] = aMax - bMin;
	//	overlaps[1] = bMax - aMin;

	//	for (int i = 0; i < 2; i++)
	//	{
	//		if (overlaps[i] < overlaps[smallestOverlapIndex])
	//		{
	//			smallestOverlapIndex = i;
	//		}
	//	}

	//	if (overlaps[smallestOverlapIndex] < info.overlapAmount)
	//	{
	//		info.overlapAmount = overlaps[smallestOverlapIndex];
	//		info.collisionNormal = -collectiveNormals[i];
	//	}
	//}

	//info.bIsOverlapping = info.overlapAmount > 0 ? true : false;
	//info.contactPoint = Vec2();
	//return info;
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
CollisionInfo PolygonToBoxCollision(PhysicsObject* polyA, PhysicsObject* bB) // Can reuse polygon to polygon here
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
	for (Vec2& nA : polygonA->mNormals)
	{
		collectiveNormals.push_back(nA);
	}

	for (Vec2& nB : polygonB->mNormals)
	{
		collectiveNormals.push_back(nB);
	}

	std::vector<float> polyAProjections;
	std::vector<float> polyBProjections;

	Vec2 displacement = polyA->GetPos() - polyB->GetPos();

	float aPoint;
	float bPoint;

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
		float overlaps[3];
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
	info.contactPoint2 = polygonA->GetPos() + info.collisionNormal * (1.6 - info.overlapAmount);
	info.contactPoint = polygonB->GetPos() + -info.collisionNormal * (1.6 -info.overlapAmount);
	return info;
}




