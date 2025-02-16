#include "CollisionInfo.h"
#include <iostream>

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
		//objB->SetVelocity(Vec2());
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

	Vec2 perp = Vec2(collisionNormal.y, -collisionNormal.x);
	float r1 = Dot(contactPoint - objA->GetPos(), -perp);

	objA->ApplyImpulse(-force);
	objB->ApplyImpulse(force);
}

void CollisionInfo::ResolveRotation()
{
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
		//objB->SetVelocity(Vec2());
		return;
	}


	float totalInverseMass = objA->GetInverseMass() + objB->GetInverseMass();

	objA->GetPos() -= collisionNormal * overlapAmount * objA->GetInverseMass() / totalInverseMass;
	objB->GetPos() += collisionNormal * overlapAmount * objB->GetInverseMass() / totalInverseMass;
	
	Vec2 perp = Vec2(collisionNormal.y, -collisionNormal.x);
	float ra = Dot(contactPoint - objA->GetPos(), -perp);
	float rb = Dot(contactPoint - objB->GetPos(), perp);
	
	float va = Dot(objA->GetVelocity(), collisionNormal) - ra * objA->GetAngularVelocity();
	float vb = Dot(objB->GetVelocity(), collisionNormal) - rb * objB->GetAngularVelocity();
	
	if(va > vb)
	{
		float massA = 1.0f / objA->GetInverseMass() + (ra * ra) / objA->GetInertia();
		float massB = 1.0f / objB->GetInverseMass() + (rb * rb) / objB->GetInertia();
	
		Vec2 force = 2 * massA * massB / (massA + massB) * (va - vb) * collisionNormal;
		objA->ApplyImpulse(-force, contactPoint);
		objB->ApplyImpulse(force, contactPoint);
	}

	//Vec2 ra = contactPoint - objA->GetPos();
	//Vec2 rb = contactPoint - objA->GetPos();
	//
	//// Find perpendicular angle from the vector
	//Vec2 raPerp = Vec2(-ra.y, ra.x);
	//Vec2 rbPerp = Vec2(-rb.y, rb.x);
	//
	//// Find angular linear velocity
	//Vec2 angLinearVelA = raPerp * objA->GetAngularVelocity();
	//Vec2 angLinearVelB = rbPerp * objB->GetAngularVelocity();
	//
	//// Relative Velocity
	//Vec2 relVel = (objB->GetVelocity() + angLinearVelB) - (objA->GetVelocity() + angLinearVelA);
	//
	//float contactVelMag = Dot(relVel, collisionNormal);
	//if (contactVelMag > 0) { return; }
	//
	//
	//
	//float raPerpDotN = Dot(raPerp, collisionNormal);
	//float rbPerpDotN = Dot(rbPerp, collisionNormal);
	//
	//float raDenom = (raPerpDotN * raPerpDotN) * (1.0f / objA->GetInertia());
	//float rbDenom = (rbPerpDotN * rbPerpDotN) * (1.0f / objB->GetInertia());
	//
	//float impulseMag = (( - 2 * contactVelMag) / totalInverseMass) + raDenom + rbDenom;
	//// Split between each objects contact point
	//impulseMag *= 0.5;
	//
	//Vec2 force = collisionNormal * impulseMag;
	//
	//objA->ApplyImpulse(-force, contactPoint);
	//objB->ApplyImpulse(force, contactPoint);
}
