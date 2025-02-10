#include "Circle.h"
#include "LineRenderer.h"

Circle::Circle(Vec2 pos, float radius, float mass) : PhysicsObject(pos, mass), mRadius(radius)
{
	mMomentOfInertia = (2 / 5) * (mMass * mRadius * mRadius);
}

void Circle::Draw(LineRenderer* lines)
{
	lines->DrawCircle(mPos, mRadius, mColour);
	lines->DrawLineSegment(mPos, Vec2(mPos.x + mRadius, mPos.y));
}

