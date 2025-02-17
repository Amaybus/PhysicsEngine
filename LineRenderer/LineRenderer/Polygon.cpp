#include "Polygon.h"

#include "LineRenderer.h"

Polygon::Polygon(Vec2 pos, int numOfVerts, float mass) : PhysicsObject(pos, mass), mVertCount(numOfVerts)
{
	// divide into triangles 
	// calc moment for triangle ( 0.5*mass*(length*length) * ( 1 - (2/3)*sin2(vertexAngle))
	// multiply moment by 2n
	mInertia = 1.4; 


	float padding = 1.6;
	Vec2 offsetVec(padding, 0);
	for (int i = 0; i < mVertCount; i++)
	{
		mVertices.push_back(offsetVec);
		offsetVec.RotateBy(-2 * PI / mVertCount);
	}

	Vec2 next;
	for (int i = 0; i < mVertices.size(); i++)
	{
		if (i == mVertices.size() - 1) { next = mVertices[0]; }
		else { next = mVertices[i + 1]; }

		mNormals.push_back(Vec2(-(next - mVertices[i]).y, (next - mVertices[i]).x).Normalise());
		//mNormals.push_back((next - mVertices[i]).GetRotatedBy270().GetNormalised());
		mEdgeCentre.push_back(Vec2((mVertices[i].x + next.x) * 0.5, (mVertices[i].y + next.y) * 0.5));
	}
}

void Polygon::Draw(LineRenderer* lines)
{
	lines->DrawCircle(mPos, 0.1, Colour::YELLOW);

	for (Vec2 vert : mVertices)
	{
		lines->DrawCircle(vert + mPos, 0.1f);
	}

	for(int i = 0; i < mVertCount; i++)
	{
		lines->AddPointToLine(mVertices[i] + mPos, mColour);
	}
	lines->FinishLineLoop();

	lines->DrawLineWithArrow(mPos, mPos + GetLocalY(), Colour::GREEN, 0.1);
	lines->DrawLineWithArrow(mPos, mPos + GetLocalX(), Colour::RED, 0.1);

	//for(int i = 0; i < mNormals.size(); i++)
	//{
	//	lines->DrawLineWithArrow(mEdgeCentre[i] + mPos, mEdgeCentre[i] + mPos + mNormals[i], Colour::MAGENTA.Lighten(), 0.2);
	//}

	//for(Vec2 e : mEdgeCentre)
	//{
	//	lines->DrawCross(e + mPos, 0.1, Colour::MAGENTA);
	//}
}

