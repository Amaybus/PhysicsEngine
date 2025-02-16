#include "Box.h"
#include "LineRenderer.h"

Box::Box(Vec2 pos, float w, float h, float mass) : PhysicsObject(pos, mass), mWidth(w), mHeight(h)
{
	// Intertia cal is setting to 0??
	//mInertia = (1/12) * mMass * (mWidth * mWidth + mHeight * mHeight);
	mInertia = 1.4;
}


void Box::Update(float delta)
{
	PhysicsObject::Update(delta);

	float cs = cosf(mOrientation);
	float sn = sinf(mOrientation);
	mLocalX = Vec2(cs, sn).Normalise();
	mLocalY = Vec2(-sn, cs).Normalise();
}

void Box::Draw(LineRenderer* lines)
{
	Vec2 mXMin = mPos - mLocalX * (0.5f * mWidth) - mLocalY * (0.5f * mHeight);
	Vec2 mXMax = mPos + mLocalX * (0.5f * mWidth) - mLocalY * (0.5f * mHeight);
	Vec2 mYMin = mPos - mLocalX * (0.5f * mWidth) + mLocalY * (0.5f * mHeight);
	Vec2 mYMax = mPos + mLocalX * (0.5f * mWidth) + mLocalY * (0.5f * mHeight);

	//Vec2 mXMax = Vec2((GetXMax()), (GetYMax()));
	//Vec2 mXMin = Vec2((GetXMin()), (GetYMin()));
	//Vec2 mYMin = Vec2((GetXMax()), (GetYMin()));
	//Vec2 mYMax = Vec2((GetXMin()), (GetYMax()));

	lines->DrawLineSegment(mYMax, mYMin, mColour);
	lines->DrawLineSegment(mYMin, mXMin, mColour);
	lines->DrawLineSegment(mXMin, mXMax, mColour);
	lines->DrawLineSegment(mXMax, mYMax, mColour);
	lines->FinishLineLoop();


	//lines->DrawLineSegment(mYMax, mXMax, mColour);
	//lines->DrawLineSegment(mXMax, mYMin, mColour);
	//lines->DrawLineSegment(mYMin, mXMin, mColour);
	//lines->DrawLineSegment(mXMin, mYMax, mColour);
	//lines->FinishLineLoop();
}
