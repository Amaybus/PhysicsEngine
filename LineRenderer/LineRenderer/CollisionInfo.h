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

	std::vector<Vec2> nomrals;
	std::vector<std::vector<float>> avalues;
	std::vector<std::vector<float>> bvalues;

	void Resolve();
};

