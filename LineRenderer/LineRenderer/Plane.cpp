#include "Plane.h"
#include "LineRenderer.h"


Plane::Plane(Vec2 unitNormal, float distanceToOrigin) : mNormal(unitNormal.GetNormalised()), mDistanceToOrigin(distanceToOrigin)
{
	mCentre = mNormal * mDistanceToOrigin;
	SetIsKinematic(false);
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
