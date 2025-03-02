#include "Box.h"
#include "LineRenderer.h"

Box::Box(Vec2 pos, float w, float h, float mass) : Polygon(pos, mass), mWidth(w), mHeight(h)
{
	mInertia = (1.0f/12.0f) * mMass * (mWidth * mWidth + mHeight * mHeight);

	for (int i = 0; i < GetVertexCount(); i++)
	{
		mVertices.push_back(Vec2());
		mNormals.push_back(Vec2());
	}
}

Box::Box(Vec2 pos, float w, float h, float mass, float elasticity) : Polygon(pos, mass, elasticity), mWidth(w), mHeight(h)
{
	mInertia = (1.0f / 12.0f) * mMass * (mWidth * mWidth + mHeight * mHeight);

	for (int i = 0; i < GetVertexCount(); i++)
	{
		mVertices.push_back(Vec2());
		mNormals.push_back(Vec2());
	}
}


void Box::Draw(LineRenderer* lines)
{
	PhysicsObject::Draw(lines);

	Vec2 topRight = mPos + mLocalX * (0.5f * mWidth) + mLocalY * (0.5f * mHeight);
	Vec2 bottomLeft = mPos - mLocalX * (0.5f * mWidth) - mLocalY * (0.5f * mHeight);
	
	Vec2 topLeft = mPos - mLocalX * (0.5f * mWidth) + mLocalY * (0.5f * mHeight);
	Vec2 bottomRight = mPos + mLocalX * (0.5f * mWidth) - mLocalY * (0.5f * mHeight);
	
	lines->AddPointToLine(topRight, mColour);
	lines->AddPointToLine(bottomRight, mColour);
	lines->AddPointToLine(bottomLeft, mColour);
	lines->AddPointToLine(topLeft, mColour);
	
	lines->FinishLineLoop();
}
