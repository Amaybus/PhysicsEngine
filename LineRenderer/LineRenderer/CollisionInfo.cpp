#include "CollisionInfo.h"
#include <iostream>

#include "dirent.h"

void CollisionInfo::Resolve()
{
	if (!bIsOverlapping) return;

	// If objA is a plane
	if (objA->GetMass() <= 0)
	{
		float totalInverseMass = objB->GetInverseMass();
		//Depen Step
		objB->GetPos() += collisionNormal * overlapAmount;

		//Apply Force
		float impulseMag = Dot(-2 * objB->GetVelocity(), collisionNormal) / totalInverseMass;
		Vec2 force = collisionNormal * impulseMag;

		objB->ApplyImpulse(force, contactPoint);
		return;
	}

	float totalInverseMass = objA->GetInverseMass() + objB->GetInverseMass();

	// Depen step
	objA->GetPos() -= collisionNormal * overlapAmount * objA->GetInverseMass() / totalInverseMass;
	objB->GetPos() += collisionNormal * overlapAmount * objB->GetInverseMass() / totalInverseMass;

	// Apply Force
	Vec2 relVe = objB->GetVelocity() - objA->GetVelocity();
	float impulseMag = Dot(-2 * relVe, collisionNormal) / totalInverseMass;
	if (impulseMag < 0) return;
	Vec2 force = collisionNormal * impulseMag;

	objA->ApplyImpulse(-force);
	objB->ApplyImpulse(force);
}

void CollisionInfo::ResolveRotation()
{
	// If objA is a plane
	//if (objA->GetMass() <= 0)
	//{
	//	//Depen Step
	//	objB->GetPos() += collisionNormal * overlapAmount;

	//	Vec2 perp = Vec2(collisionNormal.y, -collisionNormal.x);
	//	float rb = Dot(contactPoint - objB->GetPos(), perp);

	//	float vb = Dot(objB->GetVelocity(), collisionNormal) - rb * objB->GetAngularVelocity();

	//	float massB = 1.0f / objB->GetInverseMass() + (rb * rb) / objB->GetInertia();

	//	Vec2 force = 2 * massB / +massB * -vb * collisionNormal;
	//	objB->ApplyImpulse(force, contactPoint);

	//	return;
	//}


	float totalInverseMass = objA->GetInverseMass() + objB->GetInverseMass();

	objA->GetPos() -= collisionNormal * overlapAmount * objA->GetInverseMass() / totalInverseMass;
	objB->GetPos() += collisionNormal * overlapAmount * objB->GetInverseMass() / totalInverseMass;

	//// Find perpendicular angle from the vector
	//Vec2 raPerp = Vec2(-objA->GetPos().y, objA->GetPos().x);
	//Vec2 rbPerp = Vec2(-objB->GetPos().y, objB->GetPos().x);
	//
	//// Find angular linear velocity
	//Vec2 angLinearVelA = raPerp * objA->GetAngularVelocity();
	//Vec2 angLinearVelB = rbPerp * objB->GetAngularVelocity();
	//
	//// Relative Velocity
	//Vec2 relVel = (objB->GetVelocity() + angLinearVelB) - (objA->GetVelocity() + angLinearVelA);
	//
	//float contactVelMag = Dot(relVel, collisionNormal);
	//if (contactVelMag > 0)
	//{
	//	return;
	//}
	//
	//float raPerpDotN = Dot(raPerp, collisionNormal);
	//float rbPerpDotN = Dot(rbPerp, collisionNormal);
	//
	//float raDenom = (raPerpDotN * raPerpDotN) * (1.0f / objA->GetInertia());
	//float rbDenom = (rbPerpDotN * rbPerpDotN) * (1.0f / objB->GetInertia());
	//
	//float impulseMag = ((-2 * contactVelMag) / totalInverseMass) + raDenom + rbDenom;
	//
	//Vec2 force = collisionNormal * impulseMag;
	//
	//objA->ApplyImpulse(-force, contactPoint);
	//objB->ApplyImpulse(force, contactPoint);


	Vec2 perp = Vec2(collisionNormal.y, -collisionNormal.x); //Vec2 perp = collisionNormal.GetRotatedBy270();
	float aPerp = Dot(contactPoint - objA->GetPos(), -perp);
	float bPerp = Dot(contactPoint - objB->GetPos(), perp);
	
	float velA = Dot(objA->GetVelocity(), collisionNormal) - aPerp * objA->GetAngularVelocity();
	float velB = Dot(objB->GetVelocity(), collisionNormal) - bPerp * objB->GetAngularVelocity();
	
	//float vaCrossvb = PseudoCross(collisionNormal, objB->GetVelocity().GetNormalised());
	
	if (velA > velB)
	{
		float massA = 1.0f / (objA->GetInverseMass() + (aPerp * aPerp) / objA->GetInertia());
		float massB = 1.0f / (objB->GetInverseMass() + (bPerp * bPerp) / objB->GetInertia());

		//Vec2 force = 2 * massA * massB / (massA + massB) * (velA - velB) * collisionNormal;
		Vec2 relVel = (objB->GetVelocity() + (perp * objB->GetAngularVelocity()) - (objA->GetVelocity() + (perp * objA->GetAngularVelocity())));
		float impulseMag = Dot(- 2 * relVel, collisionNormal) / (massA + massB);
		
		Vec2 force = collisionNormal * impulseMag;

		objA->ApplyImpulse(-force, contactPoint);
		objB->ApplyImpulse(force, contactPoint);
	}
}
