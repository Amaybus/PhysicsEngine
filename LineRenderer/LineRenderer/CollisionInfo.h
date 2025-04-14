#pragma once
#include "Vec2.h"
#include "PhysicsObject.h"
#include <vector>


class CollisionInfo
{
public:
	PhysicsObject* objA = nullptr;
	PhysicsObject* objB = nullptr;

	float overlapAmount = 0;
	bool bIsOverlapping = false;
	Vec2 collisionNormal;
	bool bIsTrigger = false;
	std::vector<Vec2> aContactPoints;
	std::vector<Vec2> bContactPoints;

	void Resolve();
	void ResolveWithRotation();
	Vec2 GetVelocityAtPoint(PhysicsObject* object, Vec2 contactPoint);
};
