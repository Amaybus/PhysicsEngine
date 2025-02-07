#include "Polygon.h"

#include "LineRenderer.h"

Polygon::Polygon(Vec2 pos, int numOfVerts, float mass) : PhysicsObject(pos, mass), mVertCount(numOfVerts)
{
	for (int i = 0; i < mVertCount; i++)
	{
		mVertices.push_back(Vec2(rand() %10 +  -numOfVerts, rand() % 10 + - numOfVerts));
	}
}

void Polygon::Draw(LineRenderer* lines)
{
	for (Vec2 vert : mVertices)
	{
		lines->DrawCircle(vert, 0.1);
	}

	for(int i = 0; i < mVertCount; i++)
	{
		lines->AddPointToLine(mVertices[i]);
	}
	lines->FinishLineLoop();
}
