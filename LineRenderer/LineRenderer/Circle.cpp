#include "Circle.h"
#include "LineRenderer.h"

Circle::Circle(Vec2 pos, float radius, float mass) : PhysicsObject(pos, mass), mRadius(radius)
{
	mInertia = 0.5f * mMass * (mRadius * mRadius);
}

void Circle::Draw(LineRenderer* lines)
{
	lines->DrawCircle(mPos, mRadius, mColour);

	lines->DrawLineWithArrow(mPos, mPos + GetLocalY(), Colour::GREEN, 0.1);
	lines->DrawLineWithArrow(mPos, mPos + GetLocalX(), Colour::RED, 0.1);
}

