#pragma once
#include "Vec2.h"
#include "PhysicsObject.h"
#include <vector>


class CollisionInfo
{
public:
	PhysicsObject* objA;
	PhysicsObject* objB;

	float overlapAmount;
	bool bIsOverlapping;
	Vec2 collisionNormal;
	Vec2 contactPoint;
	std::vector<Vec2> normals;
	std::vector<Vec2> verts;

	void Resolve();
	void ResolveRotation();
};
