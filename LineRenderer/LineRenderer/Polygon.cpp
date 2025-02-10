#include "Polygon.h"

#include "LineRenderer.h"

Polygon::Polygon(Vec2 pos, int numOfVerts, float mass) : PhysicsObject(pos, mass), mVertCount(numOfVerts)
{
	for (int i = 0; i < mVertCount; i++)
	{
		mVertices.push_back(Vec2(rand() %10 +  -numOfVerts, rand() % 10 + - numOfVerts));
	}

	Vec2 next;
	for (int i = 0; i < mVertices.size(); i++)
	{
		if (i == mVertices.size() - 1) { next = mVertices[0]; }
		else { next = mVertices[i + 1]; }

		mNormals.push_back(Vec2(-(next - mVertices[i]).y, (next - mVertices[i]).x).Normalise());
		mEdgeCentre.push_back(Vec2((mVertices[i].x + next.x) * 0.5, (mVertices[i].y + next.y) * 0.5));
	}
}

void Polygon::Update(float delta)
{
	PhysicsObject::Update(delta);

	Vec2 next;
	for (int i = 0; i < mVertices.size(); i++)
	{
		if (i == mVertices.size() - 1) { next = mVertices[0]; }
		else { next = mVertices[i + 1]; }

		mNormals[i] = (Vec2(-(next - mVertices[i]).y, (next - mVertices[i]).x).Normalise());
		mEdgeCentre[i] = (Vec2((mVertices[i].x + next.x) * 0.5, (mVertices[i].y + next.y) * 0.5));
	}
}

void Polygon::Draw(LineRenderer* lines)
{
	lines->DrawCircle(mPos, 0.5);

	for (Vec2 vert : mVertices)
	{
		lines->DrawCircle(vert, 0.1);
	}

	for(int i = 0; i < mVertCount; i++)
	{
		lines->AddPointToLine(mVertices[i]);
	}
	lines->FinishLineLoop();

	for(int i = 0; i < mNormals.size(); i++)
	{
		lines->DrawLineWithArrow(mEdgeCentre[i], mEdgeCentre[i] + mNormals[i], Colour::MAGENTA.Lighten(), 0.2);
	}

	for(Vec2 e : mEdgeCentre)
	{
		lines->DrawCross(e, 0.1, Colour::MAGENTA);
	}
}
