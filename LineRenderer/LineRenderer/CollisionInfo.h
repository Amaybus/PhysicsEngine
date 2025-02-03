#pragma once
#include "Vec2.h"
#include "PhysicsObject.h"


class CollisionInfo
{
public:
	PhysicsObject* objA;
	PhysicsObject* objB;

	float overlapAmount;
	bool bIsOverlapping;
	Vec2 collisionNormal;

	void Resolve();
};

