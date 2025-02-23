#include "Circle.h"
#include "LineRenderer.h"

Circle::Circle(Vec2 pos, float radius, float mass) : PhysicsObject(pos, mass), mRadius(radius)
{
	mInertia = 0.5f * mMass * (mRadius * mRadius);
}

void Circle::Draw(LineRenderer* lines)
{
	PhysicsObject::Draw(lines);
	lines->DrawCircle(mPos, mRadius, mColour);
}

