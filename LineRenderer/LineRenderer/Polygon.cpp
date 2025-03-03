#include "Polygon.h"

#include "LineRenderer.h"

Polygon::Polygon(Vec2 pos, int numOfVerts, float mass) : PhysicsObject(pos, mass), mVertCount(numOfVerts)
{
	// divide into triangles 
	// calc moment for triangle ( 0.5*mass*(length*length) * ( 1 - (2/3)*sin2(vertexAngle))
	// multiply moment by 2n
	mInertia = 1.4; 

	mPadding = 1;
	Vec2 offsetVec(mPadding, 0);
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

		mNormals.push_back((next - mVertices[i]).GetRotatedBy270().GetNormalised());
		mEdgeCentres.push_back(Vec2((mVertices[i].x + next.x) * 0.5, (mVertices[i].y + next.y) * 0.5));
	}
}

Polygon::Polygon(Vec2 pos, int numOfVerts, float mass, float elasticity) : PhysicsObject(pos, mass, elasticity), mVertCount(numOfVerts)
{
}

Polygon::Polygon(Vec2 pos, float mass) : PhysicsObject(pos, mass), mVertCount(4)
{
}

Polygon::~Polygon()
{
	for(int i = 0; i < mVertices.size();i++)
	{
		mVertices.erase(mVertices.begin() + i);
	}

	for (int i = 0; i < mEdgeCentres.size(); i++)
	{
		mEdgeCentres.erase(mEdgeCentres.begin() + i);
	}

	for (int i = 0; i < mNormals.size(); i++)
	{
		mNormals.erase(mNormals.begin() + i);
	}
}

std::vector<Vec2> Polygon::GetNormals()
{
	Vec2 next;
	for (int i = 0; i < mVertices.size(); i++)
	{
		if (i == mVertices.size() - 1) { next = mVertices[0]; }
		else { next = mVertices[i + 1]; }

		mNormals[i] = ((next - mVertices[i]).GetRotatedBy270().GetNormalised());
	}

	return mNormals;
}

std::vector<Vec2> Polygon::GetEdgeCentres()
{
	Vec2 next;
	for (int i = 0; i < mVertices.size(); i++)
	{
		if (i == mVertices.size() - 1) { next = mVertices[0]; }
		else { next = mVertices[i + 1]; }

		mEdgeCentres[i]=(Vec2((mVertices[i].x + next.x) * 0.5, (mVertices[i].y + next.y) * 0.5));
	}

	return mEdgeCentres;
}

void Polygon::Draw(LineRenderer* lines)
{
	PhysicsObject::Draw(lines);

	float angle = asin(PseudoCross(mVertices[0], mLocalX) / (mVertices[0]).GetMagnitude() * mLocalX.GetMagnitude());
	for (int i = 0; i < mVertCount; i++)
	{
		Vec2 vert = mVertices[i].RotateBy(angle);

		mVertices[i] = vert;
	}

	for (Vec2& vert : mVertices)
	{
		lines->DrawCircle(vert + mPos, 0.1f, Colour::BLUE);
	}

	for(int i = 0; i < mVertCount; i++)
	{
		lines->AddPointToLine(mVertices[i] + mPos, mColour);
	}
	lines->FinishLineLoop();
}

