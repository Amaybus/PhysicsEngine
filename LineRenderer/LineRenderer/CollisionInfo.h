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
	bool bIsTrigger = false;
	std::vector<Vec2> aContactPoints;
	std::vector<Vec2> bContactPoints;

	void Resolve();
	void ResolveWithRotation();
};
