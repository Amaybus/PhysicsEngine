#include "Box.h"
#include "LineRenderer.h"

Box::Box(Vec2 pos, float w, float h, float mass) : PhysicsObject(pos, mass), mWidth(w), mHeight(h)
{
	mXMax = Vec2((pos.x + 0.5 * w), (pos.y + 0.5 * h));
	mXMin = Vec2((pos.x - 0.5 * w), (pos.y - 0.5 * h));

	mYMin = Vec2((pos.x + 0.5 * w), (pos.y - 0.5 * h));
	mYMax = Vec2((pos.x - 0.5 * w), (pos.y + 0.5 * h));
}

void Box::Update(float delta)
{
}

void Box::Draw(LineRenderer* lines)
{
	mXMax = Vec2((mPos.x + 0.5 * mWidth), (mPos.y + 0.5 * mHeight));
	mXMin = Vec2((mPos.x - 0.5 * mWidth), (mPos.y - 0.5 * mHeight));

	mYMin = Vec2((mPos.x + 0.5 * mWidth), (mPos.y - 0.5 * mHeight));
	mYMax = Vec2((mPos.x - 0.5 * mWidth), (mPos.y + 0.5 * mHeight));

	lines->DrawLineSegment(mYMax, mXMax, mColour);
	lines->DrawLineSegment(mXMax, mYMin, mColour);
	lines->DrawLineSegment(mYMin, mXMin, mColour);
	lines->DrawLineSegment(mXMin, mYMax, mColour);
	lines->FinishLineLoop();
}
