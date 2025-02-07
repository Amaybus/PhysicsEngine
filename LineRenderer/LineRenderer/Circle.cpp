#include "Circle.h"
#include "LineRenderer.h"

Circle::Circle(Vec2 pos, float radius, float mass) : PhysicsObject(pos, mass), mRadius(radius)
{
}

void Circle::Draw(LineRenderer* lines)
{
	lines->DrawCircle(mPos, mRadius, mColour);
}

