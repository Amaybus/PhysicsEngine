#include "Circle.h"
#include "LineRenderer.h"

Circle::Circle(Vec2 pos, float radius, float mass) : PhysicsObject(pos, mass), mRadius(radius)
{
	mInertia = 0.5 * mMass * (mRadius * mRadius);
}

void Circle::Draw(LineRenderer* lines)
{
	Vec2 end = Vec2(cos(mOrientation), sin(mOrientation)) * mRadius;
	lines->DrawCircle(mPos, mRadius, mColour);
	lines->DrawLineSegment(mPos, mPos +end);
}

