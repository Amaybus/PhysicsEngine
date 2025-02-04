#include "Plane.h"
#include "LineRenderer.h"


Plane::Plane(Vec2 unitNormal, float distanceToOrigin, float length) : mNormal(unitNormal), mDistanceToOrigin(distanceToOrigin), mLength(length)
{
	mCentre = mNormal * mDistanceToOrigin;
}

Plane::~Plane()
{
}

void Plane::Update(float delta)
{
}

void Plane::Draw(LineRenderer* lines)
{
	Vec2 parallel = Vec2(mNormal.y, -mNormal.x);
	Vec2 start = mCentre - (parallel * mLength);
	Vec2 end = mCentre + (parallel * mLength);

	lines->DrawLineSegment(start, end, Colour::MAGENTA);
	lines->DrawCircle(mCentre, 0.1, Colour::BLUE);
	lines->DrawLineWithArrow(mCentre, mCentre + mNormal, Colour::BLUE.Lighten(), 0.3f);
}
