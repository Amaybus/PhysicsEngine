#include "Box.h"
#include "LineRenderer.h"

Box::Box(Vec2 pos, float w, float h, float mass) : PhysicsObject(pos, mass), mWidth(w), mHeight(h)
{
	mInertia = (1.0f/12.0f) * mMass * (mWidth * mWidth + mHeight * mHeight);
}


void Box::Draw(LineRenderer* lines)
{
	Vec2 topRight = mPos + mLocalX * (0.5f * mWidth) + mLocalY * (0.5f * mHeight);
	Vec2 bottomLeft = mPos - mLocalX * (0.5f * mWidth) - mLocalY * (0.5f * mHeight);

	Vec2 topLeft = mPos - mLocalX * (0.5f * mWidth) + mLocalY * (0.5f * mHeight);
	Vec2 bottomRight = mPos + mLocalX * (0.5f * mWidth) - mLocalY * (0.5f * mHeight);


	lines->AddPointToLine(topRight, mColour);
	lines->AddPointToLine(bottomRight, mColour);
	lines->AddPointToLine(bottomLeft, mColour);
	lines->AddPointToLine(topLeft, mColour);

	lines->FinishLineLoop();

	lines->DrawLineWithArrow(mPos, mPos + mLocalY, Colour::GREEN, 0.1);
	lines->DrawLineWithArrow(mPos, mPos + mLocalX, Colour::RED, 0.1);
}
