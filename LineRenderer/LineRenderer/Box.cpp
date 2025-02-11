#include "Box.h"
#include "LineRenderer.h"

Box::Box(Vec2 pos, float w, float h, float mass) : PhysicsObject(pos, mass), mWidth(w), mHeight(h)
{
	//mXMax = Vec2((pos.x + 0.5 * w), (pos.y + 0.5 * h));
	//mXMin = Vec2((pos.x - 0.5 * w), (pos.y - 0.5 * h));
	//
	//mYMin = Vec2((pos.x + 0.5 * w), (pos.y - 0.5 * h));
	//mYMax = Vec2((pos.x - 0.5 * w), (pos.y + 0.5 * h));
}


void Box::Update(float delta)
{
	PhysicsObject::Update(delta);
}

void Box::Draw(LineRenderer* lines)
{
	Vec2 mXMax = Vec2((GetXMax()), (GetYMax()));
	Vec2 mXMin = Vec2((GetXMin()), (GetYMin()));
	Vec2 mYMin = Vec2((GetXMax()), (GetYMin()));
	Vec2 mYMax = Vec2((GetXMin()), (GetYMax()));

	lines->DrawLineSegment(mYMax, mXMax, mColour);
	lines->DrawLineSegment(mXMax, mYMin, mColour);
	lines->DrawLineSegment(mYMin, mXMin, mColour);
	lines->DrawLineSegment(mXMin, mYMax, mColour);
	lines->FinishLineLoop();
}
