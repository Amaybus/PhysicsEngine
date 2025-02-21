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
	std::vector<Vec2> aContactPoints;
	std::vector<Vec2> bContactPoints;

	std::vector<Vec2> aNorm;
	std::vector<Vec2> bNorm;


	void Resolve();
	void ResolveRotation();
};
